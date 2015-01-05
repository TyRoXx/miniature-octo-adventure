#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "base/unused.h"
#include <stddef.h>

typedef void *Allocation;
typedef void *PrivateAllocatorState;

typedef struct Allocator
{
	Allocation (*realloc)(Allocation, size_t, PrivateAllocatorState);
	PrivateAllocatorState state;
}
Allocator;

MOA_USE_RESULT
static inline Allocation Allocator_alloc(Allocator allocator, size_t size)
{
	return allocator.realloc(NULL, size, allocator.state);
}

MOA_USE_RESULT
static inline Allocation Allocator_realloc(Allocator allocator, Allocation allocation, size_t size)
{
	return allocator.realloc(allocation, size, allocator.state);
}

MOA_USE_RESULT
Allocation Allocator_calloc(Allocator allocator, size_t a, size_t b);

typedef struct Deallocator
{
	void (*free)(Allocation, PrivateAllocatorState);
	PrivateAllocatorState state;
}
Deallocator;

void Deallocator_free(Deallocator deallocator, Allocation allocation);

typedef struct MemoryManager
{
	Allocator allocator;
	Deallocator deallocator;
}
MemoryManager;

MOA_USE_RESULT
MemoryManager create_standard_memory_manager(void);

#endif
