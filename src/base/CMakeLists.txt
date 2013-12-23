file(GLOB sources "*.h" "*.c")
add_library(base ${sources})
target_link_libraries(base gui)
