#include "image_manager.h"
#include "path.h"
#include "base/algorithm.h"
#include "SDL_image.h"
#include <assert.h>
#include <string.h>

static SDL_Surface *load_texture(char const *file_name,
									 SDL_PixelFormat *format)
{
	SDL_Color const AlphaKey = {255, 0, 255, 0};
	SDL_Surface * const bitmap = IMG_Load(file_name);
	SDL_Surface * converted;

	assert(format);

	if (!bitmap)
	{
		fprintf(stderr, "Could not load image %s\n", file_name);
		return NULL;
	}

	converted = SDL_ConvertSurface(bitmap, format, 0);
	if (!converted)
	{
		fprintf(stderr, "Could not convert image %s\n", file_name);
		return bitmap;
	}

	SDL_FreeSurface(bitmap);

	if (SDL_SetColorKey(
			converted,
#if SDL_MAJOR_VERSION >= 2
			SDL_TRUE,
#else
			SDL_SRCCOLORKEY,
#endif
			SDL_MapRGB(converted->format, AlphaKey.r, AlphaKey.g, AlphaKey.b)) < 0)
	{
		SDL_FreeSurface(converted);
		return NULL;
	}

	return converted;
}

void Image_init(Image *im, char *name, SDL_Surface *surface)
{
	im->name = name;
	im->surface = surface;
}

void Image_free(Image *im, Deallocator deallocator)
{
	Deallocator_free(deallocator, im->name);
	SDL_FreeSurface(im->surface);
}


void ImageManager_init(ImageManager *im,
					   char *directory,
					   SDL_PixelFormat *format)
{
	im->directory = directory;
	im->format = format;
	Vector_init(&im->images);
}

static void free_image(void *image, void *user)
{
	Deallocator *deallocator = user;
	Image_free(image, *deallocator);
}

void ImageManager_free(ImageManager *im, Deallocator images_deallocator)
{
	for_each(Vector_begin(&im->images),
			 Vector_end(&im->images),
			 sizeof(Image),
			 free_image,
			 &images_deallocator);
	Vector_free(&im->images, images_deallocator);
	Deallocator_free(images_deallocator, im->directory);
}

static Bool is_image_name(void *element, void *user)
{
	Image const * const image = element;
	return !strcmp(image->name, user);
}

static Image *add_image(
	ImageManager *manager,
	char *name,
	MemoryManager memory)
{
	char * const full_name = join_paths(manager->directory, name, memory);
	Image image;

	if (!full_name)
	{
		return NULL;
	}

	image.name = name;
	image.surface = load_texture(full_name, manager->format);
	Deallocator_free(memory.deallocator, full_name);

	if (!image.surface)
	{
		return NULL;
	}

	if (!Vector_push_back(&manager->images, &image, sizeof(image), memory.allocator))
	{
		SDL_FreeSurface(image.surface);
		return NULL;
	}

	return ((Image *)Vector_end(&manager->images)) - 1;
}

static char *copy_string(char const *str, Allocator allocator)
{
	size_t const length = strlen(str) + 1;
	char *copy = Allocator_alloc(allocator, length);
	if (copy)
	{
		memcpy(copy, str, length);
	}
	return copy;
}

SDL_Surface *ImageManager_get(ImageManager *im, char const *name, MemoryManager memory)
{
	Image *created;
	Image * const end = (Image *)Vector_end(&im->images);
	Image * const existing = find(Vector_begin(&im->images),
		 end,
		 sizeof(Image),
		 is_image_name,
		 (void *)name
		 );
	char *name_copy;

	if (existing != end)
	{
		return existing->surface;
	}

	name_copy = copy_string(name, memory.allocator);
	created = add_image(im, name_copy, memory);

	if (created)
	{
		return created->surface;
	}

	Deallocator_free(memory.deallocator, name_copy);
	return NULL;
}
