#include "allocator.h"
#include <stdlib.h>
#include <string.h>

Allocation Allocator_calloc(Allocator allocator, size_t a, size_t b)
{
	size_t total = a * b;
	Allocation result;
	if (a != 0 && (total / a) != b)
	{
		return NULL;
	}
	result = allocator.realloc(NULL, total, allocator.state);
	memset(result, 0, total);
	return result;
}

void Deallocator_free(Deallocator deallocator, Allocation allocation)
{
	deallocator.free(allocation, deallocator.state);
}

static Allocation wrapped_realloc(Allocation allocation, size_t size, PrivateAllocatorState state)
{
	(void)state;
	return realloc(allocation, size);
}

static void wrapped_free(Allocation allocation, PrivateAllocatorState state)
{
	(void)state;
	free(allocation);
}

MemoryManager create_standard_memory_manager(void)
{
	MemoryManager manager = {{wrapped_realloc, NULL}, {wrapped_free, NULL}};
	return manager;
}
