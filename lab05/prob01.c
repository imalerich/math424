#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#define SIZE 3
#define MOD_RAND 10

// Forward declarations HO!
void print_sq_mat(float mat[][SIZE]);

/* ----------------------------------------------------
 * void mat_mul(float mat1[][SIZE], float mat1[][SIZE])
 * Performs matrix multiplication on square matrices.
 * ---------------------------------------------------- */

void mat_mul(float mat0[][SIZE], float mat1[][SIZE]) {
	float res[SIZE][SIZE];

	for (int i=0; i<SIZE; i++) {
		for (int j=0; j<SIZE; j++) {
			// take the dot product of row i in mat0
			// with column j of mat1
			float dot = 0;
			for (int k=0; k<SIZE; k++) {
				dot += mat0[i][k] * mat1[k][j];
			}

			res[i][j] = dot;
		}
	}

	print_sq_mat(res);
}

// Problem #01
int main(int argc, char ** argv) {
	// seed the random number generator using the system clock
	srand(time(NULL));

	float mat0[SIZE][SIZE];
	float mat1[SIZE][SIZE];

	// generate a random test case
	for (int i=0; i<SIZE; i++) {
		for (int j=0; j<SIZE; j++) {
			mat0[i][j] = rand() % MOD_RAND;
			mat1[i][j] = rand() % MOD_RAND;
	}}

	// print the results of matrix multiplication on tha test case
	printf("The first matrix is: \n");
	print_sq_mat(mat0);
	printf("\nThe second matrix is: \n");
	print_sq_mat(mat1);
	printf("\nThe product is: \n");
	mat_mul(mat0, mat1);

	return 0;
}

/* --------------------------------
 * Utility to print a square matrix
 * -------------------------------- */

void print_sq_mat(float mat[][SIZE]) {
	for (int i=0; i<SIZE; i++) {
		for (int j=0; j<SIZE; j++) {
			printf("%.4f\t", mat[i][j]);
		}

		printf("\n");
	}
}
