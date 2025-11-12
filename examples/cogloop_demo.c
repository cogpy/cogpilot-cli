/**
 * @file cogloop_demo.c
 * @brief Cognitive loop demonstration
 * 
 * Demonstrates the cognitive cycle with attention allocation and inference.
 */

#include <stdio.h>
#include <cogkern.h>

void print_av(const char *name, atom_handle_t atom) {
    struct attention_value av;
    if (dtesn_sched_get_av(atom, &av) == 0) {
        printf("  %s: STI=%.2f, LTI=%.2f, VLTI=%.2f\n",
               name, av.sti, av.lti, av.vlti);
    } else {
        printf("  %s: No attention value\n", name);
    }
}

int main(void) {
    printf("OpenCog Kernel - Cognitive Loop Demo\n");
    printf("=====================================\n\n");
    
    /* Initialize all stages */
    printf("Initializing cognitive kernel...\n");
    cogloop_boot_stage(STAGE0_INIT);
    cogloop_boot_stage(STAGE1_HYPERGRAPH);
    cogloop_boot_stage(STAGE2_SCHEDULER);
    cogloop_boot_stage(STAGE3_COGNITIVE);
    printf("✓ All bootstrap stages complete\n\n");
    
    /* Create a simple knowledge base */
    printf("Creating knowledge base...\n");
    atom_handle_t human = cog_atom_alloc(ATOM_CONCEPT, "human");
    atom_handle_t mortal = cog_atom_alloc(ATOM_CONCEPT, "mortal");
    atom_handle_t socrates = cog_atom_alloc(ATOM_CONCEPT, "socrates");
    
    /* Create inference chain: socrates -> human -> mortal */
    atom_handle_t outgoing[2];
    outgoing[0] = socrates;
    outgoing[1] = human;
    cog_link_create(ATOM_INHERITANCE, outgoing, 2);
    
    outgoing[0] = human;
    outgoing[1] = mortal;
    cog_link_create(ATOM_INHERITANCE, outgoing, 2);
    
    printf("✓ Created atoms: human, mortal, socrates\n");
    printf("✓ Created links: socrates->human, human->mortal\n\n");
    
    /* Set initial attention values */
    printf("Setting initial attention values...\n");
    struct attention_value av_high = { .sti = 100.0f, .lti = 80.0f, .vlti = 20.0f };
    struct attention_value av_med = { .sti = 60.0f, .lti = 50.0f, .vlti = 15.0f };
    struct attention_value av_low = { .sti = 30.0f, .lti = 25.0f, .vlti = 10.0f };
    
    dtesn_sched_set_av(socrates, &av_high);
    dtesn_sched_set_av(human, &av_med);
    dtesn_sched_set_av(mortal, &av_low);
    
    print_av("socrates", socrates);
    print_av("human", human);
    print_av("mortal", mortal);
    printf("\n");
    
    /* Run cognitive loop */
    printf("Running cognitive loop (10 iterations)...\n");
    printf("------------------------------------------\n");
    
    for (int i = 0; i < 10; i++) {
        printf("Iteration %d:\n", i + 1);
        
        /* Tick the cognitive loop */
        cogloop_tick();
        
        /* Show attention values */
        print_av("  socrates", socrates);
        print_av("  human", human);
        print_av("  mortal", mortal);
        
        /* Perform importance spreading every 5 iterations */
        if ((i + 1) % 5 == 0) {
            printf("  >>> Spreading importance from socrates\n");
            dtesn_sched_spread_importance(socrates, 0.2f);
        }
        
        printf("\n");
    }
    
    /* Final attention values */
    printf("Final attention values after cognitive cycle:\n");
    print_av("socrates", socrates);
    print_av("human", human);
    print_av("mortal", mortal);
    printf("\n");
    
    /* Perform PLN inference */
    printf("Performing PLN inference...\n");
    struct truth_value tv;
    
    pln_infer(socrates, &tv);
    printf("  socrates: strength=%.2f, confidence=%.2f\n", tv.strength, tv.confidence);
    
    pln_infer(human, &tv);
    printf("  human: strength=%.2f, confidence=%.2f\n", tv.strength, tv.confidence);
    
    pln_infer(mortal, &tv);
    printf("  mortal: strength=%.2f, confidence=%.2f\n\n", tv.strength, tv.confidence);
    
    /* Stop cognitive loop */
    printf("Stopping cognitive loop...\n");
    cogloop_stop();
    
    /* Cleanup */
    cogkern_shutdown();
    printf("✓ Shutdown complete\n\n");
    
    printf("Cognitive loop demonstration complete!\n");
    return 0;
}
