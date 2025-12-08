# Pythia 8 Event Display

This code generates events directly from Pythia 8 and displays them with correlation plots.

## Requirements

- ALICE O2Physics environment (you're already in it!)
- Pythia 8 (available via `$PYTHIA8`)
- FastJet (available via `alienv q | grep fastjet`)
- ROOT with TEve support

## Quick Start

### Option 1: Using setup macro (Recommended)
```bash
root -l
```

Then in ROOT:
```cpp
.L setup_pythia.C
.L draw_jet_pythia.C
draw_jet_pythia();
```

The `setup_pythia.C` macro automatically loads all required libraries.

### Option 2: Using the script
```bash
./run_pythia.sh
```

### Option 3: Manual loading
If you prefer to load libraries manually:
```cpp
gInterpreter->SetClassAutoloading(false);
gSystem->Load("libGCocoa");
gSystem->Load("libGui");
gSystem->Load("libEve");
gSystem->Load("libGeom");
gSystem->Load("libpythia8");
gSystem->Load("libfastjet");
.L draw_jet_pythia.C
draw_jet_pythia();
```

**Note:** Libraries must be loaded before parsing the macro (`.L`) because ROOT needs them to resolve Pythia8 and FastJet headers.

## Features

- **Direct Pythia 8 integration**: Generates pp collisions at 13 TeV
- **FastJet jet finding**: Uses anti-kT algorithm (R=0.4) with pT > 5 GeV
- **3D event display**: Same visualization as the original draw_jet.C
- **Correlation plots**: Delta eta/phi histograms for particle pairs
- **Event navigation**: Forward/backward buttons to generate new events

## Configuration

The Pythia configuration is in `draw_jet_pythia()`:
- Center-of-mass energy: 13 TeV
- Process: Hard QCD (HardQCD:all = on)
- Minimum pT hat: 20 GeV

You can modify these settings in the code if needed.

## Differences from draw_jet.C

- Reads directly from Pythia 8 instead of text files
- Uses FastJet for jet finding
- Event navigation generates new events (can't go backwards in Pythia)

## Troubleshooting

If you get library loading errors:
1. Make sure you're in the ALICE environment: `alienv printenv`
2. Check Pythia 8: `echo $PYTHIA8`
3. Check FastJet: `echo $FASTJET`
4. Try loading libraries manually in ROOT:
   ```cpp
   gSystem->Load("libpythia8");
   gSystem->Load("libfastjet");
   ```
