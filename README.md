miniature-octo-adventure
========================

how to build
============

for Unix-like
-------------

You need to have installed:
- CMake 2.8+ http://cmake.org
- GCC or Clang
- SDL 1.2
- SDL_TTF 2
- Jansson 2.4+

On Ubuntu you can install the dependencies like this:
```sh
sudo apt-get install cmake build-essential libsdl1.2-dev libsdl-ttf2.0-dev libjansson-dev
```

Create a new directory for the build output somewhere and change into it.
Assuming that the MOA repository is at *~/moa*:
```sh
cmake ~/moa
make
```

If the build was successful, there should be an executable *game* in *game/* now.

for Nintendo DS
---------------

Note that the game is not playable at all on the NDS currently.

Install devkitARM as described at http://devkitpro.org/wiki/Getting_Started/devkitARM.

```sh
cd src/nds_main
make
```

There should be a *nds_main.nds* file in the current directory. You can run this executable using an NDS or an emulator, for example Desmume.
