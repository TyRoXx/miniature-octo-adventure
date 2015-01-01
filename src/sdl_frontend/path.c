#include "path.h"
#include "base/vector.h"
#include <assert.h>
#include <string.h>


static Bool ends_with_separator(char const *data, size_t size)
{
	if (size > 0)
	{
		return (data[size - 1] == '/');
	}
	return False;
}

static Bool append_path(Vector *left, Allocator left_allocator, char const *right, size_t right_size)
{
	Bool need_separator;
	size_t const left_previous_size = Vector_size(left);

	if (right_size == 0)
	{
		return True;
	}

	need_separator =
			!ends_with_separator(Vector_data(left), left_previous_size) &&
			(*right != '/');

	if (need_separator)
	{
		char const separator[1] = "/";
		if (!Vector_push_back(left, &separator, sizeof(separator), left_allocator))
		{
			return False;
		}
	}

	if (!Vector_push_back(left, right, right_size + 1, left_allocator))
	{
		Vector_resize_smaller(left, left_previous_size);
		return False;
	}

	return True;
}

static Bool zero_terminate(Vector *str, Allocator str_allocator)
{
	char const terminator = '\0';
	return Vector_push_back(str, &terminator, 1, str_allocator);
}

char *join_paths(char const *left, char const *right, MemoryManager memory)
{
	Vector joined;
	Vector_init(&joined);

	if (!Vector_push_back(&joined, left, strlen(left), memory.allocator) ||
		!append_path(&joined, memory.allocator, right, strlen(right)) ||
		!zero_terminate(&joined, memory.allocator))
	{
		Vector_free(&joined, memory.deallocator);
		return NULL;
	}

	return Vector_release(&joined);
}
