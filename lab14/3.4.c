#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

#define COUNT 8
#define ROOT 0
#define MAX_RAND 4

int sum(int * arr, int size) {
	int comm_sz, my_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// scatter our data to all other processes
	int count = size/comm_sz;
	int * rec = malloc(sizeof(int) * count);
	MPI_Scatter(arr, count, MPI_INT, rec, count, MPI_INT, 
			ROOT, MPI_COMM_WORLD);

	// compute the partial sum assigned to this process
	int psum = 0;
	for (int i=0; i<count; i++) {
		psum += rec[i];
	}
	free(rec);

	// now perform the butterfly algorithm
	// to add all partial sums together
	for (int n=1; n<comm_sz; n*=2) {
		int recv = 0;

		int partner = 0;
		float idx = my_rank / (float)n;
		if ((my_rank/n) % 2 == 0) {
			partner = (idx+1) * n;
		} else {
			partner = (idx-1) * n;
		}

		MPI_Send(&psum, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
		MPI_Recv(&recv, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, 0);
		
		psum += recv;
	}

	return psum;
}

int main(int argc, char ** argv) {
	srand(time(NULL));

	int my_rank;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int * arr = NULL;
	if (my_rank == ROOT) {
		// initialize a random array
		arr = malloc(sizeof(int) * COUNT);
		printf("[");
		for (int i=0; i<COUNT; i++) {
			arr[i] = rand() % MAX_RAND;
			printf("%d ", arr[i]);
		}
		printf("]\n");
	}

	int s = sum(arr, COUNT);

	// if you comment out this check
	// you will verify that all processes get the
	// same (and correct) result
	if (my_rank == ROOT) {
		printf("Sum = %d\n", s);
	}

	MPI_Finalize();
	return 0;
}
