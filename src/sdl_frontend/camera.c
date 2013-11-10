#include "camera.h"
#include "base/entity.h"
#include <assert.h>
#include <stdlib.h>


Bool Camera_init(Camera *cam)
{
	assert(cam);
	cam->position.vector = Vector2i_zero;
	return 1;
}

void Camera_free(Camera *cam)
{
	assert(cam);
	(void)cam;
}

void Camera_focus_on(Camera *cam, struct Mover const *target)
{
	assert(cam);
	assert(target);

	cam->position = target->body.position;
}
