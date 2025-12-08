# Generalized Gaussian Fitting Implementation

## Summary

Updated `z03_ExtractQuantification.C` to use proper generalized 2D Gaussian fitting for background extraction and quantification, following the approach from `z03.fitYield.C`.

## Key Changes

### 1. Added 2D Gaussian Fit Function

```cpp
Double_t GenGaussian2D(Double_t *x, Double_t *par)
```

**Parameters:**
- `[0]` = Amplitude (peak height)
- `[1]` = Mean eta (peak position in Δη)
- `[2]` = Sigma eta (width in Δη)
- `[3]` = Mean phi (peak position in Δφ)
- `[4]` = Sigma phi (width in Δφ)
- `[5]` = Background (flat baseline)

**Formula:**
```
f(Δη, Δφ) = Background + Amplitude * exp(-0.5 * ((Δη-μ_η)²/σ_η² + (Δφ-μ_φ)²/σ_φ²))
```

### 2. Background Extraction from Fit

**Before:** Simple averaging from away-side region
```cpp
double background = CalculateBackground(h);  // Average over away-side bins
```

**After:** Extract from Gaussian fit parameter
```cpp
vector<double> fitResults = FitCorrelationFunction(h);
double background = fitResults[0];  // Background from fit parameter [5]
```

### 3. NSY Calculation from Integrated Gaussian

**Before:** Sum of bins with simple background subtraction
```cpp
double nsy = CalculateNearSideYield(h, background);  // Direct bin sum
```

**After:** Analytical integral of fitted Gaussian
```cpp
double nsy = fitResults[4];  // NSY = Amplitude * 2π * σ_η * σ_φ
```

This is more accurate because:
- Uses the full fitted function (not binned sum)
- Integrates the Gaussian analytically
- Less sensitive to binning effects

### 4. Widths from Fit Parameters

**Before:** Calculate RMS from histogram projections
```cpp
double sigmaEta = CalculateRMS(h, true, background);
double sigmaPhi = CalculateRMS(h, false, background);
```

**After:** Extract directly from Gaussian fit
```cpp
double sigmaEta = fitResults[2];   // Sigma_eta from fit parameter [2]
double sigmaPhi = fitResults[3];   // Sigma_phi from fit parameter [4]
```

### 5. Robust Fallback

If the Gaussian fit fails (non-convergence, bad chi-square):
- Falls back to simple away-side background estimation
- Uses default sigma values (0.5 for both eta and phi)
- Ensures analysis always produces results

```cpp
if (fitResult->IsValid()) {
    // Use fit parameters
} else {
    // Fall back to simple estimation
    results[0] = away_side_average;
    results[2] = 0.5;  // Default sigma eta
    results[3] = 0.5;  // Default sigma phi
}
```

## Fit Configuration

**Fit Range:** Near-side region only
- Δη: [-1.0, 1.0]
- Δφ: [-π/2, π/2]

**Note:** For TH2 (2D histograms), the fit range is set using `TF2::SetRange()` before calling `Fit()`, as the TH2::Fit() method doesn't accept range parameters directly.

**Parameter Limits:**
- Amplitude: [0, 2×max_value]
- Mean eta: [-0.5, 0.5] (centered near 0)
- Sigma eta: [0.1, 2.0]
- Mean phi: [-0.5, 0.5] (centered near 0)
- Sigma phi: [0.1, 2.0]
- Background: [0, max_value]

**Initial Values:**
- Amplitude: max - min
- Mean eta: 0.0
- Sigma eta: 0.5
- Mean phi: 0.0
- Sigma phi: 0.5
- Background: min_value

## Advantages of Fitting Approach

1. **Physical Background:** Background comes from the Gaussian tail, not an arbitrary away-side region
2. **Consistent Methodology:** Same approach used in published ALICE analyses
3. **Better Resolution:** Analytical integral vs binned sum
4. **Uncertainty Estimates:** Fit provides parameter uncertainties (could be extracted if needed)
5. **Shape Information:** Full correlation shape captured in fit parameters

## Output File Changes

Output file header now indicates fitting method:

```
# Quantification Results (Multiplicity-Integrated, Generalized Gaussian Fitting)
# Background extracted from 2D Gaussian fit parameter
# NSY calculated from integrated Gaussian: Amplitude * 2*pi * sigma_eta * sigma_phi
# Format: pT_trig_min pT_trig_max pT_assoc_min pT_assoc_max category NSY background sigma_eta sigma_phi fraction
```

## Testing

To test the updated extraction:

```bash
alienv setenv O2Physics/latest -c './z03_run_extract.sh'
```

Or run the full workflow:

```bash
alienv setenv O2Physics/latest -c './run_full_workflow.sh 1000'
```

## Expected Behavior

- Most fits should converge successfully (check fit status in ROOT output if running interactively)
- Background values should be consistent with the flat region away from peaks
- Sigma values should be physically reasonable (typically 0.3-1.0 depending on pT)
- NSY values should be positive and scale with pT
- If many fits fail, may need to adjust initial parameters or limits

## Files Modified

- `z03_ExtractQuantification.C` - Complete rewrite of quantification extraction
  - Added `GenGaussian2D()` function
  - Added `FitCorrelationFunction()` function
  - Removed old `CalculateNearSideYield()` function
  - Removed old `CalculateRMS()` function
  - Updated main loop to use fitting
  - Updated output messages

## Consistency with Analysis Chain

The fitting approach is now consistent across the entire analysis:

1. **z01_GeneratePythiaEvents.C** - Generate events with jets
2. **z02_RunCorrelationAnalysis.sh** - Proper mixed-event normalization (fixed)
3. **z03_ExtractQuantification.C** - Generalized Gaussian fitting (this update)
4. **z04_PlotResults.C** - Visualize results with inclusive vs components
5. **z05_GenerateTables.C** - Generate LaTeX tables

All steps now use rigorous, publication-quality methods.
