/**
 * @file basic_usage.c
 * @brief Basic usage example for OpenCog Kernel
 * 
 * Demonstrates initialization and basic operations with the cognitive kernel.
 */

#include <stdio.h>
#include <cogkern.h>

int main(void) {
    printf("OpenCog Kernel - Basic Usage Example\n");
    printf("=====================================\n\n");
    
    /* Initialize the kernel */
    printf("1. Initializing cognitive kernel...\n");
    if (cogkern_init(64 * 1024 * 1024) != 0) {
        fprintf(stderr, "Failed to initialize kernel\n");
        return 1;
    }
    printf("   ✓ Kernel initialized with 64MB memory\n\n");
    
    /* Bootstrap stages */
    printf("2. Running bootstrap sequence...\n");
    
    printf("   Stage 0: Core initialization\n");
    cogloop_boot_stage(STAGE0_INIT);
    
    printf("   Stage 1: Hypergraph filesystem\n");
    cogloop_boot_stage(STAGE1_HYPERGRAPH);
    
    printf("   Stage 2: Scheduler and memory regions\n");
    cogloop_boot_stage(STAGE2_SCHEDULER);
    
    printf("   Stage 3: Cognitive loop\n");
    cogloop_boot_stage(STAGE3_COGNITIVE);
    
    printf("   ✓ Bootstrap complete\n\n");
    
    /* Create some atoms */
    printf("3. Creating atoms in AtomSpace...\n");
    atom_handle_t concept1 = cog_atom_alloc(ATOM_CONCEPT, "cat");
    atom_handle_t concept2 = cog_atom_alloc(ATOM_CONCEPT, "animal");
    printf("   ✓ Created concept: 'cat' (handle: %lu)\n", concept1);
    printf("   ✓ Created concept: 'animal' (handle: %lu)\n", concept2);
    
    /* Create inheritance link */
    atom_handle_t outgoing[2] = {concept1, concept2};
    atom_handle_t link = cog_link_create(ATOM_INHERITANCE, outgoing, 2);
    printf("   ✓ Created link: cat -> animal (handle: %lu)\n\n", link);
    
    /* Set attention values */
    printf("4. Setting attention values...\n");
    struct attention_value av = {
        .sti = 100.0f,
        .lti = 50.0f,
        .vlti = 10.0f
    };
    dtesn_sched_set_av(concept1, &av);
    printf("   ✓ Set attention for 'cat': STI=%.1f, LTI=%.1f, VLTI=%.1f\n\n",
           av.sti, av.lti, av.vlti);
    
    /* Run cognitive loop for a few iterations */
    printf("5. Running cognitive loop...\n");
    for (int i = 0; i < 5; i++) {
        cogloop_tick();
        printf("   Iteration %d complete\n", i + 1);
    }
    printf("\n");
    
    /* Check attention value after decay */
    printf("6. Checking attention after decay...\n");
    struct attention_value av_after;
    dtesn_sched_get_av(concept1, &av_after);
    printf("   ✓ Updated attention for 'cat': STI=%.1f, LTI=%.1f, VLTI=%.1f\n\n",
           av_after.sti, av_after.lti, av_after.vlti);
    
    /* Shutdown */
    printf("7. Shutting down...\n");
    cogkern_shutdown();
    printf("   ✓ Kernel shutdown complete\n\n");
    
    printf("Example completed successfully!\n");
    return 0;
}
