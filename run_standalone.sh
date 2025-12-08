#!/bin/bash
# Standalone script to run Pythia event generation without O2Physics conflicts
# This script uses a minimal ROOT setup to avoid autoloading issues
# Usage: ./run_standalone.sh [nEvents] [outputFile] [jetR]

# Check if ALICE environment is loaded
if [ -z "$PYTHIA8" ] || [ -z "$FASTJET" ]; then
    echo "Error: PYTHIA8 or FASTJET not set!"
    echo "Please run: alienv enter O2Physics/latest-master-o2"
    exit 1
fi

# Get arguments or use defaults
nEvents=${1:-1000}
outputFile=${2:-pythia_events.root}
jetR=${3:-0.4}

echo "========================================"
echo "Standalone Pythia Event Generation"
echo "========================================"
echo "PYTHIA8: $PYTHIA8"
echo "FASTJET: $FASTJET"
echo "Events: $nEvents"
echo "Output: $outputFile"
echo "Jet R: $jetR"
echo ""

# Run with minimal ROOT setup
# Use here-doc without quotes to allow variable expansion
root -b -n <<EOF
// Disable autoloading immediately
gInterpreter->SetClassAutoloading(false);
gInterpreter->SetClassAutoloading(false);  // Call twice

cout << "Loading libraries explicitly..." << endl;

// Load ROOT libraries first
gSystem->Load("libTree");
gSystem->Load("libPhysics");
cout << "✓ ROOT libraries loaded" << endl;

// Load Pythia8
TString pythia8Path = gSystem->Getenv("PYTHIA8");
if(pythia8Path.Length() > 0) {
  TString pythiaLib = pythia8Path + "/lib/libpythia8";
  int result = gSystem->Load(pythiaLib);
  if(result >= 0) {
    cout << "✓ Pythia 8 loaded" << endl;
  } else {
    cerr << "✗ Error loading Pythia 8" << endl;
    exit(1);
  }
}

// Load FastJet
TString fastjetPath = gSystem->Getenv("FASTJET");
if(fastjetPath.Length() > 0) {
  TString fastjetLib = fastjetPath + "/lib/libfastjet";
  int result = gSystem->Load(fastjetLib);
  if(result >= 0) {
    cout << "✓ FastJet loaded" << endl;
  } else {
    cerr << "✗ Error loading FastJet" << endl;
    exit(1);
  }
}

cout << "\nLoading macro (interpreted mode)..." << endl;
.L z01_GeneratePythiaEvents.C

cout << "\nGenerating events..." << endl;
cout << "  Events: $nEvents" << endl;
cout << "  Output: $outputFile" << endl;
cout << "  Jet R: $jetR" << endl;
cout << "" << endl;

GeneratePythiaEvents($nEvents, "$outputFile", $jetR);
EOF

echo ""
echo "Done!"
