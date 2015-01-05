#ifndef AVATAR_CONTROLLER_H
#define AVATAR_CONTROLLER_H

#include "base/unused.h"
#include "base/entity.h"

struct Mover;

typedef struct AvatarController
{
	struct Mover *avatar;
	int pressed_direction_keys[4];
}
AvatarController;

void AvatarController_init(AvatarController *a, struct Mover *avatar);

void AvatarController_handle_input(AvatarController *a,
								   struct TileGrid const *world,
								   Direction dir,
								   Bool is_down);
void AvatarController_update(AvatarController *a, struct TileGrid const *world);

#endif
