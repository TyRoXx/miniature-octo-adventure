#ifndef APPEARANCE_H
#define APPEARANCE_H


#include "texture_ref.h"
#include "base/entity.h"
#include "base/vector.h"
#include <SDL.h>


struct ImageManager;

typedef struct AnimationFrame
{
	SDL_Rect section;
}
AnimationFrame;

typedef struct AnimationSide
{
	AnimationFrame *frames;
	size_t frame_count;
}
AnimationSide;

MOA_USE_RESULT
Bool AnimationSide_init(AnimationSide *a, size_t frame_count, Allocator allocator);
void AnimationSide_free(AnimationSide *a, Deallocator deallocator);


typedef struct Animation
{
	AnimationSide sides[DIR_COUNT];
}
Animation;

void Animation_free(Animation *a, Deallocator deallocator);


typedef struct AppearanceLayout
{
	Animation animations[Anim_COUNT];
	Vector2i offset;
}
AppearanceLayout;


void AppearanceLayout_free(AppearanceLayout *a, Deallocator deallocator);


typedef struct Appearance
{
	SDL_Surface *image;
	AppearanceLayout const *layout;
}
Appearance;

void Appearance_init(Appearance *appearance,
					 SDL_Surface *image,
					 AppearanceLayout const *layout);
void Appearance_free(Appearance *appearance);

typedef struct AppearanceManager
{
	AppearanceLayout static_layout;
	AppearanceLayout dynamic_layout_1;
	AppearanceLayout dynamic_layout_2;
	Vector appearances;
}
AppearanceManager;

MOA_USE_RESULT
Bool AppearanceManager_init(AppearanceManager *a, MemoryManager appearances_memory);

MOA_USE_RESULT
Bool AppearanceManager_parse_file(
        AppearanceManager *a,
		MemoryManager appearances_memory,
        char const *begin,
        size_t length,
        struct ImageManager *images);
void AppearanceManager_free(AppearanceManager *a, Deallocator deallocator);

MOA_USE_RESULT
Appearance const *AppearanceManager_get(AppearanceManager const *a,
										AppearanceId id);


#endif
