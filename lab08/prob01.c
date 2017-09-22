#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#define MOD_RAND 10
#define LENGTH 10

struct llist {
	int data;
	struct llist * next;
};

typedef struct llist List;

void print_list(List * l);

// Recursively prints a list in reverse order
void printListBackward(List * l) {
	if (l == NULL) { return; }
	printListBackward(l->next);
	printf("%d ", l->data);
}

// searches the input list for value 'v'
int * searchList(List * l, int val) {
	if (l == NULL) { return NULL; }
	// return a pointer to the stored VALUE
	if (l->data == val) { return &l->data; }
	searchList(l->next, val);
}

// adds (count) random character data to the input linked list
void rand_list(List * l, size_t count) {
	l->data = rand() % MOD_RAND;
	if (count > 1) {
		l->next = malloc(sizeof(List));
		rand_list(l->next, count-1);
	} else {
		l->next = NULL;
	}
}

int main(int argc, char ** argv) {
	// seed the random number generator using the system clock
	srand(time(NULL));

	List * l = malloc(sizeof(List));
	rand_list(l, LENGTH);

	printf("---------------------------------------\n");
	printf("l: [ ");
	print_list(l);
	printf("]\n");
	printf("---------------------------------------\n");
	printf("printListBackward(l): \n[ ");
	printListBackward(l);
	printf("]\n");
	printf("---------------------------------------\n");
	int find = rand() % MOD_RAND;
	printf("Searching for value: %d\n", find);
	int * addr = searchList(l, find);
	if (addr == NULL) {
		printf("Value not found!\n");
	} else {
		printf("Value found at address: %d\n", addr);
	}
	printf("---------------------------------------\n");

	return 0;
}

void print_list(List * l) {
	if (l == NULL) { return; }
	printf("%d ", l->data);
	print_list(l->next);
}
