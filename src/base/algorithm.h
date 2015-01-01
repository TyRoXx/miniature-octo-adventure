#ifndef ALGORITHM_H
#define ALGORITHM_H


#include "bool.h"
#include <stddef.h>


void for_each(void *begin,
			  void *end,
			  size_t element_size,
			  void (*action)(void *element, void *user),
			  void *user);

void *find(void *begin,
		   void *end,
		   size_t element_size,
		   Bool (*predicate)(void *element, void *user),
		   void *user);

#define MOA_ARRAY_SIZE(array) (sizeof(array) / sizeof(*(array)))
#define MOA_ARRAY_END(array) ((array) + MOA_ARRAY_SIZE((array)))


#endif
