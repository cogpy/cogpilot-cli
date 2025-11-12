#!/bin/bash
# Test script for cogpilot-cli
# Demonstrates all CLI commands and functionality

set -e  # Exit on error

CLI="./build/cogpilot-cli"

echo "=========================================="
echo "cogpilot-cli Test Suite"
echo "=========================================="
echo ""

echo "1. Testing initialization..."
$CLI init 64
echo ""

echo "2. Testing bootstrap sequence..."
$CLI boot 0
$CLI boot 1
$CLI boot 2
$CLI boot 3
echo ""

echo "3. Testing AtomSpace - Creating concepts..."
$CLI atom create concept "human"
$CLI atom create concept "mortal"
$CLI atom create concept "Socrates"
echo ""

echo "4. Testing AtomSpace - Creating links..."
$CLI link create inheritance 1 2
$CLI link create inheritance 3 1
echo ""

echo "5. Testing atom listing..."
$CLI atom list
echo ""

echo "6. Testing ECAN - Setting attention values..."
$CLI attention set 1 100.0 50.0 10.0
$CLI attention set 2 80.0 40.0 8.0
$CLI attention set 3 120.0 60.0 12.0
echo ""

echo "7. Testing ECAN - Getting attention values..."
$CLI attention get 1
$CLI attention get 3
echo ""

echo "8. Testing ECAN - Spreading importance..."
$CLI attention spread 3 0.5
echo ""

echo "9. Testing PLN - Inference..."
$CLI infer 1
$CLI infer 3
echo ""

echo "10. Testing Cognitive Loop - Manual tick..."
$CLI loop tick
$CLI loop tick
$CLI loop tick
echo ""

echo "11. Testing shutdown..."
$CLI shutdown
echo ""

echo "=========================================="
echo "All tests passed successfully!"
echo "=========================================="
