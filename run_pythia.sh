#!/bin/bash
# Simple script to run draw_jet_pythia in ALICE environment
# Usage: ./run_pythia.sh

# Make sure we're in ALICE environment
if [ -z "$ALICE_ROOT" ] && [ -z "$ROOTSYS" ]; then
    echo "Error: ALICE environment not loaded!"
    echo "Please run: alienv enter O2Physics/latest-master-o2"
    exit 1
fi

echo "Starting ROOT with draw_jet_pythia..."
echo ""

# Start ROOT and run commands
root -n -l << 'ROOTSCRIPT'
// Disable autoloading to avoid O2/Boost conflicts
gInterpreter->SetClassAutoloading(false);

// Ensure we're in GUI mode (not batch)
if(gApplication) {
  cout << "ROOT application already initialized" << endl;
} else {
  cout << "Note: Running in batch mode - GUI may not work properly" << endl;
  cout << "Try starting ROOT interactively: root -l" << endl;
}

// Load required libraries
cout << "Loading libraries..." << endl;
// Load GUI libraries first (needed for macOS)
gSystem->Load("libGCocoa");
gSystem->Load("libGui");
gSystem->Load("libEve");
gSystem->Load("libGeom");

TString pythia8Path = gSystem->Getenv("PYTHIA8");
if(pythia8Path.Length() > 0) {
  TString pythiaLib = pythia8Path + "/lib/libpythia8";
  if(gSystem->Load(pythiaLib) >= 0) {
    cout << "✓ Pythia 8 loaded" << endl;
  }
}

TString fastjetPath = gSystem->Getenv("FASTJET");
if(fastjetPath.Length() > 0) {
  TString fastjetLib = fastjetPath + "/lib/libfastjet";
  if(gSystem->Load(fastjetLib) >= 0) {
    cout << "✓ FastJet loaded" << endl;
  }
}

// Load the macro
cout << "\nLoading draw_jet_pythia.C..." << endl;
gROOT->LoadMacro("draw_jet_pythia.C");

cout << "\n========================================" << endl;
cout << "Ready! The event display should open." << endl;
cout << "Use the forward/back buttons to navigate events." << endl;
cout << "========================================" << endl;

// Start the display
draw_jet_pythia();
ROOTSCRIPT
