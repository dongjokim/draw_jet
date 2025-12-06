#include<TEveManager.h>
#include<TVector3.h>
#include<TVector2.h>
#include<TH1D.h>
#include<TH2D.h>
#include<TCanvas.h>
#include<TMath.h>
#include<TLegend.h>
#include<TLatex.h>
#include<vector>
#include<algorithm>

TVector3 jets[20];
TVector3 tracks[100];
TVector3 LP;
int iJet=-1;
int iTrk=-1;
int ievent=-1;
vector<int>  cons[20];
set<int> allcons;
TString fileNames[1000];
int nFile=0;
int cFile=0;
TEveElementList *gElementList = 0;

// Histograms for delta eta and delta phi
TH1D *hDeltaEtaAll = 0;
TH1D *hDeltaPhiAll = 0;
TH1D *hDeltaEtaJets = 0;
TH1D *hDeltaPhiJets = 0;
TH2D *hDeltaEtaPhiAll = 0;
TH2D *hDeltaEtaPhiJets = 0;
// Histograms for pT > 1 GeV/c
TH1D *hDeltaEtaAllPt1 = 0;
TH1D *hDeltaPhiAllPt1 = 0;
TH1D *hDeltaEtaJetsPt1 = 0;
TH1D *hDeltaPhiJetsPt1 = 0;
TH2D *hDeltaEtaPhiAllPt1 = 0;
TH2D *hDeltaEtaPhiJetsPt1 = 0;
// Histograms for single-jet, dijet, and multi-jet events
TH1D *hDeltaEtaJetsSingle = 0;  // Single-jet events (nJets == 1)
TH1D *hDeltaPhiJetsSingle = 0;
TH1D *hDeltaEtaJetsDijet = 0;   // Dijet events (nJets == 2)
TH1D *hDeltaPhiJetsDijet = 0;
TH1D *hDeltaEtaJetsMulti = 0;   // Multi-jet events (nJets >= 3)
TH1D *hDeltaPhiJetsMulti = 0;
TH2D *hDeltaEtaPhiJetsSingle = 0;
TH2D *hDeltaEtaPhiJetsDijet = 0;
TH2D *hDeltaEtaPhiJetsMulti = 0;
TH1D *hJetMultiplicity = 0;  // Distribution of jet multiplicity
TCanvas *cDeltaPlots = 0;

void DrawBarrel();
void draw_jet(const char * filename = "list.txt");
void LoadFile( const char * filename );
void DrawArrow();
void make_gui();
void FillDeltaPlots();
void DrawDeltaPlots();

void draw_jet(const char * filename = "list.txt"){
    ifstream in( filename );
    while ( in.good() ){
        in>>fileNames[nFile++];
    }
    in.close();

        

    DrawBarrel(); 
    make_gui();
    gElementList = new TEveElementList("PythiaTracks");
    gEve->AddElement( gElementList );
    
    // Initialize histograms
    hDeltaEtaAll = new TH1D("hDeltaEtaAll", "#Delta#eta of all particle pairs", 100, -5, 5);
    hDeltaPhiAll = new TH1D("hDeltaPhiAll", "#Delta#phi of all particle pairs", 100, -TMath::Pi(), TMath::Pi());
    hDeltaEtaJets = new TH1D("hDeltaEtaJets", "#Delta#eta of particle pairs in jets", 100, -5, 5);
    hDeltaPhiJets = new TH1D("hDeltaPhiJets", "#Delta#phi of particle pairs in jets", 100, -TMath::Pi(), TMath::Pi());
    
    hDeltaEtaAll->SetLineColor(kBlack);
    hDeltaPhiAll->SetLineColor(kBlack);
    hDeltaEtaJets->SetLineColor(kRed);
    hDeltaPhiJets->SetLineColor(kRed);
    
    // Initialize 2D histograms
    hDeltaEtaPhiAll = new TH2D("hDeltaEtaPhiAll", "#Delta#eta vs #Delta#phi (all pairs)", 
                               50, -TMath::Pi(), TMath::Pi(), 50, -5, 5);
    hDeltaEtaPhiJets = new TH2D("hDeltaEtaPhiJets", "#Delta#eta vs #Delta#phi (jet pairs)", 
                                50, -TMath::Pi(), TMath::Pi(), 50, -5, 5);
    hDeltaEtaPhiAll->SetXTitle("#Delta#phi");
    hDeltaEtaPhiAll->SetYTitle("#Delta#eta");
    hDeltaEtaPhiJets->SetXTitle("#Delta#phi");
    hDeltaEtaPhiJets->SetYTitle("#Delta#eta");
    
    // Initialize histograms for pT > 1 GeV/c
    hDeltaEtaAllPt1 = new TH1D("hDeltaEtaAllPt1", "#Delta#eta of all particle pairs (p_{T}>1 GeV/c)", 100, -5, 5);
    hDeltaPhiAllPt1 = new TH1D("hDeltaPhiAllPt1", "#Delta#phi of all particle pairs (p_{T}>1 GeV/c)", 100, -TMath::Pi(), TMath::Pi());
    hDeltaEtaJetsPt1 = new TH1D("hDeltaEtaJetsPt1", "#Delta#eta of particle pairs in jets (p_{T}>1 GeV/c)", 100, -5, 5);
    hDeltaPhiJetsPt1 = new TH1D("hDeltaPhiJetsPt1", "#Delta#phi of particle pairs in jets (p_{T}>1 GeV/c)", 100, -TMath::Pi(), TMath::Pi());
    
    hDeltaEtaAllPt1->SetLineColor(kBlue);
    hDeltaEtaAllPt1->SetLineStyle(2);
    hDeltaEtaAllPt1->SetLineWidth(2);
    hDeltaPhiAllPt1->SetLineColor(kBlue);
    hDeltaPhiAllPt1->SetLineStyle(2);
    hDeltaPhiAllPt1->SetLineWidth(2);
    hDeltaEtaJetsPt1->SetLineColor(kMagenta);
    hDeltaEtaJetsPt1->SetLineStyle(2);
    hDeltaEtaJetsPt1->SetLineWidth(3);
    hDeltaPhiJetsPt1->SetLineColor(kMagenta);
    hDeltaPhiJetsPt1->SetLineStyle(2);
    hDeltaPhiJetsPt1->SetLineWidth(3);
    
    // Initialize 2D histograms for pT > 1 GeV/c
    hDeltaEtaPhiAllPt1 = new TH2D("hDeltaEtaPhiAllPt1", "#Delta#eta vs #Delta#phi (all pairs, p_{T}>1 GeV/c)", 
                                  50, -TMath::Pi(), TMath::Pi(), 50, -5, 5);
    hDeltaEtaPhiJetsPt1 = new TH2D("hDeltaEtaPhiJetsPt1", "#Delta#eta vs #Delta#phi (jet pairs, p_{T}>1 GeV/c)", 
                                   50, -TMath::Pi(), TMath::Pi(), 50, -5, 5);
    hDeltaEtaPhiAllPt1->SetXTitle("#Delta#phi");
    hDeltaEtaPhiAllPt1->SetYTitle("#Delta#eta");
    hDeltaEtaPhiJetsPt1->SetXTitle("#Delta#phi");
    hDeltaEtaPhiJetsPt1->SetYTitle("#Delta#eta");
    
    // Initialize histograms for single-jet, dijet, and multi-jet analysis
    hDeltaEtaJetsSingle = new TH1D("hDeltaEtaJetsSingle", "#Delta#eta (single-jet events)", 100, -5, 5);
    hDeltaPhiJetsSingle = new TH1D("hDeltaPhiJetsSingle", "#Delta#phi (single-jet events)", 100, -TMath::Pi(), TMath::Pi());
    hDeltaEtaJetsDijet = new TH1D("hDeltaEtaJetsDijet", "#Delta#eta (dijet events)", 100, -5, 5);
    hDeltaPhiJetsDijet = new TH1D("hDeltaPhiJetsDijet", "#Delta#phi (dijet events)", 100, -TMath::Pi(), TMath::Pi());
    hDeltaEtaJetsMulti = new TH1D("hDeltaEtaJetsMulti", "#Delta#eta (multi-jet events, nJets#geq3)", 100, -5, 5);
    hDeltaPhiJetsMulti = new TH1D("hDeltaPhiJetsMulti", "#Delta#phi (multi-jet events, nJets#geq3)", 100, -TMath::Pi(), TMath::Pi());
    
    hDeltaEtaJetsSingle->SetLineColor(kGreen);
    hDeltaEtaJetsSingle->SetLineWidth(2);
    hDeltaPhiJetsSingle->SetLineColor(kGreen);
    hDeltaPhiJetsSingle->SetLineWidth(2);
    hDeltaEtaJetsDijet->SetLineColor(kCyan);
    hDeltaEtaJetsDijet->SetLineWidth(2);
    hDeltaPhiJetsDijet->SetLineColor(kCyan);
    hDeltaPhiJetsDijet->SetLineWidth(2);
    hDeltaEtaJetsMulti->SetLineColor(kOrange);
    hDeltaEtaJetsMulti->SetLineWidth(2);
    hDeltaPhiJetsMulti->SetLineColor(kOrange);
    hDeltaPhiJetsMulti->SetLineWidth(2);
    
    hDeltaEtaPhiJetsSingle = new TH2D("hDeltaEtaPhiJetsSingle", "#Delta#eta vs #Delta#phi (single-jet)", 
                                      50, -TMath::Pi(), TMath::Pi(), 50, -5, 5);
    hDeltaEtaPhiJetsDijet = new TH2D("hDeltaEtaPhiJetsDijet", "#Delta#eta vs #Delta#phi (dijet)", 
                                     50, -TMath::Pi(), TMath::Pi(), 50, -5, 5);
    hDeltaEtaPhiJetsMulti = new TH2D("hDeltaEtaPhiJetsMulti", "#Delta#eta vs #Delta#phi (multi-jet, nJets#geq3)", 
                                    50, -TMath::Pi(), TMath::Pi(), 50, -5, 5);
    hDeltaEtaPhiJetsSingle->SetXTitle("#Delta#phi");
    hDeltaEtaPhiJetsSingle->SetYTitle("#Delta#eta");
    hDeltaEtaPhiJetsDijet->SetXTitle("#Delta#phi");
    hDeltaEtaPhiJetsDijet->SetYTitle("#Delta#eta");
    hDeltaEtaPhiJetsMulti->SetXTitle("#Delta#phi");
    hDeltaEtaPhiJetsMulti->SetYTitle("#Delta#eta");
    
    // Jet multiplicity distribution
    hJetMultiplicity = new TH1D("hJetMultiplicity", "Jet Multiplicity Distribution", 10, 0, 10);
    hJetMultiplicity->SetXTitle("Number of Jets");
    hJetMultiplicity->SetYTitle("Events");
    
    LoadFile(fileNames[0]);
    FillDeltaPlots();
    DrawDeltaPlots();
    //DrawArrow();



}

void LoadFile( const char * filename ){
    TString dummy;
    ifstream in(filename);
    TString typeIN="";
    int ipar=-1;
    int ijet=-1;
    double px=-1;
    double py=-1;
    double pz=-1;
    int dumi;

    while( in.good() ){
        in>>dummy>>typeIN;
        if( typeIN.Contains("START") ){
            in>>dummy>>dummy>>dummy>>dummy>>dummy;
            iJet=iTrk=-1;
            for( int i=0;i<20;i++ ){
                cons[i].clear();
            }
            allcons.clear();
        } else if( typeIN == "LP" ){
            in>>ievent>>dumi>>px>>py>>pz;
            LP.SetXYZ(px,py,pz);
        } else if ( typeIN == "IP" ){
            iTrk++;
            in>>ievent>>ipar>>px>>py>>pz;
            tracks[iTrk] = TVector3(px,py,pz);
            tracks[iTrk].SetUniqueID(ipar); 
        } else if ( typeIN == "JET" ){
            iJet++;
            in>>ievent>>ipar>>px>>py>>pz;
            jets[iJet] = TVector3(px,py,pz);
            jets[iJet].SetUniqueID(ipar); 
        } else if ( typeIN == "con" || typeIN == "CON" ){
            in>>ievent>>ijet>>ipar>>dumi>>dumi;
            cons[ iJet ].push_back( ipar );
            allcons.insert( ipar );
        } else if ( typeIN == "END" ){
            in>>dummy>>dummy>>dummy>>dummy>>dummy;
            return;

        } else {
            in>>dummy>>dummy>>dummy>>dummy>>dummy;
        }
    }
}

void DrawBarrel()
{

    TEveManager::Create();
    // const Valuse
    Double_t lwidth = 0.005;
    Double_t kR_min = 1-lwidth;
    Double_t kR_max = 1;
    Double_t kZ_d   = 1;

    Double_t EtaPartition[7]={-1, -0.8, -0.4, 0, 0.4, 0.8, 1};
    Double_t PzPartition[7]={-1.17520, -0.881059, -0.410752, 0, 0.410752,
        0.881059, 1.17520}; 

    // Set origin point
    TEvePointSet *Origin_Point = new TEvePointSet();
    Origin_Point->SetName("Origin Point");
    Origin_Point->SetMarkerColor(kRed);
    Origin_Point->SetMarkerStyle(3);
    Origin_Point->SetPoint(0, 0., 0., 0.);
    gEve->AddElement(Origin_Point);

    // Draw main barrel (pT = 1, pZ = +-1);
    TEveGeoShape *b;
    b =  new TEveGeoShape("Barrel");
    b->SetShape(new TGeoTube("Tube", kR_min, kR_max, kZ_d));
    b->SetMainColor(kCyan);
    b->SetMainTransparency(80);
    gEve->AddElement(b); 

    // Draw Partition
    for(int iPartition=0; iPartition <7; iPartition++){
        b = new TEveGeoShape("Barrel partiton");
        b->SetShape(new TGeoTube(0, kR_max, lwidth));
        b->SetMainColor(kWhite);
        b->SetMainTransparency(90);
        b->SetTransMatrix( TGeoTranslation(0,0, PzPartition[iPartition]) );
        gEve->AddElement(b);
        TEveText * t1 = new TEveText( Form("%.1f",EtaPartition[iPartition]) );
        t1->SetFontSize(1);
        t1->RefMainTrans().SetPos(0,1.2,PzPartition[iPartition]);
        gEve->AddElement( t1);
    }
    gEve->FullRedraw3D(kTRUE);
}

void DrawArrow(){
    gElementList->DestroyElements();

    // Reference pT for normalization (2.0 GeV/c) to make low pT particles visible
    // Using 2.0 reduces arrow length by factor of 2 compared to 1.0
    const double refPt = 2.0;
    double norm;

    // TEXT
    TEveText * t1 = new TEveText(Form("Event = %d",ievent));
    t1->SetFontSize(20);
    t1->RefMainTrans().SetPos(0,1.5,1);
    gElementList->AddElement( t1 );

    // JET  -- leading jet yellow, others pink
    // First find leading jet
    int leadingJetIdx = -1;
    double maxPt = 0;
    if(iJet >= 0) {
        for(int ij = 0; ij <= iJet; ij++) {
            if(jets[ij].Pt() > maxPt) {
                maxPt = jets[ij].Pt();
                leadingJetIdx = ij;
            }
        }
    }
    
    for( int ij=0;ij<=iJet;ij++ ){
        TVector3 j = jets[ij];
        // Use larger reference pT for jets to make them shorter
        const double jetRefPt = 4.0;  // Larger than particle refPt to reduce jet arrow length
        norm = 1./jetRefPt;
        TEveArrow * aj = new TEveArrow( j.Px()*norm,j.Py()*norm,j.Pz()*norm,0.,0.,0.);
        aj->SetName(Form("Jet pT=%f",j.Pt() ));
        // Leading jet is yellow, others are pink
        if(ij == leadingJetIdx) {
            aj->SetMainColor( kYellow );
        } else {
            aj->SetMainColor( kPink );
        }
        // Thicker arrows for better visibility
        aj->SetTubeR( 0.008 );
        aj->SetConeR( 0.02);
        aj->SetPickable(kTRUE );
        gElementList->AddElement( aj );

        TEveJetCone *jc = new TEveJetCone("Cone");
        jc->AddCone( j.Eta(),j.Phi(),0.4,1 );
        jc->SetMainTransparency( 80 );
        // Leading jet cone is yellow, others are pink
        if(ij == leadingJetIdx) {
            jc->SetMainColor( kYellow );
        } else {
            jc->SetMainColor( kPink );
        }
        gElementList->AddElement( jc );


    }
    // Tracks -- Color by pT and jet membership
    // Use reference pT for normalization so low pT particles are visible
    for( int it=0;it<=iTrk;it++ ){
        TVector3 t = tracks[it];
        norm = 1./refPt;  // Use fixed reference pT instead of LP.Pt()
        
        TEveArrow * at = new TEveArrow( t.Px()*norm,t.Py()*norm,t.Pz()*norm,0.,0.,0.);
        at->SetName(Form("par pT=%f",t.Pt()));
        
        // Set arrow size based on pT
        if ( t.Pt() < 0.5 ){
            continue;
            at->SetTubeR( 0.002 );
            at->SetConeR( 0.01);
        }else{
            at->SetTubeR( 0.004 );
            at->SetConeR( 0.02);
        }
        
        // Set color: blue for jet constituents, yellow for pT < 1 GeV/c, white otherwise
        if( allcons.count( t.GetUniqueID() )){
            at->SetMainColor( kBlue );
        } else if( t.Pt() < 1.0 ){
            at->SetMainColor( kYellow );
        } else {
            at->SetMainColor( kWhite );
        }

        at->SetPickable(kTRUE );   // for arrow
    
        TEveText *pt_text = new TEveText(Form("%.1f",t.Pt()));
        //pt_text->SetFontSize(1); 
        pt_text->RefMainTrans().SetPos(at->GetVector());
  
        gElementList->AddElement( pt_text );
        gElementList->AddElement( at );
    }

    // Draw LP - also use reference pT for consistent scaling
    norm = 1./refPt;
    TEveArrow * aLP = new TEveArrow( LP.Px()*norm,LP.Py()*norm,LP.Pz()*norm,0.,0.,0.);
    aLP->SetName(Form("LP pT=%f",LP.Pt() ));
    aLP->SetMainColor( kYellow );
    // Same thickness as regular particles
    aLP->SetTubeR( 0.004 );
    aLP->SetConeR( 0.02 );
    aLP->SetPickable(kTRUE );
    gElementList->AddElement( aLP );

    //gEve->AddElement( gElementList );

    //gEve->FullRedraw3D(kTRUE);
    gEve->Redraw3D();
}

//==============================================================================
// Fill histograms for delta eta and delta phi
//------------------------------------------------------------------------------
void FillDeltaPlots()
{
    // Clear histograms for new event
    if(hDeltaEtaAll) hDeltaEtaAll->Reset();
    if(hDeltaPhiAll) hDeltaPhiAll->Reset();
    if(hDeltaEtaJets) hDeltaEtaJets->Reset();
    if(hDeltaPhiJets) hDeltaPhiJets->Reset();
    if(hDeltaEtaPhiAll) hDeltaEtaPhiAll->Reset();
    if(hDeltaEtaPhiJets) hDeltaEtaPhiJets->Reset();
    if(hDeltaEtaAllPt1) hDeltaEtaAllPt1->Reset();
    if(hDeltaPhiAllPt1) hDeltaPhiAllPt1->Reset();
    if(hDeltaEtaJetsPt1) hDeltaEtaJetsPt1->Reset();
    if(hDeltaPhiJetsPt1) hDeltaPhiJetsPt1->Reset();
    if(hDeltaEtaPhiAllPt1) hDeltaEtaPhiAllPt1->Reset();
    if(hDeltaEtaPhiJetsPt1) hDeltaEtaPhiJetsPt1->Reset();
    if(hDeltaEtaJetsSingle) hDeltaEtaJetsSingle->Reset();
    if(hDeltaPhiJetsSingle) hDeltaPhiJetsSingle->Reset();
    if(hDeltaEtaJetsDijet) hDeltaEtaJetsDijet->Reset();
    if(hDeltaPhiJetsDijet) hDeltaPhiJetsDijet->Reset();
    if(hDeltaEtaJetsMulti) hDeltaEtaJetsMulti->Reset();
    if(hDeltaPhiJetsMulti) hDeltaPhiJetsMulti->Reset();
    if(hDeltaEtaPhiJetsSingle) hDeltaEtaPhiJetsSingle->Reset();
    if(hDeltaEtaPhiJetsDijet) hDeltaEtaPhiJetsDijet->Reset();
    if(hDeltaEtaPhiJetsMulti) hDeltaEtaPhiJetsMulti->Reset();
    
    // Calculate jet multiplicity for this event
    int nJets = (iJet >= 0) ? (iJet + 1) : 0;
    if(hJetMultiplicity && nJets > 0) hJetMultiplicity->Fill(nJets);
    
    // Determine event category: single-jet, dijet, or multi-jet
    bool isSingleJet = (nJets == 1);
    bool isDijet = (nJets == 2);
    bool isMultiJet = (nJets >= 3);
    
    // Fill histograms for all particle pairs
    for(int i = 0; i <= iTrk; i++) {
        for(int j = i + 1; j <= iTrk; j++) {
            TVector3 p1 = tracks[i];
            TVector3 p2 = tracks[j];
            
            int id1 = p1.GetUniqueID();
            int id2 = p2.GetUniqueID();
            
            double deta = p1.Eta() - p2.Eta();
            double dphi = TVector2::Phi_mpi_pi(p1.Phi() - p2.Phi());
            double absDphi = TMath::Abs(dphi);
            
            // Near side condition: |Δφ| < π/2
            bool nearSide = (absDphi < TMath::Pi()/2.0);
            
            // Fill all pairs
            // Delta eta only for near side
            if(nearSide && hDeltaEtaAll) hDeltaEtaAll->Fill(deta);
            // Delta phi for all pairs
            if(hDeltaPhiAll) hDeltaPhiAll->Fill(dphi);
            // 2D histogram for all pairs
            if(hDeltaEtaPhiAll) hDeltaEtaPhiAll->Fill(dphi, deta);
            
            // Fill pairs with pT > 1 GeV/c
            if(p1.Pt() > 1.0 && p2.Pt() > 1.0) {
                // Delta eta only for near side
                if(nearSide && hDeltaEtaAllPt1) hDeltaEtaAllPt1->Fill(deta);
                // Delta phi for all pairs
                if(hDeltaPhiAllPt1) hDeltaPhiAllPt1->Fill(dphi);
                // 2D histogram for all pairs
                if(hDeltaEtaPhiAllPt1) hDeltaEtaPhiAllPt1->Fill(dphi, deta);
            }
            
            // Check if both particles are in jets (any jet)
            // This checks if both particles are constituents of any jet
            bool inJets = (allcons.count(id1) > 0 && allcons.count(id2) > 0);
            
            if(inJets) {
                // Delta eta only for near side
                if(nearSide && hDeltaEtaJets) hDeltaEtaJets->Fill(deta);
                // Delta phi for all pairs
                if(hDeltaPhiJets) hDeltaPhiJets->Fill(dphi);
                // 2D histogram for all pairs
                if(hDeltaEtaPhiJets) hDeltaEtaPhiJets->Fill(dphi, deta);
                
                // Fill single-jet, dijet, and multi-jet histograms
                if(isSingleJet) {
                    if(nearSide && hDeltaEtaJetsSingle) hDeltaEtaJetsSingle->Fill(deta);
                    if(hDeltaPhiJetsSingle) hDeltaPhiJetsSingle->Fill(dphi);
                    if(hDeltaEtaPhiJetsSingle) hDeltaEtaPhiJetsSingle->Fill(dphi, deta);
                } else if(isDijet) {
                    if(nearSide && hDeltaEtaJetsDijet) hDeltaEtaJetsDijet->Fill(deta);
                    if(hDeltaPhiJetsDijet) hDeltaPhiJetsDijet->Fill(dphi);
                    if(hDeltaEtaPhiJetsDijet) hDeltaEtaPhiJetsDijet->Fill(dphi, deta);
                } else if(isMultiJet) {
                    if(nearSide && hDeltaEtaJetsMulti) hDeltaEtaJetsMulti->Fill(deta);
                    if(hDeltaPhiJetsMulti) hDeltaPhiJetsMulti->Fill(dphi);
                    if(hDeltaEtaPhiJetsMulti) hDeltaEtaPhiJetsMulti->Fill(dphi, deta);
                }
                
                // Fill jet pairs with pT > 1 GeV/c
                if(p1.Pt() > 1.0 && p2.Pt() > 1.0) {
                    // Delta eta only for near side
                    if(nearSide && hDeltaEtaJetsPt1) hDeltaEtaJetsPt1->Fill(deta);
                    // Delta phi for all pairs
                    if(hDeltaPhiJetsPt1) hDeltaPhiJetsPt1->Fill(dphi);
                    // 2D histogram for all pairs
                    if(hDeltaEtaPhiJetsPt1) hDeltaEtaPhiJetsPt1->Fill(dphi, deta);
                }
            }
        }
    }
}

//==============================================================================
// Draw histograms for delta eta and delta phi
//------------------------------------------------------------------------------
void DrawDeltaPlots()
{
    if(!hDeltaEtaAll || !hDeltaPhiAll || !hDeltaEtaJets || !hDeltaPhiJets) return;
    
    if(!cDeltaPlots) {
        cDeltaPlots = new TCanvas("cDeltaPlots", "Delta Eta and Delta Phi Plots", 1200, 800);
        cDeltaPlots->Divide(2, 2);
    }
    
    // Find leading and subleading jets
    double leadingPt = 0;
    double subleadingPt = 0;
    int nJets = 0;
    if(iJet >= 0) {
        nJets = iJet + 1;
        // Find leading and subleading jets by pT
        for(int ij = 0; ij <= iJet; ij++) {
            double pt = jets[ij].Pt();
            if(pt > leadingPt) {
                subleadingPt = leadingPt;
                leadingPt = pt;
            } else if(pt > subleadingPt) {
                subleadingPt = pt;
            }
        }
    }
    
    // Set styles for jet histograms
    hDeltaEtaJets->SetLineColor(kRed);
    hDeltaEtaJets->SetLineWidth(3);
    hDeltaPhiJets->SetLineColor(kRed);
    hDeltaPhiJets->SetLineWidth(3);
    
    // Set styles for all histograms
    hDeltaEtaAll->SetLineColor(kBlack);
    hDeltaEtaAll->SetLineWidth(2);
    hDeltaPhiAll->SetLineColor(kBlack);
    hDeltaPhiAll->SetLineWidth(2);
    
    // Top left: Delta Eta 1D
    cDeltaPlots->cd(1);
    gPad->Clear();
    hDeltaEtaAll->SetStats(0);
    hDeltaEtaAll->Draw();
    if(hDeltaEtaJets->GetEntries() > 0) {
        hDeltaEtaJets->SetStats(0);
        hDeltaEtaJets->Draw("same");
    }
    if(hDeltaEtaAllPt1 && hDeltaEtaAllPt1->GetEntries() > 0) {
        hDeltaEtaAllPt1->SetStats(0);
        hDeltaEtaAllPt1->Draw("same");
    }
    if(hDeltaEtaJetsPt1 && hDeltaEtaJetsPt1->GetEntries() > 0) {
        hDeltaEtaJetsPt1->SetStats(0);
        hDeltaEtaJetsPt1->Draw("same");
    }
    // Overlay single-jet, dijet, and multi-jet histograms
    if(hDeltaEtaJetsSingle && hDeltaEtaJetsSingle->GetEntries() > 0) {
        hDeltaEtaJetsSingle->SetStats(0);
        hDeltaEtaJetsSingle->Draw("same");
    }
    if(hDeltaEtaJetsDijet && hDeltaEtaJetsDijet->GetEntries() > 0) {
        hDeltaEtaJetsDijet->SetStats(0);
        hDeltaEtaJetsDijet->Draw("same");
    }
    if(hDeltaEtaJetsMulti && hDeltaEtaJetsMulti->GetEntries() > 0) {
        hDeltaEtaJetsMulti->SetStats(0);
        hDeltaEtaJetsMulti->Draw("same");
    }
    TLegend *leg1 = new TLegend(0.5, 0.4, 0.9, 0.9);
    leg1->AddEntry(hDeltaEtaAll, "All pairs", "l");
    leg1->AddEntry(hDeltaEtaJets, "Jet pairs (all)", "l");
    leg1->AddEntry(hDeltaEtaAllPt1, "All pairs (p_{T}>1)", "l");
    leg1->AddEntry(hDeltaEtaJetsPt1, "Jet pairs (p_{T}>1)", "l");
    if(hDeltaEtaJetsSingle && hDeltaEtaJetsSingle->GetEntries() > 0) {
        leg1->AddEntry(hDeltaEtaJetsSingle, "Single-jet (nJets=1)", "l");
    }
    if(hDeltaEtaJetsDijet && hDeltaEtaJetsDijet->GetEntries() > 0) {
        leg1->AddEntry(hDeltaEtaJetsDijet, "Dijet (nJets=2)", "l");
    }
    if(hDeltaEtaJetsMulti && hDeltaEtaJetsMulti->GetEntries() > 0) {
        leg1->AddEntry(hDeltaEtaJetsMulti, "Multi-jet (nJets#geq3)", "l");
    }
    leg1->Draw();
    TLatex *tex1 = new TLatex();
    tex1->SetNDC();
    tex1->SetTextSize(0.035);
    tex1->SetTextAlign(31);
    double yPos = 0.95;
    tex1->DrawLatex(0.95, yPos, Form("Event %d", ievent));
    yPos -= 0.05;
    if(nJets > 0) {
        tex1->DrawLatex(0.95, yPos, Form("nJets = %d", nJets));
        yPos -= 0.05;
    }
    if(subleadingPt > 0) {
        tex1->DrawLatex(0.95, yPos, Form("Leading: p_{T}=%.1f GeV/c", leadingPt));
        yPos -= 0.05;
        tex1->DrawLatex(0.95, yPos, Form("Subleading: p_{T}=%.1f GeV/c", subleadingPt));
    } else if(leadingPt > 0) {
        tex1->DrawLatex(0.95, yPos, Form("Leading: p_{T}=%.1f GeV/c", leadingPt));
    }
    
    // Top right: Delta Phi 1D
    cDeltaPlots->cd(2);
    gPad->Clear();
    hDeltaPhiAll->SetStats(0);
    hDeltaPhiAll->Draw();
    if(hDeltaPhiJets->GetEntries() > 0) {
        hDeltaPhiJets->SetStats(0);
        hDeltaPhiJets->Draw("same");
    }
    if(hDeltaPhiAllPt1 && hDeltaPhiAllPt1->GetEntries() > 0) {
        hDeltaPhiAllPt1->SetStats(0);
        hDeltaPhiAllPt1->Draw("same");
    }
    if(hDeltaPhiJetsPt1 && hDeltaPhiJetsPt1->GetEntries() > 0) {
        hDeltaPhiJetsPt1->SetStats(0);
        hDeltaPhiJetsPt1->Draw("same");
    }
    // Overlay single-jet, dijet, and multi-jet histograms
    if(hDeltaPhiJetsSingle && hDeltaPhiJetsSingle->GetEntries() > 0) {
        hDeltaPhiJetsSingle->SetStats(0);
        hDeltaPhiJetsSingle->Draw("same");
    }
    if(hDeltaPhiJetsDijet && hDeltaPhiJetsDijet->GetEntries() > 0) {
        hDeltaPhiJetsDijet->SetStats(0);
        hDeltaPhiJetsDijet->Draw("same");
    }
    if(hDeltaPhiJetsMulti && hDeltaPhiJetsMulti->GetEntries() > 0) {
        hDeltaPhiJetsMulti->SetStats(0);
        hDeltaPhiJetsMulti->Draw("same");
    }
    TLegend *leg2 = new TLegend(0.5, 0.4, 0.9, 0.9);
    leg2->AddEntry(hDeltaPhiAll, "All pairs", "l");
    leg2->AddEntry(hDeltaPhiJets, "Jet pairs (all)", "l");
    leg2->AddEntry(hDeltaPhiAllPt1, "All pairs (p_{T}>1)", "l");
    leg2->AddEntry(hDeltaPhiJetsPt1, "Jet pairs (p_{T}>1)", "l");
    if(hDeltaPhiJetsSingle && hDeltaPhiJetsSingle->GetEntries() > 0) {
        leg2->AddEntry(hDeltaPhiJetsSingle, "Single-jet (nJets=1)", "l");
    }
    if(hDeltaPhiJetsDijet && hDeltaPhiJetsDijet->GetEntries() > 0) {
        leg2->AddEntry(hDeltaPhiJetsDijet, "Dijet (nJets=2)", "l");
    }
    if(hDeltaPhiJetsMulti && hDeltaPhiJetsMulti->GetEntries() > 0) {
        leg2->AddEntry(hDeltaPhiJetsMulti, "Multi-jet (nJets#geq3)", "l");
    }
    leg2->Draw();
    TLatex *tex2 = new TLatex();
    tex2->SetNDC();
    tex2->SetTextSize(0.035);
    tex2->SetTextAlign(31);
    double yPos2 = 0.95;
    tex2->DrawLatex(0.95, yPos2, Form("Event %d", ievent));
    yPos2 -= 0.05;
    if(nJets > 0) {
        tex2->DrawLatex(0.95, yPos2, Form("nJets = %d", nJets));
        yPos2 -= 0.05;
    }
    if(subleadingPt > 0) {
        tex2->DrawLatex(0.95, yPos2, Form("Leading: p_{T}=%.1f GeV/c", leadingPt));
        yPos2 -= 0.05;
        tex2->DrawLatex(0.95, yPos2, Form("Subleading: p_{T}=%.1f GeV/c", subleadingPt));
    } else if(leadingPt > 0) {
        tex2->DrawLatex(0.95, yPos2, Form("Leading: p_{T}=%.1f GeV/c", leadingPt));
    }
    
    // Bottom left: 2D Delta Eta vs Delta Phi (all pairs)
    cDeltaPlots->cd(3);
    gPad->Clear();
    if(hDeltaEtaPhiAll && hDeltaEtaPhiAll->GetEntries() > 0) {
        hDeltaEtaPhiAll->SetStats(0);
        hDeltaEtaPhiAll->Draw("colz");
    }
    if(hDeltaEtaPhiAllPt1 && hDeltaEtaPhiAllPt1->GetEntries() > 0) {
        hDeltaEtaPhiAllPt1->SetStats(0);
        hDeltaEtaPhiAllPt1->SetLineColor(kBlue);
        hDeltaEtaPhiAllPt1->SetLineWidth(2);
        hDeltaEtaPhiAllPt1->Draw("cont same");
    }
    TLatex *tex3 = new TLatex();
    tex3->SetNDC();
    tex3->SetTextSize(0.035);
    tex3->SetTextAlign(31);
    if(subleadingPt > 0) {
        tex3->DrawLatex(0.95, 0.95, Form("Event %d", ievent));
        tex3->DrawLatex(0.95, 0.90, Form("Leading: p_{T}=%.1f GeV/c", leadingPt));
        tex3->DrawLatex(0.95, 0.85, Form("Subleading: p_{T}=%.1f GeV/c", subleadingPt));
    } else if(leadingPt > 0) {
        tex3->DrawLatex(0.95, 0.95, Form("Event %d", ievent));
        tex3->DrawLatex(0.95, 0.90, Form("Leading: p_{T}=%.1f GeV/c", leadingPt));
    } else {
        tex3->DrawLatex(0.95, 0.95, Form("Event %d", ievent));
    }
    
    // Bottom right: 2D Delta Eta vs Delta Phi (jet pairs)
    cDeltaPlots->cd(4);
    gPad->Clear();
    if(hDeltaEtaPhiJets && hDeltaEtaPhiJets->GetEntries() > 0) {
        hDeltaEtaPhiJets->SetStats(0);
        hDeltaEtaPhiJets->Draw("colz");
    }
    if(hDeltaEtaPhiJetsPt1 && hDeltaEtaPhiJetsPt1->GetEntries() > 0) {
        hDeltaEtaPhiJetsPt1->SetStats(0);
        hDeltaEtaPhiJetsPt1->SetLineColor(kMagenta);
        hDeltaEtaPhiJetsPt1->SetLineWidth(2);
        hDeltaEtaPhiJetsPt1->Draw("cont same");
    }
    TLatex *tex4 = new TLatex();
    tex4->SetNDC();
    tex4->SetTextSize(0.035);
    tex4->SetTextAlign(31);
    if(subleadingPt > 0) {
        tex4->DrawLatex(0.95, 0.95, Form("Event %d", ievent));
        tex4->DrawLatex(0.95, 0.90, Form("Leading: p_{T}=%.1f GeV/c", leadingPt));
        tex4->DrawLatex(0.95, 0.85, Form("Subleading: p_{T}=%.1f GeV/c", subleadingPt));
    } else if(leadingPt > 0) {
        tex4->DrawLatex(0.95, 0.95, Form("Event %d", ievent));
        tex4->DrawLatex(0.95, 0.90, Form("Leading: p_{T}=%.1f GeV/c", leadingPt));
    } else {
        tex4->DrawLatex(0.95, 0.95, Form("Event %d", ievent));
    }
    
    cDeltaPlots->Update();
    cDeltaPlots->Modified();
    cDeltaPlots->Show();
}

//==============================================================================
// GUI stuff
//------------------------------------------------------------------------------
class EvNavHandler
{
    public:
        void Fwd()
        {

            if( cFile < nFile ){
                LoadFile( fileNames[++cFile] );
                FillDeltaPlots();
                DrawDeltaPlots();
                DrawArrow();
            }
        };
        void Bck(){
            if( cFile > 0 ){
                LoadFile( fileNames[--cFile] );
                FillDeltaPlots();
                DrawDeltaPlots();
                DrawArrow();
            }
        };

};

//______________________________________________________________________________
void make_gui()
{
    // Create minimal GUI for event navigation.

    TEveBrowser* browser = gEve->GetBrowser();
    browser->StartEmbedding(TRootBrowser::kLeft);

    TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600);
    frmMain->SetWindowName("XX GUI");
    frmMain->SetCleanup(kDeepCleanup);

    TGHorizontalFrame* hf = new TGHorizontalFrame(frmMain);
    {

        TString icondir( Form("%s/icons/", gSystem->Getenv("ROOTSYS")) );
        TGPictureButton* b = 0;
        EvNavHandler    *fh = new EvNavHandler;

        b = new TGPictureButton(hf, gClient->GetPicture(icondir+"GoBack.gif"));
        //b->SetEnabled(kFALSE);
        b->SetToolTipText("Go to previous event - not supported.");
        hf->AddFrame(b);
        b->Connect("Clicked()", "EvNavHandler", fh, "Bck()");

        b = new TGPictureButton(hf, gClient->GetPicture(icondir+"GoForward.gif"));
        b->SetToolTipText("Generate new event.");
        hf->AddFrame(b);
        b->Connect("Clicked()", "EvNavHandler", fh, "Fwd()");
    }
    frmMain->AddFrame(hf);

    frmMain->MapSubwindows();
    frmMain->Resize();
    frmMain->MapWindow();

    browser->StopEmbedding();
    browser->SetTabTitle("Event Control", 0);
}
