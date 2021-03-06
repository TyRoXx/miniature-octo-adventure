#include "camera.h"
#include "base/entity.h"
#include <assert.h>

void Camera_init(Camera *cam)
{
	assert(cam);
	cam->position.vector = Vector2i_zero;
}

void Camera_focus_on(Camera *cam, struct Mover const *target, Vector2i target_size)
{
	assert(cam);
	assert(target);

	cam->position = target->body.position;
	cam->position.vector.x += target_size.x / 2;
	cam->position.vector.y += target_size.y / 2;
}
