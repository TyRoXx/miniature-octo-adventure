#include "all_tests.h"
#include "base/vector.h"

static void test_vector(void);

void moa_test_run_all(void)
{
	test_vector();
}

#define MOA_IMPL_STRINGIZE(x) #x
#define MOA_STRINGIZE(x) MOA_IMPL_STRINGIZE(x)
#define TEST(x)   do { if (!(x)) { moa_test_on_message("TEST("   MOA_STRINGIZE(x) ") fail", __FILE__, __LINE__);         } } while (0)
#define ASSERT(x) do { if (!(x)) { moa_test_on_message("ASSERT(" MOA_STRINGIZE(x) ") fail", __FILE__, __LINE__); return; } } while (0)

static void test_vector(void)
{
	{
		Vector v;
		Vector_init(&v);
		TEST(Vector_empty(&v));
		TEST(Vector_size(&v) == 0);
		Vector_free(&v);
	}

	{
		size_t i;
		size_t const count = 1000;
		Vector v;
		Vector_init(&v);
		for (i = 0; i < count; ++i)
		{
			char c = (char)('0' + (i % 10));
			ASSERT(Vector_push_back(&v, &c, 1));
		}
		for (i = 0; i < count; ++i)
		{
			char c = (char)('0' + (i % 10));
			ASSERT(Vector_data(&v)[i] == c);
		}
		Vector_free(&v);
	}
}
