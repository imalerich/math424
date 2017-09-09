#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#define SIZE 7

int MIN(int i, int j) {
	if (i < j) { return i; }
	return j;
}

void print_sq_mat(int mat[][SIZE]) {
	for (int i=0; i<SIZE; i++) {
		for (int j=0; j<SIZE; j++) {
			printf("%d ", mat[i][j]);
		}

		printf("\n");
	}
}

void find_max_sub(int mat[][SIZE]) {
	// each coordinate of this matrix represents the largest submatrix
	// for which that coordinate is the lower right value
	int subs[SIZE][SIZE] = {0};

	// thus the first row and maintain their initial value of 0 or 1
	// so copy them over here
	for (int i=0; i<SIZE; i++) { subs[i][0] = mat[i][0]; }
	for (int j=0; j<SIZE; j++) { subs[0][j] = mat[0][j]; }

	for (int i=1; i<SIZE; i++) {
		for (int j=1; j<SIZE; j++) {
			if (mat[i][j] == 0) {
				// the matrix value is 0
				// thus, this value is not a submatrix we should consider
				subs[i][j] = 0;

			} else { // mat[i][j] == 1
				// check our three adjacent neighbers 
				// (that we have already looked at)
				// this 1 extends the minimum existing submatrix
				// for example, if we have 0 above us, we cannot use
				// any of those neighbors in a submatrix (so we are only size 0+1=1)
				// if we have neigbors {2,2,1}, then this index has a maximum
				// submatrix of size 2 = min(2,2,1)+1
				subs[i][j] = MIN(MIN(subs[i-1][j-1], subs[i-1][j]), subs[i][j-1]) + 1;
			}
		}
	}

	// now that we know the sizes of each submatrix
	// loop through our temp array and finding the largest submatrix 
	// that we can
	int max_i = -1, max_j = -1, max_size = 0;
	for (int i=0; i<SIZE; i++) {
		for (int j=0; j<SIZE; j++) {
			if (subs[i][j] > max_size) {
				max_i = i;
				max_j = j;
				max_size = subs[i][j];
			}
		}
	}

	// if we found a submatrix, print out its location
	if (max_i >= 0) {
		// offset our maximum indecies as these point to the lower right hand corner
		// and we would like to know the upper left
	 	printf("The maximum square matrix is at (%d, %d) with size %d.\n", 
	 		max_i - (max_size-1), max_j - (max_size-1), max_size);
	} else {
		printf("No maximum square matrix found.\n");
	}
}

int main(int argc, char ** argv) {
	// seed the random number generator using the system clock
	srand(time(NULL));

	// given example matrix of size 7x7
	int mat[SIZE][SIZE] = {
		{1, 0, 1, 0, 1, 0, 0},
		{1, 1, 1, 0, 1, 0, 0},
		{1, 0, 1, 1, 1, 0, 0},
		{1, 0, 1, 1, 1, 0, 0},
		{1, 0, 1, 1, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0}
	};

	// ... or use the folliwng to randomize it
	
	// for (int i=0; i<SIZE; i++) {
	// 	for (int j=0; j<SIZE; j++) {
	// 		mat[i][j] = rand() % 2;
	// }}

	printf("Given matrix is: \n\n");
	print_sq_mat(mat);
	printf("\n");
	find_max_sub(mat);

	return 0;
}
