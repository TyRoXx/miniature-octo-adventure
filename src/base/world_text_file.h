#ifndef MAP_TEXT_FILE_H
#define MAP_TEXT_FILE_H


#include "bool.h"
#include "base/allocator.h"
#include <stdio.h>


struct World;
struct TileKind;

MOA_USE_RESULT
Bool load_world_from_text(struct World *world, MemoryManager world_memory, struct TileKind const *tile_kinds, size_t tile_kind_count, FILE *in, FILE *error_out);
void save_world_to_text(struct World const *world, struct TileKind const *tile_kinds, FILE *out);


#endif
