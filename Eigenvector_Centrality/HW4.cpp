//============================================================================
// Name        : HW4.cpp
// Author      : Kathryn McArdle
//============================================================================

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "SparseMatrix.h"
using namespace std;

#define T 50

void part_b(SparseMatrix* s_ptr) {
	double t;
	int n_cols = s_ptr->get_n_cols();

	// Perform sparse matrix-vector multiplication:
	int threads[4] = {1, 4, 8, 16};
	for (int i = 0; i < 4; i++) {
		double total_time = 0;
		for (int rep = 0; rep < 10; ++rep) {
			// Create random vector x
			double x[n_cols];
			for (int j = 0; j < n_cols; ++j) {
				x[j] = rand() % 10;
			}

			double* y;
			t = omp_get_wtime();
			y = s_ptr->multiply(x, threads[i]);
			t = omp_get_wtime() - t;
			total_time += t;
			free(y);
		}
		double ave_time = total_time/10;
		printf("Average time taken to calculate y = A*x for %d threads: %f\n", threads[i], ave_time);
	}

}

double get_norm(double* x, int len) {
	double norm = 0;
	for (int i = 0; i < len; ++i) {
		norm += x[i]*x[i];
	}
	return sqrt(norm);
}

bool sort_pair(const pair<double, int>& l, const pair<double, int>& r) {
	return (l.first > r.first);
}

int main(int argc, char *argv[]) {
	double t;
	// Load matrix A from file
	t = omp_get_wtime();
	char * path_to_sparse = argv[1];
	SparseMatrix A(path_to_sparse);
	t = omp_get_wtime() - t;
	cout << "Time taken to read matrix A = " << t << endl;

	// -------------- Problem 2, part b ----------------------------------
	SparseMatrix* s_ptr = &A;
	part_b(s_ptr);

	// -------------- Problem 3, part 1 ----------------------------------
	t = omp_get_wtime();
	int n_cols = A.get_n_cols();
	int n_threads = 16;
	// Steps 1 and 2: initialize x
	double* x;
	x = (double*) malloc(n_cols*sizeof(double*));
	double x0 = 1/sqrt(n_cols);
	for (int j = 0; j < n_cols; ++j) {
		x[j] = x0;
	}
	// Step 3:
	for (int t = 0; t < T; ++t) {
		double* y;
		y = A.multiply(x, n_threads);
		free(x);
		x = y;
		double norm = get_norm(x, n_cols);
		for (int j = 0; j < n_cols; ++j) {
			x[j] /= norm;
		}
	}
	t = omp_get_wtime() - t;
	printf("Time to compute Eigenvector centrality: %f\n", t);

	// -------------- Problem 3, part 2 ----------------------------------
	// Converged eigenvalue:
	double* y;
	y = A.multiply(x, n_threads);
	double eig = 0;
	for (int i = 0; i < n_cols; i++) {
		eig += x[i]*y[i];
	}
	printf("Converged eigenvalue = %f\n", eig);
	free(y);

	// -------------- Problem 3, part 3 ----------------------------------
	// Top 100 nodes of A with highest Eigenvector centrality measure:
	vector<pair<double, int> > nodes;
	nodes.reserve(n_cols);
	for (int i = 0; i < n_cols; i++) {
		nodes.push_back(make_pair(x[i], i));
	}
	sort(nodes.begin(), nodes.end(), sort_pair);
	printf("---------------------\n");
	printf("| Rank | Node Index |\n");
	printf("|------|------------|\n");
	int rank = 1;
	for (vector<pair<double, int> >::iterator it = nodes.begin(); it != nodes.begin()+100; ++it) {
		pair<double, int> p = *it;
		printf("| %*d  | %*d    |\n", 3, rank, 7, p.second);
		++rank;
	}
	printf("---------------------\n");
	free(x);
}
