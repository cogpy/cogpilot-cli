/**
 * @file atomspace_demo.c
 * @brief AtomSpace demonstration
 * 
 * Demonstrates creation and manipulation of atoms and links in the hypergraph.
 */

#include <stdio.h>
#include <cogkern.h>

int main(void) {
    printf("OpenCog Kernel - AtomSpace Demo\n");
    printf("================================\n\n");
    
    /* Initialize */
    cogkern_init(64 * 1024 * 1024);
    cogloop_boot_stage(STAGE0_INIT);
    cogloop_boot_stage(STAGE1_HYPERGRAPH);
    
    printf("Building knowledge graph: Animals taxonomy\n");
    printf("-------------------------------------------\n\n");
    
    /* Create concept nodes */
    atom_handle_t animal = cog_atom_alloc(ATOM_CONCEPT, "animal");
    atom_handle_t mammal = cog_atom_alloc(ATOM_CONCEPT, "mammal");
    atom_handle_t cat = cog_atom_alloc(ATOM_CONCEPT, "cat");
    atom_handle_t dog = cog_atom_alloc(ATOM_CONCEPT, "dog");
    atom_handle_t bird = cog_atom_alloc(ATOM_CONCEPT, "bird");
    atom_handle_t sparrow = cog_atom_alloc(ATOM_CONCEPT, "sparrow");
    
    printf("Created concepts:\n");
    printf("  - animal (handle: %lu)\n", animal);
    printf("  - mammal (handle: %lu)\n", mammal);
    printf("  - cat (handle: %lu)\n", cat);
    printf("  - dog (handle: %lu)\n", dog);
    printf("  - bird (handle: %lu)\n", bird);
    printf("  - sparrow (handle: %lu)\n\n", sparrow);
    
    /* Create inheritance hierarchy */
    printf("Creating inheritance links:\n");
    
    atom_handle_t outgoing[2];
    
    outgoing[0] = mammal;
    outgoing[1] = animal;
    atom_handle_t link1 = cog_link_create(ATOM_INHERITANCE, outgoing, 2);
    printf("  - mammal -> animal (handle: %lu)\n", link1);
    
    outgoing[0] = bird;
    outgoing[1] = animal;
    atom_handle_t link2 = cog_link_create(ATOM_INHERITANCE, outgoing, 2);
    printf("  - bird -> animal (handle: %lu)\n", link2);
    
    outgoing[0] = cat;
    outgoing[1] = mammal;
    atom_handle_t link3 = cog_link_create(ATOM_INHERITANCE, outgoing, 2);
    printf("  - cat -> mammal (handle: %lu)\n", link3);
    
    outgoing[0] = dog;
    outgoing[1] = mammal;
    atom_handle_t link4 = cog_link_create(ATOM_INHERITANCE, outgoing, 2);
    printf("  - dog -> mammal (handle: %lu)\n", link4);
    
    outgoing[0] = sparrow;
    outgoing[1] = bird;
    atom_handle_t link5 = cog_link_create(ATOM_INHERITANCE, outgoing, 2);
    printf("  - sparrow -> bird (handle: %lu)\n\n", link5);
    
    /* Create similarity links */
    printf("Creating similarity links:\n");
    
    outgoing[0] = cat;
    outgoing[1] = dog;
    atom_handle_t sim1 = cog_link_create(ATOM_SIMILARITY, outgoing, 2);
    printf("  - cat <-> dog (handle: %lu)\n\n", sim1);
    
    /* Add truth values for inference */
    printf("Adding truth values for PLN inference:\n");
    
    struct truth_value tv_high = { .strength = 0.9f, .confidence = 0.8f };
    struct truth_value tv_med = { .strength = 0.7f, .confidence = 0.6f };
    
    cog_link_infer(cat, mammal, &tv_high);
    printf("  - cat -> mammal: strength=%.2f, confidence=%.2f\n", 
           tv_high.strength, tv_high.confidence);
    
    cog_link_infer(cat, dog, &tv_med);
    printf("  - cat <-> dog: strength=%.2f, confidence=%.2f\n\n",
           tv_med.strength, tv_med.confidence);
    
    printf("AtomSpace construction complete!\n");
    printf("Total atoms created: 6 concepts + 6 links = 12 atoms\n\n");
    
    /* Cleanup */
    cogkern_shutdown();
    
    return 0;
}
