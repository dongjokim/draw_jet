#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "src/JBaseTrack.h"
#include "src/JTreeDataManager.h"

void debug_correlation() {
    JTreeDataManager* dmg = new JTreeDataManager();
    dmg->ChainInputStream("input_trees.txt");
    
    TClonesArray *trackList = new TClonesArray("JBaseTrack", 1000);
    
    int nEvents = (dmg->GetNEvents() < 10) ? dmg->GetNEvents() : 10;
    
    cout << "Checking first " << nEvents << " events..." << endl;
    
    for(int evt = 0; evt < nEvents; evt++) {
        dmg->LoadEvent(evt);
        
        trackList->Clear();
        dmg->RegisterList(trackList, NULL);
        int nTracks = trackList->GetEntries();
        
        cout << "\nEvent " << evt << ": " << nTracks << " tracks" << endl;
        
        int accepted = 0;
        for(int i = 0; i < nTracks && i < 5; i++) {  // Check first 5 tracks
            JBaseTrack *trk = (JBaseTrack*)trackList->At(i);
            cout << "  Track " << i << ": "
                 << "pt=" << trk->Pt() 
                 << ", eta=" << trk->Eta() 
                 << ", phi=" << trk->Phi()
                 << ", charge=" << (int)trk->GetCharge()
                 << ", ID=" << trk->GetID();
                 
            bool pass = (TMath::Abs(trk->Eta()) <= 1.0 && 
                        trk->Pt() >= 0.2 && 
                        trk->GetCharge() != 0);
            cout << (pass ? " PASS" : " FAIL") << endl;
            if(pass) accepted++;
        }
        cout << "  Accepted in this event: " << accepted << " tracks" << endl;
    }
}
