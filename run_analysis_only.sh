#!/bin/bash
# Run only analysis steps 2-5 (skip event generation)
# Usage: alienv setenv O2Physics/latest -c ./run_analysis_only.sh

echo "========================================"
echo "Running Analysis Steps 2-5 Only"
echo "========================================"

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

echo "Step 3: Extracting Quantification with Fitting..."
root -l -b << EOF
.L z03_ExtractQuantification.C
ExtractQuantification("results/correlations_with_jets.root", "results/quantification.txt")
.q
EOF

if [ $? -ne 0 ]; then
    echo "Error: Quantification extraction failed!"
    exit 1
fi

echo ""
echo "Step 4: Generating Figures..."
root -l -b << EOF
.L z04_PlotResults.C
PlotResults("results/correlations_with_jets.root", "results/figures")
.q
EOF

if [ $? -ne 0 ]; then
    echo "Error: Figure generation failed!"
    exit 1
fi

echo ""
echo "Step 5: Generating LaTeX Tables..."
root -l -b << EOF
.L z05_GenerateTables.C
GenerateTables("results/quantification.txt", "results/tables")
.q
EOF

if [ $? -ne 0 ]; then
    echo "Error: Table generation failed!"
    exit 1
fi

echo ""
echo "========================================"
echo "Analysis Complete!"
echo "========================================"
echo "Results:"
echo "  - Quantification: results/quantification.txt"
echo "  - Figures: results/figures/"
echo "  - Tables: results/tables/"
echo "========================================"
