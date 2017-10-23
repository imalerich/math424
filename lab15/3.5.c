#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

#define ROOT 0
#define MAX_RAND 10

/*
 * @file 3.3
 * @author Ian Malerich
 *
 * Implement a matrix-vector multiplication using 
 * block-column distribution of the matrix. You can have process 0 
 * read in the matrix and simply use a loop of sends to distribute it
 * among the processes. Assume the matrix is a square of order n
 * and that n is evenly divisible by comm_sz. You may want to look at 
 * the MPI function MPI_Reduce_scatter.
 */

void print_mat(float * mat, int n);
void print_vec(float * vec, int n);

// only ROOT needs to input the correct mat & v
// mat is assumed to be in row order
// only ROOT will receive a buffer containing the result
// this buffer will need to be free'd by ROOT only
// all other processes will receive a NULL result
float * mult_mat_vec(float * mat, float * v, int n) {
	int comm_sz, my_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// broadcast the size and data for the matrix and vector to all processes
	MPI_Bcast(&n, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

	// each process will receive count vectors of size n
	// they well then multiply this by a corresponding element of v
	int count = n/comm_sz;

	// contiguously stores each 'count' column vectors of length 'n' for this process
	float * c_buffer = malloc(sizeof(float) * count * n);
	// scalar elements of v to multiply with each column vector
	float * v_buffer = malloc(sizeof(float) * count);

	// 'ROOT' sends all matrix data, tagging by row
	if (my_rank == ROOT) {
		for (int r=0; r<n; r++) {
			for (int c=0; c<n; c++) {
				float val = mat[r*n + c];
				// column determines which process to send to
				MPI_Send(&val, 1, MPI_FLOAT, c%comm_sz, (r*n + c), MPI_COMM_WORLD);
			}
		}
	}

	// we will received count columns of data
	for (int k=0; k<count; k++) {
		int c = my_rank + k * comm_sz;
		for (int r=0; r<n; r++) {
			MPI_Recv(&c_buffer[k*n + r], 1, MPI_FLOAT, ROOT, (r*n + c), MPI_COMM_WORLD, 0);
		}
	}

	// next 'ROOT' sends vector data, tagging by element
	if (my_rank == ROOT) {
		for (int i=0; i<n; i++) {
			float val = v[i];
			MPI_Send(&val, 1, MPI_FLOAT, i%comm_sz, i, MPI_COMM_WORLD);
		}
	}

	// we will receive count vector values
	for (int k=0; k<count; k++) {
		MPI_Recv(&v_buffer[k], 1, MPI_FLOAT, ROOT, (my_rank + k*comm_sz), MPI_COMM_WORLD, 0);
	}

	// reduce our 'count' columns in this process to 1 column 
	// by adding each partial result together
	// we will also multiply by the corresponding value in the vector
	float * p_res = malloc(sizeof(float) * n);
	for (int i=0; i<n; i++) { p_res[i] = 0; }
	for (int i=0; i<count*n; i++) {
		p_res[i % n] += (c_buffer[i] * v_buffer[i/n]);
	}

	// next all we need to do is perform a global sum
	// on each value of p_res, then we will be left with 
	// the vector produced by matrix multiplication
	for (int i=0; i<n; i++) {
		float tmp;
		MPI_Reduce(&p_res[i], &tmp, 1, MPI_FLOAT, MPI_SUM, ROOT, MPI_COMM_WORLD);
		if (my_rank == ROOT) { p_res[i] = tmp; }
	}

	free(c_buffer);
	free(v_buffer);
	// p_res @ ROOT contains the result, free all other memory
	if (my_rank != ROOT) { free(p_res); p_res = NULL; }
	return p_res;
}

int main(int argc, char ** argv) {
	srand(time(NULL));
	MPI_Init(NULL, NULL);

	int comm_sz, my_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int n; // matrix will be nxn, n % comm_sz = 0
	float * mat, * vec;

	if (my_rank == ROOT) {
		printf("Enter Matrix Size (multiple of %d): ", comm_sz);
		fflush(stdout);
		scanf("%d", &n);

		// bad user!
		if (n % comm_sz != 0) {
			n -= n % comm_sz;
			printf("Invalid n - Correcting To %d.\n", n);
		}
	
		// generate random data for the matrix
		mat = malloc(sizeof(float) * n * n);
		for (int i=0; i<n*n; i++) {
			mat[i] = rand() % MAX_RAND;
		}

		// generate random data for the vector
		vec = malloc(sizeof(float) * n);
		for (int i=0; i<n; i++) {
			vec[i] = rand() % MAX_RAND;
		}

		printf("Generated random matrix of size %dx%d\n", n, n);
		print_mat(mat, n);
		printf("Generated random vector of size 1x%d\n", n);
		print_vec(vec, n);
		fflush(stdout);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	float * res = mult_mat_vec(mat, vec, n);
	if (my_rank == ROOT) {
		printf("-------------------------------------------------\n");
		printf("mat x vec = ");
		print_vec(res, n);
		free(res);
	}

	MPI_Finalize();
	return 0;
}

void print_mat(float * mat, int n) {
	for (int x=0; x<n; x++) {
		printf("| ");
		for (int y=0; y<n; y++) {
			printf("%f ", mat[x*n + y]);
		}

		printf(" |\n");
	}
}

void print_vec(float * vec, int n) {
	printf("[ ");
	for (int x=0; x<n; x++) {
		printf("%f ", vec[x]);
	}

	printf("]\n");
}
