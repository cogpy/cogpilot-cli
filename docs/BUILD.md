# Building OpenCog Kernel

This guide explains how to build, install, and use the OpenCog Kernel (cogkern) library.

## Prerequisites

### Required

- **C Compiler:** GCC 7+ or Clang 6+ (C99 support)
- **C++ Compiler:** G++ 7+ or Clang++ 6+ (C++17 support)
- **CMake:** Version 3.14 or higher
- **Make:** GNU Make or compatible

### Optional

- **Doxygen:** For generating API documentation
- **GGML:** For tensor operations (planned integration)
- **llama.cpp:** For neural operations (planned integration)

## Quick Start

### 1. Clone and Build

```bash
# Clone the repository (if not already done)
git clone https://github.com/cogpy/cogpilot-cli.git
cd cogpilot-cli

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
make

# Run examples
./examples/basic_usage
./examples/atomspace_demo
./examples/cogloop_demo
```

### 2. Install (Optional)

```bash
# Install to system directories (may require sudo)
sudo make install

# Or install to custom prefix
cmake -DCMAKE_INSTALL_PREFIX=/custom/path ..
make
make install
```

## Build Options

### CMake Configuration Options

```bash
# Build shared library instead of static
cmake -DBUILD_SHARED_LIBS=ON ..

# Disable examples
cmake -DBUILD_EXAMPLES=OFF ..

# Disable documentation generation
cmake -DBUILD_DOCS=OFF ..

# Set build type (Debug/Release/RelWithDebInfo/MinSizeRel)
cmake -DCMAKE_BUILD_TYPE=Release ..

# Custom C/C++ flags
cmake -DCMAKE_C_FLAGS="-O3 -march=native" ..
cmake -DCMAKE_CXX_FLAGS="-O3 -march=native" ..
```

### Example: Optimized Release Build

```bash
mkdir build-release
cd build-release
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_FLAGS="-O3 -march=native -flto" \
      -DCMAKE_CXX_FLAGS="-O3 -march=native -flto" \
      ..
make -j$(nproc)
```

## Directory Structure

```
cogpilot-cli/
├── CMakeLists.txt          # Main build configuration
├── include/
│   └── cogkern.h           # Public API header
├── src/
│   ├── cogkern.c           # Core kernel
│   ├── atomspace.c         # AtomSpace implementation
│   ├── ecan.c              # ECAN scheduler
│   ├── pln.c               # PLN inference
│   └── cogloop.c           # Cognitive loop
├── examples/
│   ├── CMakeLists.txt      # Examples build config
│   ├── basic_usage.c       # Basic usage example
│   ├── atomspace_demo.c    # AtomSpace demonstration
│   └── cogloop_demo.c      # Cognitive loop demo
├── docs/
│   ├── KERNEL_FUNCTION_MANIFEST.md
│   ├── KERNEL_STATUS_REPORT.md
│   └── BUILD.md            # This file
└── build/                  # Build output (created by user)
```

## Build Targets

### Library

```bash
# Build the cogkern library
make cogkern

# Install library and headers
make install
```

The library will be built as:
- **Static:** `libcogkern.a` (default)
- **Shared:** `libcogkern.so` (if `-DBUILD_SHARED_LIBS=ON`)

### Examples

```bash
# Build all examples
make

# Build specific example
make basic_usage
make atomspace_demo
make cogloop_demo

# Run examples
./examples/basic_usage
./examples/atomspace_demo
./examples/cogloop_demo
```

### Documentation

```bash
# Generate Doxygen documentation (requires Doxygen)
make docs

# View generated documentation
firefox docs/html/index.html  # Linux
open docs/html/index.html     # macOS
```

## Using the Library

### In Your CMake Project

Add to your `CMakeLists.txt`:

```cmake
# Find the cogkern library
find_library(COGKERN_LIB cogkern)
find_path(COGKERN_INCLUDE cogkern.h)

# Add to your target
include_directories(${COGKERN_INCLUDE})
target_link_libraries(your_program ${COGKERN_LIB})
```

### Manual Compilation

```bash
# Compile your program
gcc -I/path/to/include -c your_program.c -o your_program.o

# Link with cogkern
gcc your_program.o -L/path/to/lib -lcogkern -o your_program

# Run
./your_program
```

### Simple Example

```c
#include <stdio.h>
#include <cogkern.h>

int main(void) {
    // Initialize kernel
    cogkern_init(64 * 1024 * 1024);
    
    // Bootstrap all stages
    cogloop_boot_stage(STAGE0_INIT);
    cogloop_boot_stage(STAGE1_HYPERGRAPH);
    cogloop_boot_stage(STAGE2_SCHEDULER);
    cogloop_boot_stage(STAGE3_COGNITIVE);
    
    // Create atoms
    atom_handle_t cat = cog_atom_alloc(ATOM_CONCEPT, "cat");
    atom_handle_t animal = cog_atom_alloc(ATOM_CONCEPT, "animal");
    
    // Create link
    atom_handle_t outgoing[2] = {cat, animal};
    cog_link_create(ATOM_INHERITANCE, outgoing, 2);
    
    // Run cognitive loop
    for (int i = 0; i < 10; i++) {
        cogloop_tick();
    }
    
    // Cleanup
    cogkern_shutdown();
    
    return 0;
}
```

Compile and run:

```bash
gcc -I./include example.c -L./build -lcogkern -o example
./example
```

## Troubleshooting

### Common Issues

**1. CMake can't find compiler**

```bash
# Specify compiler explicitly
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
```

**2. Build fails with "command not found"**

Ensure CMake and Make are installed:

```bash
# Ubuntu/Debian
sudo apt-get install cmake build-essential

# macOS
brew install cmake

# Fedora/RHEL
sudo dnf install cmake gcc gcc-c++ make
```

**3. Examples fail to run**

```bash
# Set library path (Linux)
export LD_LIBRARY_PATH=/path/to/build:$LD_LIBRARY_PATH

# macOS
export DYLD_LIBRARY_PATH=/path/to/build:$DYLD_LIBRARY_PATH
```

**4. Doxygen documentation not generated**

Install Doxygen:

```bash
# Ubuntu/Debian
sudo apt-get install doxygen

# macOS
brew install doxygen

# Fedora/RHEL
sudo dnf install doxygen
```

## Platform-Specific Notes

### Linux

Tested on:
- Ubuntu 20.04, 22.04, 24.04
- Fedora 38, 39
- Debian 11, 12

### macOS

Tested on:
- macOS 12 (Monterey)
- macOS 13 (Ventura)
- macOS 14 (Sonoma)

May require Xcode Command Line Tools:
```bash
xcode-select --install
```

### Windows

For Windows builds, use one of:

1. **Visual Studio 2019+** with CMake support
2. **MinGW-w64** with CMake and Make
3. **WSL (Windows Subsystem for Linux)** - recommended

Example with Visual Studio:
```powershell
mkdir build
cd build
cmake -G "Visual Studio 16 2019" ..
cmake --build . --config Release
```

## Performance Optimization

### Compiler Flags

For maximum performance:

```bash
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_FLAGS="-O3 -march=native -flto -ffast-math" \
      -DCMAKE_CXX_FLAGS="-O3 -march=native -flto -ffast-math" \
      ..
```

### Profiling

Build with profiling support:

```bash
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DCMAKE_C_FLAGS="-pg" \
      -DCMAKE_CXX_FLAGS="-pg" \
      ..
make
./examples/cogloop_demo
gprof ./examples/cogloop_demo gmon.out > profile.txt
```

## Next Steps

- Read [KERNEL_FUNCTION_MANIFEST.md](KERNEL_FUNCTION_MANIFEST.md) for API reference
- Check [KERNEL_STATUS_REPORT.md](KERNEL_STATUS_REPORT.md) for implementation status
- Explore examples in `examples/` directory
- Review Doxygen documentation (run `make docs`)

## Support

For issues and questions:
- GitHub Issues: https://github.com/cogpy/cogpilot-cli/issues
- Documentation: `docs/` directory
- Examples: `examples/` directory

---

**Version:** 0.1.0  
**Last Updated:** 2025-11-12
