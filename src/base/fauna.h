#ifndef FAUNA_H
#define FAUNA_H

#include "base/npc.h"
#include "base/vector.h"

typedef struct Fauna
{
	Vector npcs;
}
Fauna;

void Fauna_init(Fauna *fauna);
void Fauna_free(Fauna *fauna, Deallocator deallocator);

#endif
