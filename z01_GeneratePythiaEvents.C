// Batch macro to generate Pythia events and save to ROOT trees
// Usage: root -b -q 'z01_GeneratePythiaEvents.C(10000, "pythia_events.root", 0.4)'
// Arguments: nEvents, outputFile, jetR
//
// Configuration:
//   - pp collisions at 5.36 TeV
//   - Monash 2013 tune (Tune:pp = 14)
//   - Hard QCD processes enabled
//
// IMPORTANT: This macro must be compiled with ACLiC (.L z01_GeneratePythiaEvents.C+)
// to avoid O2Physics autoloading conflicts

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TMath.h"
#include "TSystem.h"
#include <iostream>
#include <vector>

// Load libraries BEFORE including headers to prevent autoloading
// This function is called at the start of GeneratePythiaEvents
void LoadRequiredLibraries() {
    gInterpreter->SetClassAutoloading(false);
    // Note: SetAutoLoading may not exist in all ROOT versions
    
    TString pythia8Path = gSystem->Getenv("PYTHIA8");
    if (pythia8Path.Length() > 0) {
        TString pythiaLib = pythia8Path + "/lib/libpythia8";
        gSystem->Load(pythiaLib);
    }
    
    TString fastjetPath = gSystem->Getenv("FASTJET");
    if (fastjetPath.Length() > 0) {
        TString fastjetLib = fastjetPath + "/lib/libfastjet";
        gSystem->Load(fastjetLib);
    }
}

// Include Pythia8 and FastJet headers AFTER library loading function
#include "Pythia8/Pythia.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/JetDefinition.hh"

using namespace fastjet;
// Don't use "using namespace Pythia8" to avoid ambiguity with ROOT's TPythia8
// Use Pythia8:: prefix explicitly instead

// Note: Using fixed-size arrays instead of structs for ROOT tree compatibility

int GeneratePythiaEvents(int nEvents = 10000, 
                          const char* outputFile = "pythia_events.root",
                          double jetR = 0.4) {
    
    // Load libraries first (before using Pythia/FastJet classes)
    LoadRequiredLibraries();
    
    cout << "========================================" << endl;
    cout << "Generating Pythia Events" << endl;
    cout << "========================================" << endl;
    cout << "Number of events: " << nEvents << endl;
    cout << "Output file: " << outputFile << endl;
    cout << "Jet R parameter: " << jetR << endl;
    cout << "========================================" << endl;
    
    // Check if Pythia headers are available
    #ifndef PYTHIA_VERSION_INTEGER
    cerr << "Warning: Pythia8 headers may not be properly included" << endl;
    #endif
    
    // Check if FastJet headers are available  
    #ifndef FASTJET_VERSION
    cerr << "Warning: FastJet headers may not be properly included" << endl;
    #endif
    
    // Initialize Pythia (use fully qualified name to avoid ambiguity with ROOT's TPythia8)
    Pythia8::Pythia pythia;
    
    // Check for Pythia data directory
    TString pythia8Path = gSystem->Getenv("PYTHIA8");
    if (pythia8Path.Length() > 0) {
        TString pythiaData = pythia8Path + "/share/Pythia8/xmldoc";
        if (gSystem->AccessPathName(pythiaData.Data())) {
            cerr << "Warning: Pythia data directory not found at: " << pythiaData << endl;
        } else {
            cout << "Pythia data directory: " << pythiaData << endl;
        }
    }
    
    // Configure Pythia for pp collisions at 5.36 TeV with Monash tune
    pythia.readString("Beams:eCM = 5360.");   // 5.36 TeV
    pythia.readString("Tune:pp = 14");        // Monash 2013 tune
    pythia.readString("HardQCD:all = on");    // Hard QCD processes
    pythia.readString("PhaseSpace:pTHatMin = 20.");  // Minimum pT hat
    
    // Print Pythia configuration
    cout << "Pythia configuration:" << endl;
    cout << "  Center-of-mass energy: 5.36 TeV" << endl;
    cout << "  Tune: Monash 2013 (Tune:pp = 14)" << endl;
    cout << "  Hard QCD processes: on" << endl;
    cout << "  Minimum pT hat: 20 GeV" << endl;
    
    // Initialize
    cout << "Initializing Pythia..." << endl;
    bool initSuccess = pythia.init();
    if (!initSuccess) {
        cerr << "\n========================================" << endl;
        cerr << "Error: Pythia initialization failed!" << endl;
        cerr << "========================================" << endl;
        
        // Try to get more information
        cerr << "\nPythia status information:" << endl;
        try {
            pythia.info.list();
        } catch (...) {
            cerr << "Could not retrieve Pythia error information" << endl;
        }
        
        cerr << "\nTroubleshooting steps:" << endl;
        cerr << "1. Verify PYTHIA8 environment variable:" << endl;
        cerr << "   echo $PYTHIA8" << endl;
        cerr << "2. Check Pythia8 installation:" << endl;
        cerr << "   ls $PYTHIA8/lib/libpythia8*" << endl;
        cerr << "3. Verify Pythia data files exist:" << endl;
        cerr << "   ls $PYTHIA8/share/Pythia8/xmldoc" << endl;
        cerr << "4. Try loading Pythia manually in ROOT:" << endl;
        cerr << "   root -l" << endl;
        cerr << "   gSystem->Load(\"$PYTHIA8/lib/libpythia8\");" << endl;
        cerr << "========================================\n" << endl;
        
        return 1;
    }
    cout << "✓ Pythia initialized successfully" << endl;
    
    // Create output file and tree
    TFile* file = new TFile(outputFile, "RECREATE");
    TTree* tree = new TTree("events", "Pythia Events");
    
    // Event-level variables
    int eventID = 0;
    int nJets = 0;
    int nTracks = 0;
    
    // Arrays for tracks and jets (fixed size)
    const int maxTracks = 1000;
    const int maxJets = 20;
    
    // Track branches (using fixed-size arrays)
    float track_px[maxTracks], track_py[maxTracks], track_pz[maxTracks], track_e[maxTracks];
    float track_pt[maxTracks], track_eta[maxTracks], track_phi[maxTracks];
    int track_charge[maxTracks], track_id[maxTracks];
    
    // Jet branches
    float jet_px[maxJets], jet_py[maxJets], jet_pz[maxJets], jet_e[maxJets];
    float jet_pt[maxJets], jet_eta[maxJets], jet_phi[maxJets];
    int jet_nConstituents[maxJets];
    
    // Branch definitions
    tree->Branch("eventID", &eventID, "eventID/I");
    tree->Branch("nJets", &nJets, "nJets/I");
    tree->Branch("nTracks", &nTracks, "nTracks/I");
    
    tree->Branch("track_px", track_px, Form("track_px[%d]/F", maxTracks));
    tree->Branch("track_py", track_py, Form("track_py[%d]/F", maxTracks));
    tree->Branch("track_pz", track_pz, Form("track_pz[%d]/F", maxTracks));
    tree->Branch("track_e", track_e, Form("track_e[%d]/F", maxTracks));
    tree->Branch("track_pt", track_pt, Form("track_pt[%d]/F", maxTracks));
    tree->Branch("track_eta", track_eta, Form("track_eta[%d]/F", maxTracks));
    tree->Branch("track_phi", track_phi, Form("track_phi[%d]/F", maxTracks));
    tree->Branch("track_charge", track_charge, Form("track_charge[%d]/I", maxTracks));
    tree->Branch("track_id", track_id, Form("track_id[%d]/I", maxTracks));
    
    tree->Branch("jet_px", jet_px, Form("jet_px[%d]/F", maxJets));
    tree->Branch("jet_py", jet_py, Form("jet_py[%d]/F", maxJets));
    tree->Branch("jet_pz", jet_pz, Form("jet_pz[%d]/F", maxJets));
    tree->Branch("jet_e", jet_e, Form("jet_e[%d]/F", maxJets));
    tree->Branch("jet_pt", jet_pt, Form("jet_pt[%d]/F", maxJets));
    tree->Branch("jet_eta", jet_eta, Form("jet_eta[%d]/F", maxJets));
    tree->Branch("jet_phi", jet_phi, Form("jet_phi[%d]/F", maxJets));
    tree->Branch("jet_nConstituents", jet_nConstituents, Form("jet_nConstituents[%d]/I", maxJets));
    
    // Event selection cuts
    const double ptMin = 0.2;   // Minimum track pT (GeV/c)
    const double etaMax = 0.8;   // Maximum |eta|
    const double jetPtMin = 5.0; // Minimum jet pT (GeV/c)
    
    // Event loop
    int nGoodEvents = 0;
    for (int iEvent = 0; iEvent < nEvents; iEvent++) {
        if (!pythia.next()) continue;
        
        // Initialize arrays
        for (int i = 0; i < maxTracks; i++) {
            track_px[i] = track_py[i] = track_pz[i] = track_e[i] = 0.0;
            track_pt[i] = track_eta[i] = track_phi[i] = 0.0;
            track_charge[i] = track_id[i] = 0;
        }
        for (int i = 0; i < maxJets; i++) {
            jet_px[i] = jet_py[i] = jet_pz[i] = jet_e[i] = 0.0;
            jet_pt[i] = jet_eta[i] = jet_phi[i] = 0.0;
            jet_nConstituents[i] = 0;
        }
        
        // Collect final state charged particles
        vector<PseudoJet> particles;
        vector<int> particleIDs;
        
        nTracks = 0;
        for (int i = 0; i < pythia.event.size(); i++) {
            Pythia8::Particle& p = pythia.event[i];
            
            if (!p.isFinal() || !p.isCharged()) continue;
            
            double pt = p.pT();
            double eta = p.eta();
            
            // Apply track cuts
            if (pt < ptMin || TMath::Abs(eta) > etaMax) continue;
            
            // Store track
            if (nTracks >= maxTracks) break;
            
            track_px[nTracks] = p.px();
            track_py[nTracks] = p.py();
            track_pz[nTracks] = p.pz();
            track_e[nTracks] = p.e();
            track_pt[nTracks] = pt;
            track_eta[nTracks] = eta;
            track_phi[nTracks] = p.phi();
            track_charge[nTracks] = p.charge();
            track_id[nTracks] = i;
            
            // Add to FastJet input
            PseudoJet pj(p.px(), p.py(), p.pz(), p.e());
            pj.set_user_index(nTracks);
            particles.push_back(pj);
            particleIDs.push_back(i);
            
            nTracks++;
        }
        
        // Skip events with no tracks
        if (nTracks == 0) continue;
        
        // Find jets using FastJet
        JetDefinition jet_def(antikt_algorithm, jetR);
        ClusterSequence cs(particles, jet_def);
        vector<PseudoJet> jets_fj = sorted_by_pt(cs.inclusive_jets(jetPtMin));
        
        // Store jets
        nJets = 0;
        for (unsigned int ij = 0; ij < jets_fj.size() && ij < maxJets; ij++) {
            PseudoJet& jet = jets_fj[ij];
            
            // Apply jet cuts
            if (TMath::Abs(jet.eta()) > etaMax) continue;
            
            jet_px[nJets] = jet.px();
            jet_py[nJets] = jet.py();
            jet_pz[nJets] = jet.pz();
            jet_e[nJets] = jet.e();
            jet_pt[nJets] = jet.pt();
            jet_eta[nJets] = jet.eta();
            jet_phi[nJets] = jet.phi();
            jet_nConstituents[nJets] = jet.constituents().size();
            
            nJets++;
        }
        
        // Fill tree
        eventID = iEvent;
        tree->Fill();
        nGoodEvents++;
        
        // Progress report
        if ((iEvent + 1) % 1000 == 0) {
            cout << "Processed " << (iEvent + 1) << " events (" 
                 << nGoodEvents << " good events)" << endl;
        }
    }
    
    // Write and close
    tree->Write();
    file->Close();
    
    cout << "========================================" << endl;
    cout << "Event generation complete!" << endl;
    cout << "Total events: " << nEvents << endl;
    cout << "Good events: " << nGoodEvents << endl;
    cout << "Output file: " << outputFile << endl;
    cout << "========================================" << endl;
    
    if (nGoodEvents == 0) {
        cerr << "Warning: No good events were generated!" << endl;
        return 1;
    }
    
    return 0;
}
