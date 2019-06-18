#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

///Return the max value of x or y
#define MAX(x,y) ((x) > (y) ? (x) : (y))

///Return the min value of x or y
#define MIN(x,y) ((x) < (y) ? (x) : (y))

///Defining a node in the Tree
struct node {
	int key;
	struct node* left;
	struct node* right;
	int height;
};

///Pointer to the root node
struct node* ROOT = NULL;

///Node pointing to the current node
struct node* CURRENT = NULL;

///Create a node function
struct node* createNode(int key) {
	struct node* newNode = (struct node*) malloc(sizeof(struct node));
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->height = 1;
	CURRENT = newNode;
	return (newNode);
}

///A function to get height of the tree
int height(struct node* N) {
	if (N == NULL) {
		return 0;
	}
	return N->height;
}

////TREE ROTATION FUNCTIONS////
///A utility function to right rotate subtree rooted with y
struct node* rightRotate(struct node* y){
	struct node* x = y->left;
	struct node* T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = MAX(height(y->left), height(y->right)) + 1;
	x->height = MAX(height(x->left), height(x->right)) + 1;

	// Return new root
	return x;
}

///A utility function to left rotate subtree rooted with x 
struct node* leftRotate(struct node* x){
	struct node* y = x->right;
	struct node* T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	//  Update heights
	x->height = MAX(height(x->left), height(x->right)) + 1;
	y->height = MAX(height(y->left), height(y->right)) + 1;

	// Return new root
	return y;
}

///Get Balance Factor
int getBalance(struct node* N){
	if (N == NULL) {
		return 0;
	}
	return height(N->left) - height(N->right);
}

