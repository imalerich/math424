#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

const unsigned N = 100000000;

int main(int argc, char ** argv) {
	// get the number of threads to test
	size_t thread_count = strtol(argv[1], NULL, 10);

	double start = omp_get_wtime();
	#pragma omp parallel num_threads(thread_count)
	{
		int i;
		double my_sum = 0.0;

		for (i=0; i<N; i++) {
			#pragma omp atomic
			my_sum += sin(i);
		}
	}

	// determine the runtime and output the results
	double end = omp_get_wtime();
	double runtime = end - start;
	printf("%d : %f\n", thread_count, runtime);
}
