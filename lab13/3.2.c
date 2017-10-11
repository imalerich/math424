#include <stdio.h>
#include <string.h>
#include <mpi.h>

double f(double x) {
	return x*x + x + 3;
}

double Trap(double left, double right, int c, double base) {
	double estimate, x;

	estimate = (f(left) + f(right))/2.0;
	for (int i=1; i<c; i++) {
		x = left + i * base;
		estimate += f(x);
	}

	return (estimate * base);
}

int main(int argc, char ** argv) {
	int my_rank, comm_sz, n=512, source, local_n;
	double a = 0.0, b = 1.0, local_a, local_b, h;
	double total_int = 0, local_int;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	// compute the width of each trapezoid, 
	// as well as how mayn trapezoids
	// each process needs to count 
	// (in order to have 1024 total)
	h = (b-a)/n;
	local_n = n/comm_sz;

	// each process calculates it's local trapezoidal area
	// we will sum all of these to obtain an estimate for the integral
	local_a = a + my_rank * local_n * h;
	local_b = local_a + local_n * h;

	if (my_rank == comm_sz-1) {
		// pick up the remaining trapezoid computations
		int remaining = n - (comm_sz * local_n);
		local_int = Trap(local_a, local_b, local_n + remaining, h);
	} else {
		local_int = Trap(local_a, local_b, local_n, h);
	}

	if (my_rank != 0) {
		MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	} else {
		// starting with rank 0's area...
		total_int = local_int;
		// add the area found by all other processes
		for (source=1; source<comm_sz; source++) {
			MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, 
				MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			total_int += local_int;
		}
	}

	if (my_rank == 0) {
		printf("With n = %d trapezoids, our estiamte\n", n);
		printf("of the integral from %f to %f = %.15e\n", 
			a, b, total_int);
		// Should be about 3.83333...
	}

	MPI_Finalize();
	return 0;
}
