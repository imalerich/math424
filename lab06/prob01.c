#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#define MOD_RAND 10

void print_arr(char * dat, size_t count);

int asc(char a, char b) { return a < b;  }
int dec(char a, char b) { return b < a; }

void selection_sort(char * dat, size_t count, int (*compare)(char, char)) {
	for (int i=0; i<count; i++) {
		// Find the smallest remaining element, that is
		// the element who belongs at index i
		int min = i;
		for (int j=i+1; j<count; j++) {
			// if we find something smaller, keep track of the index
			if (compare(dat[j], dat[min])) {
				min = j;
			}
		}

		// swap the values (this happens in place)
		char tmp = dat[i];
		dat[i] = dat[min];
		dat[min] = tmp;
	}
}

int main(int argc, char ** argv) {
	// seed the random number generator using the system clock
	srand(time(NULL));

	const size_t count = 10;
	char * arr = malloc(sizeof(char) * count);
	for (int i=0; i<count; i++) {
		arr[i] = 'a' + rand() % 26;
	}

	printf("---------------------------------------\n");
	printf("Input array is:\n   ");
	print_arr(arr, count);
	printf("\nSorted (alphabetical) array is:\n   ");
	selection_sort(arr, count, asc);
	print_arr(arr, count);
	printf("\nSorted (reverse alphabetical) array is:\n   ");
	selection_sort(arr, count, dec);
	print_arr(arr, count);
	printf("---------------------------------------\n");

	free(arr);
	return 0;
}

void print_arr(char * dat, size_t count) {
	printf("[");
	for (int i=0; i<count; i++) {
		printf("%c", dat[i]);
		if (i < count-1) { printf(", "); }
	}

	printf("]\n");
}
