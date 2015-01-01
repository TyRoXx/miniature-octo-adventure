#include "all_tests.h"
#include "base/vector.h"
#include "base/stringize.h"

static void test_vector(void);

void moa_test_run_all(void)
{
	test_vector();
}

#define TEST(x)   do { if (!(x)) { moa_test_on_message("TEST("   MOA_STRINGIZE(x) ") fail", __FILE__, __LINE__);         } } while (0)
#define ASSERT(x) do { if (!(x)) { moa_test_on_message("ASSERT(" MOA_STRINGIZE(x) ") fail", __FILE__, __LINE__); return; } } while (0)

static void test_vector(void)
{
	MemoryManager memory = create_standard_memory_manager();

	{
		Vector v;
		Vector_init(&v);
		TEST(Vector_empty(&v));
		TEST(Vector_size(&v) == 0);
		Vector_free(&v, memory.deallocator);
	}

	{
		Vector v;
		Vector_init(&v);
		TEST(Vector_reserve(&v, 100, memory.allocator));
		TEST(Vector_reserve(&v, 200, memory.allocator));
		TEST(Vector_reserve(&v, 50, memory.allocator));
		TEST(Vector_reserve(&v, 0, memory.allocator));
		Vector_free(&v, memory.deallocator);
	}

	{
		Vector v;
		Vector_init(&v);
		TEST(Vector_resize(&v, 100, memory.allocator));
		TEST(Vector_resize(&v, 200, memory.allocator));
		TEST(Vector_resize(&v, 50, memory.allocator));
		TEST(Vector_resize(&v, 0, memory.allocator));
		Vector_free(&v, memory.deallocator);
	}

	{
		size_t i;
		size_t const count = 1000;
		Vector v;
		Vector_init(&v);
		for (i = 0; i < count; ++i)
		{
			char c = (char)('0' + (i % 10));
			ASSERT(Vector_push_back(&v, &c, 1, memory.allocator));
		}
		for (i = 0; i < count; ++i)
		{
			char c = (char)('0' + (i % 10));
			ASSERT(Vector_data(&v)[i] == c);
		}
		Vector_free(&v, memory.deallocator);
	}
}
