// $Id: JTreeDataManager_Pythia.cxx,v 1.0 2025/12/08 djkim Exp $
////////////////////////////////////////////////////
/*!
  \file JTreeDataManager_Pythia.cxx
  \brief Modified version to read standalone Pythia-generated trees
  \author D.J.Kim (University of Jyvaskyla)
  \email: djkim@jyu.fi
  \version $Revision: 1.0 $
  \date $Date: 2025/12/08 $
 */
////////////////////////////////////////////////////

#include "JTreeDataManager_Pythia.h"

//______________________________________________________________________________
JTreeDataManager_Pythia::JTreeDataManager_Pythia():
	fChain(NULL),
	fTrackList(NULL),
	fEventHeader(NULL),
	fEventHeaderList(NULL)
{
	// constructor
	// Use simple "events" tree instead of O2Physics "JCIaa/jTree"
	fChain = new TChain("events");

	// Create dummy event header list (Pythia standalone doesn't have event headers)
	fEventHeaderList = new TClonesArray("JBaseEventHeader", 1);
	// Create a dummy event header so GetEventHeaderList()->At(0) doesn't crash
	fEventHeader = new ((*fEventHeaderList)[0]) JBaseEventHeader();
}

//______________________________________________________________________________
JTreeDataManager_Pythia::~JTreeDataManager_Pythia(){
	if( fChain ) delete fChain;
	if( fTrackList ) delete fTrackList;
	if( fEventHeaderList ) delete fEventHeaderList;
}

//______________________________________________________________________________
bool JTreeDataManager_Pythia::IsGoodEvent(){
	return true;
}

//______________________________________________________________________________
void JTreeDataManager_Pythia::RegisterList(TClonesArray* listToFill, TClonesArray* listFromToFill) {
	// Convert from simple track arrays to JBaseTrack objects

	listToFill->Clear();

	int counter = 0;
	for(int ii = 0; ii < nTracks && ii < 1000; ii++) {
		// Apply eta cut (same as used in event generation)
		if(TMath::Abs(track_eta[ii]) > 0.8) continue;

		// Create JBaseTrack (not AliJBaseTrack - SimpleCorrelation uses JBaseTrack)
		JBaseTrack *track = new ((*listToFill)[counter]) JBaseTrack();
		track->SetPxPyPzE(track_px[ii], track_py[ii], track_pz[ii], track_e[ii]);
		track->SetID(track_id[ii]);
		track->SetCharge(track_charge[ii]);

		counter++;
	}
}

//______________________________________________________________________________
void JTreeDataManager_Pythia::ChainInputStream(const char* infileList){
	// read root nano data files in a list
	char inFile[200];
	ifstream infiles(infileList);
	while ( infiles >> inFile){
		fChain->Add(inFile);
	}

	if(fChain->GetEntriesFast()<=0){
		cout<<"Empty chain from "<<infileList<<endl;
		exit(0);
	}
	cout<<Form("there are %d events.\n", (int)fChain->GetEntries())<<endl;

	// Set branch addresses for standalone Pythia tree structure
	fChain->SetBranchAddress("nTracks", &nTracks);
	fChain->SetBranchAddress("nJets", &nJets);
	fChain->SetBranchAddress("track_px", track_px);
	fChain->SetBranchAddress("track_py", track_py);
	fChain->SetBranchAddress("track_pz", track_pz);
	fChain->SetBranchAddress("track_e", track_e);
	fChain->SetBranchAddress("track_pt", track_pt);
	fChain->SetBranchAddress("track_eta", track_eta);
	fChain->SetBranchAddress("track_phi", track_phi);
	fChain->SetBranchAddress("track_charge", track_charge);
	fChain->SetBranchAddress("track_id", track_id);

	// Allocate TClonesArray for compatibility
	fTrackList = new TClonesArray("AliJBaseTrack", 1000);
}

//______________________________________________________________________________
int JTreeDataManager_Pythia::LoadEvent(int ievt){
	//clear clones array and counters
	//load the new event
	int v = ((TTree*)fChain)->GetEntry(ievt);

	return v;
}
