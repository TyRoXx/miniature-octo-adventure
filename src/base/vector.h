#ifndef VECTOR_H
#define VECTOR_H

#include "bool.h"
#include "allocator.h"
#include <stddef.h>
#include <stdio.h>
#include <assert.h>

typedef struct Vector
{
	char *data;
	size_t capacity;
	size_t size;
}
Vector;

static inline void Vector_init(Vector *v)
{
	v->data = 0;
	v->capacity = v->size = 0;
}

static inline void Vector_free(Vector *v, Deallocator deallocator)
{
	Deallocator_free(deallocator, v->data);
}

MOA_USE_RESULT
static inline char *Vector_release(Vector *v)
{
	return v->data;
}

MOA_USE_RESULT
Bool Vector_push_back(Vector *v, void const *element, size_t size, Allocator allocator);

MOA_USE_RESULT
static inline size_t Vector_size(Vector const *v)
{
	return v->size;
}

MOA_USE_RESULT
static inline Bool Vector_empty(Vector const *v)
{
	return v->size == 0;
}

MOA_USE_RESULT
static inline char *Vector_data(Vector const *v)
{
	return v->data;
}

MOA_USE_RESULT
Bool Vector_reserve(Vector *v, size_t capacity, Allocator allocator);

MOA_USE_RESULT
Bool Vector_resize(Vector *v, size_t size, Allocator allocator);

static inline void Vector_resize_smaller(Vector *v, size_t size)
{
	assert(size <= v->size);
	v->size = size;
}

MOA_USE_RESULT
static inline char *Vector_begin(Vector const *v)
{
	return Vector_data(v);
}

MOA_USE_RESULT
static inline char *Vector_end(Vector const *v)
{
	return Vector_data(v) + Vector_size(v);
}

MOA_USE_RESULT
Bool Vector_append_binary_file(Vector *v, Allocator v_allocator, FILE *in);

MOA_USE_RESULT
Bool Vector_printf(Vector *v, Allocator allocator, char const *format, ...);


#endif
