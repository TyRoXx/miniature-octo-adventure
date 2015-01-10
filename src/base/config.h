#ifndef MOA_CONFIG_H
#define MOA_CONFIG_H

#include "unreachable.h"
#include "unused.h"
#include "bool.h"
#include <assert.h>

#ifdef _MSC_VER
#define MOA_INLINE static __inline
#else
#define MOA_INLINE static inline
#endif

#endif
