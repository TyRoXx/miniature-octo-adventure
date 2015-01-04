#ifndef ENTITY_H
#define ENTITY_H


#include "vector2i.h"
#include "bool.h"
#include "base/timespan.h"
#include "base/unreachable.h"

struct TileGrid;

typedef size_t AppearanceId;

typedef struct PixelPosition
{
	Vector2i vector;
}
PixelPosition;

MOA_USE_RESULT
static inline PixelPosition PixelPosition_new(Vector2i vector)
{
	PixelPosition result = {vector};
	return result;
}


typedef enum Direction
{
	Dir_North,
	Dir_West,
	Dir_South,
	Dir_East
}
Direction;

#define DIR_COUNT 4

MOA_USE_RESULT
static inline Direction2 Direction_to_direction2(Direction dir)
{
	switch (dir)
	{
	case Dir_North: return Direction2_Down;
	case Dir_West: return Direction2_Right;
	case Dir_South: return Direction2_Down;
	case Dir_East: return Direction2_Right;
	}
	MOA_UNREACHABLE();
}

typedef enum AnimationType
{
	Anim_Idle,
	Anim_Move,
	Anim_Die,
	Anim_Dead,
	Anim_COUNT
}
AnimationType;

MOA_USE_RESULT
Vector2i direction_to_vector(Direction dir);

typedef struct Entity
{
	PixelPosition position;
	Direction direction;
	AppearanceId appearance;
	AnimationType animation;
	size_t current_animation_frame;
}
Entity;

MOA_USE_RESULT
Bool Entity_init(
	Entity *e,
	PixelPosition position,
	AppearanceId appearance
	);
void Entity_free(Entity *e);


typedef struct Mover
{
	Entity body;
	size_t steps_to_go;
	TimeSpan remaining_time;
	TimeSpan time_per_pixel;
	TimePoint animation_start;
}
Mover;

void Mover_init(Mover *m,
				TimeSpan time_per_pixel,
				Entity body);
void Mover_free(Mover *m);


int is_possible_step(
	Vector2i const *from,
	Direction dir,
	struct TileGrid const *world
);

void Mover_move(Mover *m, struct TileGrid const *world, size_t steps_to_go);
void Mover_stop(Mover *m);
void Mover_update(Mover *m, struct TileGrid const *world, TimeSpan delta, TimePoint now);


#endif
