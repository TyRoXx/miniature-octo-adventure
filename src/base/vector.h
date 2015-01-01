#ifndef VECTOR_H
#define VECTOR_H


#include "bool.h"
#include "allocator.h"
#include <stddef.h>
#include <stdio.h>


typedef struct Vector
{
	char *data;
	size_t capacity;
	size_t size;
}
Vector;


void Vector_init(Vector *v);
void Vector_free(Vector *v, Deallocator deallocator);

MOA_USE_RESULT
char *Vector_release(Vector *v);

MOA_USE_RESULT
Bool Vector_push_back(Vector *v, void const *element, size_t size, Allocator allocator);

MOA_USE_RESULT
size_t Vector_size(Vector const *v);

MOA_USE_RESULT
Bool Vector_empty(Vector const *v);

MOA_USE_RESULT
char *Vector_data(Vector const *v);

MOA_USE_RESULT
Bool Vector_reserve(Vector *v, size_t capacity, Allocator allocator);

MOA_USE_RESULT
Bool Vector_resize(Vector *v, size_t size, Allocator allocator);

void Vector_resize_smaller(Vector *v, size_t size);

MOA_USE_RESULT
char *Vector_begin(Vector const *v);

MOA_USE_RESULT
char *Vector_end(Vector const *v);

MOA_USE_RESULT
Bool Vector_append_binary_file(Vector *v, Allocator v_allocator, FILE *in);


#endif
