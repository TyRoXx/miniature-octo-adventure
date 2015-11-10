#ifndef SPACIAL_FINDER_H
#define SPACIAL_FINDER_H

#include "base/entity.h"
#include "base/ptr_vector.h"
#include "base/rectangle.h"

typedef struct SpacialFinder
{
	PtrVector all;
}
SpacialFinder;

void SpacialFinder_init(SpacialFinder *finder);
void SpacialFinder_free(SpacialFinder *finder, Deallocator deallocator);

typedef enum ContinueFlag
{
	Continue_Yes,
	Continue_No
}
ContinueFlag;

void SpacialFinder_enumerate_area(SpacialFinder const *finder, ContinueFlag(on_element)(void *, void *), void *user);

MOA_USE_RESULT
Bool SpacialFinder_add(SpacialFinder *finder, void *element, Allocator allocator);

#endif
