#include "vector.h"
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

void Vector_init(Vector *v)
{
	v->data = 0;
	v->capacity = v->size = 0;
}

void Vector_free(Vector *v, Deallocator deallocator)
{
	Deallocator_free(deallocator, v->data);
}

char *Vector_release(Vector *v)
{
	return v->data;
}

Bool Vector_push_back(Vector *v, void const *element, size_t size, Allocator allocator)
{
	if (!Vector_reserve(v, v->size + size, allocator))
	{
		return False;
	}
	memcpy(v->data + v->size, element, size);
	v->size += size;
	return True;
}

size_t Vector_size(Vector const *v)
{
	return v->size;
}

Bool Vector_empty(Vector const *v)
{
	return v->size == 0;
}

char *Vector_data(Vector const *v)
{
	return v->data;
}

Bool Vector_reserve(Vector *v, size_t capacity, Allocator allocator)
{
	char *new_data;

	if (v->capacity >= capacity)
	{
		return True;
	}

	capacity *= 2;

	new_data = Allocator_realloc(allocator, v->data, capacity);
	if (!new_data)
	{
		return False;
	}

	v->data = new_data;
	v->capacity = capacity;
	return True;
}

Bool Vector_resize(Vector *v, size_t size, Allocator allocator)
{
	size_t const old_size = v->size;
	if (Vector_reserve(v, size, allocator))
	{
		if (size > old_size)
		{
			memset(v->data + old_size, 0, size - old_size);
		}
		v->size = size;
		return True;
	}
	return False;
}

void Vector_resize_smaller(Vector *v, size_t size)
{
	assert(size <= v->size);
	v->size = size;
}

char *Vector_begin(Vector const *v)
{
	return Vector_data(v);
}

char *Vector_end(Vector const *v)
{
	return Vector_data(v) + Vector_size(v);
}

Bool Vector_append_binary_file(Vector *v, Allocator v_allocator, FILE *in)
{
	size_t const original_size = Vector_size(v);
	size_t buffer_size = 8192;
	for (;;)
	{
		size_t const offset_to_read_to = Vector_size(v);
		size_t const max_size_t = ~(size_t)0;
		size_t resizing = original_size + buffer_size;
		size_t read_size;
		size_t actually_read;
		if (resizing > max_size_t)
		{
			resizing = max_size_t;
		}
		if (!Vector_resize(v, resizing, v_allocator))
		{
			Vector_resize_smaller(v, original_size);
			return False;
		}
		read_size = resizing - offset_to_read_to;
		actually_read = fread(Vector_data(v) + offset_to_read_to, 1, read_size, in);
		if (actually_read < read_size)
		{
			Vector_resize_smaller(v, offset_to_read_to + actually_read);
			return True;
		}
	}
}

Bool Vector_printf(Vector *v, Allocator allocator, char const *format, ...)
{
	size_t const speculative_result_length = strlen(format) * 2;
	va_list args;
	ptrdiff_t printed;

	if (!Vector_resize(v, speculative_result_length, allocator))
	{
		return False;
	}

	va_start(args, format);
	printed = vsnprintf(Vector_data(v), Vector_size(v), format, args);
	assert(printed >= 0);
	if ((size_t)printed == Vector_size(v))
	{
		if (!Vector_resize(v, (size_t)printed + 1, allocator))
		{
			va_end(args);
			return False;
		}
		printed = vsnprintf(Vector_data(v), Vector_size(v), format, args);
		assert((size_t)printed == Vector_size(v));
	}
	Vector_resize_smaller(v, (size_t)printed + 1);
	va_end(args);
	return True;
}
