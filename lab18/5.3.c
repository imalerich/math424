#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define MOD_RAND 100

void print_vec(int * arr, int n);

void Para_Count_sort(int a[], int n, int thread_count) {
	int i, j, count;
	int * tmp = malloc(sizeof(int) * n);

	#pragma omp parallel for num_threads(thread_count) private(j, count)
	for (i=0; i<n; i++) {
		count = 0; // count is how many things are smaller than a[i]
		for (j=0; j<n; j++) {
			if (a[j] < a[i] || (a[j] == a[i] && j < i)) {
				count++;
		}}

		// place a[i] after everything smaller than it...
		// but before everything bigger than it
		
		// note that each iteration writes to a unique location
		// in the array, so we don't have to worry about
		// having this in a critical section
		tmp[count] = a[i];
	}

	// read values from tmp and write them to their corresponding
	// location in a (the output array), this behave the same
	// as the original memcpy of Count_sort, but will be performed
	// in parallel
	#pragma omp parallel for num_threads(thread_count)
	for (i=0; i<n; i++) { a[i] = tmp[i]; }
	free(tmp);
}

void Count_sort(int a[], int n) {
	int i, j, count;
	int * tmp = malloc(sizeof(int) * n);

	for (int i=0; i<n; i++) {
		count = 0; // count is how many things are smaller than a[i]
		for (int j=0; j<n; j++) {
			if (a[j] < a[i] || (a[j] == a[i] && j < i)) {
				count++;
		}}

		// place a[i] after everything smaller than it...
		// but before everything bigger than it
		tmp[count] = a[i];
	}

	memcpy(a, tmp, sizeof(int)*n);
	free(tmp);
}

int comp(const void * p, const void * q) {
	int l = *(const int *)p;
	int r = *(const int *)q;
	return l - r;
}

int main(int argc, char ** argv) {
	srand(time(NULL));
	double start, end;
	int thread_count = strtol(argv[1], NULL, 10);
	int n;

	printf("Enter array size: ");
	scanf("%d", &n);

	// to be fair, each method will sort the same array
	// but as this is an in place sort, we will keep
	// an 'original' copy that will never be sorted
	int * arr = malloc(sizeof(int) * n);
	int * orig = malloc(sizeof(int) * n);
	for (int i=0; i<n; i++) {
		orig[i] = rand() % MOD_RAND;
	}
	memcpy(arr, orig, sizeof(int)*n);

	printf("arr: ");
	print_vec(arr, n);

	/* --- COUNT SORT --- */

	start = omp_get_wtime();
	Count_sort(arr, n);
	end = omp_get_wtime();

	printf("\nCount_sort(arr): ");
	print_vec(arr, n);
	printf("Sorted in %f seconds.\n", (end - start));

	memcpy(arr, orig, sizeof(int)*n);

	/* --- PARALLEL COUNT SORT --- */

	start = omp_get_wtime();
	Para_Count_sort(arr, n, thread_count);
	end = omp_get_wtime();

	printf("\nPara_Count_sort(arr): ");
	print_vec(arr, n);
	printf("Sorted in %f seconds.\n", (end - start));

	memcpy(arr, orig, sizeof(int)*n);

	/* --- QUICK SORT --- */

	start = omp_get_wtime();
	qsort((void**)arr, n, sizeof(int), comp);
	end = omp_get_wtime();

	printf("\nqsort(arr): ");
	print_vec(arr, n);
	printf("Sorted in %f seconds\n.", (end - start));
	
	free(orig);
	free(arr);
	return 0;
}

void print_vec(int * arr, int n) {
	printf("[ ");
	for (int i=0; i<n; i++) { printf("%d ", arr[i]); }
	printf("]\n");
}
