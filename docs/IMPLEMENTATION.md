# OpenCog Kernel Implementation

A C/C++ implementation of OpenCog cognitive primitives using GGML tensor operations, designed for integration with cogpilot-cli.

## Overview

This implementation provides core OpenCog subsystems as a high-performance kernel library:

- **AtomSpace** - Hypergraph-based knowledge representation using tensor allocators
- **ECAN** - Economic attention allocation with real-time scheduling
- **PLN** - Probabilistic logic networks for inference
- **Cognitive Loop** - Multi-stage bootstrap and event-driven processing

## Features

### ✨ Core Capabilities

- **Tensor-Based Architecture** - Built for GGML/llama.cpp integration
- **Real-Time Performance** - Designed for ≤5µs scheduler ticks, ≤100ns memory ops
- **C99/C++17 API** - Clean, well-documented interface
- **Modular Design** - Each subsystem can be used independently
- **Example Programs** - Working demonstrations of all features

### 🏗️ Architecture

```
┌─────────────────────────────────────┐
│       Cognitive Loop                │
│   (Bootstrap & Orchestration)       │
└─────────────┬───────────────────────┘
              │
    ┌─────────┼─────────┐
    ▼         ▼         ▼
┌────────┐ ┌──────┐ ┌──────┐
│AtomSpace│ │ ECAN │ │ PLN  │
│(Hyper- │ │(Attn)│ │(Infer)│
│ graph) │ │      │ │      │
└────┬────┘ └───┬──┘ └───┬──┘
     │          │        │
     └──────────┴────────┘
              │
    ┌─────────▼─────────┐
    │  GGML Tensor Ops  │
    │  (Future Phase)    │
    └───────────────────┘
```

## Quick Start

### Installation

```bash
# Clone repository
git clone https://github.com/cogpy/cogpilot-cli.git
cd cogpilot-cli

# Build
mkdir build && cd build
cmake ..
make

# Run examples
./examples/basic_usage
./examples/atomspace_demo
./examples/cogloop_demo
```

### Basic Usage

```c
#include <cogkern.h>

int main(void) {
    // Initialize kernel
    cogkern_init(64 * 1024 * 1024);
    
    // Bootstrap
    cogloop_boot_stage(STAGE0_INIT);
    cogloop_boot_stage(STAGE1_HYPERGRAPH);
    cogloop_boot_stage(STAGE2_SCHEDULER);
    cogloop_boot_stage(STAGE3_COGNITIVE);
    
    // Create knowledge
    atom_handle_t human = cog_atom_alloc(ATOM_CONCEPT, "human");
    atom_handle_t mortal = cog_atom_alloc(ATOM_CONCEPT, "mortal");
    
    atom_handle_t outgoing[2] = {human, mortal};
    cog_link_create(ATOM_INHERITANCE, outgoing, 2);
    
    // Set attention
    struct attention_value av = {100.0f, 50.0f, 10.0f};
    dtesn_sched_set_av(human, &av);
    
    // Run cognitive cycle
    for (int i = 0; i < 100; i++) {
        cogloop_tick();
    }
    
    // Cleanup
    cogkern_shutdown();
    return 0;
}
```

## Project Structure

```
cogpilot-cli/
├── include/
│   └── cogkern.h              # Main API header (all functions)
├── src/
│   ├── cogkern.c              # Core initialization
│   ├── atomspace.c            # Hypergraph allocator
│   ├── ecan.c                 # Attention scheduler
│   ├── pln.c                  # Probabilistic inference
│   └── cogloop.c              # Bootstrap & event loop
├── examples/
│   ├── basic_usage.c          # Getting started
│   ├── atomspace_demo.c       # Knowledge representation
│   └── cogloop_demo.c         # Cognitive cycle
├── docs/
│   ├── BUILD.md               # Build instructions
│   ├── KERNEL_FUNCTION_MANIFEST.md  # API reference
│   └── KERNEL_STATUS_REPORT.md      # Implementation status
└── CMakeLists.txt             # Build configuration
```

## Documentation

- **[BUILD.md](docs/BUILD.md)** - Detailed build and installation guide
- **[KERNEL_FUNCTION_MANIFEST.md](docs/KERNEL_FUNCTION_MANIFEST.md)** - Complete API reference
- **[KERNEL_STATUS_REPORT.md](docs/KERNEL_STATUS_REPORT.md)** - Implementation status and roadmap
- **API Documentation** - Generate with `make docs` (requires Doxygen)

## API Overview

### Core Functions

```c
int cogkern_init(size_t mem_size);
void cogkern_shutdown(void);
struct ggml_context *cogkern_get_context(void);
```

### AtomSpace (Hypergraph)

```c
void *hgfs_alloc(size_t size, uint32_t depth);
atom_handle_t hgfs_edge(atom_handle_t from, atom_handle_t to, enum atom_type edge_type);
atom_handle_t cog_atom_alloc(enum atom_type type, const char *name);
atom_handle_t cog_link_create(enum atom_type type, const atom_handle_t *outgoing, size_t count);
```

### ECAN (Attention)

```c
int dtesn_sched_init(uint32_t tick_interval_us);
int dtesn_sched_tick(void);
int dtesn_sched_set_av(atom_handle_t atom, const struct attention_value *av);
int dtesn_sched_get_av(atom_handle_t atom, struct attention_value *av);
int dtesn_sched_spread_importance(atom_handle_t source, float diffusion_rate);
```

### PLN (Inference)

```c
int pln_eval_tensor(struct ggml_tensor *expr, struct truth_value *result);
int pln_unify_graph(struct ggml_tensor *pattern, struct ggml_tensor *target, 
                    struct ggml_tensor **result);
int pln_infer(atom_handle_t atom, struct truth_value *tv);
atom_handle_t cog_link_infer(atom_handle_t premise, atom_handle_t conclusion,
                              const struct truth_value *tv);
```

### Cognitive Loop

```c
int cogloop_boot_stage(enum boot_stage stage);
int stage1_init_hypergraph_fs(void);
int dtesn_mem_init_regions(size_t num_regions);
int cogloop_tick(void);
int cogloop_start(uint32_t hz);
void cogloop_stop(void);
```

## Examples

### Example 1: Basic Usage

Demonstrates initialization, bootstrap sequence, atom creation, and cognitive loop.

```bash
./examples/basic_usage
```

**Output:**
```
OpenCog Kernel - Basic Usage Example
=====================================

1. Initializing cognitive kernel...
   ✓ Kernel initialized with 64MB memory

2. Running bootstrap sequence...
   Stage 0: Core initialization
   ...
```

### Example 2: AtomSpace Demo

Shows knowledge graph construction with concepts and inheritance links.

```bash
./examples/atomspace_demo
```

**Output:**
```
OpenCog Kernel - AtomSpace Demo
================================

Building knowledge graph: Animals taxonomy
-------------------------------------------

Created concepts:
  - animal (handle: 1)
  - mammal (handle: 2)
  ...
```

### Example 3: Cognitive Loop Demo

Demonstrates attention allocation, importance spreading, and PLN inference.

```bash
./examples/cogloop_demo
```

**Output:**
```
OpenCog Kernel - Cognitive Loop Demo
=====================================

Initializing cognitive kernel...
✓ All bootstrap stages complete
...
```

## Implementation Status

### ✅ Completed (v0.1.0)

- Core kernel initialization and shutdown
- AtomSpace with nodes and links
- ECAN attention allocation (stub)
- PLN basic inference (stub)
- Cognitive loop with 4-stage bootstrap
- Complete API documentation
- Working example programs
- CMake build system

### 🔄 In Progress

- GGML backend integration
- Real tensor operations
- Performance benchmarking

### 📋 Planned

- Full PLN rule engine (deduction, induction, abduction)
- Pattern matching with tensor operations
- Multi-threaded cognitive loop
- Persistence (GGUF format)
- Distributed AtomSpace

## Performance Targets

| Component | Target | Status |
|-----------|--------|--------|
| Boot time | < 100ms | ✅ Met (stub) |
| Scheduler tick | ≤ 5µs | ⏳ Pending GGML |
| Memory ops | ≤ 100ns | ⏳ Pending GGML |
| Cognitive loop | 1000Hz | ⏳ Pending validation |

## Development Roadmap

### Phase 1: Foundation ✅ (Current - v0.1.0)
- Complete API design
- Stub implementations
- Build system and documentation
- Example programs

### Phase 2: GGML Integration 🔄 (Next)
- Integrate ggml and llama.cpp
- Replace stubs with tensor operations
- ESN reservoir implementation
- Performance benchmarking

### Phase 3: Advanced Features 📋
- Full PLN rule engine
- Pattern matching
- LoRA adapters for inference
- Persistence layer

### Phase 4: Distributed System 📋
- Multi-core/Multi-GPU support
- Network-distributed AtomSpace
- Neuromorphic hardware integration

## Requirements

- C99 compiler (GCC 7+ or Clang 6+)
- C++17 compiler (for future GGML integration)
- CMake 3.14+
- Make or Ninja

### Optional
- Doxygen (for documentation)
- GGML (planned integration)
- llama.cpp (planned integration)

## Contributing

This is a specialized implementation aligned with the OpenCog Kernel GGML agent specifications. For contributions:

1. Review [KERNEL_FUNCTION_MANIFEST.md](docs/KERNEL_FUNCTION_MANIFEST.md)
2. Follow C99/C++17 standards
3. Maintain Doxygen-style comments
4. Ensure performance targets are met
5. Add tests and examples

## License

See [LICENSE.md](LICENSE.md)

## Related Projects

- **cogpy/occ** - OpenCog Cognitive Core
- **cogpy/echo-kern** - Echo Kernel specifications
- **cogpy/ggml-cog** - GGML cognitive operations
- **cogpy/llama-bridge** - LLaMA integration layer

## References

- [OpenCog Framework](https://wiki.opencog.org/)
- [GGML](https://github.com/ggerganov/ggml)
- [llama.cpp](https://github.com/ggerganov/llama.cpp)
- [Echo.Kern Specifications](https://github.com/cogpy/echo-kern)

---

**Version:** 0.1.0  
**Date:** 2025-11-12  
**Status:** Initial Implementation Complete
