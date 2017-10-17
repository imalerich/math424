#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define COUNT 8
#define ROOT 0
#define MAX_RAND 4

int sum(int * arr, int size) {
	int comm_sz, my_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int S = comm_sz * ceil(size/(float)comm_sz);
	if (arr != NULL) {
		// reallocate arr, and fill in some extra 0's
		// that way we can evenly scatter it across all processes
		arr = realloc(arr, S * sizeof(int));
		for (int i=size; i<S; i++) {
			arr[i] = 0;
		}
	}

	int count = S / comm_sz;
	int * rec = malloc(sizeof(int) * count);
	MPI_Scatter(arr, count, MPI_INT, rec, count, MPI_INT, 
			ROOT, MPI_COMM_WORLD);

	// add all of our elements together
	int psum = 0;
	for (int i=0; i<count; i++) {
		psum += rec[i];
	}
	free(rec);

	// if comm_sz is not a power of 2, the code below will break
	// find the next largest power of 2, if a process should
	// receive from a power of 2 which does not exist, it simply
	// will do nothing for that iteration
	
	int CS2 = (int)ceil((float)log2(comm_sz));
	CS2 = pow(2, CS2);

	for (int R = CS2 / 2; R > 0; R /= 2) {
		if (my_rank < R) {
			// if the process we should wait for exists
			if (my_rank+R < comm_sz) {
				int recv = 0;
				MPI_Recv(&recv, 1, MPI_INT, my_rank+R, 0, MPI_COMM_WORLD, 0);
				psum += recv;
			} // else we have do nothing

		} else {
 			MPI_Send(&psum, 1, MPI_INT, my_rank - R, 0, MPI_COMM_WORLD);
 			return psum;
		}
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
	if (my_rank == ROOT) {
		printf("Sum = %d\n", s);
	}

	MPI_Finalize();
	return 0;
}
