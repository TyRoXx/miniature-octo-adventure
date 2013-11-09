#include "font_manager.h"
#include "path.h"
#include <assert.h>


void FontManager_init(FontManager *m)
{
	PtrVector_init(&m->fonts);
}

static char const * const known_fonts[] =
{
    "SourceSansPro-Regular.ttf"
};

Bool FontManager_load(FontManager *m, char const *font_directory)
{
	size_t i, c;
	for (i = 0, c = sizeof(known_fonts) / sizeof(known_fonts[0]); i < c; ++i)
	{
		char * const full_file_name = join_paths(font_directory, known_fonts[i]);
		TTF_Font *font;
		Bool success;
		if (!full_file_name)
		{
			return False;
		}
		font = TTF_OpenFont(full_file_name, 16);
		if (font)
		{
			if (PtrVector_push_back(&m->fonts, font))
			{
				success = True;
			}
			else
			{
				success = False;
			}
		}
		else
		{
			success = False;
			fprintf(stderr, "Could not load font file %s\n", full_file_name);
		}
		free(full_file_name);
		if (!success)
		{
			return False;
		}
	}
	return True;
}

TTF_Font *FontManager_find_font(FontManager const *m, FontID id)
{
	if (id < PtrVector_size(&m->fonts))
	{
		return PtrVector_get(&m->fonts, id);
	}
	return NULL;
}

void FontManager_free(FontManager *m)
{
	size_t i, c;
	for (i = 0, c = PtrVector_size(&m->fonts); i < c; ++i)
	{
		TTF_CloseFont(PtrVector_get(&m->fonts, i));
	}
	PtrVector_free(&m->fonts);
}
