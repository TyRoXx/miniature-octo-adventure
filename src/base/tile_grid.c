#include "tile_grid.h"
#include <assert.h>

Bool LayeredTile_is_walkable(LayeredTile const *tile)
{
	size_t i;
	for (i = 0; i < TILE_LAYER_COUNT; ++i)
	{
		TileKind const * const layer = tile->layers[i];

		/*cannot walk when there is no ground*/
		if (!layer && (i == 0))
		{
			return False;
		}

		if (layer &&
			!layer->is_walkable)
		{
			return False;
		}
	}
	return True;
}

Bool TileGrid_init(TileGrid *g, size_t width, size_t height, Allocator allocator)
{
	assert(g);
	g->tiles = Allocator_calloc(allocator, width * height, sizeof(*g->tiles));
	g->width = width;
	g->height = height;
	return (g->tiles != 0);
}

void TileGrid_free(TileGrid *g, Deallocator deallocator)
{
	assert(g);
	Deallocator_free(deallocator, g->tiles);
}

LayeredTile *TileGrid_get(TileGrid const *g, size_t x, size_t y)
{
	assert(g);
	assert(x < g->width);
	assert(y < g->height);
	return &g->tiles[y * g->width + x];
}
