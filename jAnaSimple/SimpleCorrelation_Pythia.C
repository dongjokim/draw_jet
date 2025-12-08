#include "TRandom3.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TStopwatch.h"
#include "TMath.h"
#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <memory>

#include "src/JBaseTrack.h"
#include "src/JTreeDataManager_Pythia.h"
#include "src/JBaseEventHeader.h"

typedef unsigned int uint;
using namespace std;

// Configuration parameters
const int kMaxMixEvents = 50;     // Number of events to mix
const int kNDeltaPhiBins = 200;   // Number of delta phi bins (from JCorrAnalysisRun3.C)
const int kNDeltaEtaBins = 32;    // Number of delta eta bins (from JCorrAnalysisRun3.C)
const double kDeltaPhiMin = -0.5*TMath::Pi(); // Delta phi min (from JCorrAnalysisRun3.C)
const double kDeltaPhiMax = 1.5*TMath::Pi();  // Delta phi max (from JCorrAnalysisRun3.C)
const double kDeltaEtaMin = -4.8; // Delta eta min (from JCorrAnalysisRun3.C)
const double kDeltaEtaMax = 4.8;  // Delta eta max (from JCorrAnalysisRun3.C)
const double kEtaCut = 1.0;       // Eta cut

// pT bins for trigger and associated particles (from JCorrAnalysisRun3.C)
const vector<double> kPtTrigBins = {1.0, 2.0, 3.0, 4.0, 8.0};
const vector<double> kPtAssocBins = {1.0, 2.0, 3.0, 4.0, 8.0};
const vector<double> kMultBins = {0, 5, 10, 20, 30, 40, 50, 60, 100.0};

// Global histograms (similar to JCorrAnalysisRun3.C)
TH2F *histS = new TH2F("histS","Same Events; #Delta#eta; #Delta#phi",
                       kNDeltaEtaBins, kDeltaEtaMin, kDeltaEtaMax,
                       kNDeltaPhiBins, kDeltaPhiMin, kDeltaPhiMax);
TH2F *histB = new TH2F("histB","Background Events; #Delta#eta; #Delta#phi",
                       kNDeltaEtaBins, kDeltaEtaMin, kDeltaEtaMax,
                       kNDeltaPhiBins, kDeltaPhiMin, kDeltaPhiMax);

// Simple track class for mixed events
class SimpleTrack {
public:
    double eta;
    double phi;
    double pt;
    int charge;
    int id;  // Add track ID
    
    SimpleTrack(double _eta, double _phi, double _pt, int _charge, int _id) :
        eta(_eta), phi(_phi), pt(_pt), charge(_charge), id(_id) {}
};

// Event buffer for mixed events
class EventBuffer {
private:
    deque<vector<SimpleTrack>> fEvents;
    int fMaxEvents;
    
public:
    EventBuffer(int maxEvents = 10) : fMaxEvents(maxEvents) {}
    
    void AddEvent(const vector<SimpleTrack>& tracks) {
        fEvents.push_back(tracks);
        if (fEvents.size() > fMaxEvents) {
            fEvents.pop_front();
        }
    }
    
    int GetNEvents() const {
        return fEvents.size();
    }
    
    const vector<SimpleTrack>& GetEvent(int i) const {
        return fEvents[i];
    }
};

// Helper function to get bin index
int GetBinIndex(double value, const vector<double>& bins) {
    for (size_t i = 0; i < bins.size() - 1; i++) {
        if (value >= bins[i] && value < bins[i+1]) {
            return i;
        }
    }
    return -1;
}

// Helper function to create histogram name
TString GetHistName(const char* base, int iTrig, int iAssoc, int iMult) {
    return TString::Format("%s_trig%d_assoc%d_mult%d", base, iTrig, iAssoc, iMult);
}

// Helper function to create histogram name with jet category
TString GetHistNameWithJetCategory(const char* base, const char* jetCat, int iTrig, int iAssoc, int iMult) {
    return TString::Format("%s_%s_trig%d_assoc%d_mult%d", base, jetCat, iTrig, iAssoc, iMult);
}

// Replace the current delta phi calculation with this function
double CalculateDeltaPhi(double phi1, double phi2) {
    double deltaPhi = phi1 - phi2;
    
    if (deltaPhi < -TMath::Pi() / 2.0)
        deltaPhi += 2 * TMath::Pi();
    if (deltaPhi > 3 * TMath::Pi() / 2.0)
        deltaPhi -= 2 * TMath::Pi();
    
    return deltaPhi;
}

// Main correlation analysis function
int SimpleCorrelation(TString inputfile="input_trees.txt", TString outputfile="simple_correlation.root") {
    cout << "========================================" << endl;
    cout << "SimpleCorrelation (Pythia Version)" << endl;
    cout << "========================================" << endl;
    cout << "Input file list: " << inputfile << endl;
    cout << "Output file: " << outputfile << endl;
    cout << endl;

    TStopwatch timer;
    timer.Start();

    // Set the same random seed as in JCorrAnalysisRun3.C
    gRandom->SetSeed(12345);  // Use the same seed
    
    // Create basic histograms
    TH1D* hPt = new TH1D("hPt", "p_{T} distribution", 100, 0, 10);
    TH1D* hEta = new TH1D("hEta", "#eta distribution", 100, -kEtaCut*2, kEtaCut*2);
    TH1D* hPhi = new TH1D("hPhi", "#phi distribution", 100, 0, 2*TMath::Pi());
    TH1D* hMult = new TH1D("hMult", "Multiplicity", 100, 0, 100);
    
    // Create correlation histograms for each pT and multiplicity bin
    const int nTrigBins = kPtTrigBins.size() - 1;
    const int nAssocBins = kPtAssocBins.size() - 1;
    const int nMultBins = kMultBins.size() - 1;
    
    // Arrays to store histograms
    vector<vector<vector<TH2F*>>> hSame(nTrigBins, vector<vector<TH2F*>>(nAssocBins, vector<TH2F*>(nMultBins)));
    vector<vector<vector<TH2F*>>> hMixed(nTrigBins, vector<vector<TH2F*>>(nAssocBins, vector<TH2F*>(nMultBins)));
    vector<vector<vector<TH2F*>>> hRatio(nTrigBins, vector<vector<TH2F*>>(nAssocBins, vector<TH2F*>(nMultBins)));

    // Arrays for jet multiplicity categories (Single=1 jet, Dijet=2 jets, Multijet>=3 jets)
    const int nJetCategories = 3;
    const char* jetCategoryNames[3] = {"Single", "Dijet", "Multijet"};
    vector<vector<vector<vector<TH2F*>>>> hSame_JetCat(nJetCategories,
        vector<vector<vector<TH2F*>>>(nTrigBins, vector<vector<TH2F*>>(nAssocBins, vector<TH2F*>(nMultBins))));
    vector<vector<vector<vector<TH2F*>>>> hMixed_JetCat(nJetCategories,
        vector<vector<vector<TH2F*>>>(nTrigBins, vector<vector<TH2F*>>(nAssocBins, vector<TH2F*>(nMultBins))));
    vector<vector<vector<vector<TH2F*>>>> hRatio_JetCat(nJetCategories,
        vector<vector<vector<TH2F*>>>(nTrigBins, vector<vector<TH2F*>>(nAssocBins, vector<TH2F*>(nMultBins))));
    
    // Create histograms for each bin combination
    for (int iTrig = 0; iTrig < nTrigBins; iTrig++) {
        for (int iAssoc = 0; iAssoc < nAssocBins; iAssoc++) {
            for (int iMult = 0; iMult < nMultBins; iMult++) {
                TString nameSame = GetHistName("hSame", iTrig, iAssoc, iMult);
                TString titleSame = TString::Format("Same Event (%.1f < p_{T}^{trig} < %.1f, %.1f < p_{T}^{assoc} < %.1f, %.1f < mult < %.1f);#Delta#eta;#Delta#phi",
                                                   kPtTrigBins[iTrig], kPtTrigBins[iTrig+1],
                                                   kPtAssocBins[iAssoc], kPtAssocBins[iAssoc+1],
                                                   kMultBins[iMult], kMultBins[iMult+1]);
                
                hSame[iTrig][iAssoc][iMult] = new TH2F(nameSame, titleSame,
                                                      kNDeltaEtaBins, kDeltaEtaMin, kDeltaEtaMax,
                                                      kNDeltaPhiBins, kDeltaPhiMin, kDeltaPhiMax);
                
                TString nameMixed = GetHistName("hMixed", iTrig, iAssoc, iMult);
                TString titleMixed = TString::Format("Mixed Event (%.1f < p_{T}^{trig} < %.1f, %.1f < p_{T}^{assoc} < %.1f, %.1f < mult < %.1f);#Delta#eta;#Delta#phi",
                                                    kPtTrigBins[iTrig], kPtTrigBins[iTrig+1],
                                                    kPtAssocBins[iAssoc], kPtAssocBins[iAssoc+1],
                                                    kMultBins[iMult], kMultBins[iMult+1]);
                
                hMixed[iTrig][iAssoc][iMult] = new TH2F(nameMixed, titleMixed,
                                                       kNDeltaEtaBins, kDeltaEtaMin, kDeltaEtaMax,
                                                       kNDeltaPhiBins, kDeltaPhiMin, kDeltaPhiMax);
                
                TString nameRatio = GetHistName("hRatio", iTrig, iAssoc, iMult);
                TString titleRatio = TString::Format("Correlation (%.1f < p_{T}^{trig} < %.1f, %.1f < p_{T}^{assoc} < %.1f, %.1f < mult < %.1f);#Delta#eta;#Delta#phi",
                                                    kPtTrigBins[iTrig], kPtTrigBins[iTrig+1],
                                                    kPtAssocBins[iAssoc], kPtAssocBins[iAssoc+1],
                                                    kMultBins[iMult], kMultBins[iMult+1]);
                
                hRatio[iTrig][iAssoc][iMult] = new TH2F(nameRatio, titleRatio,
                                                       kNDeltaEtaBins, kDeltaEtaMin, kDeltaEtaMax,
                                                       kNDeltaPhiBins, kDeltaPhiMin, kDeltaPhiMax);
            }
        }
    }

    // Create histograms for jet multiplicity categories
    for (int iJetCat = 0; iJetCat < nJetCategories; iJetCat++) {
        for (int iTrig = 0; iTrig < nTrigBins; iTrig++) {
            for (int iAssoc = 0; iAssoc < nAssocBins; iAssoc++) {
                for (int iMult = 0; iMult < nMultBins; iMult++) {
                    TString nameSame = GetHistNameWithJetCategory("hSame", jetCategoryNames[iJetCat], iTrig, iAssoc, iMult);
                    TString titleSame = TString::Format("Same Event %s (%.1f < p_{T}^{trig} < %.1f, %.1f < p_{T}^{assoc} < %.1f, %.1f < mult < %.1f);#Delta#eta;#Delta#phi",
                                                       jetCategoryNames[iJetCat],
                                                       kPtTrigBins[iTrig], kPtTrigBins[iTrig+1],
                                                       kPtAssocBins[iAssoc], kPtAssocBins[iAssoc+1],
                                                       kMultBins[iMult], kMultBins[iMult+1]);

                    hSame_JetCat[iJetCat][iTrig][iAssoc][iMult] = new TH2F(nameSame, titleSame,
                                                                           kNDeltaEtaBins, kDeltaEtaMin, kDeltaEtaMax,
                                                                           kNDeltaPhiBins, kDeltaPhiMin, kDeltaPhiMax);

                    TString nameMixed = GetHistNameWithJetCategory("hMixed", jetCategoryNames[iJetCat], iTrig, iAssoc, iMult);
                    TString titleMixed = TString::Format("Mixed Event %s (%.1f < p_{T}^{trig} < %.1f, %.1f < p_{T}^{assoc} < %.1f, %.1f < mult < %.1f);#Delta#eta;#Delta#phi",
                                                        jetCategoryNames[iJetCat],
                                                        kPtTrigBins[iTrig], kPtTrigBins[iTrig+1],
                                                        kPtAssocBins[iAssoc], kPtAssocBins[iAssoc+1],
                                                        kMultBins[iMult], kMultBins[iMult+1]);

                    hMixed_JetCat[iJetCat][iTrig][iAssoc][iMult] = new TH2F(nameMixed, titleMixed,
                                                                            kNDeltaEtaBins, kDeltaEtaMin, kDeltaEtaMax,
                                                                            kNDeltaPhiBins, kDeltaPhiMin, kDeltaPhiMax);

                    TString nameRatio = GetHistNameWithJetCategory("hRatio", jetCategoryNames[iJetCat], iTrig, iAssoc, iMult);
                    TString titleRatio = TString::Format("Correlation %s (%.1f < p_{T}^{trig} < %.1f, %.1f < p_{T}^{assoc} < %.1f, %.1f < mult < %.1f);#Delta#eta;#Delta#phi",
                                                        jetCategoryNames[iJetCat],
                                                        kPtTrigBins[iTrig], kPtTrigBins[iTrig+1],
                                                        kPtAssocBins[iAssoc], kPtAssocBins[iAssoc+1],
                                                        kMultBins[iMult], kMultBins[iMult+1]);

                    hRatio_JetCat[iJetCat][iTrig][iAssoc][iMult] = new TH2F(nameRatio, titleRatio,
                                                                            kNDeltaEtaBins, kDeltaEtaMin, kDeltaEtaMax,
                                                                            kNDeltaPhiBins, kDeltaPhiMin, kDeltaPhiMax);
                }
            }
        }
    }

    // Initialize data manager (Pythia version)
    cout << "Initializing JTreeDataManager_Pythia..." << endl;
    JTreeDataManager_Pythia* dmg = new JTreeDataManager_Pythia();

    cout << "Loading input files from: " << inputfile << endl;
    dmg->ChainInputStream(inputfile.Data());

    // Create track list AFTER initializing data manager
    TClonesArray *trackList = new TClonesArray("JBaseTrack", 1000);

    // Create event buffers for each multiplicity bin
    vector<EventBuffer> eventBuffers(nMultBins, EventBuffer(kMaxMixEvents));

    int numberEvents = dmg->GetNEvents();
    cout << "Total events in chain: " << numberEvents << endl;

    int ieout = numberEvents/20;
    if (ieout < 1) ieout = 1;

    cout << "\nProcessing " << numberEvents << " events..." << endl;
    cout << "Using " << nTrigBins << " trigger pT bins, " << nAssocBins << " associated pT bins, and "
         << nMultBins << " multiplicity bins" << endl;
    cout << "Event cuts: pT > " << 0.2 << " GeV, |eta| < " << 0.8 << endl;
    cout << endl;
    
    // Event loop
    for (int evt = 0; evt < numberEvents; evt++) {
        if (evt % ieout == 0) {
            cout << "Event " << evt << " / " << numberEvents 
                 << " (" << int(float(evt)/numberEvents*100) << "%)" << endl;
        }
        
        dmg->LoadEvent(evt);
        if (!dmg->IsGoodEvent()) continue;

        // Get number of jets for this event
        int nJets = dmg->GetNJets();

        // Determine jet multiplicity category: 0=Single(1 jet), 1=Dijet(2 jets), 2=Multijet(>=3 jets)
        int jetCategory = -1;
        if (nJets == 1) jetCategory = 0;       // Single jet
        else if (nJets == 2) jetCategory = 1;  // Dijet
        else if (nJets >= 3) jetCategory = 2;  // Multijet
        // Skip events with no jets (jetCategory == -1)

        // Note: Event header not needed for Pythia standalone
        // (Pythia standalone doesn't have event headers, only track data)

        // Get tracks
        trackList->Clear();
        dmg->RegisterList(trackList, NULL);
        int nTracks = trackList->GetEntries();
        
        // Get multiplicity (now that we have the tracks loaded)
        int multiplicity = nTracks;  // Use actual number of tracks
        hMult->Fill(multiplicity);
        
        // Skip events with zero multiplicity
        if (multiplicity <= 0) continue;
        
        // Determine multiplicity bin
        int multBin = GetBinIndex(multiplicity, kMultBins);
        if (multBin < 0) continue; // Skip if outside multiplicity range
        
        // Store tracks for this event
        vector<SimpleTrack> eventTracks;
        vector<vector<SimpleTrack>> triggerTracks(nTrigBins);
        vector<vector<SimpleTrack>> assocTracks(nAssocBins);
        
        // Loop over tracks
        for (int i = 0; i < nTracks; i++) {
            JBaseTrack *trk = (JBaseTrack*)trackList->At(i);
            
            // Apply basic track cuts
            if (TMath::Abs(trk->Eta()) > kEtaCut) continue;
            
            // Apply additional track cuts
            if (trk->Pt() < 0.2) continue;  // Minimum pT cut
            // Skip charge check since tracks have charge=0 in the data
            
            // Fill track histograms
            hPt->Fill(trk->Pt());
            hEta->Fill(trk->Eta());
            hPhi->Fill(trk->Phi());
            
            // Normalize phi to [0, 2π]
            double phi = trk->Phi();
            if (phi < 0) phi += 2 * TMath::Pi();
            
            // Create track object
            SimpleTrack track(trk->Eta(), phi, trk->Pt(), trk->GetCharge(), trk->GetID());
            
            // Store track in appropriate pT bins
            eventTracks.push_back(track);
            
            // Categorize by trigger pT
            int trigBin = GetBinIndex(trk->Pt(), kPtTrigBins);
            if (trigBin >= 0) {
                triggerTracks[trigBin].push_back(track);
            }
            
            // Categorize by associated pT
            int assocBin = GetBinIndex(trk->Pt(), kPtAssocBins);
            if (assocBin >= 0) {
                assocTracks[assocBin].push_back(track);
            }
        }
        
        // Same event correlations for each pT bin combination
        for (int iTrig = 0; iTrig < nTrigBins; iTrig++) {
            for (int iAssoc = 0; iAssoc < nAssocBins; iAssoc++) {
                // Skip if no tracks in either bin
                if (triggerTracks[iTrig].empty() || assocTracks[iAssoc].empty()) continue;
                
                // Loop over trigger tracks
                for (const auto& trig : triggerTracks[iTrig]) {
                    // Loop over associated tracks
                    for (const auto& assoc : assocTracks[iAssoc]) {
                        // Skip self-correlations
                        if (trig.id == assoc.id) continue;
                        
                        // Enforce pT ordering (trigger pT > associated pT)
                        if (assoc.pt >= trig.pt) continue;
                        
                        double dPhi = CalculateDeltaPhi(assoc.phi, trig.phi);
                        
                        double dEta = assoc.eta - trig.eta;

                        hSame[iTrig][iAssoc][multBin]->Fill(dEta, dPhi);
                        histS->Fill(dEta, dPhi);

                        // Fill jet category histograms if valid category
                        if (jetCategory >= 0) {
                            hSame_JetCat[jetCategory][iTrig][iAssoc][multBin]->Fill(dEta, dPhi);
                        }
                    }
                }
            }
        }
        
        // Mixed event correlations for each pT bin combination
        for (int iTrig = 0; iTrig < nTrigBins; iTrig++) {
            for (int iAssoc = 0; iAssoc < nAssocBins; iAssoc++) {
                // Skip if no trigger tracks
                if (triggerTracks[iTrig].empty()) continue;
                
                // Loop over events in the buffer
                for (int mixEvt = 0; mixEvt < eventBuffers[multBin].GetNEvents(); mixEvt++) {
                    const vector<SimpleTrack>& mixTracks = eventBuffers[multBin].GetEvent(mixEvt);
                    
                    // Loop over trigger tracks in current event
                    for (const auto& trig : triggerTracks[iTrig]) {
                        // Loop over all tracks in mixed event
                        for (const auto& assoc : mixTracks) {
                            // Check if associated track is in the right pT bin
                            int assocBin = GetBinIndex(assoc.pt, kPtAssocBins);
                            if (assocBin != iAssoc) continue;
                            
                            // Enforce pT ordering (trigger pT > associated pT)
                            if (assoc.pt >= trig.pt) continue;
                            
                            double dPhi = CalculateDeltaPhi(assoc.phi, trig.phi);
                            
                            double dEta = assoc.eta - trig.eta;

                            hMixed[iTrig][iAssoc][multBin]->Fill(dEta, dPhi);
                            histB->Fill(dEta, dPhi);

                            // Fill jet category histograms if valid category
                            if (jetCategory >= 0) {
                                hMixed_JetCat[jetCategory][iTrig][iAssoc][multBin]->Fill(dEta, dPhi);
                            }
                        }
                    }
                }
            }
        }
        
        // Add current event to buffer
        eventBuffers[multBin].AddEvent(eventTracks);

        // Add debug output
        //cout << "Event " << evt << ": Multiplicity = " << multiplicity << ", Tracks = " << nTracks << endl;

        // Note: Pythia standalone doesn't have event vertex information
        // (no vertex cut applied for standalone events)
    }
    
    // Calculate correlation ratios
    for (int iTrig = 0; iTrig < nTrigBins; iTrig++) {
        for (int iAssoc = 0; iAssoc < nAssocBins; iAssoc++) {
            for (int iMult = 0; iMult < nMultBins; iMult++) {
                // Skip if no entries
                if (hSame[iTrig][iAssoc][iMult]->GetEntries() == 0 || 
                    hMixed[iTrig][iAssoc][iMult]->GetEntries() == 0) continue;
                
                // Calculate ratio bin by bin
                for (int xBin = 1; xBin <= hRatio[iTrig][iAssoc][iMult]->GetNbinsX(); xBin++) {
                    for (int yBin = 1; yBin <= hRatio[iTrig][iAssoc][iMult]->GetNbinsY(); yBin++) {
                        double same = hSame[iTrig][iAssoc][iMult]->GetBinContent(xBin, yBin);
                        double mixed = hMixed[iTrig][iAssoc][iMult]->GetBinContent(xBin, yBin);
                        
                        if (mixed > 0) {
                            double ratio = same / mixed;
                            hRatio[iTrig][iAssoc][iMult]->SetBinContent(xBin, yBin, ratio);
                        }
                    }
                }
                
                // Normalize ratio histogram
                double integral = hRatio[iTrig][iAssoc][iMult]->Integral();
                if (integral > 0) {
                    hRatio[iTrig][iAssoc][iMult]->Scale(1.0 / integral * 
                                                       hRatio[iTrig][iAssoc][iMult]->GetNbinsX() * 
                                                       hRatio[iTrig][iAssoc][iMult]->GetNbinsY());
                }
            }
        }
    }

    // Calculate correlation ratios for jet category histograms
    for (int iJetCat = 0; iJetCat < nJetCategories; iJetCat++) {
        for (int iTrig = 0; iTrig < nTrigBins; iTrig++) {
            for (int iAssoc = 0; iAssoc < nAssocBins; iAssoc++) {
                for (int iMult = 0; iMult < nMultBins; iMult++) {
                    // Skip if no entries
                    if (hSame_JetCat[iJetCat][iTrig][iAssoc][iMult]->GetEntries() == 0 ||
                        hMixed_JetCat[iJetCat][iTrig][iAssoc][iMult]->GetEntries() == 0) continue;

                    // Calculate ratio bin by bin
                    for (int xBin = 1; xBin <= hRatio_JetCat[iJetCat][iTrig][iAssoc][iMult]->GetNbinsX(); xBin++) {
                        for (int yBin = 1; yBin <= hRatio_JetCat[iJetCat][iTrig][iAssoc][iMult]->GetNbinsY(); yBin++) {
                            double same = hSame_JetCat[iJetCat][iTrig][iAssoc][iMult]->GetBinContent(xBin, yBin);
                            double mixed = hMixed_JetCat[iJetCat][iTrig][iAssoc][iMult]->GetBinContent(xBin, yBin);

                            if (mixed > 0) {
                                double ratio = same / mixed;
                                hRatio_JetCat[iJetCat][iTrig][iAssoc][iMult]->SetBinContent(xBin, yBin, ratio);
                            }
                        }
                    }

                    // Normalize ratio histogram
                    double integral = hRatio_JetCat[iJetCat][iTrig][iAssoc][iMult]->Integral();
                    if (integral > 0) {
                        hRatio_JetCat[iJetCat][iTrig][iAssoc][iMult]->Scale(1.0 / integral *
                                                                            hRatio_JetCat[iJetCat][iTrig][iAssoc][iMult]->GetNbinsX() *
                                                                            hRatio_JetCat[iJetCat][iTrig][iAssoc][iMult]->GetNbinsY());
                    }
                }
            }
        }
    }

    // Write output
    TFile *outFile = new TFile(outputfile.Data(), "RECREATE");
    
    // Write basic histograms
    hPt->Write();
    hEta->Write();
    hPhi->Write();
    hMult->Write();
    
    // Write global histograms
    histS->Write();
    histB->Write();
    
    // Create a ratio histogram from the global histograms
    TH2F *histRatio = (TH2F*)histS->Clone("histRatio");
    histRatio->SetTitle("Correlation Ratio; #Delta#eta; #Delta#phi");
    
    // First normalize the mixed event histogram to have the same integral as the same event
    histB->Scale(histS->Integral() / histB->Integral());
    // Then create the ratio
    histRatio->Divide(histB);
    
    // Normalize
    double integral = histRatio->Integral();
    if (integral > 0) {
        histRatio->Scale(1.0 / integral * histRatio->GetNbinsX() * histRatio->GetNbinsY());
    }
    histRatio->Write();
    
    // Create directories for different histogram types
    TDirectory *dirSame = outFile->mkdir("SameEvent");
    TDirectory *dirMixed = outFile->mkdir("MixedEvent");
    TDirectory *dirRatio = outFile->mkdir("Correlation");
    
    // Write correlation histograms
    for (int iTrig = 0; iTrig < nTrigBins; iTrig++) {
        for (int iAssoc = 0; iAssoc < nAssocBins; iAssoc++) {
            for (int iMult = 0; iMult < nMultBins; iMult++) {
                if (hSame[iTrig][iAssoc][iMult]->GetEntries() > 0) {
                    dirSame->cd();
                    hSame[iTrig][iAssoc][iMult]->Write();
                    
                    dirMixed->cd();
                    hMixed[iTrig][iAssoc][iMult]->Write();
                    
                    dirRatio->cd();
                    hRatio[iTrig][iAssoc][iMult]->Write();
                }
            }
        }
    }

    // Write jet category histograms
    for (int iJetCat = 0; iJetCat < nJetCategories; iJetCat++) {
        for (int iTrig = 0; iTrig < nTrigBins; iTrig++) {
            for (int iAssoc = 0; iAssoc < nAssocBins; iAssoc++) {
                for (int iMult = 0; iMult < nMultBins; iMult++) {
                    dirSame->cd();
                    hSame_JetCat[iJetCat][iTrig][iAssoc][iMult]->Write();

                    dirMixed->cd();
                    hMixed_JetCat[iJetCat][iTrig][iAssoc][iMult]->Write();

                    dirRatio->cd();
                    hRatio_JetCat[iJetCat][iTrig][iAssoc][iMult]->Write();
                }
            }
        }
    }

    // At the end of processing, print histogram entries
    cout << "\n========================================" << endl;
    cout << "Processing Summary" << endl;
    cout << "========================================" << endl;
    cout << "Global histograms:" << endl;
    cout << "  histS entries: " << histS->GetEntries() << endl;
    cout << "  histB entries: " << histB->GetEntries() << endl;
    cout << "  hMult entries: " << hMult->GetEntries() << endl;
    cout << "  hPt entries: " << hPt->GetEntries() << endl;

    // Count non-empty correlation histograms
    int nNonEmptyHists = 0;
    for (int iTrig = 0; iTrig < nTrigBins; iTrig++) {
        for (int iAssoc = 0; iAssoc < nAssocBins; iAssoc++) {
            for (int iMult = 0; iMult < nMultBins; iMult++) {
                if (hSame[iTrig][iAssoc][iMult]->GetEntries() > 0) {
                    nNonEmptyHists++;
                }
            }
        }
    }
    cout << "Non-empty correlation histograms: " << nNonEmptyHists << endl;

    // Count non-empty jet category histograms
    int nNonEmptyJetCatHists[3] = {0, 0, 0};
    for (int iJetCat = 0; iJetCat < nJetCategories; iJetCat++) {
        for (int iTrig = 0; iTrig < nTrigBins; iTrig++) {
            for (int iAssoc = 0; iAssoc < nAssocBins; iAssoc++) {
                for (int iMult = 0; iMult < nMultBins; iMult++) {
                    if (hSame_JetCat[iJetCat][iTrig][iAssoc][iMult]->GetEntries() > 0) {
                        nNonEmptyJetCatHists[iJetCat]++;
                    }
                }
            }
        }
    }
    cout << "\nJet category histograms:" << endl;
    for (int iJetCat = 0; iJetCat < nJetCategories; iJetCat++) {
        cout << "  " << jetCategoryNames[iJetCat] << ": " << nNonEmptyJetCatHists[iJetCat] << " non-empty" << endl;
    }

    cout << "\nWriting output to: " << outputfile << endl;
    outFile->Close();

    timer.Stop();
    cout << "\n========================================" << endl;
    cout << "✓ Analysis Complete!" << endl;
    cout << "========================================" << endl;
    cout << "Real time: " << timer.RealTime() << " seconds" << endl;
    cout << "CPU time: " << timer.CpuTime() << " seconds" << endl;
    cout << "========================================" << endl;

    return 0;
} 