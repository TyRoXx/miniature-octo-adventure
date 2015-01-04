#include "fauna.h"

void Fauna_init(Fauna *fauna)
{
	Vector_init(&fauna->npcs);
}

void Fauna_free(Fauna *fauna, Deallocator deallocator)
{
	Vector_free(&fauna->npcs, deallocator);
}
