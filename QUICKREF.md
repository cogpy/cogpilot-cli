# cogpilot-cli Quick Reference

## Usage
```bash
./cogpilot-cli              # Interactive mode
./cogpilot-cli help         # Show help
./cogpilot-cli version      # Show version
```

## Core Commands
| Command | Description | Example |
|---------|-------------|---------|
| `init <MB>` | Initialize kernel | `init 64` |
| `shutdown` | Shutdown kernel | `shutdown` |
| `boot <stage>` | Bootstrap stage 0-3 | `boot 1` |

## AtomSpace Commands
| Command | Description | Example |
|---------|-------------|---------|
| `atom create <type> <name>` | Create atom | `atom create concept human` |
| `atom list` | List all atoms | `atom list` |
| `link create <type> <h1> <h2>` | Create link | `link create inheritance 1 2` |

## ECAN (Attention) Commands
| Command | Description | Example |
|---------|-------------|---------|
| `attention set <h> <sti> <lti> <vlti>` | Set attention | `attention set 1 100.0 50.0 10.0` |
| `attention get <handle>` | Get attention | `attention get 1` |
| `attention spread <h> <rate>` | Spread importance | `attention spread 1 0.5` |

## PLN (Inference) Commands
| Command | Description | Example |
|---------|-------------|---------|
| `infer <handle>` | Run inference | `infer 1` |

## Cognitive Loop Commands
| Command | Description | Example |
|---------|-------------|---------|
| `loop start <hz>` | Start loop | `loop start 1000` |
| `loop tick` | Manual tick | `loop tick` |
| `loop stop` | Stop loop | `loop stop` |

## Atom Types
- `node` - Basic node
- `concept` - Concept node
- `predicate` - Predicate node
- `link` - Generic link
- `inheritance` - Inheritance link
- `similarity` - Similarity link
- `evaluation` - Evaluation link

## Quick Start Example
```bash
$ ./cogpilot-cli
cogpilot> init 64
cogpilot> boot 1
cogpilot> atom create concept human
cogpilot> atom create concept mortal
cogpilot> link create inheritance 1 2
cogpilot> attention set 1 100.0 50.0 10.0
cogpilot> infer 1
cogpilot> exit
```

## Scripting Example
```bash
cat << EOF | ./cogpilot-cli
init 64
boot 1
atom create concept test
atom list
shutdown
exit
EOF
```

## Tips
- Always run `init` before other commands
- Atom handles start at 1 and increment
- Use `help` for full command list
- Press Ctrl+D or type `exit` to quit interactive mode

For full documentation, see `docs/CLI_USAGE.md`
