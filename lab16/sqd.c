#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define ROOT 0
#define MOD_RAND 10

#define MAX(a,b) (a > b) ? a : b
#define MIN(a,b) (a < b) ? a : b

/**
 * @file 3.13
 * @author Ian Malerich
 *
 * Read the problem description from square-chicken-dance.pdf.
 * 1. Assume that you will always have comm_sz=16.
 * 2. Let your rank 0 MPI process generate random 16 distinct integer numbers
 * and distribute each number to each of the 16 processes.
 * After that all 16 processes will interact to sort the numbers.
 * Collect all 16 integers in rank 0 MPI process and  let it print the sorted array.
 *
 * Now assume that your comm_sz=n*n, where in as an even number and n>2.
 * Update your program as needed. Submit your solution MPI program.
 */

int main(int argc, char ** argv) {
	srand(time(NULL));
	MPI_Init(NULL, NULL);

	int comm_sz, my_rank, n;
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	n = sqrt(comm_sz); // comm_sz = n*n, n is even and > 2

	// generate a random value for each process
	int myval = 0;
	if (my_rank == ROOT) {
		// send random values to each other process
		for (int i=1; i<comm_sz; i++) {
			myval = rand() % MOD_RAND;
			MPI_Send(&myval, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}

		// generate random value for process 0, don't need to send it to self
		myval = rand() % MOD_RAND;
	} else {
		MPI_Recv(&myval, 1, MPI_INT, ROOT, 0, MPI_COMM_WORLD, 0);
	}

	int row = my_rank / n;

	for (int i=0; i<n*n; i++) {
		int tick = i%2;

		// swap by row
		if ((my_rank + tick)%2 == 0) {
			int neighbor = my_rank+1; // neigbor rank
			int nval; // neighbor value

			// no neighbor, do nothing
			if (neighbor >= n+(row*n)) { continue; }

			MPI_Send(&myval, 1, MPI_INT, neighbor, 0, MPI_COMM_WORLD);
			MPI_Recv(&nval, 1, MPI_INT, neighbor, 0, MPI_COMM_WORLD, 0);
			myval = (row%2 == 0) ? MIN(myval, nval) : MAX(myval, nval);
		} else {
			int neighbor = my_rank-1; // neighbor index
			int nval; // neighbor value

			// no neighbor, do nothing
			if (neighbor < row*n) { continue; }

			MPI_Recv(&nval, 1, MPI_INT, neighbor, 0, MPI_COMM_WORLD, 0);
			MPI_Send(&myval, 1, MPI_INT, neighbor, 0, MPI_COMM_WORLD);
			myval = (row%2 == 0) ? MAX(myval, nval) : MIN(myval, nval);
		}

		// swap by column
		if ((row+tick)%2 == 0) {
			int neighbor = my_rank + n;
			int nval;

			if (neighbor >= comm_sz) { continue; }

			MPI_Send(&myval, 1, MPI_INT, neighbor, 0, MPI_COMM_WORLD);
			MPI_Recv(&nval, 1, MPI_INT, neighbor, 0, MPI_COMM_WORLD, 0);
			myval = MIN(myval, nval);
		} else {
			int neighbor = my_rank - n;
			int nval;

			if (neighbor < 0) { continue; }

			MPI_Recv(&nval, 1, MPI_INT, neighbor, 0, MPI_COMM_WORLD, 0);
			MPI_Send(&myval, 1, MPI_INT, neighbor, 0, MPI_COMM_WORLD);
			myval = MAX(myval, nval);
		}
	}

	// TODO - make these vectors and look nicer, all stored in one array on ROOT
	if (my_rank != ROOT) {
		MPI_Send(&myval, 1, MPI_INT, ROOT, 0, MPI_COMM_WORLD);
	} else {
		printf("-------\n");
		printf("%d ", myval);
		for (int r=0; r<n; r++) {
			for (int c=0; c<n; c++) {
				if (r == 0 && c == 0) { continue; }

				int i = r*n+c, tmp;
				MPI_Recv(&tmp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, 0);
				printf("%d ", tmp);
			}

			printf("\n");
		}
	}

	MPI_Finalize();
	return 0;
}
