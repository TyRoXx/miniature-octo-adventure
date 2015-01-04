#ifndef ENTITY_H
#define ENTITY_H


#include "vector2i.h"
#include "bool.h"
#include "base/unused.h"


struct World;


typedef size_t AppearanceId;


typedef struct PixelPosition
{
	Vector2i vector;
}
PixelPosition;


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
Vector2i direction_to_vector(Direction dir);

typedef struct Entity
{
	PixelPosition position;
	Direction direction;
	AppearanceId appearance;
	Bool is_walking;
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
	unsigned remaining_time;
	unsigned time_per_pixel;
	unsigned since_animation_frame_change;
}
Mover;

void Mover_init(Mover *m,
				unsigned time_per_pixel,
				Entity body);
void Mover_free(Mover *m);


int is_possible_step(
	PixelPosition const *from,
	Direction dir,
	struct World const *world
);

void Mover_move(Mover *m, struct World const *world, size_t steps_to_go);
void Mover_stop(Mover *m);
void Mover_update(Mover *m, struct World const *world, unsigned delta);


#endif
