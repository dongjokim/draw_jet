#include<TEveManager.h>
#include<TVector3.h>

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

void DrawBarrel();
void draw_jet(const char * filename = "list.txt");
void LoadFile( const char * filename );
void DrawArrow();
void make_gui();

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
    LoadFile(fileNames[0]);
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
        } else if ( typeIN == "con" ){
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

    //double norm = 1;
    double norm = 1./LP.Pt();

    // TEXT
    TEveText * t1 = new TEveText(Form("Event = %d",ievent));
    t1->SetFontSize(20);
    t1->RefMainTrans().SetPos(0,1.5,1);
    gElementList->AddElement( t1 );

    // JET  -- color Pink
    for( int ij=0;ij<=iJet;ij++ ){
        TVector3 j = jets[ij];
        norm = 1./j.Pt();
        //norm = 1./j.Pt();
        TEveArrow * aj = new TEveArrow( j.Px()*norm,j.Py()*norm,j.Pz()*norm,0.,0.,0.);
        aj->SetName(Form("Jet pT=%f",j.Pt() ));
        aj->SetMainColor( kPink );
        aj->SetTubeR( 0.008 );
        aj->SetConeR( 0.01);
        aj->SetPickable(kTRUE );
        gElementList->AddElement( aj );

        TEveJetCone *jc = new TEveJetCone("Cone");
        jc->AddCone( j.Eta(),j.Phi(),0.4,1 );
        jc->SetMainTransparency( 80 );
        jc->SetMainColor( kPink ); 
        gElementList->AddElement( jc );


    }
    // Tracks -- Yellow(LP)and Gray(trk)
    for( int it=0;it<=iTrk;it++ ){
        TVector3 t = tracks[it];
        norm = 1./LP.Pt();
        
        TEveArrow * at = new TEveArrow( t.Px()*norm,t.Py()*norm,t.Pz()*norm,0.,0.,0.);
        at->SetName(Form("par pT=%f",t.Pt()));
        if ( t.Pt() < 0.5 ){
            continue;
            at->SetTubeR( 0.002 );
            at->SetConeR( 0.01);
            at->SetMainColor( kGray );
        }else{
            at->SetTubeR( 0.004 );
            at->SetConeR( 0.02);
            at->SetMainColor( kWhite );
        }
        if( allcons.count( t.GetUniqueID() )){
            at->SetMainColor( kBlue );
        }

        at->SetPickable(kTRUE );   // for arrow
    
        TEveText *pt_text = new TEveText(Form("%.1f",t.Pt()));
        //pt_text->SetFontSize(1); 
        pt_text->RefMainTrans().SetPos(at->GetVector());
  
        gElementList->AddElement( pt_text );
        gElementList->AddElement( at );
    }

    // Draw LP
    norm = 1./LP.Pt();
    TEveArrow * aLP = new TEveArrow( LP.Px()*norm,LP.Py()*norm,LP.Pz()*norm,0.,0.,0.);
    aLP->SetName(Form("LP pT=%f",LP.Pt() ));
    aLP->SetMainColor( kYellow );
    aLP->SetTubeR( 0.006 );
    aLP->SetConeR( 0.008 );
    aLP->SetPickable(kTRUE );
    gElementList->AddElement( aLP );

    //gEve->AddElement( gElementList );

    //gEve->FullRedraw3D(kTRUE);
    gEve->Redraw3D();
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
                DrawArrow();
            }
        };
        void Bck(){
            if( cFile > 0 ){
                LoadFile( fileNames[--cFile] );
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
