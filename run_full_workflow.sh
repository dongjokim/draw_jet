#!/bin/bash
# Complete workflow for Pythia jet correlation analysis
# Usage: alienv setenv O2Physics/latest -c ./run_full_workflow.sh [nEvents]

set -e  # Exit on error

NEVENTS=${1:-1000000}
WORKDIR=/Users/djkim/Documents/GitHub/draw_jet

echo "========================================"
echo "Pythia Jet Correlation Analysis Workflow"
echo "========================================"
echo "Number of events: $NEVENTS"
echo "Working directory: $WORKDIR"
echo ""

cd $WORKDIR
mkdir -p results

# Step 1: Generate Pythia events
echo "========================================"
echo "Step 1: Generating Pythia Events"
echo "========================================"
root -l -b << EOF
.L z01_GeneratePythiaEvents.C
GeneratePythiaEvents($NEVENTS, "results/pythia_events.root", 0.4)
.q
EOF

if [ ! -f "results/pythia_events.root" ]; then
    echo "Error: Pythia event generation failed!"
    exit 1
fi
echo "✓ Pythia events generated"
echo ""

# Step 2: Run correlation analysis
echo "========================================"
echo "Step 2: Running Correlation Analysis"
echo "========================================"
cd jAnaSimple
make clean && make

if [ $? -ne 0 ]; then
    echo "Error: Compilation failed!"
    exit 1
fi

echo "../results/pythia_events.root" > input_trees.txt
./SimpleCorrelation input_trees.txt ../results/correlations_with_jets.root

if [ $? -ne 0 ]; then
    echo "Error: Correlation analysis failed!"
    exit 1
fi
cd ..
echo "✓ Correlation analysis complete"
echo ""

# Step 3: Extract quantification metrics
echo "========================================"
echo "Step 3: Extracting Quantification Metrics"
echo "========================================"
root -l -b << EOF
.L z03_ExtractQuantification.C
ExtractQuantification("results/correlations_with_jets.root", "results/quantification.txt")
.q
EOF

if [ ! -f "results/quantification.txt" ]; then
    echo "Error: Quantification extraction failed!"
    exit 1
fi
echo "✓ Quantification metrics extracted"
echo ""

# Step 4: Generate figures
echo "========================================"
echo "Step 4: Generating Figures"
echo "========================================"
root -l -b << EOF
.L z04_PlotResults.C
PlotResults("results/correlations_with_jets.root", "results/figures")
.q
EOF
echo "✓ Figures generated"
echo ""

# Step 5: Generate tables
echo "========================================"
echo "Step 5: Generating LaTeX Tables"
echo "========================================"
root -l -b << EOF
.L z05_GenerateTables.C
GenerateTables("results/quantification.txt", "results/tables")
.q
EOF
echo "✓ LaTeX tables generated"
echo ""

# Summary
echo "========================================"
echo "Workflow Complete!"
echo "========================================"
echo "Results directory: results/"
echo "  - Events: results/pythia_events.root"
echo "  - Correlations: results/correlations_with_jets.root"
echo "  - Quantification: results/quantification.txt"
echo "  - Figures: results/figures/"
echo "  - Tables: results/tables/"
echo "========================================"
