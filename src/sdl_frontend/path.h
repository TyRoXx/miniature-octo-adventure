#ifndef PATH_H
#define PATH_H

#include "base/allocator.h"

MOA_USE_RESULT
char *join_paths(char const *left, char const *right, MemoryManager memory);

#endif
