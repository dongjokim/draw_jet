//===========================================================
// JParticleTools.h
// DongJo Kim (dong.jo.kim@cern.ch)
//===========================================================

#ifndef JPARTICLETOOLS_H
#define JPARTICLETOOLS_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include <Pythia8/Pythia.h>

#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TRandom3.h"

#include "JBaseTrack.h"
#include "set"
#include "map"

using namespace std;
using namespace Pythia8; 

class JParticleTools {

	public:

		JParticleTools (Event &inevent): 
			event(inevent){

				unif = new TRandom3();
				fInputList = new TClonesArray("JBaseTrack");
				TrackEtaRange = 5.0;
			}

		void InitializeEvent(){
			fInputList->Clear();
			UniqueID=0;
		}

		void GetParticles();
		TClonesArray * GetInputList() const{return fInputList;}
		TRandom3 *unif;
		int GetTracks() const {return Nch;};
		int GetMidMultiplicity()  const {return Nchmid;}
		int GetFT0M() const {return NchFT0M;}
		int GetV0M()  const {return NchV0M;}
		int GetCMSMultiplicity() const {return NchCMS;}
		int UniqueID;
		TLorentzVector lvParticle;
		TClonesArray *fInputList;  // tracklist

		Event &event;
		int Nch;
		int Nchmid;
		int NchFT0M;
		int NchV0M;
		int NchCMS;

		double TrackEtaRange ;

};

#endif

