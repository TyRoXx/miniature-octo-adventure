#ifndef WORLD_H
#define WORLD_H

#include "entity.h"
#include "tile_grid.h"
#include "bool.h"
#include "vector.h"

static int const tile_size = 32;

typedef struct World
{
	TileGrid tiles;
	Vector movers;
}
World;

void World_free(World *w, Deallocator deallocator);

MOA_USE_RESULT
Bool World_add_entity(World *w, Entity const *entity);

MOA_USE_RESULT
Bool World_add_mover(World *w, Mover mover, Allocator allocator);

MOA_USE_RESULT
Bool TileGrid_is_possible_move(
	TileGrid const *world,
	Vector2i const *from,
	Direction direction
	);

#endif
