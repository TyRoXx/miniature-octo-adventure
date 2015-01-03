#ifndef SATURATING_INT_H
#define SATURATING_INT_H

#include "min_max.h"
#include "unused.h"
#include <limits.h>

MOA_USE_RESULT
static int saturating_add(int first, int second)
{
	if (first < 0)
	{
		if (second < 0)
		{
			return first + max_int(second, INT_MIN - first);
		}
		else
		{
			return saturating_add(second, first);
		}
	}
	else
	{
		if (second < 0)
		{
			return first + second;
		}
		else
		{
			return first + min_int(second, INT_MAX - first);
		}
	}
}

#endif
