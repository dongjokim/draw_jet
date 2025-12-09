void test_tracks() {
    TFile *f = TFile::Open("../multiple_events/jtree_136TeVMonash_seed10234.root");
    TTree *t = (TTree*)f->Get("jTree");
    
    TClonesArray *tracks = new TClonesArray("JBaseTrack");
    t->SetBranchAddress("JTrackList", &tracks);
    
    int nEvents = (t->GetEntries() < 100) ? t->GetEntries() : 100;
    int totalTracks = 0;
    int acceptedTracks = 0;
    
    for(int i = 0; i < nEvents; i++) {
        t->GetEntry(i);
        int nTracks = tracks->GetEntries();
        totalTracks += nTracks;
        
        for(int j = 0; j < nTracks; j++) {
            JBaseTrack *trk = (JBaseTrack*)tracks->At(j);
            if(TMath::Abs(trk->Eta()) <= 1.0 && trk->Pt() >= 0.2 && trk->GetCharge() != 0) {
                acceptedTracks++;
                if(i == 0) {  // Print first event's tracks
                    cout << "Track " << j << ": pt=" << trk->Pt() 
                         << " eta=" << trk->Eta() 
                         << " phi=" << trk->Phi() 
                         << " charge=" << (int)trk->GetCharge() 
                         << " ID=" << trk->GetID() << endl;
                }
            }
        }
    }
    
    cout << "\nSummary for first " << nEvents << " events:" << endl;
    cout << "Total tracks: " << totalTracks << endl;
    cout << "Accepted tracks (|eta|<1, pT>0.2, charged): " << acceptedTracks << endl;
    
    f->Close();
}
