//============================================================================
// Name        : HW2_Prob2.cpp
// Author      : Kathryn McArdle
//============================================================================

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <Dense>
#include <Sparse>
#include <LU>
#include <math.h>

// for file reading:
#include <fstream>
#include <cstring>

#define N_ITER 10
using namespace std;

typedef Eigen::SparseMatrix<double> SpMat;
typedef Eigen::Triplet<double> T;

double get_rmse(SpMat R_test, Eigen::MatrixXd U, Eigen::MatrixXd M, int n_ratings) {
	Eigen::VectorXd actual_ratings(n_ratings);
	Eigen::VectorXd predicted_ratings(n_ratings);
	int count = 0;
	int user, movie;
	for (int i = 0; i < R_test.outerSize(); i++) {
		for (SpMat::InnerIterator it(R_test, i); it; ++it) {
			actual_ratings(count) = it.value();
			user = it.row();
			movie = it.col();
			predicted_ratings(count) = U.transpose().row(user) * M.col(movie);
			count++;
		}
	}
	Eigen::VectorXd error_squared = (actual_ratings - predicted_ratings).array().square();
	return sqrt((error_squared.sum())/n_ratings);
}

int main(int argc, char *argv[]) {
	double t; // measures run time of segments
	double total_time = 0; // measures total run time time
	int k = atoi(argv[1]);
	int lambda = atoi(argv[2]);
	int n_threads = atoi(argv[3]);
	char * data = argv[4];
	strcat(data, "/");

	t = omp_get_wtime();
	// Parse meta file:
	char meta_filepath[20];
	strcpy(meta_filepath, data);
	strcat(meta_filepath, "meta");
	int u, m, n_ratings_train, n_ratings_test;
	char train_file[20], test_file[20];
	ifstream metafile(meta_filepath);
	metafile >> u >> m >> n_ratings_train >> train_file >> n_ratings_test >> test_file;
	metafile.close();
	char train_filepath[40];
	strcpy(train_filepath, data);
	strcat(train_filepath, train_file);
	char test_filepath[40];
	strcpy(test_filepath, data);
	strcat(test_filepath, test_file);

	// Read in testing data
	vector<T> tripletList_test;
	tripletList_test.reserve(n_ratings_test);
	ifstream testfile(test_filepath);
	int user, movie;
	double rating;
	while(testfile >> user >> movie >> rating) {
		int movie_index = movie - 1;
		int user_index = user - 1;
		tripletList_test.push_back(T(user_index, movie_index, rating));
	}
	SpMat R_test(u,m);
	R_test.setFromTriplets(tripletList_test.begin(), tripletList_test.end());

	// Read in training data
	vector<T> tripletList;
	tripletList.reserve(n_ratings_train);
	map<int, pair<vector<int>, Eigen::VectorXd> > movies_map; //key = movie index; value = pair.first is vector of user indices, pair.second is Vector of corresponding ratings
	map<int, pair<vector<int>, Eigen::VectorXd> > users_map; //key = user index; pair.first is vector of movie indices, pair.second is Vector of corresponding ratings
	map<int, vector<double> > movies_to_ratings;
	map<int, vector<double> > users_to_ratings;
	set<int> movies_with_ratings_set; // unique list of movie indices that have ratings. This is needed because OpenMP can't parallelize an Iterator very well.
	set<int> users_with_ratings_set; // unique list of user indices that have rated movies
	vector<int> movies_with_ratings; // unique list of movie indices that have ratings. This is needed because OpenMP can't parallelize an Iterator very well.
	vector<int> users_with_ratings; // unique list of user indices that have rated movies

	ifstream trainfile(train_filepath);
	while(trainfile >> user >> movie >> rating) {
		int movie_index = movie - 1;
		int user_index = user - 1;
		tripletList.push_back(T(user_index, movie_index, rating));

		movies_map[movie_index].first.push_back(user_index);
		movies_with_ratings_set.insert(movie_index);
		movies_to_ratings[movie_index].push_back(rating);

		users_map[user_index].first.push_back(movie_index);
		users_with_ratings_set.insert(user_index);
		users_to_ratings[user_index].push_back(rating);
	}

	SpMat R(u,m);
	R.setFromTriplets(tripletList.begin(), tripletList.end());

	// Move the set values from movies_with_ratings, users_with_ratings to the vectors
	movies_with_ratings.assign(movies_with_ratings_set.begin(), movies_with_ratings_set.end());
	users_with_ratings.assign(users_with_ratings_set.begin(), users_with_ratings_set.end());

	// Move the vector values from movies_to_ratings to the VectorXi in movies_map:
	#pragma omp parallel num_threads(n_threads) default(shared)
	{
		#pragma omp for
		for (int j = 0; j < movies_with_ratings.size(); j++) {
			int movie_index = movies_with_ratings[j];
			vector<double> ratings = movies_to_ratings[movie_index];
			movies_map[movie_index].second.resize(ratings.size());
			int i = 0;
			for (vector<double>::iterator it = ratings.begin(); it != ratings.end(); ++it) {
				movies_map[movie_index].second(i) = *it;
				i++;
			}
		}
		#pragma omp for
		for (int j = 0; j < users_with_ratings.size(); j++) {
			int user_index = users_with_ratings[j];
			vector<double> ratings = users_to_ratings[user_index];
			users_map[user_index].second.resize(ratings.size());
			int i = 0;
			for (vector<double>::iterator it = ratings.begin(); it != ratings.end(); ++it) {
				users_map[user_index].second(i) = *it;
				i++;
			}
		}
	}

	// Create and initialize U and M matrices
	Eigen::MatrixXd U = (Eigen::MatrixXd::Random(k,u) + Eigen::MatrixXd::Constant(k,u,1))/2;
	for (int i = 0; i < u; i++) {
		// if user i has rated no movies at all, set the ith column in U to all zeros
		if (users_map.count(i) == 0) {
			U.col(i) = Eigen::VectorXd::Zero(k);
		}
	}

	Eigen::MatrixXd M = (Eigen::MatrixXd::Random(k,m) + Eigen::MatrixXd::Constant(k,m,1))/2;
	for (int i = 0; i < m; i++) {
		// if movie i has no ratings at all, set the ith column in M to all zeros
		if (movies_map.count(i) == 0) {
			M.col(i) = Eigen::VectorXd::Zero(k);
		}
	}
	Eigen::MatrixXd lambdaI = Eigen::MatrixXd::Identity(k,k);
	lambdaI = lambda*lambdaI;

	printf("Time to read in data and do pre-processing = %f\n", omp_get_wtime() - t);

	double rmse = get_rmse(R_test, U, M, n_ratings_test);
	printf("Starting with RMSE = %f\n", rmse);

	// Start ALS iterations:
	for (int iteration = 0; iteration < N_ITER; iteration++) {
//		printf("Starting iteration %d\n", iteration);
		t = omp_get_wtime();
		// Fix U constant and solve for M:
		#pragma omp parallel num_threads(n_threads) default(shared)
		{
			#pragma omp for
			for (int j = 0; j < movies_with_ratings.size(); j++) {
				int movie_index = movies_with_ratings[j];
				vector<int> users = movies_map[movie_index].first; // user rows in R with ratings for movie j
				Eigen::MatrixXd U_K_j(k, users.size());
				int i = 0;
				for (vector<int>::iterator it = users.begin(); it != users.end(); ++it) {
					int user_idx = *it;
					U_K_j.col(i) = U.col(user_idx);
					i++;
				}
				Eigen::VectorXd r_j_K = movies_map[movie_index].second;
				M.col(movie_index) = (U_K_j * U_K_j.transpose() + lambdaI).lu().solve(U_K_j*r_j_K);
			}
		}
		// Fix M constant and solve for U:
		#pragma omp parallel num_threads(n_threads) default(shared)
		{
			#pragma omp for
			for (int j = 0; j < users_with_ratings.size(); j++) {
				int user_index = users_with_ratings[j];
				vector<int> movies = users_map[user_index].first; // user rows in R with ratings for movie j
				Eigen::MatrixXd M_K_j(k, movies.size());
				int i = 0;
				for (vector<int>::iterator it = movies.begin(); it != movies.end(); ++it) {
					int movie_idx = *it;
					M_K_j.col(i) = M.col(movie_idx);
					i++;
				}
				Eigen::VectorXd r_j_K = users_map[user_index].second;
				U.col(user_index) = (M_K_j * M_K_j.transpose() + lambdaI).lu().solve(M_K_j*r_j_K);
			}
		}

		// Finished iteration; calculate walltime and RMSE:
		total_time += omp_get_wtime() - t;
		rmse = get_rmse(R_test, U, M, n_ratings_test);
		printf("Walltime through iteration %d: %f. ", iteration, total_time);
		printf("test RMSE = %f\n", rmse);
	}
}
