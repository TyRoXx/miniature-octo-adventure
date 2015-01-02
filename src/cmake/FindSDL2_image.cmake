FIND_PATH(SDL2IMAGE_INCLUDE_DIR SDL_image.h
	HINTS
	$ENV{SDL2IMAGEDIR}
	$ENV{SDL2DIR}
	PATH_SUFFIXES include
	PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local/include/SDL2
	/usr/include/SDL2
	/sw/include/SDL2 # Fink
	/opt/local/include/SDL2 # DarwinPorts
	/opt/csw/include/SDL2 # Blastwave
	/opt/include/SDL2
)
FIND_LIBRARY(SDL2IMAGE_LIBRARY
	NAMES SDL2_image
	HINTS
	$ENV{SDL2IMAGEDIR}
	$ENV{SDL2DIR}
	PATH_SUFFIXES lib64 lib
	PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw
	/opt/local
	/opt/csw
	/opt
)
SET(SDL2IMAGE_FOUND "NO")
IF(SDL2IMAGE_LIBRARY AND SDL2IMAGE_INCLUDE_DIR)
	SET(SDL2IMAGE_FOUND "YES")
ENDIF(SDL2IMAGE_LIBRARY AND SDL2IMAGE_INCLUDE_DIR)
