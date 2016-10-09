file(GLOB sources "*.h" "*.c")

add_executable(game ${sources})

target_link_libraries(game sdl_frontend gui base ${CONAN_LIBS})

if(MINGW)
	target_link_libraries(game "-lmingw32")
endif()

if(UNIX)
	target_link_libraries(game "-lm")
endif()
