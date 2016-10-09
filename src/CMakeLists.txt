project(MiniatureOctoAdventure C)
cmake_minimum_required(VERSION 2.8)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if(UNIX OR MINGW)
	set(CMAKE_C_FLAGS "-Wall -Wextra -Wconversion -Werror -pedantic -Wconversion -Wunused -Wundef")
endif()

if(CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
endif()

if(MSVC)
	add_definitions("/D_CRT_SECURE_NO_WARNINGS")
	add_definitions("/MP")
	add_definitions("/W4")
	add_definitions("/wd4204")
	add_definitions("/wd4221")
endif()

include_directories(".")

add_subdirectory(base)
add_subdirectory(gui)
add_subdirectory(sdl_frontend)
add_subdirectory(game)
add_subdirectory(test)
add_subdirectory(test_main)
