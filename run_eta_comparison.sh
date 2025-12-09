#!/bin/bash
# Run eta projection comparison plots
# Usage: alienv setenv O2Physics/latest -c ./run_eta_comparison.sh

echo "========================================"
echo "Generating Delta-Eta Comparison Plots"
echo "========================================"

root -l -b << EOF
.L z06_CompareEtaProjections.C
z06_CompareEtaProjections("results/correlations_with_jets.root", "results/quantification.txt", "results/figures")
.q
EOF

echo ""
echo "========================================"
echo "Eta comparison plots generated!"
echo "========================================"
