#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

typedef void *Allocation;

typedef struct Allocator
{
	Allocation (*realloc)(Allocation, size_t);
}
Allocator;

Allocation Allocator_calloc(Allocator allocator, size_t a, size_t b);

typedef struct Deallocator
{
	void (*free)(Allocation);
}
Deallocator;

typedef struct MemoryManager
{
	Allocator allocator;
	Deallocator deallocator;
}
MemoryManager;

MemoryManager create_standard_memory_manager(void);

#endif
