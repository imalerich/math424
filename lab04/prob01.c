#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#define SIZE 10
#define MAX 100

void bubble(int arr[], size_t count) {
	int tmp, swaps;
	for (int k=count-1; k>=0; k--) {
		swaps = 0;

		// only need to count up til k
		// anything after k is already sorted
		for (int i=0; i<k; i++) {
			if (arr[i] > arr[i+1]) {
				tmp = arr[i+1];
				arr[i+1]= arr[i];
				arr[i] = tmp;
				swaps++;
			}
		}

		// no swaps were performed
		// the list must be sorted
		if (!swaps) { return; }
	}
}

void print_arr(int arr[], size_t count) {
	for (int i=0; i<count; i++) {
		if (i != count-1) {
			printf("%d, ", arr[i]);
		} else {
			printf("%d\n", arr[i]);
		}
	}
}

int main(int argc, char ** argv) {
	// seed the random number generator using the system clock
	srand(time(NULL));

	// create an array filled with random values
	int arr[SIZE];
	for (int i=0; i<SIZE; i++) { arr[i] = rand() % MAX; }

	printf("Initial array:\t");
	print_arr(arr, SIZE);

	// sort the array using our bubble sort method
	bubble(arr, SIZE);

	printf("Sorted array:\t");
	print_arr(arr, SIZE);

	return 0;
}
