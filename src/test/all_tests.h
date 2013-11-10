#ifndef ALL_TESTS_H
#define ALL_TESTS_H


typedef void (*message_handler)(char const *message, char const *file, int line);

message_handler moa_test_on_message;

void moa_test_run_all(void);


#endif
