#ifndef SAVE_H
#define SAVE_H

#include "base/unused.h"
#include "base/fauna.h"

MOA_USE_RESULT
Bool save_game_to_file(char const *file_name, Mover const *avatar, Fauna const *fauna, MemoryManager memory);

MOA_USE_RESULT
Bool load_game_from_file(char const *file_name, Mover *avatar, Fauna *fauna, MemoryManager memory);

#endif
