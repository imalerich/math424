#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#define SIZE 3
#define COUNT 5 // how many non-zero elements the sparse matrix will have
#define MOD_RAND 9

// Forward declarations HO!
void print_sq_mat(float mat[][SIZE]);
void print_coo_sq_mat(int rows[], int cols[], float dat[], size_t count);
float get_coo_mat_val(int r, int c, int rows[], int cols[], float dat[], size_t count);

/* -------------------------------------------------------------------
 * void mat_mul(int rows0[], int cols0[], float dat0[], size_t count0,	
 * 		int rows1[], int cols1[], float dat1[], size_t count1)
 *
 * Holy parameters Batman!
 * Performs matrix multiplication on square matrices.
 * ------------------------------------------------------------------- */

void mat_mul(int rows0[], int cols0[], float dat0[], size_t count0,
			 int rows1[], int cols1[], float dat1[], size_t count1) {
	// we really don't know how bit this is going to be
	// and we haven't learned dynamic arrays yet
	// so I'm not using COO for the matrix I'm outputting
	float res[SIZE][SIZE];

	// I know it would be much faster and overall better
	// to just convert from COO to a regular 2 dimmensional array
	// but where is the fun in that, plus the whole point of COO
	// is to save memory, and we wouldn't save any memory that way
	for (int i=0; i<SIZE; i++) {
		for (int j=0; j<SIZE; j++) {
			// take the dot product of row i in mat0
			// with column j of mat1
			float dot = 0;
			for (int k=0; k<SIZE; k++) {
				dot += get_coo_mat_val(i, k, rows0, cols0, dat0, count0) *
					   get_coo_mat_val(k, j, rows1, cols1, dat1, count1);
			}

			res[i][j] = dot;
		}
	}

	print_sq_mat(res);
}

int main(int argc, char ** argv) {
	// seed the random number generator using the system clock
	srand(time(NULL));

	int rows0[COUNT] = { 0 };
	int cols0[COUNT] = { 0 };
	float dat0[COUNT] = { 0 };

	int rows1[COUNT] = { 0 };
	int cols1[COUNT] = { 0 };
	float dat1[COUNT] = { 0 };

	for (int i=0; i<COUNT; i++) {
		// it is technically possible for an index
		// to be repeated in the list multiple times
		// I am going to allow for this, as the random
		// assignment is just for testing
		// note that by implementation of get_coo_mat_val(...)
		// the first one found will be used
		// and all others will be ignored
		rows0[i] = rand() % SIZE;
		cols1[i] = rand() % SIZE;
		rows1[i] = rand() % SIZE;
		cols1[i] = rand() % SIZE;

		// get some random data
		// but don't include 0, cause everything 
		// else will already be 0 anyway
		dat0[i] = (rand() % MOD_RAND) + 1;
		dat1[i] = (rand() % MOD_RAND) + 1;
	}

	// print the results of matrix multiplication on tha test case
	printf("The first matrix is: \n");
	print_coo_sq_mat(rows0, cols0, dat0, COUNT);
	printf("\nThe second matrix is: \n");
	print_coo_sq_mat(rows1, cols1, dat1, COUNT);
	printf("\nThe product is: \n");
	mat_mul(rows0, cols0, dat0, COUNT,
			rows1, cols1, dat1, COUNT);

	return 0;
}

/* ------------------------------------
 * Utilities to print square matrices,
 * either in COO format, or normal
 * format.
 * ------------------------------------ */

void print_coo_sq_mat(int rows[], int cols[], float dat[], size_t count) {
	for (int i=0; i<SIZE; i++) {
		for (int j=0; j<SIZE; j++) {
			printf("%.4f\t", get_coo_mat_val(i, j, rows, cols, dat, count));
		}

		printf("\n");
	}
}

void print_sq_mat(float mat[][SIZE]) {
	for (int i=0; i<SIZE; i++) {
		for (int j=0; j<SIZE; j++) {
			printf("%.4f\t", mat[i][j]);
		}

		printf("\n");
	}
}

/* -----------------------------------
 * Slow as &$@# utility for finding
 * a particular value in a COO matrix.
 * May be slow as balls, but makes my 
 * work super easy.
 * ----------------------------------- */
float get_coo_mat_val(int r, int c, int rows[], int cols[], float dat[], size_t count) {
	// try to find the value we are looking for
	for (int i=0; i<count; i++) {
		if (rows[i] == r && cols[i] == c) {
			return dat[i];
		}
	}

	// else not a set value in the sparse matrix
	// therefore it must be 0
	return 0.0f;
}
