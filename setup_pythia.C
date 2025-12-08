// Setup macro for draw_jet_pythia.C
// Load this first: .L setup_pythia.C
// Then load: .L draw_jet_pythia.C

{
  cout << "========================================" << endl;
  cout << "Setting up environment for draw_jet_pythia" << endl;
  cout << "========================================" << endl;
  
  // Disable autoloading to avoid O2/Boost conflicts
  gInterpreter->SetClassAutoloading(false);
  cout << "✓ Autoloading disabled" << endl;
  
  // Load required libraries
  cout << "\nLoading libraries..." << endl;
  
  // Load GUI libraries first (needed for macOS)
  gSystem->Load("libGCocoa");
  gSystem->Load("libGui");
  cout << "✓ GUI libraries loaded" << endl;
  
  gSystem->Load("libEve");
  gSystem->Load("libGeom");
  cout << "✓ TEve libraries loaded" << endl;
  
  // Load Pythia 8
  TString pythia8Path = gSystem->Getenv("PYTHIA8");
  if(pythia8Path.Length() > 0) {
    TString pythiaLib = pythia8Path + "/lib/libpythia8";
    if(gSystem->Load(pythiaLib) >= 0) {
      cout << "✓ Pythia 8 loaded from " << pythia8Path << endl;
    } else {
      cout << "✗ Warning: Could not load Pythia 8" << endl;
    }
  } else {
    if(gSystem->Load("libpythia8") >= 0) {
      cout << "✓ Pythia 8 loaded" << endl;
    } else {
      cout << "✗ Warning: Could not load Pythia 8" << endl;
    }
  }
  
  // Load FastJet
  TString fastjetPath = gSystem->Getenv("FASTJET");
  if(fastjetPath.Length() > 0) {
    TString fastjetLib = fastjetPath + "/lib/libfastjet";
    if(gSystem->Load(fastjetLib) >= 0) {
      cout << "✓ FastJet loaded from " << fastjetPath << endl;
    } else {
      cout << "✗ Warning: Could not load FastJet" << endl;
    }
  } else {
    if(gSystem->Load("libfastjet") >= 0) {
      cout << "✓ FastJet loaded" << endl;
    } else {
      cout << "✗ Warning: Could not load FastJet" << endl;
    }
  }
  
  cout << "\n========================================" << endl;
  cout << "Setup complete! You can now load:" << endl;
  cout << "  .L draw_jet_pythia.C" << endl;
  cout << "  draw_jet_pythia();" << endl;
  cout << "========================================" << endl;
}
