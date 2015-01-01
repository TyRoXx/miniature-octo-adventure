#ifndef FRONTEND_H
#define FRONTEND_H


#include "base/unused.h"
#include "base/bool.h"

struct FrontendType;


typedef struct Frontend
{
	struct FrontendType const *type;
}
Frontend;


typedef struct FrontendType
{
	void (*destroy)(Frontend *);
	MOA_USE_RESULT Bool (*main_loop)(Frontend *);
}
FrontendType;


#endif
