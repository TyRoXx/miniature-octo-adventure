project(MiniatureOctoAdventure)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(Jansson REQUIRED)
include_directories(${JANSSON_INCLUDE_DIR})

find_package(SDL2)
if(SDL2_FOUND)
	include_directories(${SDL2_INCLUDE_DIR})
	find_package(SDL2_ttf REQUIRED)
	include_directories(${SDL2TTF_INCLUDE_DIR})
endif()

if(NOT SDL2_FOUND)
	find_package(SDL REQUIRED)
	include_directories(${SDL_INCLUDE_DIR})
	find_package(SDL_ttf REQUIRED)
	include_directories(${SDLTTF_INCLUDE_DIR})
endif()

if(UNIX OR MINGW)
	set(CMAKE_C_FLAGS "-Wall -Wextra -Wconversion -Werror -pedantic -Wconversion -Wunused -Wundef")
endif()

if(CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu89")
endif()

if(MSVC)
	add_definitions("/D_CRT_SECURE_NO_WARNINGS")
	add_definitions("/MP")
endif()

include_directories(".")

add_subdirectory(base)
add_subdirectory(gui)
add_subdirectory(frontend)
add_subdirectory(sdl_frontend)
add_subdirectory(game)
add_subdirectory(test)
add_subdirectory(test_main)
