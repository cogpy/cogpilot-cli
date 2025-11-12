#!/bin/bash
# demo.sh - Comprehensive demonstration of cogpilot-cli capabilities
#
# This script demonstrates all major features of the OpenCog CLI including:
# - Knowledge graph construction
# - Attention allocation
# - Probabilistic inference
# - Cognitive loop processing

CLI="./build/cogpilot-cli"

echo "╔════════════════════════════════════════════════════════════╗"
echo "║         cogpilot-cli Demonstration                        ║"
echo "║         OpenCog Cognitive Functions CLI                   ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""

cat << 'EOF' | $CLI
init 64
boot 1
boot 2
boot 3
atom create concept Socrates
atom create concept human
atom create concept mortal
atom create concept Greek
atom create concept philosopher
link create inheritance 1 2
link create inheritance 2 3
link create inheritance 1 4
link create inheritance 1 5
atom list
attention set 1 1000.0 800.0 600.0
attention set 2 500.0 400.0 300.0
attention set 3 200.0 150.0 100.0
attention get 1
attention get 2
attention get 3
attention spread 1 0.7
infer 1
infer 2
infer 3
loop tick
loop tick
loop tick
loop tick
loop tick
attention get 1
atom create concept animal
atom create concept mammal
atom create concept bird
atom create concept dog
atom create concept cat
atom create concept eagle
atom create concept sparrow
link create inheritance 7 6
link create inheritance 8 6
link create inheritance 9 7
link create inheritance 10 7
link create inheritance 11 8
link create inheritance 12 8
attention set 6 800.0 600.0 400.0
attention set 7 600.0 450.0 300.0
attention set 8 600.0 450.0 300.0
attention spread 6 0.5
loop tick
loop tick
loop tick
atom list
shutdown
exit
EOF

echo ""
echo "╔════════════════════════════════════════════════════════════╗"
echo "║         Demonstration Complete                             ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""
echo "For more information:"
echo "  - Run: $CLI help"
echo "  - Read: docs/CLI_USAGE.md"
echo "  - Interactive mode: $CLI (without arguments)"
echo ""
