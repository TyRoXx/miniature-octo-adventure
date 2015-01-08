file(GLOB sources "*.h" "*.c")

add_executable(game ${sources})

target_link_libraries(game sdl_frontend gui base)

if(MINGW)
	target_link_libraries(game "-lmingw32")
	target_link_libraries(game ${SDLMAIN_LIBRARY})
endif()

if(MSVC)
	target_link_libraries(game ${SDLMAIN_LIBRARY})
endif()

target_link_libraries(game ${JANSSON_LIBRARY})
if(SDL2_FOUND)
	target_link_libraries(game ${SDL2TTF_LIBRARY} ${SDL2IMAGE_LIBRARY} ${SDL2_LIBRARY})
else()
	target_link_libraries(game ${SDLTTF_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDL_LIBRARY})
endif()

if(UNIX)
	target_link_libraries(game "-lm")
endif()
