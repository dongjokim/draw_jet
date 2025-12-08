# Summary of Final Changes

## 1. Fixed Mixed Event Normalization

### Problem
The original correlation analysis had incorrect normalization that didn't account for:
- Number of trigger particles
- Proper matching of same-event and mixed-event statistics

### Solution
Implemented proper two-particle correlation normalization:

```
C(Δη, Δφ) = (1/N_trig) × S(Δη, Δφ) / (α × M(Δη, Δφ))
```

Where:
- **N_trig** = Total number of trigger particles
- **α** = `Integral(S) / Integral(M)` (normalization factor)

**Modified File:** `jAnaSimple/SimpleCorrelation.C`
- Added trigger counters: `nTriggersCount` and `nTriggersCount_JetCat`
- Proper normalization in lines 438-502

## 2. Added Inclusive vs Components Comparison Plots

### Feature
Added comparison plots showing how Single-jet, Dijet, and Multi-jet components contribute to the inclusive correlation function.

**Modified File:** `z04_PlotResults.C` (lines 288-406)

**New Plots Generated:**
- `comparison_inclusive_trig*_assoc*.pdf/png`

**Plot Features:**
- **Inclusive** (thick black solid line): Sum of all jet categories
- **Components** (colored dashed lines):
  - Single-jet (Blue dashed)
  - Dijet (Red dashed)
  - Multi-jet (Green dashed)
- Side-by-side: Δφ projection (left) and Δη projection (right)

## 3. Cleaned Up File Structure

### Removed Files
- All intermediate test and check scripts
- Old non-merged analysis versions
- Temporary compilation and debugging scripts

### Renamed Scripts
Standardized workflow script names:
- `z01_GeneratePythiaEvents.C` - Generate events
- `z02_RunCorrelationAnalysis.sh` - Run correlation analysis
- `z03_ExtractQuantification.C` + `z03_run_extract.sh` - Extract quantification
- `z04_PlotResults.C` + `z04_run_plot.sh` - Generate figures
- `z05_GenerateTables.C` + `z05_run_tables.sh` - Generate tables
- `run_full_workflow.sh` - Master workflow script

### Final Structure
```
draw_jet/
├── z01_GeneratePythiaEvents.C          # Event generation with jets
├── z02_RunCorrelationAnalysis.sh       # Compile and run correlation
├── z03_ExtractQuantification.C         # Extract metrics (merged mult)
├── z03_run_extract.sh                  # Wrapper for extraction
├── z04_PlotResults.C                   # Generate figures (merged mult)
├── z04_run_plot.sh                     # Wrapper for plotting
├── z05_GenerateTables.C                # Generate LaTeX tables
├── z05_run_tables.sh                   # Wrapper for tables
├── run_full_workflow.sh                # Master workflow
├── jAnaSimple/                         # Correlation analysis code
│   ├── SimpleCorrelation.C             # Main analysis (FIXED NORMALIZATION)
│   ├── JTreeDataManager_Pythia.h/cxx   # Pythia tree reader
│   └── Makefile                        # Build system
├── results/                            # Output directory
│   ├── pythia_events.root              # Generated events
│   ├── correlations_with_jets.root     # Correlation histograms
│   ├── quantification.txt              # Extracted metrics
│   ├── figures/                        # Plots
│   └── tables/                         # LaTeX tables
└── Documentation
    ├── README_WORKFLOW_FINAL.md        # Complete workflow guide
    ├── NORMALIZATION_FIX.md            # Normalization fix details
    └── FINAL_CHANGES_SUMMARY.md        # This file
```

## 4. Output Files

### Figures Generated (per pT bin)
1. **2D Correlation Plots**
   - `correlation_trig*_assoc*.pdf/png`
   - Side-by-side view of Single-jet, Dijet, Multi-jet

2. **1D Projections - Components Only**
   - `projection_phi_trig*_assoc*.pdf/png` - Δφ projections
   - `projection_eta_trig*_assoc*.pdf/png` - Δη projections
   - Color-coded overlays of three jet categories

3. **1D Projections - Inclusive vs Components** (NEW!)
   - `comparison_inclusive_trig*_assoc*.pdf/png`
   - Shows how components sum to inclusive
   - Thick black line (inclusive) + colored dashed lines (components)

### Tables Generated
1. `table_nsy.tex` - Near-Side Yield with fractions
2. `table_sigma_eta.tex` - RMS width in Δη
3. `table_sigma_phi.tex` - RMS width in Δφ
4. `table_fractions.tex` - Jet multiplicity fractions

## 5. Usage

### Run Complete Workflow
```bash
# Generate events, run analysis, extract metrics, create figures and tables
alienv setenv O2Physics/latest -c './run_full_workflow.sh 1000'
```

### Run Individual Steps
```bash
# Step 1: Generate events
root -l -b << EOF
.L z01_GeneratePythiaEvents.C
GeneratePythiaEvents(1000, "results/pythia_events.root", 0.4)
.q
EOF

# Step 2: Run correlation analysis
alienv setenv O2Physics/latest -c ./z02_RunCorrelationAnalysis.sh

# Step 3: Extract quantification
alienv setenv O2Physics/latest -c ./z03_run_extract.sh

# Step 4: Generate figures
alienv setenv O2Physics/latest -c ./z04_run_plot.sh

# Step 5: Generate tables
alienv setenv O2Physics/latest -c ./z05_run_tables.sh
```

## 6. Key Physics Results

With proper normalization, the correlation function now shows:
- **Physically meaningful magnitudes**: Per-trigger yields
- **Proper baseline**: Flat away from peaks after background subtraction
- **Jet category decomposition**: Clear separation of Single/Dijet/Multi-jet contributions
- **Quantitative comparisons**: NSY, σ_η, σ_φ can be compared with theory

### Expected Trends
- **Low pT (1-2 GeV)**: Balanced contributions (~37% Single, 32% Dijet, 31% Multi-jet)
- **High pT (4-8 GeV)**: Single-jet dominance (48.7% at highest pT)
- **Near-side peak**: Jet fragmentation signal
- **Away-side peak**: Back-to-back jets

## 7. Next Steps

To use this analysis framework:

1. **Generate more statistics**: Run with higher number of events (10000+)
   ```bash
   alienv setenv O2Physics/latest -c './run_full_workflow.sh 10000'
   ```

2. **Modify physics settings**: Edit `z01_GeneratePythiaEvents.C`
   - Change collision energy
   - Modify jet algorithm parameters
   - Adjust pT and eta cuts

3. **Customize analysis bins**: Edit bin definitions in
   - `jAnaSimple/SimpleCorrelation.C` - Analysis bins
   - `z03_ExtractQuantification.C` - Quantification bins
   - `z04_PlotResults.C` - Plotting bins

4. **Add new observables**: Extend the quantification extraction to calculate additional metrics

## 8. Important Notes

- All scripts use multiplicity-integrated analysis (merged multiplicity bins)
- Proper normalization ensures physically meaningful correlation functions
- Jet categories based on nJets from tree: Single (1), Dijet (2), Multi-jet (≥3)
- Inclusive = Sum of all jet categories
- All outputs use consistent naming conventions
