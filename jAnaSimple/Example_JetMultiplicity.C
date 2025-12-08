// Example code showing how to add jet multiplicity categorization to SimpleCorrelation.C
// This demonstrates the key modifications needed

// ============================================================================
// STEP 1: Add jet multiplicity information to event processing
// ============================================================================

// In SimpleCorrelation function, after loading event header:
JBaseEventHeader *hdr = (JBaseEventHeader*)eventHeader->At(0);

// Option A: If jet information is stored in event header
int nJets = hdr->GetNJets();  // Assuming this method exists

// Option B: If jets need to be found on-the-fly
// You would need to:
// 1. Collect tracks with pT > 0.2 GeV/c, |η| < 0.8
// 2. Run FastJet clustering (anti-kT, configurable R)
// 3. Count jets with pT > 5 GeV/c, |η| < 0.8
// Example:
/*
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/JetDefinition.hh"
using namespace fastjet;

// Make R configurable for systematic studies
const double kJetR = 0.4;  // Baseline: 0.4, can test: 0.2, 0.6, 0.8

vector<PseudoJet> particles;
for (int i = 0; i < nTracks; i++) {
    JBaseTrack *trk = (JBaseTrack*)trackList->At(i);
    if (TMath::Abs(trk->Eta()) > 0.8) continue;
    if (trk->Pt() < 0.2) continue;
    
    PseudoJet pj(trk->Px(), trk->Py(), trk->Pz(), trk->E());
    particles.push_back(pj);
}

JetDefinition jet_def(antikt_algorithm, kJetR);  // Use configurable R
ClusterSequence cs(particles, jet_def);
vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets(5.0));

int nJets = 0;
for (unsigned int ij = 0; ij < jets.size(); ij++) {
    if (TMath::Abs(jets[ij].eta()) < 0.8 && jets[ij].pt() > 5.0) {
        nJets++;
    }
}
*/

// Classify event
int jetMultCategory = -1;  // -1: no jets, 0: single, 1: dijet, 2: multijet
if (nJets == 1) {
    jetMultCategory = 0;  // Single-jet
} else if (nJets == 2) {
    jetMultCategory = 1;  // Dijet
} else if (nJets >= 3) {
    jetMultCategory = 2;  // Multi-jet
}

// Skip events with no jets (or handle separately)
if (jetMultCategory < 0) continue;

// ============================================================================
// STEP 2: Create separate histograms for each jet multiplicity category
// ============================================================================

// Add to histogram creation section (around line 132):
const int nJetMultBins = 3;  // Single, dijet, multijet

// Modify histogram arrays to include jet multiplicity dimension:
vector<vector<vector<vector<TH2F*>>>> hSame_jetMult(
    nTrigBins, 
    vector<vector<vector<TH2F*>>>(
        nAssocBins, 
        vector<vector<TH2F*>>(
            nMultBins,
            vector<TH2F*>(nJetMultBins)
        )
    )
);

// Similar for hMixed and hRatio

// Create histograms with jet multiplicity in name:
for (int iTrig = 0; iTrig < nTrigBins; iTrig++) {
    for (int iAssoc = 0; iAssoc < nAssocBins; iAssoc++) {
        for (int iMult = 0; iMult < nMultBins; iMult++) {
            for (int iJetMult = 0; iJetMult < nJetMultBins; iJetMult++) {
                TString categoryName = "";
                if (iJetMult == 0) categoryName = "Single";
                else if (iJetMult == 1) categoryName = "Dijet";
                else if (iJetMult == 2) categoryName = "Multijet";
                
                TString nameSame = TString::Format("hSame_%s_trig%d_assoc%d_mult%d", 
                                                   categoryName.Data(), iTrig, iAssoc, iMult);
                TString titleSame = TString::Format("Same Event (%s, %.1f < p_{T}^{trig} < %.1f, %.1f < p_{T}^{assoc} < %.1f);#Delta#eta;#Delta#phi",
                                                   categoryName.Data(),
                                                   kPtTrigBins[iTrig], kPtTrigBins[iTrig+1],
                                                   kPtAssocBins[iAssoc], kPtAssocBins[iAssoc+1]);
                
                hSame_jetMult[iTrig][iAssoc][iMult][iJetMult] = new TH2F(
                    nameSame, titleSame,
                    kNDeltaEtaBins, kDeltaEtaMin, kDeltaEtaMax,
                    kNDeltaPhiBins, kDeltaPhiMin, kDeltaPhiMax
                );
                
                // Similar for hMixed and hRatio...
            }
        }
    }
}

// ============================================================================
// STEP 3: Fill correlations separately for each jet multiplicity category
// ============================================================================

// In the same-event correlation loop (around line 266):
for (int iTrig = 0; iTrig < nTrigBins; iTrig++) {
    for (int iAssoc = 0; iAssoc < nAssocBins; iAssoc++) {
        if (triggerTracks[iTrig].empty() || assocTracks[iAssoc].empty()) continue;
        
        for (const auto& trig : triggerTracks[iTrig]) {
            for (const auto& assoc : assocTracks[iAssoc]) {
                if (trig.id == assoc.id) continue;
                if (assoc.pt >= trig.pt) continue;
                
                double dPhi = CalculateDeltaPhi(assoc.phi, trig.phi);
                double dEta = assoc.eta - trig.eta;
                
                // Fill histogram for this jet multiplicity category
                hSame_jetMult[iTrig][iAssoc][multBin][jetMultCategory]->Fill(dEta, dPhi);
                
                // Also fill original histogram (for comparison)
                hSame[iTrig][iAssoc][multBin]->Fill(dEta, dPhi);
            }
        }
    }
}

// Similar modification for mixed-event correlations

// ============================================================================
// STEP 4: Calculate quantification metrics
// ============================================================================

// Function to calculate near-side yield
double CalculateNearSideYield(TH2F* hCorr) {
    const double dPhiNearMin = -TMath::Pi()/2.0;
    const double dPhiNearMax = TMath::Pi()/2.0;
    const double dEtaNearMin = -1.0;
    const double dEtaNearMax = 1.0;
    
    int xBinMin = hCorr->GetXaxis()->FindBin(dEtaNearMin);
    int xBinMax = hCorr->GetXaxis()->FindBin(dEtaNearMax);
    int yBinMin = hCorr->GetYaxis()->FindBin(dPhiNearMin);
    int yBinMax = hCorr->GetYaxis()->FindBin(dPhiNearMax);
    
    double yield = 0.0;
    for (int xBin = xBinMin; xBin <= xBinMax; xBin++) {
        for (int yBin = yBinMin; yBin <= yBinMax; yBin++) {
            yield += hCorr->GetBinContent(xBin, yBin);
        }
    }
    
    return yield;
}

// Function to calculate contribution fractions
void CalculateContributionFractions() {
    // After calculating correlation ratios (hRatio)
    
    for (int iTrig = 0; iTrig < nTrigBins; iTrig++) {
        for (int iAssoc = 0; iAssoc < nAssocBins; iAssoc++) {
            for (int iMult = 0; iMult < nMultBins; iMult++) {
                double nsy_single = 0.0, nsy_dijet = 0.0, nsy_multijet = 0.0;
                
                if (hRatio_jetMult[iTrig][iAssoc][iMult][0]) {
                    nsy_single = CalculateNearSideYield(hRatio_jetMult[iTrig][iAssoc][iMult][0]);
                }
                if (hRatio_jetMult[iTrig][iAssoc][iMult][1]) {
                    nsy_dijet = CalculateNearSideYield(hRatio_jetMult[iTrig][iAssoc][iMult][1]);
                }
                if (hRatio_jetMult[iTrig][iAssoc][iMult][2]) {
                    nsy_multijet = CalculateNearSideYield(hRatio_jetMult[iTrig][iAssoc][iMult][2]);
                }
                
                double total = nsy_single + nsy_dijet + nsy_multijet;
                
                if (total > 0) {
                    double f_single = nsy_single / total * 100.0;
                    double f_dijet = nsy_dijet / total * 100.0;
                    double f_multijet = nsy_multijet / total * 100.0;
                    
                    cout << "pT_trig: " << kPtTrigBins[iTrig] << "-" << kPtTrigBins[iTrig+1]
                         << ", pT_assoc: " << kPtAssocBins[iAssoc] << "-" << kPtAssocBins[iAssoc+1]
                         << ", mult: " << kMultBins[iMult] << "-" << kMultBins[iMult+1] << endl;
                    cout << "  Single-jet contribution: " << f_single << "%" << endl;
                    cout << "  Dijet contribution: " << f_dijet << "%" << endl;
                    cout << "  Multi-jet contribution: " << f_multijet << "%" << endl;
                }
            }
        }
    }
}
