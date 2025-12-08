// $Id: JTreeDataManager_Pythia.h,v 1.0 2025/12/08 djkim Exp $
////////////////////////////////////////////////////
/*!
  \file JTreeDataManager_Pythia.h
  \brief Modified version to read standalone Pythia-generated trees
  \author D.J.Kim (University of Jyvaskyla)
  \email: djkim@jyu.fi
  \version $Revision: 1.0 $
  \date $Date: 2025/12/08 $
 */
////////////////////////////////////////////////////

#ifndef JTREEDATAMANAGER_PYTHIA_H
#define JTREEDATAMANAGER_PYTHIA_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#ifndef ROOT_TObject
#include <TObject.h>
#endif

#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TClonesArray.h>
#include <TMath.h>

#include "JBaseTrack.h"
#include "JBaseEventHeader.h"

class JTreeDataManager_Pythia  {

	public:
		JTreeDataManager_Pythia();
		virtual ~JTreeDataManager_Pythia();		                    //destructor

		void ChainInputStream(const char* infileList);
		int LoadEvent( int ievt );
		virtual void RegisterList(TClonesArray* listToFill, TClonesArray* listFromToFill);
		virtual bool IsGoodEvent();

		// GETTER
		TChain * GetChain(){ return fChain; };
		int GetNEvents(){ return fChain->GetEntries(); }
		JBaseEventHeader * GetEventHeader(){ return fEventHeader; };
		TClonesArray  *GetEventHeaderList(){ return fEventHeaderList; };
		int GetNJets() const { return nJets; };

	protected:
		TChain * fChain;
		TClonesArray  *fTrackList;
		JBaseEventHeader * fEventHeader;
		TClonesArray  *fEventHeaderList;

		// Branch variables for standalone Pythia tree
		int nTracks;
		int nJets;  // Number of jets in the event
		float track_px[1000], track_py[1000], track_pz[1000], track_e[1000];
		float track_pt[1000], track_eta[1000], track_phi[1000];
		int track_charge[1000], track_id[1000];
};

#endif
