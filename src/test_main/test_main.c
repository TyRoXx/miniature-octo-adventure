#include "test/all_tests.h"
#include <stdio.h>

static size_t message_count = 0;

static void on_message(char const *message, char const *file, int line)
{
	++message_count;
	fprintf(stderr, "%s (%s:%d)\n", message, file, line);
}

int main(void)
{
	moa_test_on_message = on_message;
	moa_test_run_all();
	if (message_count)
	{
		fprintf(stderr, "%lu checks failed\n", (unsigned long)message_count);
		return 1;
	}
	fprintf(stderr, "Success\n");
	return 0;
}
