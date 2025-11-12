# OpenCog Kernel Function Manifest

This document defines the complete function manifest for the OpenCog Kernel (cogkern) implementation using GGML tensor operations.

## Version: 0.1.0
**Last Updated:** 2025-11-12

---

## 1. Core Kernel Functions

### 1.1 Initialization & Management

| Function | Status | Priority | Performance Target |
|----------|--------|----------|-------------------|
| `cogkern_init()` | ✅ IMPLEMENTED | CRITICAL | < 100ms |
| `cogkern_shutdown()` | ✅ IMPLEMENTED | CRITICAL | < 50ms |
| `cogkern_get_context()` | ✅ IMPLEMENTED | HIGH | < 10ns |

---

## 2. AtomSpace - Hypergraph Tensor Allocator

### 2.1 Memory Allocation

| Function | Status | Priority | Performance Target |
|----------|--------|----------|-------------------|
| `hgfs_alloc()` | ✅ IMPLEMENTED | CRITICAL | ≤ 100ns |
| `hgfs_edge()` | ✅ IMPLEMENTED | CRITICAL | ≤ 100ns |

### 2.2 Atom Management

| Function | Status | Priority | Performance Target |
|----------|--------|----------|-------------------|
| `cog_atom_alloc()` | ✅ IMPLEMENTED | CRITICAL | ≤ 500ns |
| `cog_link_create()` | ✅ IMPLEMENTED | HIGH | ≤ 1µs |

**Dependencies:** GGML tensor allocator

**Future Enhancements:**
- Graph traversal operations
- Pattern matching with GGML ops
- Distributed hypergraph support

---

## 3. ECAN - Economic Attention Allocation

### 3.1 Scheduler Functions

| Function | Status | Priority | Performance Target |
|----------|--------|----------|-------------------|
| `dtesn_sched_init()` | ✅ IMPLEMENTED | CRITICAL | < 10ms |
| `dtesn_sched_tick()` | ✅ IMPLEMENTED | CRITICAL | ≤ 5µs |
| `dtesn_sched_set_av()` | ✅ IMPLEMENTED | HIGH | ≤ 200ns |
| `dtesn_sched_get_av()` | ✅ IMPLEMENTED | HIGH | ≤ 100ns |
| `dtesn_sched_spread_importance()` | ✅ IMPLEMENTED | MEDIUM | ≤ 10µs |

**Dependencies:** GGML tensor operations, AtomSpace

**Real-time Constraints:**
- Scheduler tick must complete in ≤5µs for 1000Hz cognitive loop
- Attention spreading should use GGML matrix operations for parallelism

---

## 4. PLN - Probabilistic Logic Networks

### 4.1 Inference Functions

| Function | Status | Priority | Performance Target |
|----------|--------|----------|-------------------|
| `pln_eval_tensor()` | ✅ IMPLEMENTED | HIGH | ≤ 10µs |
| `pln_unify_graph()` | ✅ IMPLEMENTED | HIGH | ≤ 50µs |
| `pln_infer()` | ✅ IMPLEMENTED | HIGH | ≤ 5µs |
| `cog_link_infer()` | ✅ IMPLEMENTED | MEDIUM | ≤ 1µs |

**Dependencies:** GGML tensor graphs, AtomSpace

**Inference Rules (Future):**
- Deduction
- Induction
- Abduction
- Analogy
- Fuzzy pattern matching

---

## 5. Cognitive Loop - Bootstrap & Event Loop

### 5.1 Bootstrap Stages

| Function | Status | Priority | Performance Target |
|----------|--------|----------|-------------------|
| `cogloop_boot_stage()` | ✅ IMPLEMENTED | CRITICAL | Stage-dependent |
| `stage1_init_hypergraph_fs()` | ✅ IMPLEMENTED | CRITICAL | < 50ms |
| `dtesn_mem_init_regions()` | ✅ IMPLEMENTED | CRITICAL | < 20ms |

### 5.2 Event Loop

| Function | Status | Priority | Performance Target |
|----------|--------|----------|-------------------|
| `cogloop_tick()` | ✅ IMPLEMENTED | CRITICAL | ≤ 1ms |
| `cogloop_start()` | ✅ IMPLEMENTED | HIGH | < 10ms |
| `cogloop_stop()` | ✅ IMPLEMENTED | HIGH | < 5ms |

**Bootstrap Sequence:**
1. **Stage 0:** Core kernel initialization
2. **Stage 1:** Hypergraph filesystem setup
3. **Stage 2:** Scheduler and memory regions
4. **Stage 3:** Cognitive loop activation

---

## 6. Future Kernel Primitives (Roadmap)

### 6.1 Interrupts & Syscalls

| Function | Status | Priority | Performance Target |
|----------|--------|----------|-------------------|
| `cogkern_interrupt_register()` | 🔄 PLANNED | HIGH | ≤ 1µs |
| `cogkern_syscall()` | 🔄 PLANNED | MEDIUM | ≤ 500ns |

### 6.2 I/O Operations

| Function | Status | Priority | Performance Target |
|----------|--------|----------|-------------------|
| `cogkern_io_read()` | 🔄 PLANNED | MEDIUM | ≤ 10µs |
| `cogkern_io_write()` | 🔄 PLANNED | MEDIUM | ≤ 10µs |

### 6.3 Synchronization

| Function | Status | Priority | Performance Target |
|----------|--------|----------|-------------------|
| `cogkern_mutex_lock()` | 🔄 PLANNED | HIGH | ≤ 100ns |
| `cogkern_semaphore_wait()` | 🔄 PLANNED | HIGH | ≤ 200ns |

### 6.4 Timers

| Function | Status | Priority | Performance Target |
|----------|--------|----------|-------------------|
| `cogkern_timer_create()` | 🔄 PLANNED | MEDIUM | ≤ 1µs |
| `cogkern_timer_arm()` | 🔄 PLANNED | MEDIUM | ≤ 500ns |

---

## 7. GGML Integration Status

### 7.1 Current Implementation

- **Stub Mode:** Current implementation provides function signatures and basic logic
- **GGML Headers:** Not yet integrated (requires `ggml.h` dependency)
- **Tensor Operations:** Placeholder implementations ready for GGML backend

### 7.2 Integration Roadmap

| Component | Integration Status | Target Date |
|-----------|-------------------|-------------|
| GGML Core | 🔄 Planned | Phase 1 |
| llama.cpp Kernels | 🔄 Planned | Phase 2 |
| Quantized Tensors | 🔄 Planned | Phase 2 |
| ESN Reservoir (GGML) | 🔄 Planned | Phase 3 |
| Distributed Support | 🔄 Planned | Phase 4 |

---

## 8. Performance Validation

### 8.1 Benchmarks Required

- [ ] Memory allocation latency (hgfs_alloc)
- [ ] Scheduler tick latency (dtesn_sched_tick)
- [ ] Attention spreading throughput
- [ ] PLN inference latency
- [ ] Cognitive loop frequency stability

### 8.2 Reference Models

Python reference implementations for validation:
- `psystem_membranes.py` - Membrane evolution
- `bseries_differential_calculator.py` - Bootstrap sequences
- `esn_reservoir.py` - Echo state network dynamics

---

## Legend

- ✅ **IMPLEMENTED** - Function is implemented with stub or basic logic
- 🔄 **PLANNED** - Scheduled for future implementation
- ⚠️ **IN PROGRESS** - Currently being implemented
- ❌ **BLOCKED** - Blocked by dependencies

**Priority Levels:**
- **CRITICAL** - Core functionality, required for basic operation
- **HIGH** - Important features, needed for full cognitive capability
- **MEDIUM** - Useful enhancements
- **LOW** - Nice-to-have features

---

## Notes

This manifest serves as the authoritative source for all OpenCog Kernel functions. All implementations must align with the specifications defined here, including performance targets and dependency requirements.

For implementation details and usage examples, see:
- `examples/basic_usage.c`
- `examples/atomspace_demo.c`
- `examples/cogloop_demo.c`
