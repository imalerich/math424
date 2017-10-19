#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define ROOT 0

// prints a vector inline to the standard output
void print_vec(float * vec, size_t count);

int * gen_countv(int vcount, int comm_sz) {
	// minimum number taken by each process
	int count = vcount / comm_sz;
	// how many items are left unassigned
	int remainder = vcount % comm_sz;

	// figure out how much data to send to each process
	int * countv = malloc(sizeof(int) * comm_sz); 

	// evenly distribute the remainder until none remains
	for (int i=0; i<comm_sz; i++) {
		countv[i] = count;
		if (remainder > 0) {
			countv[i]++;
			remainder--;
		}
	}

	return countv;
}

int * gen_displs(int * countv, int comm_sz) {
	int * displs = malloc(sizeof(int) * comm_sz);
	displs[0] = 0;

	for (int i=1; i<comm_sz; i++) {
		displs[i] = displs[i-1] + countv[i-1];
	}

	return displs;
}

// only returns actual result to 'root' process, all others receive 0
float vec_dot_prod(float * a, float * b, int vcount, int root) {
	int comm_sz, my_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	float * reca, * recb; // received vectors by all processes

	/* --- SYNC DATA BETWEEN PROCESSES. --- */
	MPI_Bcast(&vcount, 1, MPI_INT, root, MPI_COMM_WORLD);

	int * countv = gen_countv(vcount, comm_sz);
	int * displs = gen_displs(countv, comm_sz);

	reca = malloc(sizeof(float) * countv[my_rank]);
	recb = malloc(sizeof(float) * countv[my_rank]);

	MPI_Scatterv(a, countv, displs, MPI_FLOAT, reca, countv[my_rank], MPI_FLOAT, 
			root, MPI_COMM_WORLD);
	MPI_Scatterv(b, countv, displs, MPI_FLOAT, recb, countv[my_rank], MPI_FLOAT, 
			root, MPI_COMM_WORLD);

	/* --- COMPUTE PARTIAL SUM OF DOT PRODUCT. --- */
	float dot = 0;
	for (int i=0; i<countv[my_rank]; i++) {
		dot += reca[i] * recb[i];
	}

	/* --- COMPUTE FINAL DOT PRODUCT BY REDUCING DOT. --- */
	float dot_sum = 0;
	MPI_Reduce(&dot, &dot_sum, 1, MPI_FLOAT, MPI_SUM, 
			root, MPI_COMM_WORLD);

	free(countv);
	free(displs);
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
	MPI_Bcast(&s, 1, MPI_FLOAT, root, MPI_COMM_WORLD);

	int * countv = gen_countv(vcount, comm_sz);
	int * displs = gen_displs(countv, comm_sz);

	float * rec = malloc(sizeof(float) * countv[my_rank]);

	MPI_Scatterv(vec, countv, displs, MPI_FLOAT, rec, countv[my_rank], MPI_FLOAT, 
			root, MPI_COMM_WORLD);

	/* --- COMPUTE THE MULTIPLICATION, THEN GATHER THE RESULTS --- */
	for (int i=0; i<countv[my_rank]; i++) {
		rec[i] *= s;
	}

	float * ret = (my_rank == root) ? malloc(sizeof(float) * vcount) : NULL;
	MPI_Gatherv(rec, countv[my_rank], MPI_FLOAT, ret, countv, displs, MPI_FLOAT, 
			root, MPI_COMM_WORLD);

	free(countv);
	free(displs);
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
		printf("Enter the size of each vector: ");
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
