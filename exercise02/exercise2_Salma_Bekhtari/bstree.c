#include "bstree.h"
#include <assert.h>

typedef struct bstree_node node;

struct bstree_node {
	value_t item;
	node *left_child, *right_child;
};

struct bstree {
	int32_t size;
	node* root;
};

int32_t bstree_size(const bstree* t) {
	return t->size;
}

bstree* bstree_create() {
	bstree* new_bstree = (bstree*)malloc(sizeof(bstree));
	if(new_bstree == NULL) {
		exit(EXIT_FAILURE);
	}
	new_bstree->root = NULL;
	new_bstree->size = 0;
	return new_bstree;
}

void node_destroy(node* n) {
	if(n->left_child != NULL) node_destroy(n->left_child);
	if(n->right_child != NULL) node_destroy(n->right_child);
	free(n);
}

void bstree_destroy(bstree* t) {
	if(t->root != NULL) {
		node_destroy(t->root);
	}
}

static node* new_node(int value) {
	node* temp = (node*)malloc(sizeof(node));
	if(temp == NULL) {
		printf("An error occured while allocating memory.");
		exit(EXIT_FAILURE);
	}
	temp->item = value;
	temp->left_child = NULL;
	temp->right_child = NULL;
	return temp;
}

// returns node holding the value d (for deletion), or the parent of the first empty node child if d
// does not exist within tree(for insertion)
static node* search(node* parentOfCurrent, node* current, int d) {
	if(current == NULL) {
		return parentOfCurrent;
	} else if(current->item == d) {
		return current;
	} else if(d < current->item) {
		return search(current, current->left_child, d);
	} else if(d > current->item) {
		return search(current, current->right_child, d);
	}
	return parentOfCurrent;
}

bool bstree_contains(const bstree* t, value_t d) {
	node* temp = search(t->root, t->root, d);
	if(temp == NULL) return false;
	if(temp->item == d) return true;
	return false;
}

void bstree_insert(bstree* t, value_t d) {
	// create new node if tree is empty
	if(t->root == NULL) {
		t->root = new_node(d);
		t->size++;
		return;
	}
	// case 1: the parent of d has no or only one child
	node* d_node = search(t->root, t->root, d);
	if(d < d_node->item) {
		d_node->left_child = new_node(d);
		t->size++;
	} else if(d > d_node->item) {
		d_node->right_child = new_node(d);
		t->size++;
	}
	// catch: d doesn't exist in tree
	else if(d_node->item == d)
		return;
}

static int node_minimum(const node* t) {
	if(t->left_child == NULL)
		return t->item;
	else
		return node_minimum(t->left_child);
}

value_t bstree_minimum(const bstree* t) {
	if(t->root == NULL) return -1;
	return node_minimum(t->root);
}

static int node_maximum(const node* t) {
	if(t->right_child == NULL) return t->item;
	return node_maximum(t->right_child);
}

value_t bstree_maximum(const bstree* t) {
	if(t->root == NULL)
		return -1;
	else
		node_maximum(t->root);
	return 0;
}

static void print_tree(node* node, FILE* out) {
	if(node->left_child != NULL) {
		fprintf(out, "(");
		print_tree(node->left_child, out);
		fprintf(out, "), ");
	}
	fprintf(out, VALUE_T_FORMAT, node->item);
	if(node->right_child != NULL) {
		fprintf(out, ", (");
		print_tree(node->right_child, out);
		fprintf(out, ")");
	}
}

/**
 * Prints the given bstree 't' to the supplied output stream 'out'
 * output format: ([LEFT], VAL, [RIGHT]) : [SIZE]
 * example empty: ( NIL ) : 0
 * example 3,4,7 in a balanced tree: ((3), 4, (7)) : 3
 */
void bstree_print(const bstree* tree, FILE* out) {
	if(tree->root == NULL) {
		printf("( NIL ) : 0\n");
		return;
	} else {
		fprintf(out, "(");
		print_tree(tree->root, out);
		fprintf(out, ") : ");
		fprintf(out, "%d\n", (int)tree->size);
	}
}