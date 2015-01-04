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
void Fauna_update(Fauna *fauna, struct World const *world, TimePoint now);

MOA_USE_RESULT
Bool Fauna_add_npc(Fauna *fauna, NPC npc, Allocator allocator);

#endif
