#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

struct llist {
	char data;
	struct llist * next;
};

typedef struct llist List;

void print_list(List * l);

List * concatenate(List * a, List * b) {
	// cannot concatenate b on a NULL pointer
	if (a == NULL) { return NULL; }

	// want to find the tail element of a
	List * t = NULL;
	// find the tail element of a, that is the elemnt
	// who's next node is null
	for (t = a; t->next != NULL; t = t->next) { }

	// next, simply connect the tail of a, to the head of b
	t->next = b;
	return a;
}

// adds (count) random character data to the input linked list
void rand_list(List * l, size_t count) {
	l->data = 'a' + rand() % 26;
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

	List * a = malloc(sizeof(List));
	List * b = malloc(sizeof(List));
	rand_list(a, 2 + rand() % 9); // want at least a couple of elements in a
	rand_list(b, 2 + rand() % 9); // same for b

	printf("---------------------------------------\n");
	printf("a: [ ");
	print_list(a);
	printf("]\n");
	printf("---------------------------------------\n");
	printf("b: [ ");
	print_list(b);
	printf("]\n");
	printf("---------------------------------------\n");
	printf("concatenate(a, b): \n[ ");
	print_list(concatenate(a, b));
	printf("]\n");
	printf("---------------------------------------\n");

	return 0;
}

void print_list(List * l) {
	if (l == NULL) { return; }
	printf("%c ", l->data);
	print_list(l->next);
}
