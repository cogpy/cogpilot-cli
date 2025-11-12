# Examples CMakeLists.txt

# Basic usage example
add_executable(basic_usage basic_usage.c)
target_link_libraries(basic_usage cogkern)

# AtomSpace example
add_executable(atomspace_demo atomspace_demo.c)
target_link_libraries(atomspace_demo cogkern)

# Cognitive loop example
add_executable(cogloop_demo cogloop_demo.c)
target_link_libraries(cogloop_demo cogkern)

# Install examples
install(TARGETS basic_usage atomspace_demo cogloop_demo
    RUNTIME DESTINATION bin/examples
)
