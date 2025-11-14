cmake_minimum_required(VERSION 3.14)
project(cogkern VERSION 0.1.0 LANGUAGES C CXX)

# Set C/C++ standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_EXAMPLES "Build example programs" ON)
option(BUILD_DOCS "Build documentation" ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(COGKERN_SOURCES
    src/cogkern.c
    src/atomspace.c
    src/ecan.c
    src/pln.c
    src/cogloop.c
)

# Create library
add_library(cogkern ${COGKERN_SOURCES})

# Set library properties
set_target_properties(cogkern PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    PUBLIC_HEADER include/cogkern.h
)

# CLI executable
add_executable(cogpilot-cli src/cli.c)
target_link_libraries(cogpilot-cli cogkern)

# Installation
install(TARGETS cogkern cogpilot-cli
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)

# Examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Documentation
if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_GENERATE_HTML YES)
        set(DOXYGEN_GENERATE_MAN YES)
        set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
        
        doxygen_add_docs(docs
            ${PROJECT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/src
            COMMENT "Generating API documentation with Doxygen"
        )
    endif()
endif()

# Print configuration
message(STATUS "")
message(STATUS "cogkern configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "  CXX Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build examples: ${BUILD_EXAMPLES}")
message(STATUS "  Build docs: ${BUILD_DOCS}")
message(STATUS "")
