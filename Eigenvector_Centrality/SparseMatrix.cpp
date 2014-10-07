/*
 * SparseMatrix.cpp
 *
 *  Created on: Oct 3, 2014
 *      Author: katemcardle
 */

#include <omp.h>
#include "SparseMatrix.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
//using namespace std;

SparseMatrix::SparseMatrix() {
	// TODO Auto-generated constructor stub
	std::ifstream sparse_file("../B.txt");
	int row, col;
	double val;
	sparse_file >> n_rows >> n_cols >> nnz;
	while(sparse_file >> row >> col >> val) {
		std::cout << row << " " << col << " " << val << " " << std::endl;
	}
	sparse_file.close();
}

SparseMatrix::SparseMatrix(char *path) {
	std::ifstream sparse_file(path);
	int row, col;
	double val;
	sparse_file >> n_rows >> n_cols >> nnz;
	vals = (double*) malloc(nnz*sizeof(double));
	val_ptrs = (double**) malloc(n_rows*sizeof(double*));
	cols = (int*) malloc(nnz*sizeof(int));
	col_ptrs = (int**) malloc(n_rows*sizeof(int*));
	int i = 0;
	while(sparse_file >> row >> col >> val) {
//		std::cout << row-1 << " " << col-1 << " " << val << " " << std::endl;
		vals[i] = val;
		cols[i] = col-1; // have to decrement to be 0-based
		if (!val_ptrs[row-1]) {
			val_ptrs[row-1] = &vals[i];
			col_ptrs[row-1] = &cols[i];
		}
		i++;
	}
	sparse_file.close();
//	print_me();
}

int SparseMatrix::get_n_cols() {
	return n_cols;
}

double* SparseMatrix::multiply(double* x, int n_threads) {
	double* y;
	y = (double*) malloc(n_rows*sizeof(double*));
	#pragma omp parallel for num_threads(n_threads) default(shared)
	for (int row = 0; row < n_rows; ++row) {
		y[row] = 0;
		if (val_ptrs[row]) {
			int j = 0;
			// val_ptrs[row]+j != &vals[nnz] --> checks if the current address being assessed matches the address of the end of the vals array
			// val_ptrs[row]+j != val_ptrs[row+1] --> checks if the current address being assessed matches the address of the start of the next row
			while ( (val_ptrs[row]+j != &vals[nnz]) && (val_ptrs[row]+j != val_ptrs[row+1]) ) {
				y[row] += *(val_ptrs[row]+j) * x[*(col_ptrs[row]+j)];
				++j;
			}
		}
	}
	return y;
}

void SparseMatrix::print_me() {
	std::cout << n_rows << n_cols << nnz << std::endl;
//	int i;
//	std::cout << "Testing data members" << std::endl;
//	std::cout << "vals: ";
//	for (i = 0; i < nnz; i++) {
//		std::cout << vals[i] << " ";
//	}
//	std::cout << std::endl << "cols: ";
//	for (i = 0; i < nnz; i++) {
//		std::cout << cols[i] << " ";
//	}
	std::cout << std::endl << "values that val_ptrs are pointing to: ";
	for (int row = 0; row < n_rows; row++) {
		if (!val_ptrs[row]) { std::cout << "row" << row << " is all zeros"; }
		else {
			std::cout << *val_ptrs[row] << " ";
		}
	}
	std::cout << std::endl << "values that col_ptrs are pointing to: ";
	for (int row = 0; row < n_rows; row++) {
		if (!col_ptrs[row]) { std::cout << "row" << row << " is all zeros"; }
		else {
			std::cout << *col_ptrs[row] << " ";
		}
	}
	for (int row = 0; row < n_rows; row++) {
		std::cout << std::endl << "Non-zeros in row " << row << ": ";
		if (!val_ptrs[row]) {
			std::cout << "row" << row << " is all zeros" << std::endl;
			continue;
		}
		int j = 0;
		// val_ptrs[row]+j != &vals[nnz] --> checks if the current address being assessed matches the address of the end of the vals array
		// val_ptrs[row]+j != val_ptrs[row+1] --> checks if the current address being assessed matches the address of the start of the next row
		while ( (val_ptrs[row]+j != &vals[nnz]) && (val_ptrs[row]+j != val_ptrs[row+1]) ) {
			std::cout << *(col_ptrs[row]+j) << ": " << *(val_ptrs[row]+j) << " ";
			++j;
		}
	}
	std::cout << std::endl;
}

SparseMatrix::~SparseMatrix() {
	free(vals);
	free(val_ptrs);
	free(cols);
	free(col_ptrs);
}

