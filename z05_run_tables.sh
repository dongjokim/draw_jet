#!/bin/bash
cd /Users/djkim/Documents/GitHub/draw_jet
root -l -b << 'EOF'
.L z05_GenerateTables.C
GenerateTables("results/quantification.txt", "results/tables")
.q
EOF
