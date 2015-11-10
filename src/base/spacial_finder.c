#include "spacial_finder.h"

void SpacialFinder_init(SpacialFinder *finder)
{
	PtrVector_init(&finder->all);
}

void SpacialFinder_free(SpacialFinder *finder, Deallocator deallocator)
{
	PtrVector_free(&finder->all, deallocator);
}

void SpacialFinder_enumerate_area(SpacialFinder const *finder, ContinueFlag (on_element)(void *, void *), void *user)
{
	for (size_t i = 0; i < PtrVector_size(&finder->all); ++i)
	{
		void * const element = PtrVector_get(&finder->all, i);
		switch (on_element(element, user))
		{
		case Continue_Yes:
			break;

		case Continue_No:
			return;
		}
	}
}

Bool SpacialFinder_add(SpacialFinder *finder, void *element, Allocator allocator)
{
	return PtrVector_push_back(&finder->all, element, allocator);
}
