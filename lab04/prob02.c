#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#define SIZE 7

void find_max_sub(int mat[][SIZE]) {
	int max_i = -1, max_k = -1, max_size = 0;

	for (int i=0; i<SIZE; i++) {
	}

	if (max_i > 0) {
		printf("The maximum square matrix is at (%d, %d) with size %d.\n", 
			max_i, max_k, max_size);
	} else {
		printf("No maximum square matrix found.\n");
	}
}

void print_sq_mat(int mat[][SIZE]) {
	for (int i=0; i<SIZE; i++) {
		for (int k=0; k<SIZE; k++) {
			printf("%d ", mat[i][k]);
		}

		printf("\n");
	}
}

int main(int argc, char ** argv) {
	// seed the random number generator using the system clock
	srand(time(NULL));

	// generate a random 7x7 matrix made up of 1's and 0's
	int mat[SIZE][SIZE];
	for (int i=0; i<SIZE; i++) {
		for (int k=0; k<SIZE; k++) {
			mat[i][k] = rand() % 2;
	}}

	printf("Given matrix is: \n\n");
	print_sq_mat(mat);
	printf("\n");
	find_max_sub(mat);

	return 0;
}
