#include <stdio.h>
#include <omp.h>

int main(int argc, char ** argv) {
	int val = _OPENMP;
	printf("%d\n", val);

	/*
	 * Outputs 201511
	 * 2015 11 => November 2015
	 * Date of Release for OpenMP 4.5
	 */

	return 0;
}
