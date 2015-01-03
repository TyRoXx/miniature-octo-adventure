#include "avatar_controller.h"
#include "base/game.h"
#include "base/algorithm.h"
#include <assert.h>
#include <string.h>


static int const not_pressed = -1;

Bool AvatarController_init(AvatarController *a, struct Mover *avatar)
{
	assert(a);
	assert(avatar);
	a->avatar = avatar;
	for (size_t i = 0; i < MOA_ARRAY_SIZE(a->pressed_direction_keys); ++i)
	{
		a->pressed_direction_keys[i] = not_pressed;
	}
	return True;
}

void AvatarController_free(AvatarController *a)
{
	(void)a;
}

void AvatarController_handle_input(AvatarController *a,
								   World const *world,
								   Direction dir,
								   Bool is_down)
{
	Mover * const avatar = a->avatar;
	if (!avatar)
	{
		return;
	}

	if (is_down)
	{
		memmove(
			a->pressed_direction_keys + 1,
			a->pressed_direction_keys,
			sizeof(*a->pressed_direction_keys) * (MOA_ARRAY_SIZE(a->pressed_direction_keys) - 1)
		);
		a->pressed_direction_keys[0] = dir;
		for (size_t i = 1; i < MOA_ARRAY_SIZE(a->pressed_direction_keys); ++i)
		{
			if (a->pressed_direction_keys[i] == (int)dir)
			{
				memmove(
					a->pressed_direction_keys + i,
					a->pressed_direction_keys + i + 1,
					sizeof(*a->pressed_direction_keys) * (MOA_ARRAY_SIZE(a->pressed_direction_keys) - i - 1)
				);
				MOA_ARRAY_END(a->pressed_direction_keys)[-1] = not_pressed;
				break;
			}
		}
		if (avatar->steps_to_go == 0)
		{
			avatar->body.direction = dir;
			Mover_move(avatar, world, (size_t)-1);
		}
	}
	else
	{
		for (size_t i = 0; i < MOA_ARRAY_SIZE(a->pressed_direction_keys); ++i)
		{
			if (a->pressed_direction_keys[i] == (int)dir)
			{
				memmove(
					a->pressed_direction_keys + i,
					a->pressed_direction_keys + i + 1,
					sizeof(*a->pressed_direction_keys) * (MOA_ARRAY_SIZE(a->pressed_direction_keys) - i - 1)
				);
				MOA_ARRAY_END(a->pressed_direction_keys)[-1] = not_pressed;
				break;
			}
		}
	}
}

void AvatarController_update(AvatarController *a, World const *world)
{
	Mover * const avatar = a->avatar;
	if (!avatar)
	{
		return;
	}

	for (size_t i = 0; i < 2; ++i)
	{
		int input_dir = a->pressed_direction_keys[i];
		if (input_dir == not_pressed)
		{
			if (avatar->steps_to_go > 0)
			{
				Mover_stop(avatar);
			}
			break;
		}
		avatar->body.direction = (Direction)input_dir;
		if (is_possible_step(&avatar->body.position, avatar->body.direction, world))
		{
			Mover_move(avatar, world, (size_t)-1);
			break;
		}
	}
}
