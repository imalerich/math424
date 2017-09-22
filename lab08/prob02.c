#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#define MAX(A,B) (A > B) ? A : B
#define MOD_RAND 5

struct node {
	int data;
	struct node * right;
	struct node * left;
};

typedef struct node BIN_TREE;

int depth(BIN_TREE * tree) {
	// no tree, depth is 0
	if (tree == NULL) { return 0; }

	// find the depth of the left annd right
	int left = depth(tree->left);
	int right = depth(tree->right);

	// depth is the maximum depth of left and right
	// +1 to include self
	return MAX(left, right) + 1;
}

BIN_TREE * binaryTreeSearch(BIN_TREE * tree, int val) {
	if (tree == NULL) { return NULL; }
	// return a pointer to the node (not the stored value)
	if (tree->data == val) { return tree; }

	BIN_TREE * b0 = binaryTreeSearch(tree->left, val);
	BIN_TREE * b1 = binaryTreeSearch(tree->right, val);

	// if b0 finds the value, return the reference
	if (b0) { return b0; }
	return b1; // otherwise return whatever b1 found
}

void rand_tree(BIN_TREE * tree, int MAX_DEPTH) {
	if (tree == NULL || MAX_DEPTH == 0) { return; }
	--MAX_DEPTH;
	tree->data = rand() % MOD_RAND;
	
	// create a chance for random leaves to appear
	if (rand() % 3) {
		tree->left = malloc(sizeof(BIN_TREE));
		rand_tree(tree->left, MAX_DEPTH);
	} else { tree->left = NULL; }

	if (rand() % 3) {
		tree->right = malloc(sizeof(BIN_TREE));
		rand_tree(tree->right, MAX_DEPTH);
	} else { tree->right = NULL; }
}

void print_level(BIN_TREE * tree, int depth) {
	if (tree == NULL) { return; }
	if (depth == 1) { 
		printf("%d ", tree->data); 
	} else {
		print_level(tree->left, depth-1);
		print_level(tree->right, depth-1);
	}
}

// recursively traverse a binary tree and print it out
// start at the bottom-left node, then work your way back up
// only traverse right after all nodes below and left have been printed
void print_tree(BIN_TREE * tree) {
	if (tree == NULL) { return; }
	int d = depth(tree);
	for (int i=1; i<=d; i++) {
		print_level(tree, i);
		if (i!=d) { printf("\n"); }
	}
}

int main(int argc, char ** argv) {
	// seed the random number generator using the system clock
	srand(time(NULL));

	BIN_TREE * tree = malloc(sizeof(BIN_TREE));
	rand_tree(tree, 3);

	printf("---------------------------------------\n");
	printf("Generated Tree: \n");
	print_tree(tree);
	printf("\n");
	printf("The depth of the tree is: %d\n", depth(tree));
	int find = rand() % MOD_RAND;
	printf("Searching for value: %d\n", find);
	BIN_TREE * addr = binaryTreeSearch(tree, find);
	if (addr == NULL) {
		printf("Value not found!\n");
	} else {
		printf("Value found at address: %d\n", addr);
	}
	printf("---------------------------------------\n");

	return 0;
}
