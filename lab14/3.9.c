#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define ROOT 0

// prints a vector inline to the standard output
void print_vec(float * vec, size_t count);

// only returns actual result to 'root' process, all others receive 0
float vec_dot_prod(float * a, float * b, int vcount, int root) {
	int comm_sz;
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	float * reca, * recb; // received vectors by all processes

	/* --- SYNC DATA BETWEEN PROCESSES. --- */
	MPI_Bcast(&vcount, 1, MPI_INT, root, MPI_COMM_WORLD);
	int count = vcount / comm_sz;
	reca = malloc(sizeof(float) * count);
	recb = malloc(sizeof(float) * count);

	MPI_Scatter(a, count, MPI_FLOAT, reca, count, MPI_FLOAT, 
			root, MPI_COMM_WORLD);
	MPI_Scatter(b, count, MPI_FLOAT, recb, count, MPI_FLOAT, 
			root, MPI_COMM_WORLD);

	/* --- COMPUTE PARTIAL SUM OF DOT PRODUCT. --- */
	float dot = 0;
	for (int i=0; i<count; i++) {
		dot += reca[i] * recb[i];
	}

	/* --- COMPUTE FINAL DOT PRODUCT BY REDUCING DOT. --- */
	float dot_sum = 0;
	MPI_Reduce(&dot, &dot_sum, 1, MPI_FLOAT, MPI_SUM, 
			root, MPI_COMM_WORLD);
	free(reca);
	free(recb);
	return dot_sum;
}

// only returns data to the 'root' process, all other processes receive NULL
float * scalar_mul(float s, float * vec, int vcount, int root) {
	int comm_sz, my_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* --- SYNC DATA BETWEEN PROCESSES. --- */
	MPI_Bcast(&vcount, 1, MPI_INT, root, MPI_COMM_WORLD);
	int count = vcount / comm_sz;
	float * rec = malloc(sizeof(float) * count);
	MPI_Scatter(vec, count, MPI_FLOAT, rec, count, MPI_FLOAT, 
			root, MPI_COMM_WORLD);
	MPI_Bcast(&s, 1, MPI_FLOAT, root, MPI_COMM_WORLD);

	/* --- COMPUTE THE MULTIPLICATION, THEN GATHER THE RESULTS --- */
	for (int i=0; i<count; i++) {
		rec[i] *= s;
	}

	float * ret = (my_rank == root) ? malloc(sizeof(float) * vcount) : NULL;
	MPI_Gather(rec, count, MPI_FLOAT, ret, count, MPI_FLOAT, 
			root, MPI_COMM_WORLD);

	free(rec);
	return ret;
}

int main(int argc, char ** argv) {
	int comm_sz, my_rank;

	// These will be read by vcount
	float s;
	float * veca, * vecb; // vectors read by process 0
	int vcount;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* --- READ THE VECTORS AND SCALAR VALUE. --- */
	if (my_rank == ROOT) {
		printf("Enter the size of each vector (must be a multiple of %d): ", 
				comm_sz);
		fflush(stdout);
		scanf("%d", &vcount);

		veca = malloc(sizeof(float) * vcount);
		vecb = malloc(sizeof(float) * vcount);

		for (int i=0; i<vcount; i++) {
			printf("A[%d]: ", i);
			fflush(stdout);
			scanf("%f", &veca[i]);
		}

		printf("\n");
		for (int i=0; i<vcount; i++) {
			printf("B[%d]: ", i);
			fflush(stdout);
			scanf("%f", &vecb[i]);
		}

		printf("\nEnter a scalar value: ");
		fflush(stdout);
		scanf("%f", &s);
	}
	
	/* --- PERFORM COMPUTATIONS. --- */
	float dot = vec_dot_prod(veca, vecb, vcount, ROOT);
	float * s_times_veca = scalar_mul(s, veca, vcount, ROOT);
	float * s_times_vecb = scalar_mul(s, vecb, vcount, ROOT);

	/* --- OUTPUT THE RESULTS. --- */
	if (my_rank == ROOT) {
		// First, dot product results
		printf("\n------------------------------------------"
				"---------------------\n");
		print_vec(veca, vcount);
		printf(" dot ");
		print_vec(vecb, vcount);
		printf(" = %f\n", dot);

		printf("\n------------------------------------------"
				"---------------------\n");
		printf("%f x ", s);
		print_vec(veca, vcount);
		printf(" = ");
		print_vec(s_times_veca, vcount);
		printf("\n");

		printf("\n------------------------------------------"
				"---------------------\n");
		printf("%f x ", s);
		print_vec(vecb, vcount);
		printf(" = ");
		print_vec(s_times_vecb, vcount);
		printf("\n------------------------------------------"
				"---------------------\n");

		fflush(stdout);
	}

	if (my_rank == ROOT) {
		free(s_times_veca);
		free(s_times_vecb);
	}

	MPI_Finalize();
	return 0;
}

void print_vec(float * vec, size_t count) {
	printf("[");
	for (int i=0; i<count; i++) {
		printf("%.1f", vec[i]);
		if (i != count-1) { printf(", "); }
	}
	printf("]");
}
