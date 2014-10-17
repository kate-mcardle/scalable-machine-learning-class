//============================================================================
// Name        : HW3.cpp
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
#include <algorithm>

// for file reading:
#include <fstream>
#include <cstring>

#define N_ITER 20
using namespace std;

int main(int argc, char *argv[]) {
	double t; // measures run time of segments
	double total_time = 0; // measures total run time for CD updates
	double C = atof(argv[1]);
	double halfC = 1/(2*C);
	int n_threads = atoi(argv[2]);
	char * training_data_path = argv[3];
	char * testing_data_path = argv[4];

	t = omp_get_wtime();

	// +++++++++++++++++++ Read in and pre-process testing data +++++++++++++++++++
	// First pass - find out number of testing points and max char length of any line
	ifstream testFile_n(testing_data_path);
	int n_test = 0;
	string line;
	int test_max_chars = 0;
	while (getline(testFile_n, line)) {
		++n_test;
		if (line.length() > test_max_chars) { test_max_chars = line.length(); }
	}
	testFile_n.close();

	// X[i][j].first = feature index for jth pair in x_i; X[i][j].second = corresponding value
	vector<vector<pair<int, double> > > X_test;
	X_test.reserve(n_test);
	vector<int> y_test (n_test, 0);

	// Second pass - store the data in X_test and y_test; keep track of max feature index in X
	ifstream testFile(testing_data_path);
	char l_test[test_max_chars];
	int data_idx = 0;
	int d = 0;
	int feature_idx;
	double value;
	while (testFile.getline(l_test, test_max_chars+1)) {
		char * pch;
		pch = strtok(l_test, " :");
		y_test[data_idx] = atoi(pch);
		// make temp vector for X_test[data_idx]:
		vector<pair<int, double> > temp_vec;

		int p = 0;
		pch = strtok(NULL, " :");
		while (pch != NULL) {
			if (p == 0) {
				feature_idx = atoi(pch);
				if (feature_idx > d) { d = feature_idx; }
				p = 1;
			}
			else {
				value = atof(pch);
				temp_vec.push_back(make_pair(feature_idx-1, value)); // have to make feature indices 0-based
				p = 0;
			}
			pch = strtok(NULL, " :");
		}
		X_test.push_back(temp_vec);
		data_idx++;
	}
	testFile.close();

	// +++++++++++++++++++ Read in and pre-process training data +++++++++++++++++++
	// First pass - find out number of training points and max char length of any line
	ifstream trainFile_n(training_data_path);
	int n_train = 0;
	int train_max_chars = 0;
	while (getline(trainFile_n, line)) {
		++n_train;
		if (line.length() > train_max_chars) { train_max_chars = line.length(); }
	}
	trainFile_n.close();

	vector<vector<pair<int, double> > > X_train;
	X_train.reserve(n_train);
	vector<int> y_train (n_train, 0);

	// Second pass - store the data in X_train and y_train; keep track of max feature index in X
	ifstream trainFile(training_data_path);
	char l_train[train_max_chars];
	data_idx = 0;
	while (trainFile.getline(l_train, train_max_chars+1)) {
		char * pch;
		pch = strtok(l_train, " :");
		y_train[data_idx] = atoi(pch);
		// make temp vector for X_train[data_idx]:
		vector<pair<int, double> > temp_vec;

		int p = 0;
		pch = strtok(NULL, " :");
		while (pch != NULL) {
			if (p == 0) {
				feature_idx = atoi(pch);
				if (feature_idx > d) { d = feature_idx; }
				p = 1;
			}
			else {
				value = atof(pch);
				temp_vec.push_back(make_pair(feature_idx-1, value)); // have to make feature indices 0-based
				p = 0;
			}
			pch = strtok(NULL, " :");
		}
		X_train.push_back(temp_vec);
		data_idx++;
	}
	trainFile.close();

	// Initialize alpha, w vectors to 0
	vector<double> alpha (n_train, 0);
	vector<double> w (d, 0);

	vector<int> pi;
	pi.reserve(n_train);
	for (int i = 0; i < n_train; i++) {
		pi.push_back(i);
	}

	printf("Time to read in data and do pre-processing = %f\n", omp_get_wtime() - t);

	// Start CD iterations
	for (int iteration = 0; iteration < N_ITER; iteration++) {
		printf("Starting iteration %d\n", iteration);
		t = omp_get_wtime();
		// Choose a random permutation pi of the n data points
		random_shuffle(pi.begin(), pi.end());

		#pragma omp parallel num_threads(n_threads) default(shared)
		{
			#pragma omp for
			for (int s = 0; s < n_train; s++) {
				int i = pi[s]; // i is ith data point in training set (from 0 to n-1)
				// calculate delta*:
				double delta;
				double alpha_i = alpha[i];
				double y_i = y_train[i];
				double x_iTw = 0;
				double Qii = 0;
				int f_idx;
				for (int j = 0; j < X_train[i].size(); j++) {
					f_idx = X_train[i][j].first;
					x_iTw += X_train[i][j].second * w[f_idx];
					Qii += X_train[i][j].second * X_train[i][j].second;
				}
				double new_alpha_i = alpha_i + (1-y_i*x_iTw - halfC*alpha_i)/(Qii+halfC);
				new_alpha_i = max(new_alpha_i, 0.0);
				delta = new_alpha_i - alpha_i;

				// update alpha_i:
				alpha[i] = new_alpha_i;

				double y_id = y_i*delta;

				if (y_id != 0) {
					for (int j = 0; j < X_train[i].size(); j++) {
						f_idx = X_train[i][j].first;
						#pragma omp atomic
						w[f_idx] += y_id * X_train[i][j].second;
					}
				}
			}
		}
		total_time += omp_get_wtime() - t;
		printf("Through iteration %d: ------------------------------------------------\n", iteration);
		// Part a: wall time for coordinate descent updates
		printf("Part a: Cumulative walltime for coordinate descent updates: %f\n", total_time);

		// Part b: compute and output the dual objective function value, using current alpha and w
		double f, w2norm, alpha2norm, alpha1norm;
		w2norm = 0;
		for (int i = 0; i < d; i++) {
			w2norm += w[i]*w[i];
		}
		alpha2norm = 0;
		alpha1norm = 0;
		double Xi_i;
		double hinge = 0;
		for (int i = 0; i < n_train; i++) {
			alpha2norm += alpha[i]*alpha[i];
			alpha1norm += alpha[i];
			double wTx_i = 0;
			for (int j = 0; j < X_train[i].size(); j++) {
				feature_idx = X_train[i][j].first;
				wTx_i += w[feature_idx]*X_train[i][j].second;
			}
			Xi_i = 1 - y_train[i]*wTx_i;
			if (Xi_i > 0) {
				hinge += Xi_i*Xi_i;
			}
		}
		f = 0.5*w2norm + 0.5*halfC*alpha2norm - alpha1norm;
		printf("Part b: Dual objective function value f(alpha) = %f\n", f);

		// Part c: compute and output the primal objective function value, using current w
		double g = 0.5*w2norm + C*hinge;
		printf("Part c: Primal objective function value g(w) = %f\n", g);

		// Part d: compute and output the prediction accuracy using current w
		int n_correct = 0;
		for (int i = 0; i < n_test; i++) {
			double wTx = 0;
			for (int j = 0; j < X_test[i].size(); j++) {
				feature_idx = X_test[i][j].first;
				wTx += w[feature_idx] * X_test[i][j].second;
			}
			int y_pred;
			if (wTx >= 0) {
				y_pred = 1;
			}
			else {
				y_pred = -1;
			}
			if (y_pred == y_test[i]) {
				++n_correct;
			}
		}
		double accuracy = (n_correct + 0.0) / n_test;
		printf("Part d: Accuracy on test set = %f\n", accuracy);

		// Part e: Compute and output norm of actual w minus stored w; should be 0 if w is correctly maintained
		vector<double> new_w (d, 0);
		for (int i = 0; i < n_train; i++) {
			for (int j = 0; j < X_train[i].size(); j++) {
				feature_idx = X_train[i][j].first;
				new_w[feature_idx] += y_train[i] * alpha[i] * X_train[i][j].second;
			}
		}
		double diff_norm = 0;
		for (int i = 0; i < d; i++) {
			diff_norm += (new_w[i] - w[i])*(new_w[i] - w[i]);
		}
		printf("Part e: Calculated value is %f\n", diff_norm);

		// Part g: Calculate relative error: (g(w) - g(w*))/g(w*)
//		double g_w_opt = 6310.587335;
		double g_w_opt = 33346.944104;
		double rel_error = (g - g_w_opt)/g_w_opt;
		printf("Part g: Relative error = %f\n", rel_error);
	}

}
