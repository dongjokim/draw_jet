#!/bin/bash
# Run correlation analysis using jAnaSimple
# Usage: alienv setenv O2Physics/latest -c ./z02_RunCorrelationAnalysis.sh

cd /Users/djkim/Documents/GitHub/draw_jet

echo "========================================"
echo "Running Correlation Analysis"
echo "========================================"

# Compile jAnaSimple
cd jAnaSimple
echo "Compiling jAnaSimple..."
make clean
make

if [ $? -ne 0 ]; then
    echo "Error: Compilation failed!"
    exit 1
fi

# Create input file list
echo "../results/pythia_events.root" > input_trees.txt

# Run correlation analysis
echo "Running correlation analysis..."
./SimpleCorrelation input_trees.txt ../results/correlations_with_jets.root

if [ $? -ne 0 ]; then
    echo "Error: Correlation analysis failed!"
    exit 1
fi

cd ..

echo "✓ Correlation analysis complete: results/correlations_with_jets.root"
