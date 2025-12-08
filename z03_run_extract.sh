#!/bin/bash
cd /Users/djkim/Documents/GitHub/draw_jet
root -l -b << 'EOF'
.L z03_ExtractQuantification.C
ExtractQuantification("results/correlations_with_jets.root", "results/quantification.txt")
.q
EOF
