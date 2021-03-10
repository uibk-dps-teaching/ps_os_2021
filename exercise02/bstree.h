#ifndef _BSTREE_H_
#define _BSTREE_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * Stores a binary sorted tree of values, in which each element may only
 * occur once.
 */
struct bstree;
typedef struct bstree bstree;

/**
 * The type of value that should be stored within the tree.
 */
typedef float value_t;

/**
 * This macro defines how a value within the tree should be printed.
 * 
 * You can use this within a printf format string like so:
 * printf("Value: " VALUE_T_FORMAT "\n", my_value);
 */
#define VALUE_T_FORMAT "%.2f"

/**
 * Creates and returns a new binary tree.
 * Must be deallocated by bstree_destroy.
 */
bstree* bstree_create();

/**
 * Destroys and deallocates all memory for the given tree 't'
 */
void bstree_destroy(bstree* t);

/**
 * Inserts the given value `v` into tree `t`.
 * If the value is already in `t`, no changes are made.
 */
void bstree_insert(bstree* t, value_t v);

/**
 * Removes the given value `v` from tree `t`.
 */
void bstree_remove(bstree* t, value_t v);

/**
 * Returns true if the given tree `t` contains `v`, false otherwise.
 */
bool bstree_contains(const bstree* t, value_t v);

/**
 * Returns the smallest value in tree `t`.
 */
value_t bstree_minimum(const bstree* t);

/**
 * Returns the largest value in tree `t`.
 */
value_t bstree_maximum(const bstree* t);

/**
 * Returns the depth of the node in tree `t` containing the value `v`.
 * Returns -1 if the value `v` does not exist.
 */
int32_t bstree_depth(const bstree* t, value_t v);

/**
 * Returns the number of values in the given bstree `t`.
 * NOTE: This should complete in O(1) time.
 */
int32_t bstree_size(const bstree* t);

/**
 * Prints the given bstree `t` to the supplied output stream `out`.
 * 
 * output format: [<LEFT>, VAL, <RIGHT>] : <SIZE>
 * example empty: [ NIL ] : 0
 * example 3,4,7 in a balanced tree: [[3], 4, [7]] : 3
 */
void bstree_print(const bstree* t, FILE* out);

#endif // _BSTREE_H_
