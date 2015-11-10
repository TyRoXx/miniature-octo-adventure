[![Build Status](https://travis-ci.org/TyRoXx/miniature-octo-adventure.svg)](https://travis-ci.org/TyRoXx/miniature-octo-adventure)

miniature octo adventure (MOA)
==============================

MOA is a small 2D RPG for learning C and SDL.

Caution: Programmer art inside!

how to build and run
====================

for Desktops
------------

The game will usually compile and run on Ubuntu, Debian and other Linux
distributions. Chances are that BSDs work, too.
The game is written in C99, so any Clang or non-ancient GCC should work.

Sometimes the game is adapted to Windows 7 with Visual C++ 2013 or later.
It will not work with Visual C++ 2012 or earlier because they lack
required C99 features.

You need to have installed:
- CMake 2.8+ http://cmake.org
- GCC or Clang
- SDL 1.2 or 2.0
- SDL_TTF 2
- SDL_image with PNG support
- Jansson 2.4+
It might work with older versions of the dependencies.

On Debian or Ubuntu you can install the dependencies like this:
```sh
sudo apt-get install cmake build-essential libsdl2-dev libsdl-ttf2.0-dev libsdl2-image-dev libjansson-dev
```

Create a new directory for the build output somewhere and change into it.
Assuming that the MOA repository is at *~/moa*:
```sh
cmake ~/moa
make
```

If the build was successful, there should be an executable *game* in *game/* now.

When running the *game* the *data/* directory has to be available in the current directory.
You can either run the program from the root of this repository or create a symlink to *data/* in
the *game/* directory.

for Nintendo DS
---------------

This is currently only an experiment.
The game is not playable at all on the NDS and shows only some static
sprites.

Install devkitARM as described at http://devkitpro.org/wiki/Getting_Started/devkitARM.

```sh
cd src/nds_main
make
```

There should be a *nds_main.nds* file in the current directory. You can run this executable using an NDS or an emulator, for example Desmume.
