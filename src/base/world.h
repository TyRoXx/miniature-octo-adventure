#ifndef WORLD_H
#define WORLD_H


#include "entity.h"
#include "tile_grid.h"
#include "bool.h"
#include "vector.h"


typedef struct World
{
	TileGrid tiles;
	int tile_width;
	Vector movers;
}
World;

void World_free(World *w);
void World_update(World *w, unsigned delta);
Bool World_add_entity(World *w, Entity const *entity);
Bool World_add_mover(World *w, Mover mover);
Bool World_is_possible_move(
	World const *world,
	PixelPosition const *from,
	Direction direction
	);

void free_movers(Vector *movers);


#endif
