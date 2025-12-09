# Two-Particle Correlation Normalization Fix

## Problem

The original correlation analysis had incorrect normalization:

```cpp
// OLD (INCORRECT):
double ratio = same / mixed;  // Simple bin-by-bin division
hRatio->Scale(1.0 / integral * nBinsX * nBinsY);  // Wrong post-scaling
```

**Issues:**
1. No normalization by number of trigger particles (N_trig)
2. No proper matching of same-event and mixed-event statistics
3. Arbitrary post-division scaling that doesn't produce correct correlation function

## Solution

Implemented proper two-particle correlation normalization following standard practices:

```cpp
C(Δη, Δφ) = (1/N_trig) × S(Δη, Δφ) / (α × M(Δη, Δφ))
```

Where:
- **N_trig** = Total number of trigger particles (summed over all events)
- **S(Δη, Δφ)** = Same-event pair distribution
- **M(Δη, Δφ)** = Mixed-event pair distribution
- **α** = Normalization constant = `Integral(S) / Integral(M)`

## Implementation Details

### 1. Track Number of Triggers

Added counters to track the number of trigger particles:

```cpp
// Track number of triggers for normalization
vector<vector<vector<int>>> nTriggersCount(nTrigBins,
    vector<vector<int>>(nAssocBins, vector<int>(nMultBins, 0)));

// For jet categories
vector<vector<vector<vector<int>>>> nTriggersCount_JetCat(nJetCategories,
    vector<vector<vector<int>>>(nTrigBins,
        vector<vector<int>>(nAssocBins, vector<int>(nMultBins, 0))));
```

### 2. Increment Counters During Event Loop

In same-event correlation loop:

```cpp
// Count triggers for normalization
nTriggersCount[iTrig][iAssoc][multBin] += triggerTracks[iTrig].size();
if (jetCategory >= 0) {
    nTriggersCount_JetCat[jetCategory][iTrig][iAssoc][multBin] +=
        triggerTracks[iTrig].size();
}
```

### 3. Proper Normalization Calculation

```cpp
// Calculate normalization factor alpha = Integral(Same) / Integral(Mixed)
double integralSame = hSame[iTrig][iAssoc][iMult]->Integral();
double integralMixed = hMixed[iTrig][iAssoc][iMult]->Integral();
double alpha = (integralMixed > 0) ? integralSame / integralMixed : 0.0;

// Get number of triggers
int nTrig = nTriggersCount[iTrig][iAssoc][iMult];

// Calculate correlation function: C = (1/N_trig) * S / (alpha * M)
for (int xBin = 1; xBin <= nBinsX; xBin++) {
    for (int yBin = 1; yBin <= nBinsY; yBin++) {
        double same = hSame->GetBinContent(xBin, yBin);
        double mixed = hMixed->GetBinContent(xBin, yBin);

        if (mixed > 0 && nTrig > 0 && alpha > 0) {
            double corr = (1.0 / nTrig) * (same / (alpha * mixed));
            hRatio->SetBinContent(xBin, yBin, corr);
        }
    }
}
```

## Physical Interpretation

The corrected normalization ensures:

1. **Per-trigger normalization**: Correlation function represents average number of associated particles per trigger
2. **Proper background subtraction**: Mixed events normalized to match same-event statistics before division
3. **Acceptance and efficiency cancellation**: Ratio removes detector effects present in both same and mixed events
4. **Quantitative comparison**: Different pT bins and jet categories can be meaningfully compared

## Expected Results

The correlation function C(Δη, Δφ) now represents:
- **Near-side peak (Δφ ≈ 0)**: Jet fragmentation and short-range correlations
- **Away-side peak (Δφ ≈ π)**: Back-to-back jets
- **Baseline level**: Random combinatorial background (should be ~flat after proper normalization)

The near-side yield (NSY) and widths (σ_η, σ_φ) extracted from this properly normalized correlation function are now physically meaningful and can be compared with theoretical predictions and other experiments.

## Files Modified

- `jAnaSimple/SimpleCorrelation.C`:
  - Lines 148-149: Added nTriggersCount arrays
  - Lines 161-163: Added nTriggersCount_JetCat arrays
  - Lines 358-362: Increment trigger counters
  - Lines 438-468: Fixed normalization for standard histograms
  - Lines 470-502: Fixed normalization for jet category histograms

## Testing

To verify the fix works correctly, rerun the analysis:

```bash
# Recompile with fixed normalization
cd jAnaSimple
make clean && make

# Run correlation analysis
alienv setenv O2Physics/latest -c ./z02_RunCorrelationAnalysis.sh

# Extract quantification
alienv setenv O2Physics/latest -c ./z03_run_extract.sh

# Generate plots
alienv setenv O2Physics/latest -c ./z04_run_plot.sh
```

The correlation functions should now show properly normalized signals with:
- Near-side peaks at physical magnitudes
- Smooth baselines away from peaks
- Meaningful NSY values that scale with number of particles
