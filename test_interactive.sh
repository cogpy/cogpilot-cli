#!/bin/bash
# Interactive test script for cogpilot-cli
# Uses the interactive shell mode to test functionality

CLI="./build/cogpilot-cli"

# Test interactive mode with piped commands
cat << 'EOF' | $CLI
init 64
boot 0
boot 1
boot 2
boot 3
atom create concept human
atom create concept mortal
atom create concept Socrates
link create inheritance 1 2
link create inheritance 3 1
atom list
attention set 1 100.0 50.0 10.0
attention set 3 120.0 60.0 12.0
attention get 1
attention get 3
attention spread 3 0.5
infer 1
infer 3
loop tick
loop tick
shutdown
exit
EOF

echo ""
echo "Interactive test completed!"
