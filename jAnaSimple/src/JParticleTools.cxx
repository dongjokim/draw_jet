#include "JParticleTools.h"
#include <set>
#include <map>
#include <iostream>
#include <vector>
#include <algorithm> // For std::find
#include <cmath>     // For std::abs

vector<int> CharmPids = {2, 4, 413, 423, 21, 425, 30443, 10411, 2203, 3203, 3103, 415, 2101, 4103, 10421, 435, 4203, 2103, 1103, 4201, 4, 441, 10441, 
                        100441, 443, 10443, 20443, 100443, 30443, 9000443, 9010443, 9020443, 445, 100445, 9941003, 9940003, 9942003};

vector<int> BottomPids = {5, 511, -511, 521, -521, 5122, -5122, 531, -531, 5132, -5132, 5232, -5232, 1103, 4201, 10511, -10511, 10521, 
                        513, 523, 10513, 10523, 20513, 20523, 515, 525, 531, 10531, 533, 10533, 20533, 535, 541, 10541, 543, 10543, 
                        20543, 545, 5122, 5112, 5212, 5222, 5114, 5214, 5224, 5132, 5232, 5312, 5322, 5314, 5324, 5332, 5334, 5142, 
                        5242, 5412, 5422, 5414, 5424, 5342, 5432, 5434, 5442, 5444, 5512, 5522, 5514, 5524, 5532, 5534, 5542, 5544, 
                        5554, 5, 511, 521, 10511, 10521, 513, 523, 10513, 10523, 20513, 20523, 515, 525, 531, 10531, 533, 10533, 20533, 
                        535, 541, 10541, 543, 10543, 20543, 545,
                        551, 10551, 100551, 110551, 200551, 210551, 553, 10553, 20553, 30553, 100553, 110553, 120553, 130553, 200553, 
                        210553, 220553, 300553, 9000553,
                        9010553, 555, 10555, 20555, 100555, 110555, 120555, 200555, 557, 100557,
                        5122, 5112, 5212, 5222, 5114, 5214, 5224, 5132, 5232, 5312, 5322, 5314, 5324, 5332, 5334, 5142, 5242, 5412, 5422, 5414, 5424, 5324, 5432, 5434,
                        5442, 5444, 5512, 5522, 5514, 5524, 5532, 5534, 5542, 5544, 5554 };

using namespace std;

// Define an enum for clarity in handling particle labels
enum ParticleLabel {
    UNDEFINED = -1,
    DIRECT_PROMPT = 0,
    CHARM_PROMPT = 1,
    BOTTOM_NON_PROMPT = 2,
    ADDITIONAL_LABEL_1 = 3, // Additional labels can be added
    ADDITIONAL_LABEL_2 = 4,
};

int DetermineLabel(Particle particle, const Event& event) {
    int pid = particle.id();
    
    // Check if the particle is a Pion or Kaon (relevant for label determination)
    if (std::abs(pid) == 211 || std::abs(pid) == 321) {
        int mom1 = particle.mother1();
        int mom1pid = event[mom1].id();
        if (std::abs(mom1pid) == 421) {
            int cntB=0, cntC=0;
            // cout<<"Event size: "<<event.size()<<endl;
            // cout<<"D0 candidate found: "<<pid<<endl;
            vector<int> momList = event[mom1].motherList();
            // cout<<"Total mothers for this D0: "<<momList.size()<<endl;
            for(long unsigned int j=0; j<momList.size(); j++) {
                int momPid = std::abs(event[j].id());
                // cout<<momPid<<" ";
                if(find(BottomPids.begin(), BottomPids.end(), momPid) != BottomPids.end()){
                    // cout<<"A bottom found!"<<endl;
                    cntB++;
                    }
                else if(find(CharmPids.begin(), CharmPids.end(), momPid) != CharmPids.end()){
                    // cout<<"A charm found!"<<endl;
                    cntC++;
                    }
                else{
                    // cout<<"No bottom or charm found!"<<endl;
                    }
            }
            // cout<<endl;

            if(cntB!=0) return BOTTOM_NON_PROMPT;
            else if(cntC!=0) return CHARM_PROMPT;
            else return DIRECT_PROMPT;  
        }
        else return UNDEFINED;   
    }
    else return UNDEFINED;
                
}
void JParticleTools::GetParticles() { 
    constexpr float MinPt = 0.2f;
    
    Nch = 0;
    Nchmid = 0;
    NchV0M = 0;
    NchFT0M = 0;
    NchCMS = 0;
    
    // Create a map to hold output messages corresponding to each label
    std::map<int, std::string> labelMessages = {
        {DIRECT_PROMPT, "****Direct, prompt****"},
        {CHARM_PROMPT, "****Charm decayed, Prompt****"},
        {BOTTOM_NON_PROMPT, "****Bottom decayed, nonprompt****"},
        // Add other labels and messages here as needed
    };
    
    for (int i = 0; i < event.size(); ++i) {
        if (!event[i].isFinal() || !event[i].isCharged()) {
            continue;
        }
        
        float eta = event[i].eta();
        if(abs(eta)<1.0 && event[i].pT()>0.2){
            Nchmid++; //alice
        } 

        if( (eta>2.8 && eta<5.1) || (eta>-3.7 && eta<-1.7) ){
            NchV0M++; //alice V0M
        }

        if ((eta>3.5 && eta<4.9) || (eta>-3.3 && eta<-2.1)) {
            NchFT0M++; //alice FT0M
        }

        if(abs(eta)<2.4 && event[i].pT()>0.4){
            NchCMS++; //cms
        }
        
        if (std::abs(eta) > TrackEtaRange || event[i].pT() < MinPt) {
            continue;
        }

        float px = event[i].px();
        float py = event[i].py();
        float pz = event[i].pz();
        float energy = event[i].e();
        int pid = event[i].id();
        int stat = event[i].status();
        
	
        int label = DetermineLabel(event[i], event);

        // Use the map to print the message for the corresponding label
        // if (labelMessages.find(label) != labelMessages.end()) {
		// 	if(CHARM_PROMPT == label|| BOTTOM_NON_PROMPT == label)
        //     // std::cout << labelMessages[label] << i << " " << px << " " << py << " " << pz << " " << energy << " " << pid << " " << stat <<"\n"; // Print the corresponding message
        // } else {
		// 	//std::cout << "****Unknown label****" << i << " " << px << " " << py << " " << pz << " " << energy << " " << pid << " " << stat <<"\n"; // Default message for unknown labels
		// }
		new ((*fInputList)[Nch]) JBaseTrack(px, py, pz, energy, pid, label, stat);
        Nch++;
	}
}

