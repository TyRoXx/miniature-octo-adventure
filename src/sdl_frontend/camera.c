#include "camera.h"
#include "base/entity.h"
#include <assert.h>
#include <stdlib.h>


Bool Camera_init(Camera *cam)
{
	assert(cam);
	cam->position = Vector2f_zero;
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

	cam->position.x = (float)(target->body.position.vector.x) / 32;
	cam->position.y = (float)(target->body.position.vector.y) / 32;
}
