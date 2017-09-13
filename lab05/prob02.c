#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#define SIZE 3
#define MOD_RAND 6
#define ERR "Error - Singular Matrix!\n"

// I know you haven't tought us macro's
// (or the ternary operator)
// but the function is trivial and this is 
// so much more compact
#define MIN(a, b) (a < b) ? a : b
#define MAX(a, b) (a < b) ? b : a

void print_sq_mat(float mat[][SIZE]);
float min_det(float mat[][SIZE], int I, int J);

/**
 * \fn int det(int mat[][SIZE])
 * Computes the inverse of a 1x1, 2x2, or 3x3 matrix.
 */
float det(float mat[][SIZE]) {
	switch (SIZE) {
	case 1:
		return mat[0][0];

	case 2:
		return (mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]);

	case 3:
		return mat[0][0] * min_det(mat, 0, 0) 
			 - mat[0][1] * min_det(mat, 0, 1) 
			 + mat[0][2] * min_det(mat, 0, 2);

	default:
		// not supported
		return 0;
	}
}

// computes the deteriminent of the minor matrix
// for the given location
float min_det(float mat[][SIZE], int I, int J) {
	// assume matrix is of size 3
	if (SIZE != 3) { return 0; }

	// here are the indecies we need to consider
	int i[] = {(I+1) % 3, (I+2) % 3};
	int j[] = {(J+1) % 3, (J+2) % 3};

	// now compute the determinant
	return
		(mat[ MIN(i[0], i[1]) ][ MIN(j[0], j[1]) ]  * 
		 mat[ MAX(i[0], i[1]) ][ MAX(j[0], j[1]) ]) -
		(mat[ MIN(i[0], i[1]) ][ MAX(j[0], j[1]) ]  * 
		 mat[ MAX(i[0], i[1]) ][ MIN(j[0], j[1]) ]);
}

/* -------------------------------------
 * void mat_inv(float mat[][SIZE])
 * Takes the inverse of a square matrix.
 * Supported sizes are 1x1, 2x2, 3x3.
 * ------------------------------------- */

void mat_inv(float mat[][SIZE]) {
	float inv[SIZE][SIZE];
	float d = det(mat);

	// make sure we have a non-singular matrix
	// i.e. one that we can actually take the inverse of
	if (d == 0) { 
		printf(ERR); 
		return; 
	}

	switch (SIZE) {
	case 1:
		inv[0][0] = 1 / mat[0][0];
		break;

	case 2:
		inv[0][0] = mat[1][1]  / d;
		inv[0][1] = -mat[0][1] / d;
		inv[1][0] = -mat[1][0] / d;
		inv[1][1] = mat[0][0]  / d;
		break;

	case 3:
		for (int i=0; i<SIZE; i++) {
			for (int j=0; j<SIZE; j++) {
				// creates a grid of +1, -1 signs
				// starting with +1 in the top left corner
				int sign = -2 * (((i*SIZE) + j) % 2) + 1;

				// use (j,i) rather than (i,j)
				// to find the minor determinents
				// of the transpose matrix
				inv[i][j] = sign * min_det(mat, j, i) / d;
			}
		}

		break;

	default:
		printf("Matrix Inverse not support on SIZE: %d\n", SIZE);
		break;
	}

	print_sq_mat(inv);
}

// Problem #02
int main(int argc, char ** argv) {
	// seed the random number generator using the system clock
	srand(time(NULL));

	float mat[SIZE][SIZE];

	// generate a random test case
	for (int i=0; i<SIZE; i++) {
		for (int j=0; j<SIZE; j++) {
			mat[i][j] = rand() % MOD_RAND;
	}}
	
	// print the results of matrix multiplication on tha test case
	printf("The matrix is: \n");
	print_sq_mat(mat);
	printf("\nIt's inverse is: \n");
	mat_inv(mat);

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
