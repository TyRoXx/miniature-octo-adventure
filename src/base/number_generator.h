#ifndef NUMBER_GENERATOR_H
#define NUMBER_GENERATOR_H

#include "base/config.h"
#include <stdint.h>

typedef struct NumberGenerator
{
	uint32_t (*generate_32_bit)(void *);
	void *state;
}
NumberGenerator;

MOA_USE_RESULT
MOA_INLINE uint32_t NumberGenerator_generate_32_bit(NumberGenerator generator)
{
	return generator.generate_32_bit(generator.state);
}

uint8_t NumberGenerator_uniform_8(NumberGenerator generator, uint8_t min, uint8_t max);
uint32_t NumberGenerator_uniform_32(NumberGenerator generator, uint32_t min, uint32_t max);

NumberGenerator make_kernel_number_generator(int file);

#endif
