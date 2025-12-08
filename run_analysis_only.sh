#!/bin/bash
# Run only analysis steps 3-5 (skip event generation)

echo "========================================"
echo "Running Analysis Steps 3-5 Only"
echo "========================================"

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
