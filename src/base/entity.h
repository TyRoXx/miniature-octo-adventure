#ifndef ENTITY_H
#define ENTITY_H


#include "vector2i.h"
#include "bool.h"


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

Vector2i direction_to_vector(Direction dir);

typedef struct Entity
{
	PixelPosition position;
	Direction direction;
	AppearanceId appearance;
	struct World *world;
}
Entity;

Bool Entity_init(
	Entity *e,
	PixelPosition position,
	AppearanceId appearance,
	struct World *world
	);
void Entity_free(Entity *e);


typedef struct Mover
{
	Entity body;
	size_t steps_to_go;
	unsigned remaining_time;
	unsigned time_per_pixel;
}
Mover;

void Mover_init(Mover *m,
				unsigned time_per_pixel,
				Entity body);
void Mover_free(Mover *m);
Bool Mover_move(Mover *m, size_t steps_to_go);
void Mover_stop(Mover *m);
void Mover_update(Mover *m, unsigned delta);


#endif
