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
	result = allocator.realloc(NULL, total);
	memset(result, 0, total);
	return result;
}

MemoryManager create_standard_memory_manager(void)
{
	MemoryManager manager = {{realloc}, {free}};
	return manager;
}
