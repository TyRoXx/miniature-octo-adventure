#ifndef UNUSED_H
#define UNUSED_H


#ifdef __GNUC__
#	define MOA_UNUSED __attribute__((unused))
#else
#	define MOA_UNUSED /*TODO*/
#endif


#endif
