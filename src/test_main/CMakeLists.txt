file(GLOB sources "*.h" "*.c")
add_executable(test_main ${sources})
target_link_libraries(test_main tests gui base ${CONAN_LIBS})
