#!/bin/bash
cd /Users/djkim/Documents/GitHub/draw_jet
root -l -b << 'EOF'
.L z04_PlotResults.C
PlotResults("results/correlations_with_jets.root", "results/figures")
.q
EOF
