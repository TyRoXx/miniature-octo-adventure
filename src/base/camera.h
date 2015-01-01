#ifndef CAMERA_H
#define CAMERA_H


#include "base/vector2i.h"
#include "base/entity.h"
#include "base/unused.h"


typedef struct Camera
{
	PixelPosition position;
}
Camera;


MOA_USE_RESULT
Bool Camera_init(Camera *cam);

void Camera_free(Camera *cam);
void Camera_focus_on(Camera *cam, struct Mover const *target, Vector2i target_size);


#endif
