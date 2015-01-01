#include "data.h"
#include "path.h"
#include <assert.h>
#include <SDL_ttf.h>


static Bool init_appearances(
		AppearanceManager *appearances,
		char const *data_directory,
		ImageManager *images,
		MemoryManager memory)
{
	char * const appearance_file_name = join_paths(data_directory, "appearances.txt", memory);
	FILE *file;
	Bool result;

	if (!appearance_file_name)
	{
		return False;
	}

	file = fopen(appearance_file_name, "rb");
	if (!file)
	{
		fprintf(stderr, "Could not open file %s\n", appearance_file_name);
		Deallocator_free(memory.deallocator, appearance_file_name);
		return False;
	}

	Deallocator_free(memory.deallocator, appearance_file_name);
	result = AppearanceManager_init(appearances, memory);
	if (result)
	{
		Vector content;
		Vector_init(&content);
		result = Vector_append_binary_file(&content, memory.allocator, file);
		if (result)
		{
			result = AppearanceManager_parse_file(appearances, memory, Vector_data(&content), Vector_size(&content), images);
		}
		Vector_free(&content, memory.deallocator);
	}
	fclose(file);
	return result;
}

Bool Data_init(Data *d,
			   char const *directory,
			   SDL_PixelFormat *format,
			   MemoryManager memory)
{
	char *image_directory = join_paths(directory, "sprites", memory);
	Bool success;
	if (!image_directory)
	{
		return False;
	}

	ImageManager_init(&d->images, image_directory, format);
	if (init_appearances(&d->appearances, directory, &d->images, memory))
	{
		char * const font_directory = join_paths(directory, "fonts", memory);
		if (font_directory)
		{
			FontManager_init(&d->fonts);
			if (FontManager_load(&d->fonts, memory, font_directory))
			{
				success = True;
			}
			else
			{
				FontManager_free(&d->fonts, memory.deallocator);
				success = False;
			}
			Deallocator_free(memory.deallocator, font_directory);
			return success;
		}
	}
	ImageManager_free(&d->images, memory.deallocator);
	return False;
}

void Data_free(Data *d, Deallocator deallocator)
{
	FontManager_free(&d->fonts, deallocator);
	AppearanceManager_free(&d->appearances, deallocator);
	ImageManager_free(&d->images, deallocator);
}
