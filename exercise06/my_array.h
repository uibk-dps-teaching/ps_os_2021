#ifndef MY_ARRAY_H
#define MY_ARRAY_H

#include <stdlib.h>

typedef struct array my_array;

void* my_array_init();

size_t my_array_size(my_array* array);

void my_array_push_back(my_array* array, void* item);

void* my_array_get(my_array* array, size_t idx);

void my_array_delete(my_array* array, size_t idx);

void my_array_free(my_array* array);

#endif // MY_ARRAY_H
