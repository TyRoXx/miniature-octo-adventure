#ifndef UNREACHABLE_H
#define UNREACHABLE_H


#ifdef _MSC_VER
#define MOA_UNREACHABLE() __assume(0)
#else
#define MOA_UNREACHABLE() __builtin_unreachable()
#endif


#endif
