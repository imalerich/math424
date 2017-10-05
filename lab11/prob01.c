#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

// random tree generation options
#define MOD_RAND 10
#define DEPTH 5
#define CHANCE 0.05f

struct node {
	int data;
	struct node * right;
	struct node * left;
};

typedef struct node BIN_TREE;

void print_spaces(int count) {
	for (int i=0; i<count; i++) { printf(" "); }
}

void outputTree(BIN_TREE * tree, int numspace) {
	if (tree == NULL) { return; }

	// 1. To the right, to the right.
	outputTree(tree->right, numspace + 5);
	// 2. Print a bunch of spaces.
	print_spaces(numspace);
	// 3. Output the current value of the node.
	printf("%d\n", tree->data);
	// 4.To the left, to the left.
	outputTree(tree->left, numspace + 5);
}

void rand_tree(BIN_TREE * tree, int MAX_DEPTH, float LEAF_CHANCE) {
	if (tree == NULL) { return; }
	tree->data = rand() % MOD_RAND;
	if (MAX_DEPTH-- == 0) { return; }
	
	// create a chance for random leaves to appear
	if ((rand() % 100) >= (100 * LEAF_CHANCE)) {
		tree->left = malloc(sizeof(BIN_TREE));
		rand_tree(tree->left, MAX_DEPTH, LEAF_CHANCE);
	} else { tree->left = NULL; }

	if ((rand() % 100) >= (100 * LEAF_CHANCE)) {
		tree->right = malloc(sizeof(BIN_TREE));
		rand_tree(tree->right, MAX_DEPTH, LEAF_CHANCE);
	} else { tree->right = NULL; }
}


int main(int argc, char ** argv) {
	// seed the random number generator using the system clock
	srand(time(NULL));

	// generate a random tree with a maximum depth of 6
	// and a 25% chance of any node creating a left/right child
	// then output it using the algorithm for lab11
	BIN_TREE * tree = malloc(sizeof(BIN_TREE));
	rand_tree(tree, DEPTH, CHANCE);
	outputTree(tree, 0);

	return 0;
}
