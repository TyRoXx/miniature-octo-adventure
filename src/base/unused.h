#ifndef UNUSED_H
#define UNUSED_H


#ifdef __GNUC__
#	define MOA_UNUSED __attribute__((unused))
#else
#	define MOA_UNUSED /*TODO*/
#endif

#ifdef _MSC_VER
#	define MOA_USE_RESULT _Check_return_
#else
#	define MOA_USE_RESULT __attribute__((warn_unused_result))
#endif


#endif
