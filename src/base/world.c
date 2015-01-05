#include "world.h"
#include <assert.h>

void World_free(World *w, Deallocator deallocator)
{
	Vector_free(&w->movers, deallocator);
	TileGrid_free(&w->tiles, deallocator);
}

Bool World_add_mover(World *w, Mover mover, Allocator allocator)
{
	return Vector_push_back(&w->movers, &mover, sizeof(mover), allocator);
}

MOA_USE_RESULT
static Bool is_walkable_tile(
	TileGrid const *tiles,
	Vector2i const *position)
{
	if (position->x < 0 ||
		position->y < 0 ||
		position->x >= (ptrdiff_t)tiles->width ||
		position->y >= (ptrdiff_t)tiles->height)
	{
		return False;
	}

	return LayeredTile_is_walkable(
		TileGrid_get(tiles, (size_t)position->x, (size_t)position->y)
		);
}

MOA_USE_RESULT
static int divide_floor(int x, int y)
{
	int q = x / y;
	int r = x % y;
	if ((r != 0) && ((r < 0) != (y < 0)))
	{
		--q;
	}
	return q;
}

MOA_USE_RESULT
static Bool is_walkable_pixel(TileGrid const *world, Vector2i const *position)
{
	Vector2i next_tile;
	next_tile.x = divide_floor(position->x, tile_size);
	next_tile.y = divide_floor(position->y, tile_size);
	return is_walkable_tile(world, &next_tile);
}

static size_t const collision_vertex_count = 2;
static Vector2i const collision_vertex_offsets_by_direction[2][DIR_COUNT] =
{
	{
		{0, 0},
		{0, 0},
		{0, 1},
		{1, 0}
	},
	{
		{1, 0},
		{0, 1},
		{1, 1},
		{1, 1}
	}
};

Bool TileGrid_is_possible_move(
	TileGrid const *world,
	Vector2i const *from,
	Direction direction
	)
{
	Vector2i const pixel_delta = direction_to_vector(direction);
	size_t i;
	for (i = 0; i < collision_vertex_count; ++i)
	{
		Vector2i vertex = collision_vertex_offsets_by_direction[i][direction];
		Vector2i_scale(&vertex, tile_size - 1);
		Vector2i_add(&vertex, from);
		Vector2i_add(&vertex, &pixel_delta);
		if (!is_walkable_pixel(world, &vertex))
		{
			return False;
		}
	}
	return True;
}
