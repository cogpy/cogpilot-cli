# cogpilot-cli Usage Guide

A command-line interface for interacting with OpenCog cognitive functions.

## Version: 0.1.0

---

## Table of Contents

1. [Overview](#overview)
2. [Installation](#installation)
3. [Usage Modes](#usage-modes)
4. [Command Reference](#command-reference)
5. [Examples](#examples)
6. [Workflows](#workflows)

---

## Overview

`cogpilot-cli` provides a terminal interface to the OpenCog kernel, enabling direct interaction with cognitive primitives including AtomSpace, ECAN attention allocation, PLN inference, and the cognitive loop.

### Features

- **Interactive Shell**: REPL-style interface for exploring cognitive functions
- **Command Mode**: Single-command execution for scripting and automation
- **Full API Coverage**: Access to all OpenCog kernel functions
- **State Management**: Persistent state within a session
- **Easy Testing**: Scriptable interface for cognitive experiments

---

## Installation

### Build from Source

```bash
# Clone the repository
git clone https://github.com/cogpy/cogpilot-cli.git
cd cogpilot-cli

# Build
mkdir build && cd build
cmake ..
make

# The executable will be at: build/cogpilot-cli
```

### Install (Optional)

```bash
# From the build directory
sudo make install

# This installs:
# - /usr/local/bin/cogpilot-cli (executable)
# - /usr/local/lib/libcogkern.a (library)
# - /usr/local/include/cogkern.h (header)
```

---

## Usage Modes

### Interactive Mode

Run without arguments to start an interactive shell:

```bash
./cogpilot-cli
```

This starts a REPL session where you can enter commands interactively:

```
cogpilot-cli v0.1.0 - Interactive Mode
Type 'help' for available commands, 'exit' to quit

cogpilot> init 64
✓ Cognitive kernel initialized with 64MB memory

cogpilot> help
...

cogpilot> exit
Goodbye!
```

### Command Mode

Execute a single command and exit:

```bash
./cogpilot-cli help
./cogpilot-cli version
```

### Scripted Mode

Pipe commands from a script or stdin:

```bash
# From a file
cat commands.txt | ./cogpilot-cli

# From here-doc
./cogpilot-cli << EOF
init 64
boot 1
atom create concept test
exit
EOF
```

---

## Command Reference

### Core Commands

#### `init <mem_size>`
Initialize the cognitive kernel.

**Parameters:**
- `mem_size`: Memory pool size in megabytes (MB)

**Example:**
```bash
cogpilot> init 64
✓ Cognitive kernel initialized with 64MB memory
```

#### `shutdown`
Shutdown the cognitive kernel and free all resources.

**Example:**
```bash
cogpilot> shutdown
✓ Cognitive kernel shutdown complete
```

#### `boot <stage>`
Execute a bootstrap stage.

**Parameters:**
- `stage`: Stage number (0-3)
  - `0`: Core initialization
  - `1`: Hypergraph filesystem
  - `2`: Scheduler and memory regions
  - `3`: Cognitive loop

**Example:**
```bash
cogpilot> boot 1
✓ Stage 1 complete: Hypergraph filesystem
```

---

### AtomSpace Commands

#### `atom create <type> <name>`
Create a new atom in the AtomSpace.

**Parameters:**
- `type`: Atom type (node, link, concept, predicate, evaluation, inheritance, similarity)
- `name`: Atom name (string)

**Returns:** Atom handle (numeric ID)

**Example:**
```bash
cogpilot> atom create concept human
✓ Created concept atom 'human' (handle: 1)
```

#### `atom list`
List all atoms created in the current session.

**Example:**
```bash
cogpilot> atom list
Created atoms (3 total):
  - Handle: 1
  - Handle: 2
  - Handle: 3
```

#### `link create <type> <handle1> <handle2>`
Create a link between two atoms.

**Parameters:**
- `type`: Link type (inheritance, similarity, evaluation, etc.)
- `handle1`: First atom handle
- `handle2`: Second atom handle

**Returns:** Link handle (numeric ID)

**Example:**
```bash
cogpilot> link create inheritance 1 2
✓ Created inheritance link: 1 -> 2 (handle: 3)
```

---

### ECAN (Attention) Commands

#### `attention set <handle> <sti> <lti> <vlti>`
Set attention values for an atom.

**Parameters:**
- `handle`: Atom handle
- `sti`: Short-term importance (float)
- `lti`: Long-term importance (float)
- `vlti`: Very long-term importance (float)

**Example:**
```bash
cogpilot> attention set 1 100.0 50.0 10.0
✓ Set attention for atom 1: STI=100.0, LTI=50.0, VLTI=10.0
```

#### `attention get <handle>`
Get attention values for an atom.

**Parameters:**
- `handle`: Atom handle

**Example:**
```bash
cogpilot> attention get 1
Attention for atom 1:
  STI (Short-term): 100.0
  LTI (Long-term):  50.0
  VLTI (Very long): 10.0
```

#### `attention spread <handle> <rate>`
Spread importance from an atom to connected atoms.

**Parameters:**
- `handle`: Source atom handle
- `rate`: Diffusion rate (0.0-1.0)

**Example:**
```bash
cogpilot> attention spread 1 0.5
✓ Spread importance from atom 1 (affected 2 atoms)
```

---

### PLN (Inference) Commands

#### `infer <handle>`
Perform probabilistic inference on an atom.

**Parameters:**
- `handle`: Atom handle

**Returns:** Truth value (strength and confidence)

**Example:**
```bash
cogpilot> infer 1
Inference result for atom 1:
  Strength:   0.750
  Confidence: 0.850
```

---

### Cognitive Loop Commands

#### `loop start <hz>`
Start the cognitive loop at a specified frequency.

**Parameters:**
- `hz`: Frequency in Hertz (0 for manual tick mode)

**Example:**
```bash
cogpilot> loop start 1000
✓ Cognitive loop started at 1000 Hz
```

#### `loop tick`
Execute one iteration of the cognitive loop manually.

**Example:**
```bash
cogpilot> loop tick
✓ Cognitive loop tick complete
```

#### `loop stop`
Stop the cognitive loop.

**Example:**
```bash
cogpilot> loop stop
✓ Cognitive loop stopped
```

---

### Utility Commands

#### `help`
Display help information with all available commands.

**Example:**
```bash
cogpilot> help
cogpilot-cli v0.1.0 - OpenCog Cognitive Functions CLI
...
```

#### `version`
Display version information.

**Example:**
```bash
cogpilot> version
cogpilot-cli version 0.1.0
OpenCog Kernel Library v0.1.0
```

#### `exit` / `quit`
Exit the interactive shell (interactive mode only).

---

## Examples

### Example 1: Basic Knowledge Graph

Create a simple ontology about animals:

```bash
$ ./cogpilot-cli
cogpilot> init 64
cogpilot> boot 1

# Create concepts
cogpilot> atom create concept animal
cogpilot> atom create concept mammal
cogpilot> atom create concept dog
cogpilot> atom create concept cat

# Create inheritance hierarchy
cogpilot> link create inheritance 2 1
cogpilot> link create inheritance 3 2
cogpilot> link create inheritance 4 2

cogpilot> atom list
cogpilot> exit
```

### Example 2: Attention Allocation

Demonstrate attention spreading in a knowledge graph:

```bash
$ ./cogpilot-cli
cogpilot> init 64
cogpilot> boot 1
cogpilot> boot 2

# Create atoms
cogpilot> atom create concept important_fact
cogpilot> atom create concept related_fact

# Set high attention on important fact
cogpilot> attention set 1 1000.0 500.0 100.0

# Check initial attention
cogpilot> attention get 2

# Spread importance
cogpilot> attention spread 1 0.8

# Check updated attention
cogpilot> attention get 2

cogpilot> exit
```

### Example 3: Inference Pipeline

Run a simple inference demonstration:

```bash
$ ./cogpilot-cli
cogpilot> init 64
cogpilot> boot 1

# Create Socrates syllogism
cogpilot> atom create concept Socrates
cogpilot> atom create concept human
cogpilot> atom create concept mortal

# Socrates is human
cogpilot> link create inheritance 1 2

# Humans are mortal
cogpilot> link create inheritance 2 3

# Perform inference
cogpilot> infer 1

cogpilot> exit
```

### Example 4: Cognitive Loop

Run the cognitive loop with manual ticks:

```bash
$ ./cogpilot-cli
cogpilot> init 64
cogpilot> boot 1
cogpilot> boot 2
cogpilot> boot 3

# Create some atoms
cogpilot> atom create concept test1
cogpilot> attention set 1 100.0 50.0 10.0

# Run loop iterations
cogpilot> loop tick
cogpilot> loop tick
cogpilot> loop tick

# Check attention after decay
cogpilot> attention get 1

cogpilot> exit
```

---

## Workflows

### Workflow 1: Interactive Exploration

Use the interactive shell for exploring cognitive functions:

1. Start shell: `./cogpilot-cli`
2. Initialize: `init 64`
3. Bootstrap: `boot 1`, `boot 2`, `boot 3`
4. Experiment with atoms and attention
5. Exit: `exit`

### Workflow 2: Automated Testing

Create a script file `test.txt`:

```
init 64
boot 1
atom create concept test
atom list
shutdown
exit
```

Run it:
```bash
cat test.txt | ./cogpilot-cli
```

### Workflow 3: Integration with Shell Scripts

```bash
#!/bin/bash

# Create a knowledge base
./cogpilot-cli << EOF
init 64
boot 1
atom create concept $1
atom create concept $2
link create inheritance 1 2
shutdown
exit
EOF
```

---

## Tips and Best Practices

1. **Always Initialize First**: Run `init <mem_size>` before any other commands
2. **Bootstrap Stages**: Run boot stages in order (1, 2, 3) for full functionality
3. **Track Handles**: Note atom handles when creating atoms for later reference
4. **Attention Values**: Use STI for short-term relevance, LTI for persistent importance
5. **Interactive Mode**: Best for exploration and learning
6. **Scripted Mode**: Best for automated testing and reproducible experiments

---

## Troubleshooting

### Common Errors

**"Error: kernel not initialized"**
- Solution: Run `init <mem_size>` first

**"Error: invalid memory size"**
- Solution: Provide a valid positive number for memory size (e.g., `init 64`)

**"Error: failed to create atom"**
- Solution: Ensure kernel is initialized and you've run at least `boot 1`

**"Error: unknown command"**
- Solution: Type `help` to see available commands

---

## Further Reading

- [OpenCog Kernel Implementation Guide](IMPLEMENTATION.md)
- [API Function Manifest](KERNEL_FUNCTION_MANIFEST.md)
- [OpenCog Framework Documentation](https://wiki.opencog.org/)

---

**Version:** 0.1.0  
**Last Updated:** 2025-11-12
