#ifndef PATH_H
#define PATH_H

#include "base/allocator.h"

char *join_paths(char const *left, char const *right, MemoryManager memory);

#endif
