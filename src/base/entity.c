#include "entity.h"
#include "world.h"
#include <assert.h>


Vector2i direction_to_vector(Direction dir)
{
	Vector2i v = {0, 0};
	switch (dir)
	{
	case Dir_North: v.y = -1; break;
	case Dir_West : v.x = -1; break;
	case Dir_South: v.y =  1; break;
	case Dir_East : v.x =  1; break;
	}
	return v;
}

Bool Entity_init(
	Entity *e,
	PixelPosition position,
	AppearanceId appearance
	)
{
	e->position = position;
	e->direction = Dir_South;
	e->appearance = appearance;
	return True;
}

void Entity_free(Entity *e)
{
	(void)e;
}


void Mover_init(Mover *m,
				unsigned time_per_pixel,
				Entity body)
{
	m->time_per_pixel = time_per_pixel;
	m->body = body;
	m->remaining_time = 0;
	m->steps_to_go = 0;
}

void Mover_free(Mover *m)
{
	Entity_free(&m->body);
}

int is_possible_step(
	Entity const *entity,
	Direction dir,
	World const *world
	)
{
	return World_is_possible_move(world, &entity->position, dir);
}

void Mover_move(Mover *m, World const *world, size_t steps_to_go)
{
	if (m->steps_to_go > 0)
	{
		return;
	}

	if (!is_possible_step(&m->body, m->body.direction, world))
	{
		return;
	}

	m->steps_to_go = steps_to_go;
	m->remaining_time = 0;
}

void Mover_stop(Mover *m)
{
	assert(m);
	assert(m->steps_to_go > 0);

	m->steps_to_go = 1;
}

static void add_step(
	PixelPosition *dest,
	Direction dir)
{
	Vector2i delta = direction_to_vector(dir);
	Vector2i_add(&dest->vector, &delta);
}

void Mover_update(Mover *m, World const *world, unsigned delta)
{
	if (m->steps_to_go > 0)
	{
		m->remaining_time += delta;
		while (m->remaining_time >= m->time_per_pixel)
		{
			if (m->steps_to_go != (size_t)-1)
			{
				--(m->steps_to_go);
				if (m->steps_to_go == 0)
				{
					break;
				}
			}

			if (!is_possible_step(&m->body, m->body.direction, world))
			{
				m->steps_to_go = 0;
				break;
			}

			add_step(&m->body.position, m->body.direction);

			m->remaining_time -= m->time_per_pixel;
		}
	}
}
