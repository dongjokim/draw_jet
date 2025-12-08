#!/bin/bash
cd /Users/djkim/Documents/GitHub/draw_jet/jAnaSimple
echo "Copying SimpleCorrelation_Pythia.C to SimpleCorrelation.C..."
cp SimpleCorrelation_Pythia.C SimpleCorrelation.C
echo "Cleaning..."
make clean
echo "Compiling..."
make
