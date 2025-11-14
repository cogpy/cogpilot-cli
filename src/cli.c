/**
 * @file cli.c
 * @brief cogpilot-cli - Command-line interface for OpenCog Kernel
 * 
 * Provides a CLI wrapper around the OpenCog cognitive primitives,
 * enabling terminal-based interaction with AtomSpace, ECAN, PLN,
 * and the cognitive loop.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cogkern.h>

#define VERSION "0.1.0"
#define MAX_ATOMS 100

/**
 * CLI state
 */
static struct {
    int initialized;
    int running;
    atom_handle_t atoms[MAX_ATOMS];
    size_t atom_count;
} cli_state = {0};

/**
 * Print usage information
 */
static void print_usage(const char *prog_name) {
    printf("cogpilot-cli v%s - OpenCog Cognitive Functions CLI\n\n", VERSION);
    printf("Usage: %s <command> [options]\n\n", prog_name);
    printf("Commands:\n");
    printf("  init <mem_size>          Initialize cognitive kernel (mem_size in MB)\n");
    printf("  shutdown                 Shutdown cognitive kernel\n");
    printf("  boot <stage>             Run bootstrap stage (0-3)\n");
    printf("\n");
    printf("AtomSpace Commands:\n");
    printf("  atom create <type> <name>    Create an atom\n");
    printf("  link create <type> <a1> <a2> Create a link between atoms\n");
    printf("  atom list                    List all created atoms\n");
    printf("\n");
    printf("ECAN Commands:\n");
    printf("  attention set <atom> <sti> <lti> <vlti>  Set attention values\n");
    printf("  attention get <atom>                      Get attention values\n");
    printf("  attention spread <atom> <rate>            Spread importance\n");
    printf("\n");
    printf("PLN Commands:\n");
    printf("  infer <atom>             Perform inference on atom\n");
    printf("\n");
    printf("Cognitive Loop Commands:\n");
    printf("  loop start <hz>          Start cognitive loop at frequency\n");
    printf("  loop tick                Execute one loop iteration\n");
    printf("  loop stop                Stop cognitive loop\n");
    printf("\n");
    printf("Utility Commands:\n");
    printf("  help                     Show this help message\n");
    printf("  version                  Show version information\n");
    printf("\n");
    printf("Atom Types:\n");
    printf("  node, link, concept, predicate, evaluation, inheritance, similarity\n");
    printf("\n");
}

/**
 * Print version information
 */
static void print_version(void) {
    printf("cogpilot-cli version %s\n", VERSION);
    printf("OpenCog Kernel Library v0.1.0\n");
}

/**
 * Parse atom type from string
 */
static enum atom_type parse_atom_type(const char *type_str) {
    if (strcmp(type_str, "node") == 0) return ATOM_NODE;
    if (strcmp(type_str, "link") == 0) return ATOM_LINK;
    if (strcmp(type_str, "concept") == 0) return ATOM_CONCEPT;
    if (strcmp(type_str, "predicate") == 0) return ATOM_PREDICATE;
    if (strcmp(type_str, "evaluation") == 0) return ATOM_EVALUATION;
    if (strcmp(type_str, "inheritance") == 0) return ATOM_INHERITANCE;
    if (strcmp(type_str, "similarity") == 0) return ATOM_SIMILARITY;
    return ATOM_NODE; /* default */
}

/**
 * Get atom type name
 */
static const char *get_atom_type_name(enum atom_type type) {
    switch (type) {
        case ATOM_NODE: return "node";
        case ATOM_LINK: return "link";
        case ATOM_CONCEPT: return "concept";
        case ATOM_PREDICATE: return "predicate";
        case ATOM_EVALUATION: return "evaluation";
        case ATOM_INHERITANCE: return "inheritance";
        case ATOM_SIMILARITY: return "similarity";
        default: return "unknown";
    }
}

/**
 * Handle 'init' command
 */
static int cmd_init(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Error: init requires memory size in MB\n");
        fprintf(stderr, "Usage: cogpilot-cli init <mem_size>\n");
        return 1;
    }
    
    if (cli_state.initialized) {
        fprintf(stderr, "Error: kernel already initialized\n");
        return 1;
    }
    
    size_t mem_size_mb = atoi(argv[2]);
    if (mem_size_mb == 0) {
        fprintf(stderr, "Error: invalid memory size\n");
        return 1;
    }
    
    size_t mem_size = mem_size_mb * 1024 * 1024;
    if (cogkern_init(mem_size) != 0) {
        fprintf(stderr, "Error: failed to initialize kernel\n");
        return 1;
    }
    
    cli_state.initialized = 1;
    printf("✓ Cognitive kernel initialized with %zuMB memory\n", mem_size_mb);
    return 0;
}

/**
 * Handle 'shutdown' command
 */
static int cmd_shutdown(int argc, char **argv) {
    (void)argc;
    (void)argv;
    
    if (!cli_state.initialized) {
        fprintf(stderr, "Error: kernel not initialized\n");
        return 1;
    }
    
    cogkern_shutdown();
    cli_state.initialized = 0;
    cli_state.atom_count = 0;
    printf("✓ Cognitive kernel shutdown complete\n");
    return 0;
}

/**
 * Handle 'boot' command
 */
static int cmd_boot(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Error: boot requires stage number (0-3)\n");
        fprintf(stderr, "Usage: cogpilot-cli boot <stage>\n");
        return 1;
    }
    
    if (!cli_state.initialized) {
        fprintf(stderr, "Error: kernel not initialized (run 'init' first)\n");
        return 1;
    }
    
    int stage = atoi(argv[2]);
    if (stage < 0 || stage > 3) {
        fprintf(stderr, "Error: invalid stage (must be 0-3)\n");
        return 1;
    }
    
    if (cogloop_boot_stage((enum boot_stage)stage) != 0) {
        fprintf(stderr, "Error: boot stage %d failed\n", stage);
        return 1;
    }
    
    const char *stage_names[] = {
        "Core initialization",
        "Hypergraph filesystem",
        "Scheduler and memory regions",
        "Cognitive loop"
    };
    
    printf("✓ Stage %d complete: %s\n", stage, stage_names[stage]);
    return 0;
}

/**
 * Handle 'atom create' command
 */
static int cmd_atom_create(int argc, char **argv) {
    if (argc < 5) {
        fprintf(stderr, "Error: atom create requires type and name\n");
        fprintf(stderr, "Usage: cogpilot-cli atom create <type> <name>\n");
        return 1;
    }
    
    if (!cli_state.initialized) {
        fprintf(stderr, "Error: kernel not initialized (run 'init' first)\n");
        return 1;
    }
    
    enum atom_type type = parse_atom_type(argv[3]);
    const char *name = argv[4];
    
    atom_handle_t handle = cog_atom_alloc(type, name);
    if (handle == 0) {
        fprintf(stderr, "Error: failed to create atom\n");
        return 1;
    }
    
    /* Store atom handle */
    if (cli_state.atom_count < MAX_ATOMS) {
        cli_state.atoms[cli_state.atom_count++] = handle;
    }
    
    printf("✓ Created %s atom '%s' (handle: %lu)\n", 
           get_atom_type_name(type), name, handle);
    return 0;
}

/**
 * Handle 'link create' command
 */
static int cmd_link_create(int argc, char **argv) {
    if (argc < 6) {
        fprintf(stderr, "Error: link create requires type and two atom handles\n");
        fprintf(stderr, "Usage: cogpilot-cli link create <type> <handle1> <handle2>\n");
        return 1;
    }
    
    if (!cli_state.initialized) {
        fprintf(stderr, "Error: kernel not initialized (run 'init' first)\n");
        return 1;
    }
    
    enum atom_type type = parse_atom_type(argv[3]);
    atom_handle_t handle1 = atol(argv[4]);
    atom_handle_t handle2 = atol(argv[5]);
    
    atom_handle_t outgoing[2] = {handle1, handle2};
    atom_handle_t link = cog_link_create(type, outgoing, 2);
    
    if (link == 0) {
        fprintf(stderr, "Error: failed to create link\n");
        return 1;
    }
    
    printf("✓ Created %s link: %lu -> %lu (handle: %lu)\n",
           get_atom_type_name(type), handle1, handle2, link);
    return 0;
}

/**
 * Handle 'atom list' command
 */
static int cmd_atom_list(int argc, char **argv) {
    (void)argc;
    (void)argv;
    
    if (!cli_state.initialized) {
        fprintf(stderr, "Error: kernel not initialized (run 'init' first)\n");
        return 1;
    }
    
    if (cli_state.atom_count == 0) {
        printf("No atoms created yet\n");
        return 0;
    }
    
    printf("Created atoms (%zu total):\n", cli_state.atom_count);
    for (size_t i = 0; i < cli_state.atom_count; i++) {
        printf("  - Handle: %lu\n", cli_state.atoms[i]);
    }
    
    return 0;
}

/**
 * Handle 'attention set' command
 */
static int cmd_attention_set(int argc, char **argv) {
    if (argc < 7) {
        fprintf(stderr, "Error: attention set requires atom handle and STI, LTI, VLTI values\n");
        fprintf(stderr, "Usage: cogpilot-cli attention set <handle> <sti> <lti> <vlti>\n");
        return 1;
    }
    
    if (!cli_state.initialized) {
        fprintf(stderr, "Error: kernel not initialized (run 'init' first)\n");
        return 1;
    }
    
    atom_handle_t handle = atol(argv[3]);
    struct attention_value av;
    av.sti = atof(argv[4]);
    av.lti = atof(argv[5]);
    av.vlti = atof(argv[6]);
    
    if (dtesn_sched_set_av(handle, &av) != 0) {
        fprintf(stderr, "Error: failed to set attention values\n");
        return 1;
    }
    
    printf("✓ Set attention for atom %lu: STI=%.1f, LTI=%.1f, VLTI=%.1f\n",
           handle, av.sti, av.lti, av.vlti);
    return 0;
}

/**
 * Handle 'attention get' command
 */
static int cmd_attention_get(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Error: attention get requires atom handle\n");
        fprintf(stderr, "Usage: cogpilot-cli attention get <handle>\n");
        return 1;
    }
    
    if (!cli_state.initialized) {
        fprintf(stderr, "Error: kernel not initialized (run 'init' first)\n");
        return 1;
    }
    
    atom_handle_t handle = atol(argv[3]);
    struct attention_value av;
    
    if (dtesn_sched_get_av(handle, &av) != 0) {
        fprintf(stderr, "Error: failed to get attention values\n");
        return 1;
    }
    
    printf("Attention for atom %lu:\n", handle);
    printf("  STI (Short-term): %.1f\n", av.sti);
    printf("  LTI (Long-term):  %.1f\n", av.lti);
    printf("  VLTI (Very long): %.1f\n", av.vlti);
    return 0;
}

/**
 * Handle 'attention spread' command
 */
static int cmd_attention_spread(int argc, char **argv) {
    if (argc < 5) {
        fprintf(stderr, "Error: attention spread requires atom handle and diffusion rate\n");
        fprintf(stderr, "Usage: cogpilot-cli attention spread <handle> <rate>\n");
        return 1;
    }
    
    if (!cli_state.initialized) {
        fprintf(stderr, "Error: kernel not initialized (run 'init' first)\n");
        return 1;
    }
    
    atom_handle_t handle = atol(argv[3]);
    float rate = atof(argv[4]);
    
    int affected = dtesn_sched_spread_importance(handle, rate);
    if (affected < 0) {
        fprintf(stderr, "Error: failed to spread importance\n");
        return 1;
    }
    
    printf("✓ Spread importance from atom %lu (affected %d atoms)\n", handle, affected);
    return 0;
}

/**
 * Handle 'infer' command
 */
static int cmd_infer(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Error: infer requires atom handle\n");
        fprintf(stderr, "Usage: cogpilot-cli infer <handle>\n");
        return 1;
    }
    
    if (!cli_state.initialized) {
        fprintf(stderr, "Error: kernel not initialized (run 'init' first)\n");
        return 1;
    }
    
    atom_handle_t handle = atol(argv[2]);
    struct truth_value tv;
    
    if (pln_infer(handle, &tv) != 0) {
        fprintf(stderr, "Error: inference failed\n");
        return 1;
    }
    
    printf("Inference result for atom %lu:\n", handle);
    printf("  Strength:   %.3f\n", tv.strength);
    printf("  Confidence: %.3f\n", tv.confidence);
    return 0;
}

/**
 * Handle 'loop start' command
 */
static int cmd_loop_start(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Error: loop start requires frequency in Hz\n");
        fprintf(stderr, "Usage: cogpilot-cli loop start <hz>\n");
        return 1;
    }
    
    if (!cli_state.initialized) {
        fprintf(stderr, "Error: kernel not initialized (run 'init' first)\n");
        return 1;
    }
    
    uint32_t hz = atoi(argv[3]);
    
    if (cogloop_start(hz) != 0) {
        fprintf(stderr, "Error: failed to start cognitive loop\n");
        return 1;
    }
    
    cli_state.running = 1;
    printf("✓ Cognitive loop started at %u Hz\n", hz);
    return 0;
}

/**
 * Handle 'loop tick' command
 */
static int cmd_loop_tick(int argc, char **argv) {
    (void)argc;
    (void)argv;
    
    if (!cli_state.initialized) {
        fprintf(stderr, "Error: kernel not initialized (run 'init' first)\n");
        return 1;
    }
    
    if (cogloop_tick() != 0) {
        fprintf(stderr, "Error: tick failed\n");
        return 1;
    }
    
    printf("✓ Cognitive loop tick complete\n");
    return 0;
}

/**
 * Handle 'loop stop' command
 */
static int cmd_loop_stop(int argc, char **argv) {
    (void)argc;
    (void)argv;
    
    if (!cli_state.initialized) {
        fprintf(stderr, "Error: kernel not initialized (run 'init' first)\n");
        return 1;
    }
    
    cogloop_stop();
    cli_state.running = 0;
    printf("✓ Cognitive loop stopped\n");
    return 0;
}

/**
 * Parse command line and dispatch to appropriate handler
 */
static int dispatch_command(int argc, char **argv) {
    if (argc < 1) {
        return 0;
    }
    
    const char *cmd = argv[0];
    
    /* Help and version commands */
    if (strcmp(cmd, "help") == 0 || strcmp(cmd, "?") == 0) {
        print_usage("cogpilot-cli");
        return 0;
    }
    
    if (strcmp(cmd, "version") == 0) {
        print_version();
        return 0;
    }
    
    if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0) {
        return -1; /* Signal to exit */
    }
    
    /* Core commands */
    if (strcmp(cmd, "init") == 0) {
        char *fake_argv[] = {"cogpilot-cli", "init", argc >= 2 ? argv[1] : NULL};
        return cmd_init(argc >= 2 ? 3 : 2, fake_argv);
    }
    
    if (strcmp(cmd, "shutdown") == 0) {
        char *fake_argv[] = {"cogpilot-cli", "shutdown"};
        return cmd_shutdown(2, fake_argv);
    }
    
    if (strcmp(cmd, "boot") == 0) {
        char *fake_argv[] = {"cogpilot-cli", "boot", argc >= 2 ? argv[1] : NULL};
        return cmd_boot(argc >= 2 ? 3 : 2, fake_argv);
    }
    
    /* AtomSpace commands */
    if (strcmp(cmd, "atom") == 0 && argc >= 2) {
        if (strcmp(argv[1], "create") == 0) {
            char *fake_argv[] = {"cogpilot-cli", "atom", "create", 
                                argc >= 3 ? argv[2] : NULL, argc >= 4 ? argv[3] : NULL};
            return cmd_atom_create(argc >= 4 ? 5 : argc + 2, fake_argv);
        } else if (strcmp(argv[1], "list") == 0) {
            char *fake_argv[] = {"cogpilot-cli", "atom", "list"};
            return cmd_atom_list(3, fake_argv);
        }
    }
    
    if (strcmp(cmd, "link") == 0 && argc >= 2 && strcmp(argv[1], "create") == 0) {
        char *fake_argv[] = {"cogpilot-cli", "link", "create",
                            argc >= 3 ? argv[2] : NULL, argc >= 4 ? argv[3] : NULL,
                            argc >= 5 ? argv[4] : NULL};
        return cmd_link_create(argc >= 5 ? 6 : argc + 1, fake_argv);
    }
    
    /* ECAN commands */
    if (strcmp(cmd, "attention") == 0 && argc >= 2) {
        if (strcmp(argv[1], "set") == 0) {
            char *fake_argv[] = {"cogpilot-cli", "attention", "set",
                                argc >= 3 ? argv[2] : NULL, argc >= 4 ? argv[3] : NULL,
                                argc >= 5 ? argv[4] : NULL, argc >= 6 ? argv[5] : NULL};
            return cmd_attention_set(argc >= 6 ? 7 : argc + 1, fake_argv);
        } else if (strcmp(argv[1], "get") == 0) {
            char *fake_argv[] = {"cogpilot-cli", "attention", "get", argc >= 3 ? argv[2] : NULL};
            return cmd_attention_get(argc >= 3 ? 4 : argc + 1, fake_argv);
        } else if (strcmp(argv[1], "spread") == 0) {
            char *fake_argv[] = {"cogpilot-cli", "attention", "spread",
                                argc >= 3 ? argv[2] : NULL, argc >= 4 ? argv[3] : NULL};
            return cmd_attention_spread(argc >= 4 ? 5 : argc + 1, fake_argv);
        }
    }
    
    /* PLN commands */
    if (strcmp(cmd, "infer") == 0) {
        char *fake_argv[] = {"cogpilot-cli", "infer", argc >= 2 ? argv[1] : NULL};
        return cmd_infer(argc >= 2 ? 3 : 2, fake_argv);
    }
    
    /* Cognitive loop commands */
    if (strcmp(cmd, "loop") == 0 && argc >= 2) {
        if (strcmp(argv[1], "start") == 0) {
            char *fake_argv[] = {"cogpilot-cli", "loop", "start", argc >= 3 ? argv[2] : NULL};
            return cmd_loop_start(argc >= 3 ? 4 : argc + 1, fake_argv);
        } else if (strcmp(argv[1], "tick") == 0) {
            char *fake_argv[] = {"cogpilot-cli", "loop", "tick"};
            return cmd_loop_tick(3, fake_argv);
        } else if (strcmp(argv[1], "stop") == 0) {
            char *fake_argv[] = {"cogpilot-cli", "loop", "stop"};
            return cmd_loop_stop(3, fake_argv);
        }
    }
    
    fprintf(stderr, "Error: unknown command '%s'\n", cmd);
    fprintf(stderr, "Type 'help' for usage information\n");
    return 1;
}

/**
 * Interactive shell mode
 */
static int run_interactive_shell(void) {
    char line[1024];
    char *argv[32];
    int argc;
    
    printf("cogpilot-cli v%s - Interactive Mode\n", VERSION);
    printf("Type 'help' for available commands, 'exit' to quit\n\n");
    
    while (1) {
        printf("cogpilot> ");
        fflush(stdout);
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        /* Remove trailing newline */
        line[strcspn(line, "\n")] = 0;
        
        /* Skip empty lines */
        if (strlen(line) == 0) {
            continue;
        }
        
        /* Parse command line */
        argc = 0;
        char *token = strtok(line, " \t");
        while (token != NULL && argc < 32) {
            argv[argc++] = token;
            token = strtok(NULL, " \t");
        }
        
        if (argc == 0) {
            continue;
        }
        
        /* Dispatch command */
        int ret = dispatch_command(argc, argv);
        if (ret == -1) {
            break; /* Exit requested */
        }
        
        printf("\n");
    }
    
    /* Cleanup if still initialized */
    if (cli_state.initialized) {
        cogkern_shutdown();
    }
    
    printf("Goodbye!\n");
    return 0;
}

/**
 * Main CLI dispatcher
 */
int main(int argc, char **argv) {
    /* Interactive mode if no arguments */
    if (argc < 2) {
        return run_interactive_shell();
    }
    
    const char *cmd = argv[1];
    
    /* Help and version commands */
    if (strcmp(cmd, "help") == 0 || strcmp(cmd, "--help") == 0 || strcmp(cmd, "-h") == 0) {
        print_usage(argv[0]);
        return 0;
    }
    
    if (strcmp(cmd, "version") == 0 || strcmp(cmd, "--version") == 0 || strcmp(cmd, "-v") == 0) {
        print_version();
        return 0;
    }
    
    /* Core commands */
    if (strcmp(cmd, "init") == 0) {
        return cmd_init(argc, argv);
    }
    
    if (strcmp(cmd, "shutdown") == 0) {
        return cmd_shutdown(argc, argv);
    }
    
    if (strcmp(cmd, "boot") == 0) {
        return cmd_boot(argc, argv);
    }
    
    /* AtomSpace commands */
    if (strcmp(cmd, "atom") == 0 && argc >= 3) {
        if (strcmp(argv[2], "create") == 0) {
            return cmd_atom_create(argc, argv);
        } else if (strcmp(argv[2], "list") == 0) {
            return cmd_atom_list(argc, argv);
        }
    }
    
    if (strcmp(cmd, "link") == 0 && argc >= 3 && strcmp(argv[2], "create") == 0) {
        return cmd_link_create(argc, argv);
    }
    
    /* ECAN commands */
    if (strcmp(cmd, "attention") == 0 && argc >= 3) {
        if (strcmp(argv[2], "set") == 0) {
            return cmd_attention_set(argc, argv);
        } else if (strcmp(argv[2], "get") == 0) {
            return cmd_attention_get(argc, argv);
        } else if (strcmp(argv[2], "spread") == 0) {
            return cmd_attention_spread(argc, argv);
        }
    }
    
    /* PLN commands */
    if (strcmp(cmd, "infer") == 0) {
        return cmd_infer(argc, argv);
    }
    
    /* Cognitive loop commands */
    if (strcmp(cmd, "loop") == 0 && argc >= 3) {
        if (strcmp(argv[2], "start") == 0) {
            return cmd_loop_start(argc, argv);
        } else if (strcmp(argv[2], "tick") == 0) {
            return cmd_loop_tick(argc, argv);
        } else if (strcmp(argv[2], "stop") == 0) {
            return cmd_loop_stop(argc, argv);
        }
    }
    
    fprintf(stderr, "Error: unknown command '%s'\n", cmd);
    fprintf(stderr, "Run '%s help' for usage information\n", argv[0]);
    return 1;
}
