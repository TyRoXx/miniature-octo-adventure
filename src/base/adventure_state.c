#include "adventure_state.h"
#include "world_text_file.h"
#include "game.h"
#include "algorithm.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

static TileKind const TileKinds[] =
{
	{0, 1},
	{1, 1},
	{2, 1},
	{3, 1},
	{4, 0},
	{5, 1},
	{6, 1},
	{7, 1},
	{8, 0},
	{9, 0},
};

MOA_USE_RESULT
static Bool load_world(char const *file_name, World *world, MemoryManager world_memory)
{
	Bool result;
	FILE * const file = fopen(file_name, "r");
	if (!file)
	{
		fprintf(stderr, "Cannot open world file %s\n", file_name);
		return False;
	}

	result = load_world_from_text(world,
								  world_memory,
								  TileKinds,
								  sizeof(TileKinds) / sizeof(*TileKinds),
								  file,
								  stderr);
	fclose(file);
	return result;
}

static GameState *AdventureState_create(Game *game)
{
	AdventureState * const adv_state = Allocator_realloc(game->memory.allocator, NULL, sizeof(*adv_state));
	if (!adv_state)
	{
		return NULL;
	}

	SpacialFinder_init(&adv_state->movers);
	Fauna_init(&adv_state->fauna);

	adv_state->memory = game->memory;
#if MOA_MEMORY_DEBUGGING
	adv_state->memory_statistics = &game->memory_statistics;
#endif

	{
		char const * const world_file_name = "data/world.txt";
		World * const world = &adv_state->world;

		if (load_world(world_file_name, world, adv_state->memory))
		{
			/*if there is a mover, choose the first one as the avatar*/
			if (!Vector_empty(&world->movers))
			{
				adv_state->avatar = (Mover *)Vector_begin(&world->movers);

				Bool all_added = True;
				for (Mover *m = (Mover *)Vector_begin(&world->movers), *end = (Mover *)Vector_end(&world->movers); m != end; ++m)
				{
					if (!SpacialFinder_add(&adv_state->movers, m, adv_state->memory.allocator))
					{
						all_added = False;
						break;
					}
				}

				if (all_added)
				{
					return (GameState *)adv_state;
				}
			}

			World_free(world, adv_state->memory.deallocator);
		}

		Fauna_free(&adv_state->fauna, adv_state->memory.deallocator);
		SpacialFinder_free(&adv_state->movers, adv_state->memory.deallocator);
		return NULL;
	}
}

static void AdventureState_destroy(GameState *state)
{
	AdventureState * const adv_state = (AdventureState *)state;
	SpacialFinder_free(&adv_state->movers, adv_state->memory.deallocator);
	World_free(&adv_state->world, adv_state->memory.deallocator);
	Fauna_free(&adv_state->fauna, adv_state->memory.deallocator);
	Deallocator_free(adv_state->memory.deallocator, state);
}

static void AdventureState_update(GameState *state, TimeSpan delta, TimePoint now)
{
	AdventureState * const adv_state = (AdventureState *)state;
	World_update(&adv_state->world, delta, now);
}

GameStateDefinition const AdventureStateDef =
{
	AdventureState_create,
	AdventureState_destroy,
	AdventureState_update
};
