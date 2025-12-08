void PlotCorrelations(const char* filename = "test.root") {
    // Open the file
    TFile *f = new TFile(filename, "READ");
    if (!f || f->IsZombie()) {
        printf("Error opening file %s\n", filename);
        return;
    }

    // Get the directory containing histograms
    TDirectory *dir = gDirectory;
    if (!dir) {
        printf("Cannot find correlation-task directory\n");
        f->Close();
        return;
    }

    // Get the histograms
    TH2F *histS = (TH2F*)dir->Get("histS");
    TH2F *histB = (TH2F*)dir->Get("histB");
    
    if (!histS || !histB) {
        printf("Cannot find histograms\n");
        f->Close();
        return;
    }

    // Create canvas
    TCanvas *c1 = new TCanvas("c1", "Correlation Analysis", 1200, 400);
    c1->Divide(3,1);

    // Draw same event correlations
    c1->cd(1);
    histS->SetTitle("Same Event");
    histS->Draw("SURF1, FB");

    // Draw mixed event correlations
    c1->cd(2);
    histB->SetTitle("Mixed Event");
    histB->Draw("SURF1, FB");

    // Calculate and draw ratio
    c1->cd(3);
    TH2F *ratio = (TH2F*)histS->Clone("ratio");
    ratio->SetTitle("Correlation Function");
    ratio->Divide(histB);
    ratio->Draw("SURF1, FB");

    c1->Update();

    // Save the canvas
    c1->SaveAs("correlations_simple.pdf");

    // Clean up
    // f->Close();
    // delete f;
} 
