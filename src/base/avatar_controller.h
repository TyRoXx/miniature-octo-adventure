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

MOA_USE_RESULT
Bool AvatarController_init(AvatarController *a, struct Mover *avatar);

void AvatarController_free(AvatarController *a);
void AvatarController_handle_input(AvatarController *a,
								   struct World const *world,
								   Direction dir,
								   Bool is_down);
void AvatarController_update(AvatarController *a, struct World const *world);

#endif
