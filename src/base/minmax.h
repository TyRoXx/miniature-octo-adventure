#ifndef MINMAX_H
#define MINMAX_H


#include "unused.h"


#define DEFINE_MAX_FUNCTION(type_) \
type_ max_ ## type_(type_ left, type_ right) \
{ \
	return (left > right) ? left : right; \
}

#define DEFINE_MIN_FUNCTION(type_) \
type_ min_ ## type_(type_ left, type_ right) \
{ \
	return (left < right) ? left : right; \
}


#define DEFINE_STATIC_MIN_MAX(type_) \
static MOA_UNUSED DEFINE_MIN_FUNCTION(type_) \
static MOA_UNUSED DEFINE_MAX_FUNCTION(type_)

DEFINE_STATIC_MIN_MAX(int)
DEFINE_STATIC_MIN_MAX(ptrdiff_t)


#endif
