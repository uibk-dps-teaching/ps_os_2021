#include "bstree.h"

#include <assert.h>

int main() {
	bstree* t = bstree_create();

	assert(bstree_contains(t, 4.2f) == false);
	bstree_print(t, stdout);

	bstree_insert(t, 4.2f);
	bstree_insert(t, 77.7f);
	bstree_insert(t, 3.12f);

	assert(bstree_contains(t, 4.2f) == true);
	assert(bstree_contains(t, 77.7f) == true);
	assert(bstree_contains(t, 3.12f) == true);
	bstree_print(t, stdout);

	bstree_insert(t, 2.f);
	bstree_insert(t, -3.14f);
	bstree_insert(t, 666.f);
	bstree_insert(t, 9.99f);
	bstree_insert(t, 999.99f);
	bstree_insert(t, 123.45f);
	bstree_print(t, stdout);

	bstree_insert(t, 5.5f);
	bstree_insert(t, 5.5f);

	assert(bstree_contains(t, 5.5f) == true);
	bstree_print(t, stdout);

	bstree_destroy(t);
}
