#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

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

int main(int argc, char ** argv) {
	return 0;
}
