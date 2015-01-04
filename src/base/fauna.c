#include "fauna.h"
#include "base/algorithm.h"

void Fauna_init(Fauna *fauna)
{
	Vector_init(&fauna->npcs);
}

void Fauna_free(Fauna *fauna, Deallocator deallocator)
{
	Vector_free(&fauna->npcs, deallocator);
}

typedef struct NPCUpdateArgs
{
	struct TileGrid const *world;
	TimePoint now;
}
NPCUpdateArgs;

static void update_npc(void *element, void *user)
{
	NPCUpdateArgs *args = user;
	NPC_update(element, args->world, args->now);
}

void Fauna_update(Fauna *fauna, struct TileGrid const *world, TimePoint now)
{
	NPCUpdateArgs args = {world, now};
	for_each(
		Vector_begin(&fauna->npcs),
		Vector_end(&fauna->npcs),
		sizeof(NPC),
		update_npc,
		&args
	);
}

Bool Fauna_add_npc(Fauna *fauna, NPC npc, Allocator allocator)
{
	return Vector_push_back(&fauna->npcs, &npc, sizeof(npc), allocator);
}
