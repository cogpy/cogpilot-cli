/**
 * @file cogkern.h
 * @brief OpenCog Kernel - Main header for cognitive primitives
 * 
 * This file provides the main interface for OpenCog cognitive subsystems
 * implemented as GGML tensor-based operations. It integrates AtomSpace,
 * ECAN, PLN, and Cognitive Loop primitives.
 * 
 * @author OpenCog Kernel GGML Agent
 * @date 2025-11-12
 */

#ifndef COGKERN_H
#define COGKERN_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations for GGML types */
struct ggml_context;
struct ggml_tensor;

/**
 * @defgroup cogkern_core Core Kernel API
 * @{
 */

/**
 * Initialize the cognitive kernel subsystem
 * 
 * @param mem_size Memory pool size in bytes
 * @return 0 on success, negative on error
 */
int cogkern_init(size_t mem_size);

/**
 * Shutdown the cognitive kernel and free resources
 */
void cogkern_shutdown(void);

/**
 * Get the global GGML context
 * 
 * @return Pointer to GGML context or NULL if not initialized
 */
struct ggml_context *cogkern_get_context(void);

/** @} */

/**
 * @defgroup atomspace AtomSpace - Hypergraph Tensor Allocator
 * @{
 */

/**
 * Atom handle type
 */
typedef uint64_t atom_handle_t;

/**
 * Atom types
 */
enum atom_type {
    ATOM_NODE = 0,
    ATOM_LINK = 1,
    ATOM_CONCEPT = 2,
    ATOM_PREDICATE = 3,
    ATOM_EVALUATION = 4,
    ATOM_INHERITANCE = 5,
    ATOM_SIMILARITY = 6
};

/**
 * Allocate a hypergraph node as a GGML tensor
 * 
 * @param size Size in bytes
 * @param depth Membrane depth (OEIS A000081)
 * @return Pointer to allocated memory or NULL on failure
 */
void *hgfs_alloc(size_t size, uint32_t depth);

/**
 * Create a hypergraph edge connecting atoms
 * 
 * @param from Source atom handle
 * @param to Destination atom handle
 * @param edge_type Type of the edge
 * @return Edge handle or 0 on failure
 */
atom_handle_t hgfs_edge(atom_handle_t from, atom_handle_t to, enum atom_type edge_type);

/**
 * Allocate an atom in the AtomSpace
 * 
 * @param type Atom type
 * @param name Atom name (can be NULL for links)
 * @return Atom handle or 0 on failure
 */
atom_handle_t cog_atom_alloc(enum atom_type type, const char *name);

/**
 * Create a link between atoms
 * 
 * @param type Link type
 * @param outgoing Array of outgoing atom handles
 * @param outgoing_count Number of outgoing atoms
 * @return Link handle or 0 on failure
 */
atom_handle_t cog_link_create(enum atom_type type, const atom_handle_t *outgoing, size_t outgoing_count);

/** @} */

/**
 * @defgroup ecan ECAN - Economic Attention Allocation
 * @{
 */

/**
 * Attention value structure
 */
struct attention_value {
    float sti;  /**< Short-term importance */
    float lti;  /**< Long-term importance */
    float vlti; /**< Very long-term importance */
};

/**
 * Initialize the ECAN scheduler
 * 
 * @param tick_interval_us Scheduler tick interval in microseconds
 * @return 0 on success, negative on error
 */
int dtesn_sched_init(uint32_t tick_interval_us);

/**
 * Execute one scheduler tick
 * 
 * @return Number of tasks processed
 */
int dtesn_sched_tick(void);

/**
 * Set attention value for an atom
 * 
 * @param atom Atom handle
 * @param av Pointer to attention value structure
 * @return 0 on success, negative on error
 */
int dtesn_sched_set_av(atom_handle_t atom, const struct attention_value *av);

/**
 * Get attention value for an atom
 * 
 * @param atom Atom handle
 * @param av Pointer to structure to receive attention value
 * @return 0 on success, negative on error
 */
int dtesn_sched_get_av(atom_handle_t atom, struct attention_value *av);

/**
 * Spread importance across connected atoms
 * 
 * @param source Source atom handle
 * @param diffusion_rate Rate of importance diffusion (0.0-1.0)
 * @return Number of atoms affected
 */
int dtesn_sched_spread_importance(atom_handle_t source, float diffusion_rate);

/** @} */

/**
 * @defgroup pln PLN - Probabilistic Logic Networks
 * @{
 */

/**
 * Truth value structure
 */
struct truth_value {
    float strength;    /**< Probability estimate (0.0-1.0) */
    float confidence;  /**< Confidence in the estimate (0.0-1.0) */
};

/**
 * Evaluate a PLN expression using tensor operations
 * 
 * @param expr Expression tensor
 * @param result Pointer to receive truth value result
 * @return 0 on success, negative on error
 */
int pln_eval_tensor(struct ggml_tensor *expr, struct truth_value *result);

/**
 * Unify two graph patterns
 * 
 * @param pattern Pattern tensor
 * @param target Target tensor
 * @param result Pointer to receive unified tensor
 * @return 0 on success, negative on error
 */
int pln_unify_graph(struct ggml_tensor *pattern, struct ggml_tensor *target, 
                    struct ggml_tensor **result);

/**
 * Perform PLN inference on an atom
 * 
 * @param atom Atom handle
 * @param tv Pointer to receive inferred truth value
 * @return 0 on success, negative on error
 */
int pln_infer(atom_handle_t atom, struct truth_value *tv);

/**
 * Create inference link between atoms
 * 
 * @param premise Premise atom handle
 * @param conclusion Conclusion atom handle
 * @param tv Truth value of the inference
 * @return Inference link handle or 0 on failure
 */
atom_handle_t cog_link_infer(atom_handle_t premise, atom_handle_t conclusion, 
                              const struct truth_value *tv);

/** @} */

/**
 * @defgroup cogloop Cognitive Loop - Bootstrap and Event Loop
 * @{
 */

/**
 * Bootstrap stages
 */
enum boot_stage {
    STAGE0_INIT = 0,      /**< Initialize hypergraph allocator */
    STAGE1_HYPERGRAPH = 1,/**< Initialize hypergraph filesystem */
    STAGE2_SCHEDULER = 2, /**< Initialize scheduler and memory regions */
    STAGE3_COGNITIVE = 3  /**< Initialize cognitive loop */
};

/**
 * Initialize bootstrap stage
 * 
 * @param stage Bootstrap stage to initialize
 * @return 0 on success, negative on error
 */
int cogloop_boot_stage(enum boot_stage stage);

/**
 * Initialize Stage 1: Hypergraph filesystem
 * 
 * @return 0 on success, negative on error
 */
int stage1_init_hypergraph_fs(void);

/**
 * Initialize memory regions
 * 
 * @param num_regions Number of memory regions
 * @return 0 on success, negative on error
 */
int dtesn_mem_init_regions(size_t num_regions);

/**
 * Run one iteration of the cognitive loop
 * 
 * @return 0 on success, negative on error
 */
int cogloop_tick(void);

/**
 * Start the cognitive loop
 * 
 * @param hz Frequency in Hz (0 for manual tick mode)
 * @return 0 on success, negative on error
 */
int cogloop_start(uint32_t hz);

/**
 * Stop the cognitive loop
 */
void cogloop_stop(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* COGKERN_H */
