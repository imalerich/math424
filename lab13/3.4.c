#include <stdio.h>
#include <mpi.h>

int main(int argv, char ** argc) {
	int my_rank, comm_sz, recv;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// wait for the previous process to send a message before printing
	// the previous process will send it's message once it has finished
	// printing, the initial process my_rank = 0 does not have to wait
	if (my_rank > 0) {
		MPI_Recv(&recv, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	printf("Proc %d of %d > Does anyone have a toothpick?\n", 
		my_rank, comm_sz);

	// done printing, send message to next process and let it print
	if (my_rank < comm_sz-1) {
		MPI_Send(&my_rank, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
