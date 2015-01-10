#ifndef TIMESPAN_H
#define TIMESPAN_H

#include "base/config.h"

typedef struct TimeSpan
{
	unsigned milliseconds;
}
TimeSpan;

MOA_INLINE void TimeSpan_add(TimeSpan *first, TimeSpan second)
{
	first->milliseconds += second.milliseconds;
}

MOA_USE_RESULT
MOA_INLINE TimeSpan TimeSpan_from_milliseconds(unsigned milliseconds)
{
	TimeSpan result = { milliseconds };
	return result;
}

typedef struct TimePoint
{
	unsigned milliseconds;
}
TimePoint;

MOA_USE_RESULT
MOA_INLINE TimePoint TimePoint_from_milliseconds(unsigned milliseconds)
{
	TimePoint result = { milliseconds };
	return result;
}

MOA_USE_RESULT
MOA_INLINE TimeSpan TimePoint_between(TimePoint first, TimePoint second)
{
	assert(first.milliseconds <= second.milliseconds);
	return TimeSpan_from_milliseconds(second.milliseconds - first.milliseconds);
}

MOA_USE_RESULT
MOA_INLINE Bool TimePoint_later(TimePoint first, TimePoint second)
{
	return (second.milliseconds > first.milliseconds);
}

MOA_USE_RESULT
MOA_INLINE TimePoint TimePoint_add(TimePoint base, TimeSpan delta)
{
	return TimePoint_from_milliseconds(base.milliseconds + delta.milliseconds);
}

#endif
