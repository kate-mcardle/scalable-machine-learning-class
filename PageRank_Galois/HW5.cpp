//============================================================================
// Name        : HW5.cpp
// Author      : Kathryn McArdle
//============================================================================

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "Galois/Galois.h"
#include "Galois/Graph/LCGraph.h"
#include "Galois/Statistic.h"

using namespace std;

#define T 50
#define ALPHA 0.15
#define N_THREADS 16

typedef Galois::Graph::LC_CSR_Graph<double, double> Graph;

struct P {
	P(double* x, double* y, Graph* g_ptr) { this->x = x; this->y = y; this->g_ptr = g_ptr; }
	void operator()(const Graph::GraphNode& n) {
		y[n] = 0;
		for (auto edge : g_ptr->out_edges(n)) {
			Graph::GraphNode dest = g_ptr->getEdgeDst(edge);
			double edgeVal = g_ptr->getEdgeData(edge);
			y[n] += x[dest]*edgeVal;
		}
	}

	private:
		double* x;
		double* y;
		Graph* g_ptr;
};

double* multiply(Graph* g_ptr, double* x) {
	int n_nodes = g_ptr->size();
	double* y;
	y = (double*) malloc(n_nodes*sizeof(double*));
	Galois::do_all(g_ptr->begin(), g_ptr->end(), P(x, y, g_ptr), Galois::loopname("mult"));
	return y;
}

void evaluate_multiplication(Graph* g_ptr) {
//	double t;
	int n_nodes = g_ptr->size();

	// Perform sparse matrix-vector multiplication:
	int threads[4] = {1, 4, 8, 16};
	for (int i = 0; i < 4; i++) {
		Galois::setActiveThreads(threads[i]);
		printf("With %d Threads:\n", threads[i]);
		for (int rep = 0; rep < 10; ++rep) {
			printf("Rep %d:\n", rep);

			// Create random vector x
			double x[n_nodes];
			for (int j = 0; j < n_nodes; ++j) {
				x[j] = rand() % 10;
			}

			double* y;
			Galois::StatManager sm;
			char t[3];
			sprintf(t, "%d", threads[i]);
			Galois::StatTimer multTime(t);
			multTime.start();
			y = multiply(g_ptr, x);
			multTime.stop();
			free(y);
		}
	}
}

struct P_vec {
	P_vec(double* d, double* r, double* y, Graph* g_ptr) { this->d = d; this->r = r; this->y = y; this->g_ptr = g_ptr; }
	void operator()(const Graph::GraphNode& n) {
		y[n] = d[n]*r[n];
	}

	private:
		double* d;
		double* r;
		double* y;
		Graph* g_ptr;
};

double* multiply_vec(Graph* g_ptr, double* d, double* r) {
	int n_nodes = g_ptr->size();
	double* y;
	y = (double*) malloc(n_nodes*sizeof(double*));
	Galois::do_all(g_ptr->begin(), g_ptr->end(), P_vec(d, r, y, g_ptr));
	return y;
}

struct P_add {
	P_add(double* Adr, double vr, double* y, Graph* g_ptr) { this->Adr = Adr; this->vr = vr; this->y = y; this->g_ptr = g_ptr; }
	void operator()(const Graph::GraphNode& n) {
		y[n] = Adr[n] + vr;
	}

	private:
		double* Adr;
		double vr;
		double* y;
		Graph* g_ptr;
};

double* add_vecs(Graph* g_ptr, double* Adr, double vr) {
	int n_nodes = g_ptr->size();
	double* y;
	y = (double*) malloc(n_nodes*sizeof(double*));
	Galois::do_all(g_ptr->begin(), g_ptr->end(), P_add(Adr, vr, y, g_ptr));
	return y;
}

int main(int argc, char *argv[]) {
	char * path_to_sparse = argv[1];
	Graph g;
	Galois::Graph::readGraph(g, path_to_sparse);
	int n_nodes = g.size();
	printf("number of nodes = %d\n", n_nodes);

	// -------------- Problem 2, part b ----------------------------------
	Graph* g_ptr = &g;
//	evaluate_multiplication(g_ptr);

	// -------------- Problem 3, part b ----------------------------------
	Galois::setActiveThreads(N_THREADS);
	Galois::StatManager sm_pagerank;
	Galois::StatTimer totalTime;
	totalTime.start();
	// Step 0: Calculate (1-alpha)*D_inv (store as vector d):
	double d[n_nodes];
	for(auto n : g) {
		d[n] = 0;
		for (auto edge : g.out_edges(n)) {
			d[n] += g.getEdgeData(edge);
		}
		d[n] = (1-ALPHA)/d[n];
	}

	// Step 1: initialize r:
	double* r;
	r = (double*) malloc(n_nodes*sizeof(double*));
	double r0 = 1/n_nodes;
	for (int i = 0; i < n_nodes; ++i) {
		r[i] = r0;
	}

	// Step 2: iterations
	for (int t = 0; t < T; ++t) {
		// Calculate dr = (1-alpha)*D_inv*r_t:
		double* dr;
		dr = multiply_vec(g_ptr, d, r);
		// Multiply A*dr:
		double* Adr;
		Adr = multiply(g_ptr, dr);
		free(dr);

		// Get sum of r vector, multiply by alpha/n_nodes
		double sum = 0;
		for (int i = 0; i < n_nodes; ++i) {
			sum += r[i];
		}
		double vr = sum*ALPHA/n_nodes;

		// Add vr to every elem in Adr:
		double* r_new;
		r_new = add_vecs(g_ptr, Adr, vr);
		free(Adr);
		free(r);
		r = r_new;
	}
	totalTime.stop();

	// -------------- Problem 3, part b ----------------------------------



	free(r);
}
