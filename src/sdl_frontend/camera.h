#ifndef CAMERA_H
#define CAMERA_H


#include "base/vector2f.h"
#include "base/entity.h"


typedef struct Camera
{
	Vector2f position;
}
Camera;


Bool Camera_init(Camera *cam);
void Camera_free(Camera *cam);
void Camera_focus_on(Camera *cam, struct Mover const *target);


#endif
