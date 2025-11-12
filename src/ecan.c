/**
 * @file ecan.c
 * @brief ECAN - Economic Attention Allocation Implementation
 * 
 * Implements attention allocation mechanisms using tensor-based scheduling
 * and importance spreading algorithms.
 */

#include "cogkern.h"
#include <stdlib.h>
#include <string.h>

/**
 * Maximum number of attention values
 */
#define MAX_AVS 1000000

/**
 * Attention value entry
 */
struct av_entry {
    atom_handle_t atom;
    struct attention_value av;
    int active;
};

/**
 * ECAN scheduler state
 */
static struct {
    struct av_entry avs[MAX_AVS];
    size_t av_count;
    uint32_t tick_interval_us;
    uint64_t tick_count;
    int initialized;
} g_ecan = {0};

/**
 * Initialize the ECAN scheduler
 * 
 * @param tick_interval_us Scheduler tick interval in microseconds
 * @return 0 on success, negative on error
 */
int dtesn_sched_init(uint32_t tick_interval_us) {
    if (g_ecan.initialized) {
        return -1;
    }
    
    g_ecan.tick_interval_us = tick_interval_us;
    g_ecan.tick_count = 0;
    g_ecan.av_count = 0;
    g_ecan.initialized = 1;
    
    return 0;
}

/**
 * Execute one scheduler tick
 * 
 * Performance target: ≤5µs
 * 
 * @return Number of tasks processed
 */
int dtesn_sched_tick(void) {
    if (!g_ecan.initialized) {
        return -1;
    }
    
    g_ecan.tick_count++;
    
    /* In a real implementation:
     * - Update attention values using GGML tensor operations
     * - Apply importance diffusion across hypergraph
     * - Schedule cognitive tasks based on attention
     * - Perform forgetting (decrease STI/LTI over time)
     */
    
    int tasks_processed = 0;
    
    /* Stub: Decay all STI values slightly */
    for (size_t i = 0; i < g_ecan.av_count; i++) {
        if (g_ecan.avs[i].active) {
            g_ecan.avs[i].av.sti *= 0.999f;
            tasks_processed++;
        }
    }
    
    return tasks_processed;
}

/**
 * Set attention value for an atom
 * 
 * @param atom Atom handle
 * @param av Pointer to attention value structure
 * @return 0 on success, negative on error
 */
int dtesn_sched_set_av(atom_handle_t atom, const struct attention_value *av) {
    if (!av) {
        return -1;
    }
    
    /* Find existing entry or create new one */
    for (size_t i = 0; i < g_ecan.av_count; i++) {
        if (g_ecan.avs[i].active && g_ecan.avs[i].atom == atom) {
            g_ecan.avs[i].av = *av;
            return 0;
        }
    }
    
    /* Create new entry */
    if (g_ecan.av_count >= MAX_AVS) {
        return -1;
    }
    
    size_t idx = g_ecan.av_count++;
    g_ecan.avs[idx].atom = atom;
    g_ecan.avs[idx].av = *av;
    g_ecan.avs[idx].active = 1;
    
    return 0;
}

/**
 * Get attention value for an atom
 * 
 * @param atom Atom handle
 * @param av Pointer to structure to receive attention value
 * @return 0 on success, negative on error
 */
int dtesn_sched_get_av(atom_handle_t atom, struct attention_value *av) {
    if (!av) {
        return -1;
    }
    
    for (size_t i = 0; i < g_ecan.av_count; i++) {
        if (g_ecan.avs[i].active && g_ecan.avs[i].atom == atom) {
            *av = g_ecan.avs[i].av;
            return 0;
        }
    }
    
    return -1; /* Not found */
}

/**
 * Spread importance across connected atoms
 * 
 * @param source Source atom handle
 * @param diffusion_rate Rate of importance diffusion (0.0-1.0)
 * @return Number of atoms affected
 */
int dtesn_sched_spread_importance(atom_handle_t source, float diffusion_rate) {
    /* In a real implementation:
     * - Use GGML tensor operations to propagate STI through hypergraph
     * - Apply diffusion_rate to control spread intensity
     * - Use attention values as weights in graph traversal
     */
    
    if (diffusion_rate < 0.0f || diffusion_rate > 1.0f) {
        return -1;
    }
    
    /* Stub implementation */
    return 0;
}
