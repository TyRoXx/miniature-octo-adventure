#include "number_generator.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

uint8_t NumberGenerator_uniform_8(NumberGenerator generator, uint8_t min, uint8_t max)
{
	assert(min <= max);
	/*throw the upper 24 bits away*/
	uint8_t original = (uint8_t)NumberGenerator_generate_32_bit(generator);
	uint8_t range = (uint8_t)(max - min);
	unsigned scaled_up = (unsigned)original * (unsigned)(range + 1);
	uint8_t scaled_to_original = (uint8_t)(scaled_up / 256);
	return (uint8_t)(min + scaled_to_original);
}

uint32_t NumberGenerator_uniform_32(NumberGenerator generator, uint32_t min, uint32_t max)
{
	uint32_t original = NumberGenerator_generate_32_bit(generator);
	uint32_t range = max - min;
	uint64_t scaled_up = (uint64_t)original * (uint64_t)(range + 1);
	uint32_t scaled_to_original = (uint32_t)(scaled_up / 0x100000000ULL);
	return (min + scaled_to_original);
}

static uint32_t generate_32_bit_from_device(void *state)
{
	uint32_t result = 0;
#ifdef _WIN32
	DWORD generated = 0;
	if (!ReadFile(state, &result, sizeof(result), &generated, NULL))
	{
		abort();
	}
	if (generated != sizeof(result))
	{
		abort();
	}
#else
	int file = (int)(ptrdiff_t)state;
	ssize_t generated = read(file, &result, sizeof(result));
	if (generated != sizeof(result))
	{
		abort();
	}
#endif
	return result;
}

NumberGenerator make_kernel_number_generator(int file)
{
	NumberGenerator generator = {generate_32_bit_from_device, (void *)(ptrdiff_t)file};
	return generator;
}
