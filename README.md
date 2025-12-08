# Pythia Jet Correlation Analysis

Two-particle correlation analysis framework for pp collisions at √s = 13 TeV, categorized by jet multiplicity (Single-jet, Dijet, Multi-jet).

## Quick Start

```bash
# Run complete workflow (default: 1M events)
alienv setenv O2Physics/latest -c ./run_full_workflow.sh

# Or specify number of events
alienv setenv O2Physics/latest -c './run_full_workflow.sh 10000'
```

## Workflow Overview

| Step | Script | Description | Output |
|------|--------|-------------|--------|
| 1 | `z01_GeneratePythiaEvents.C` | Generate Pythia8 events with jets | `results/pythia_events.root` |
| 2 | `z02_RunCorrelationAnalysis.sh` | Run correlation analysis | `results/correlations_with_jets.root` |
| 3 | `z03_ExtractQuantification.C` | Extract NSY, widths, fractions | `results/quantification.txt` |
| 4 | `z04_PlotResults.C` | Generate figures | `results/figures/` |
| 5 | `z05_GenerateTables.C` | Generate LaTeX tables | `results/tables/` |

## Key Features

### Jet Categorization
Events are categorized by jet multiplicity:
- **Single-jet**: nJets = 1
- **Dijet**: nJets = 2
- **Multi-jet**: nJets ≥ 3

### Proper Normalization
Correlation function with correct normalization:
```
C(Δη, Δφ) = (1/N_trig) × S(Δη, Δφ) / (α × M(Δη, Δφ))
```
where α = Integral(S) / Integral(M)

### Analysis Output

**Figures (per pT bin):**
1. 2D correlation plots (Δη vs Δφ) - side-by-side jet categories
2. 1D Δφ projections - colored overlays
3. 1D Δη projections - colored overlays
4. **Inclusive vs Components** - shows how jet categories contribute to total

**Quantification:**
- Near-Side Yield (NSY)
- RMS widths (σ_η, σ_φ)
- Jet category fractions (%)

**Tables:**
- LaTeX format for publication
- NSY, widths, and fractions by pT and jet category

## File Structure

```
draw_jet/
├── z01_GeneratePythiaEvents.C          # Pythia event generation
├── z02_RunCorrelationAnalysis.sh       # Compile and run analysis
├── z03_ExtractQuantification.C         # Extract metrics
├── z03_run_extract.sh                  # Wrapper script
├── z04_PlotResults.C                   # Generate figures
├── z04_run_plot.sh                     # Wrapper script
├── z05_GenerateTables.C                # Generate tables
├── z05_run_tables.sh                   # Wrapper script
├── run_full_workflow.sh                # Master workflow
├── jAnaSimple/                         # Correlation code
│   ├── SimpleCorrelation.C             # Main analysis
│   ├── JTreeDataManager_Pythia.h/cxx   # Tree reader
│   └── Makefile                        # Build system
└── results/                            # Output directory
```

## Analysis Configuration

### pT Bins
- Trigger: {1.0, 2.0, 3.0, 4.0, 8.0} GeV/c
- Associated: {1.0, 2.0, 3.0, 4.0, 8.0} GeV/c

### Event Multiplicity
- Bins: {0, 5, 10, 20, 30, 40, 50, 60, 100}
- **Final analysis**: Multiplicity-integrated (all bins merged)

### Jet Reconstruction
- Algorithm: anti-kT
- R parameter: 0.4
- pT cut: 5 GeV/c
- η range: |η| < 0.5

### Track Selection
- pT > 0.2 GeV/c
- |η| < 0.8

## Running Individual Steps

### Step 1: Generate Events
```bash
root -l -b << EOF
.L z01_GeneratePythiaEvents.C
GeneratePythiaEvents(1000, "results/pythia_events.root", 0.4)
.q
EOF
```

### Step 2: Run Correlation Analysis
```bash
alienv setenv O2Physics/latest -c ./z02_RunCorrelationAnalysis.sh
```

### Step 3: Extract Quantification
```bash
alienv setenv O2Physics/latest -c ./z03_run_extract.sh
```

### Step 4: Generate Figures
```bash
alienv setenv O2Physics/latest -c ./z04_run_plot.sh
```

### Step 5: Generate Tables
```bash
alienv setenv O2Physics/latest -c ./z05_run_tables.sh
```

## Physics Results

Expected trends from the analysis:

| pT Range | Single-jet | Dijet | Multi-jet | Interpretation |
|----------|-----------|-------|-----------|----------------|
| 1-2 GeV | ~37% | ~32% | ~31% | Balanced multi-jet production |
| 3-4 GeV | ~40% | ~30% | ~30% | Single-jet emerging |
| 4-8 GeV | ~49% | ~27% | ~24% | Hard scattering dominance |

**Physical interpretation:**
- High pT: Hard scattering → single high-pT jet
- Low pT: Soft processes → multiple jets

## Requirements

- ALICE O2Physics environment
- ROOT 6.x
- Pythia8
- FastJet

## Customization

### Modify Physics Settings
Edit `z01_GeneratePythiaEvents.C`:
- Collision energy
- Jet algorithm parameters
- pT and η cuts

### Change Analysis Bins
Edit bin arrays in:
- `jAnaSimple/SimpleCorrelation.C` - Analysis bins
- `z03_ExtractQuantification.C` - Quantification
- `z04_PlotResults.C` - Plotting

### Add New Observables
Extend `z03_ExtractQuantification.C` to calculate additional metrics.

## Documentation

- `NORMALIZATION_FIX.md` - Details on correlation normalization
- `FINAL_CHANGES_SUMMARY.md` - Summary of recent changes

## Notes

- Multiplicity-integrated analysis (focuses on jet categories)
- Event mixing with proper normalization (α factor)
- Inclusive = Sum of Single + Dijet + Multi-jet
- Color scheme: Blue (Single), Red (Dijet), Green (Multi-jet)
