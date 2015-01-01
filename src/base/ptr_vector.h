#ifndef PTR_VECTOR_H
#define PTR_VECTOR_H


#include "vector.h"


typedef struct PtrVector
{
	Vector content;
}
PtrVector;


void PtrVector_init(PtrVector *v);
void PtrVector_free(PtrVector *v, Deallocator deallocator);
Bool PtrVector_resize(PtrVector *v, size_t size, Allocator allocator);
Bool PtrVector_push_back(PtrVector *v, void *element, Allocator allocator);
size_t PtrVector_size(PtrVector const *v);
void *PtrVector_get(PtrVector const *v, size_t index);
void PtrVector_set(PtrVector *v, size_t index, void *element);
void **PtrVector_begin(PtrVector const *v);
void **PtrVector_end(PtrVector const *v);
void PtrVector_for_each(PtrVector const *v, void (*on_element)(void *, void *), void *user);


#endif
