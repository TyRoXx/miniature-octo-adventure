#ifndef TIMESPAN_H
#define TIMESPAN_H

#include "base/unused.h"

typedef struct TimeSpan
{
	unsigned milliseconds;
}
TimeSpan;

static inline void TimeSpan_add(TimeSpan *first, TimeSpan second)
{
	first->milliseconds += second.milliseconds;
}

MOA_USE_RESULT
static inline TimeSpan TimeSpan_from_milliseconds(unsigned milliseconds)
{
	TimeSpan result = { milliseconds };
	return result;
}

#endif
