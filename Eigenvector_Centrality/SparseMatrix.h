/*
 * SparseMatrix.h
 *
 *  Created on: Oct 3, 2014
 *      Author: katemcardle
 */

#ifndef SPARSEMATRIX_H_
#define SPARSEMATRIX_H_

class SparseMatrix {
	int n_rows, n_cols, nnz;
	double *vals, **val_ptrs;
	int *cols, **col_ptrs;
	void print_me(); // for debugging

public:
	SparseMatrix();
	virtual ~SparseMatrix();
	SparseMatrix(char *path);
	int get_n_cols();
	double* multiply(double* x, int n_threads = 1);
};

#endif /* SPARSEMATRIX_H_ */
