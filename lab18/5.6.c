#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main(int argc, char ** argv) {
	int thread_count = strtol(argv[1], NULL, 10);

	int num_iterations;
	printf("Enter the number of iterations to run: ");
	scanf("%d", &num_iterations);

	int * ran_by = malloc(sizeof(int) * num_iterations);

	#pragma omp parallel for num_threads(thread_count)
	for (int i=0; i<num_iterations; i++) {
		ran_by[i] = omp_get_thread_num();
	}

	/* --- OUTPUT THE RESULTS --- */
	for (int i=0; i<thread_count; i++) {
		printf("Thread %d: Iterations " , i);
		for (int k=0; k<num_iterations; k++) {
			if (ran_by[k] == i) {
				printf("%d ", k);
			}
		}

		printf("\n");
	}

	free(ran_by);
	return 0;
}
