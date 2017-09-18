#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#define SIZE 3
#define MOD_RAND 4

void print_vec(int * vec, size_t count);
void print_mat(int mat[][SIZE]);

int main(int argc, char ** argv) {
	// seed the random number generator using the system clock
	srand(time(NULL));

	int A[SIZE][SIZE] = { 0 };
	int v[SIZE] = { 0 };

	// the number of non-zero elements of A
	size_t NNZ = 0;

	// initialize the matrix and vector with random values
	for (int i=0; i<SIZE; i++) {
		v[i] = rand() % MOD_RAND;
		for (int j=0; j<SIZE; j++) {
			A[i][j] = rand() % MOD_RAND;
			// count the number of nonzero elements in A
			if (A[i][j] != 0) { NNZ++; }
		}
	}

	// we've covered pointers, but I don't think we've covered malloc
	// if not, the below dynamic arrays could of length (SIZExSIZE)
	// then terminated by a 0 in AV (since AV is non-zero values)
	// similar to how a (char *) as a string is handled
	// If you wanted a terminating AC value, -1 would work, as that is 
	// not a valid index to a column
	// of course, this would not save on any memory over using just A

	// non-zero values of A
	int * AV = malloc(sizeof(int) * NNZ);
	// row storage of A
	int AR[SIZE+1];
	// columns corresponding to the non-zero values of A
	int * AC = malloc(sizeof(int) * NNZ);

	// create the CSR matrix representation
	AR[0] = 0;
	int idx = 0; // index into AC/AV vectors
	for (int i=0; i<SIZE; i++) {
		for (int j=0; j<SIZE; j++) {
			if (A[i][j] != 0) {
				AV[idx] = A[i][j];
				AC[idx] = j;
				idx++;
			}
		}

		AR[i+1] = idx;
	}

	// perform matrix * vector multiplication using the CSR representation
	int p[SIZE] = { 0 };
	for (int i=0; i<SIZE; i++) {
		for (int j=AR[i]; j < AR[i+1]; j++) {
			p[i] = p[i] + AV[j] * v[AC[j]];
		}
	}

	printf("---------------------------------------\n");
	// print out the initial values
	printf("A:\n");
	print_mat(A);
	printf("---------------------------------------\n");
	// now print the results of the computation
	printf("A: (CSR Format)\n");
	print_vec(AV, NNZ);
	print_vec(AR, SIZE+1);
	print_vec(AC, NNZ);
	printf("---------------------------------------\n");
	printf("v: ");
	print_vec(v, SIZE);
	printf("---------------------------------------\n");
	printf("A * v: ");
	print_vec(p, SIZE);
	printf("---------------------------------------\n");

	// cleanup, and return
	free(AV); AV = 0;
	free(AC); AC = 0;
	return 0;
}

void print_vec(int * vec, size_t count) {
	printf("[");
	for (int i=0; i<count; i++) {
		printf("%d", vec[i]);
		if (i < count-1) { printf(", "); }
	}

	printf("]\n");
}

void print_mat(int mat[][SIZE]) {
	for (int i=0; i<SIZE; i++) {
		for (int j=0; j<SIZE; j++) {
			printf("%d ", mat[i][j]);
		}

		printf("\n");
	}
}
