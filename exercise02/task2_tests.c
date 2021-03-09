#include "bstree.h"

#include <assert.h>

int main() {
	bstree* t = bstree_create();

	assert(bstree_size(t) == 0);
	assert(bstree_contains(t, 4.2f) == false);
	assert(bstree_depth(t, 4.2f) == -1);
	bstree_print(t, stdout);

	bstree_insert(t, 4.2f);
	bstree_insert(t, 77.7f);
	bstree_insert(t, 3.12f);

	assert(bstree_size(t) == 3);
	assert(bstree_depth(t, 77.7f) == 1);
	assert(bstree_contains(t, 4.2f) == true);
	assert(bstree_minimum(t) == 3.12f);
	assert(bstree_maximum(t) == 77.7f);
	bstree_print(t, stdout);

	bstree_remove(t, 4.2f);

	assert(bstree_size(t) == 2);
	assert(bstree_contains(t, 4.2f) == false);
	assert(bstree_minimum(t) == 3.12f);
	assert(bstree_maximum(t) == 77.7f);
	bstree_print(t, stdout);

	bstree_insert(t, 2.f);
	bstree_insert(t, -3.14f);
	bstree_insert(t, 666.f);
	bstree_insert(t, 9.99f);

	assert(bstree_size(t) == 6);
	assert(bstree_depth(t, -3.14f) == 2);
	bstree_print(t, stdout);

	bstree_remove(t, 666.f);

	assert(bstree_size(t) == 5);
	assert(bstree_depth(t, 9.99f) == 2);
	assert(bstree_contains(t, 666.f) == false);
	assert(bstree_depth(t, 666.f) == -1);
	bstree_print(t, stdout);

	bstree_insert(t, 5.5f);
	bstree_insert(t, 5.5f);

	assert(bstree_size(t) == 6);
	assert(bstree_contains(t, 5.5f) == true);
	bstree_print(t, stdout);

	bstree_insert(t, 666.f);
	bstree_insert(t, 4.2f);
	bstree_remove(t, 77.7f);
	bstree_remove(t, 77.7f);

	assert(bstree_size(t) == 7);
	assert(bstree_contains(t, 77.f) == false);
	bstree_print(t, stdout);

	bstree_destroy(t);
}
