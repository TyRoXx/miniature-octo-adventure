#ifndef TEXTURE_REF_H
#define TEXTURE_REF_H


#include "base/unused.h"
#include <SDL.h>


typedef struct TextureRef
{
	SDL_Surface *surface;
	SDL_Rect section;
}
TextureRef;

MOA_USE_RESULT
TextureRef TextureRef_full(SDL_Surface *surface);


#endif
