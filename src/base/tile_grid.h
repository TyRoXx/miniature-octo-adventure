#ifndef TILE_GRID_H
#define TILE_GRID_H

#include "bool.h"
#include "base/allocator.h"
#include <stddef.h>

typedef struct TileKind
{
	size_t image_id;
	int is_walkable;
}
TileKind;

#define TILE_LAYER_COUNT 3

typedef struct LayeredTile
{
	TileKind const *layers[TILE_LAYER_COUNT];
}
LayeredTile;

MOA_USE_RESULT
Bool LayeredTile_is_walkable(LayeredTile const *tile);


typedef struct TileGrid
{
	LayeredTile *tiles; /*malloc'ed*/
	size_t width, height;
}
TileGrid;

MOA_USE_RESULT
Bool TileGrid_init(TileGrid *g, size_t width, size_t height, Allocator allocator);
void TileGrid_init_empty(TileGrid *g);
void TileGrid_free(TileGrid *g, Deallocator deallocator);

MOA_USE_RESULT
LayeredTile *TileGrid_get(TileGrid const *g, size_t x, size_t y);


#endif
