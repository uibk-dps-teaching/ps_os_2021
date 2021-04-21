#include "my_array.h"

#include <assert.h>
#include <string.h>

#define ARRAY_INIT_CAPACITY 4

typedef struct array {
	void** items;
	size_t capacity;
	size_t size;
} my_array;

void* my_array_init() {
	my_array* array = malloc(sizeof(*array));
	if(!array) {
		return NULL;
	}
	array->capacity = ARRAY_INIT_CAPACITY;
	array->size = 0;
	array->items = malloc(sizeof(void*) * array->capacity);
	if(!array->items) {
		free(array);
		return NULL;
	}
	return array;
}

size_t my_array_size(my_array* array) {
	assert(array);
	return array->size;
}

static int array_resize(my_array* array, size_t capacity) {
	assert(array);
	void** items = realloc(array->items, sizeof(*items) * capacity);
	if(!items) {
		return -1;
	}
	array->items = items;
	array->capacity = capacity;
	return 0;
}

void my_array_push_back(my_array* array, void* item) {
	assert(array);
	if(array->capacity == array->size) array_resize(array, array->capacity * 2);
	array->items[array->size++] = item;
}

void* my_array_get(my_array* array, size_t idx) {
	assert(array);
	if(idx < array->size) return array->items[idx];
	return NULL;
}

void my_array_delete(my_array* array, size_t idx) {
	assert(array);
	if(idx >= array->size) return;

	memmove(array->items + idx, array->items + (idx + 1),
	        sizeof(*array->items) * (array->size - (idx + 1)));
	--array->size;

	if(array->size > 0 && array->size == array->capacity / 4)
		array_resize(array, array->capacity / 2);
}

void my_array_free(my_array* array) {
	assert(array);
	free(array->items);
	free(array);
}

void* my_array_front(my_array* array) {
	return my_array_get(array, 0);
}

void* my_array_pop_front(my_array* array) {
	assert(array);
	void* data = my_array_front(array);
	my_array_delete(array, 0);
	return data;
}
