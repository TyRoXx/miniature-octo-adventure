#include "world.h"
#include "algorithm.h"
#include <stdlib.h>
#include <assert.h>


void World_free(World *w)
{
	free_movers(&w->movers);
	TileGrid_free(&w->tiles);
}

static void update_mover(void *element, void *user)
{
	unsigned const delta = *(unsigned *)user;
	(void)user;
	Mover_update(element, delta);
}

void World_update(World *w, unsigned delta)
{
	for_each(Vector_begin(&w->movers),
			 Vector_end(&w->movers),
			 sizeof(Mover),
			 update_mover,
			 &delta);
}

Bool World_add_mover(World *w, Mover mover)
{
	return Vector_push_back(&w->movers, &mover, sizeof(mover));
}

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

static Bool is_walkable_pixel(World const *world, Vector2i const *position)
{
	Vector2i next_tile;
	next_tile.x = divide_floor(position->x, world->tile_width);
	next_tile.y = divide_floor(position->y, world->tile_width);
	return is_walkable_tile(&world->tiles, &next_tile);
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

Bool World_is_possible_move(
	World const *world,
	PixelPosition const *from,
	Direction direction
	)
{
	Vector2i const pixel_delta = direction_to_vector(direction);
	size_t i;
	for (i = 0; i < collision_vertex_count; ++i)
	{
		Vector2i vertex = collision_vertex_offsets_by_direction[i][direction];
		Vector2i_scale(&vertex, world->tile_width - 1);
		Vector2i_add(&vertex, &from->vector);
		Vector2i_add(&vertex, &pixel_delta);
		if (!is_walkable_pixel(world, &vertex))
		{
			return False;
		}
	}
	return True;
}

static void free_mover(void *element, void *user)
{
	(void)user;
	Mover_free(element);
}

void free_movers(Vector *movers)
{
	for_each(Vector_begin(movers),
			 Vector_end(movers),
			 sizeof(Mover),
			 free_mover,
			 NULL);
	Vector_free(movers);
}
