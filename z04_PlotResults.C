// Macro to generate figures from correlation results (merged multiplicity bins)
// Usage: root -b -q 'PlotResults.C("correlations.root", "figures")'

#include "TFile.h"
#include "TH2F.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TDirectory.h"
#include "TList.h"
#include "TKey.h"
#include <iostream>
#include <vector>
#include <map>

using namespace std;

void PlotResults(const char* inputFile = "correlations.root",
                 const char* outputDir = "figures") {

    cout << "========================================" << endl;
    cout << "Generating Figures (Multiplicity-Integrated)" << endl;
    cout << "========================================" << endl;
    cout << "Input file: " << inputFile << endl;
    cout << "Output directory: " << outputDir << endl;

    // Create output directory
    gSystem->Exec(Form("mkdir -p %s", outputDir));

    TFile* file = TFile::Open(inputFile, "READ");
    if (!file || file->IsZombie()) {
        cerr << "Error: Cannot open file " << inputFile << endl;
        return;
    }

    // Set style
    gStyle->SetOptStat(0);
    gStyle->SetPalette(55);
    gStyle->SetPadRightMargin(0.15);

    // Get correlation directory
    TDirectory* dirRatio = file->GetDirectory("Correlation");

    if (!dirRatio) {
        cerr << "Error: Cannot find Correlation directory" << endl;
        file->Close();
        return;
    }

    // Find histograms and merge by pT bins
    TList* keys = dirRatio->GetListOfKeys();

    // Map to store merged histograms: key = "jetCat_trigBin_assocBin"
    map<string, TH2F*> mergedHists;

    TIter next(keys);
    TKey* key;
    cout << "\nMerging histograms across multiplicity bins..." << endl;

    while ((key = (TKey*)next())) {
        TString className = key->GetClassName();
        if (className != "TH2F") continue;

        TString hname = key->GetName();

        int trigBin = -1, assocBin = -1, multBin = -1;
        int jetMultCategory = -1;

        if (hname.Contains("Single")) {
            jetMultCategory = 0;
            sscanf(hname.Data(), "hRatio_Single_trig%d_assoc%d_mult%d", &trigBin, &assocBin, &multBin);
        } else if (hname.Contains("Dijet")) {
            jetMultCategory = 1;
            sscanf(hname.Data(), "hRatio_Dijet_trig%d_assoc%d_mult%d", &trigBin, &assocBin, &multBin);
        } else if (hname.Contains("Multijet")) {
            jetMultCategory = 2;
            sscanf(hname.Data(), "hRatio_Multijet_trig%d_assoc%d_mult%d", &trigBin, &assocBin, &multBin);
        } else {
            continue;
        }

        if (trigBin < 0 || assocBin < 0 || multBin < 0 || jetMultCategory < 0) continue;

        const char* catName = (jetMultCategory == 0) ? "Single" : (jetMultCategory == 1) ? "Dijet" : "Multijet";
        char mergedKey[200];
        sprintf(mergedKey, "%d_%d_%d", jetMultCategory, trigBin, assocBin);

        TH2F* h = (TH2F*)dirRatio->Get(hname.Data());
        if (!h) continue;

        if (mergedHists.find(mergedKey) == mergedHists.end()) {
            char mergedName[200];
            sprintf(mergedName, "hRatio_%s_trig%d_assoc%d_merged", catName, trigBin, assocBin);
            mergedHists[mergedKey] = (TH2F*)h->Clone(mergedName);
        } else {
            mergedHists[mergedKey]->Add(h);
        }
    }

    cout << "Created " << mergedHists.size() << " merged histograms" << endl;

    // Group histograms by pT bins
    map<string, vector<TH2F*>> histGroups; // Key: "trig_assoc", Value: [single, dijet, multijet]

    for (const auto& entry : mergedHists) {
        string key = entry.first;
        TH2F* h = entry.second;

        int jetCat, trigBin, assocBin;
        sscanf(key.c_str(), "%d_%d_%d", &jetCat, &trigBin, &assocBin);

        char ptKey[100];
        sprintf(ptKey, "%d_%d", trigBin, assocBin);

        if (histGroups.find(ptKey) == histGroups.end()) {
            histGroups[ptKey].resize(3, nullptr);
        }
        histGroups[ptKey][jetCat] = h;
    }

    // Plot correlation functions
    vector<double> ptTrigBins = {1.0, 2.0, 3.0, 4.0, 8.0};
    vector<double> ptAssocBins = {1.0, 2.0, 3.0, 4.0, 8.0};

    string titles[3] = {"Single-jet", "Dijet", "Multi-jet"};
    int figureCount = 0;

    for (const auto& group : histGroups) {
        string ptKey = group.first;
        vector<TH2F*> hists = group.second;

        int trigBin, assocBin;
        sscanf(ptKey.c_str(), "%d_%d", &trigBin, &assocBin);

        // Skip if all histograms are empty
        bool hasData = false;
        for (int i = 0; i < 3; i++) {
            if (hists[i] && hists[i]->GetEntries() > 0) {
                hasData = true;
                break;
            }
        }
        if (!hasData) continue;

        // Create canvas
        TCanvas* c = new TCanvas(Form("c_trig%d_assoc%d", trigBin, assocBin), "", 1800, 500);
        c->Divide(3, 1);

        for (int i = 0; i < 3; i++) {
            c->cd(i+1);
            gPad->SetRightMargin(0.15);
            gPad->SetLeftMargin(0.12);
            gPad->SetBottomMargin(0.12);

            if (hists[i] && hists[i]->GetEntries() > 0) {
                hists[i]->SetTitle(Form("%s Events;#Delta#eta;#Delta#phi", titles[i].c_str()));
                hists[i]->GetXaxis()->SetTitleSize(0.05);
                hists[i]->GetYaxis()->SetTitleSize(0.05);
                hists[i]->GetXaxis()->SetLabelSize(0.04);
                hists[i]->GetYaxis()->SetLabelSize(0.04);
                hists[i]->Draw("COLZ");

                // Add pT range text
                TLatex* tex = new TLatex();
                tex->SetNDC();
                tex->SetTextSize(0.04);
                tex->DrawLatex(0.15, 0.85, Form("%.1f < p_{T}^{trig} < %.1f GeV/c",
                                                ptTrigBins[trigBin], ptTrigBins[trigBin+1]));
                tex->DrawLatex(0.15, 0.80, Form("%.1f < p_{T}^{assoc} < %.1f GeV/c",
                                                ptAssocBins[assocBin], ptAssocBins[assocBin+1]));
            } else {
                TLatex* tex = new TLatex(0.5, 0.5, Form("No %s events", titles[i].c_str()));
                tex->SetNDC();
                tex->SetTextAlign(22);
                tex->SetTextSize(0.05);
                tex->Draw();
            }
        }

        c->SaveAs(Form("%s/correlation_trig%d_assoc%d.pdf", outputDir, trigBin, assocBin));
        c->SaveAs(Form("%s/correlation_trig%d_assoc%d.png", outputDir, trigBin, assocBin));
        delete c;
        figureCount++;
    }

    // Create 1D projections for selected pT bins
    cout << "\nCreating 1D projections..." << endl;

    for (const auto& group : histGroups) {
        string ptKey = group.first;
        vector<TH2F*> hists = group.second;

        int trigBin, assocBin;
        sscanf(ptKey.c_str(), "%d_%d", &trigBin, &assocBin);

        bool hasData = false;
        for (int i = 0; i < 3; i++) {
            if (hists[i] && hists[i]->GetEntries() > 0) {
                hasData = true;
                break;
            }
        }
        if (!hasData) continue;

        // Delta phi projection
        TCanvas* cPhi = new TCanvas(Form("c_phi_trig%d_assoc%d", trigBin, assocBin), "", 800, 600);
        cPhi->SetLeftMargin(0.12);
        cPhi->SetBottomMargin(0.12);

        TLegend* legPhi = new TLegend(0.65, 0.65, 0.88, 0.88);
        legPhi->SetBorderSize(0);
        legPhi->SetFillStyle(0);

        int colors[3] = {kBlue, kRed, kGreen+2};
        double maxPhi = 0;

        for (int i = 0; i < 3; i++) {
            if (hists[i] && hists[i]->GetEntries() > 0) {
                TH1D* hPhi = hists[i]->ProjectionY(Form("hPhi_%s_trig%d_assoc%d", titles[i].c_str(), trigBin, assocBin));
                hPhi->SetLineColor(colors[i]);
                hPhi->SetLineWidth(2);
                hPhi->SetTitle(Form(";#Delta#phi;dN/d#Delta#phi"));
                hPhi->GetXaxis()->SetTitleSize(0.05);
                hPhi->GetYaxis()->SetTitleSize(0.05);

                if (hPhi->GetMaximum() > maxPhi) maxPhi = hPhi->GetMaximum();

                if (i == 0) {
                    hPhi->Draw("HIST");
                } else {
                    hPhi->Draw("HIST SAME");
                }
                legPhi->AddEntry(hPhi, titles[i].c_str(), "l");
            }
        }

        TLatex* texPhi = new TLatex();
        texPhi->SetNDC();
        texPhi->SetTextSize(0.04);
        texPhi->DrawLatex(0.15, 0.85, Form("%.1f < p_{T}^{trig} < %.1f GeV/c", ptTrigBins[trigBin], ptTrigBins[trigBin+1]));
        texPhi->DrawLatex(0.15, 0.80, Form("%.1f < p_{T}^{assoc} < %.1f GeV/c", ptAssocBins[assocBin], ptAssocBins[assocBin+1]));

        legPhi->Draw();
        cPhi->SaveAs(Form("%s/projection_phi_trig%d_assoc%d.pdf", outputDir, trigBin, assocBin));
        cPhi->SaveAs(Form("%s/projection_phi_trig%d_assoc%d.png", outputDir, trigBin, assocBin));
        delete cPhi;

        // Delta eta projection
        TCanvas* cEta = new TCanvas(Form("c_eta_trig%d_assoc%d", trigBin, assocBin), "", 800, 600);
        cEta->SetLeftMargin(0.12);
        cEta->SetBottomMargin(0.12);

        TLegend* legEta = new TLegend(0.65, 0.65, 0.88, 0.88);
        legEta->SetBorderSize(0);
        legEta->SetFillStyle(0);

        for (int i = 0; i < 3; i++) {
            if (hists[i] && hists[i]->GetEntries() > 0) {
                TH1D* hEta = hists[i]->ProjectionX(Form("hEta_%s_trig%d_assoc%d", titles[i].c_str(), trigBin, assocBin));
                hEta->SetLineColor(colors[i]);
                hEta->SetLineWidth(2);
                hEta->SetTitle(Form(";#Delta#eta;dN/d#Delta#eta"));
                hEta->GetXaxis()->SetTitleSize(0.05);
                hEta->GetYaxis()->SetTitleSize(0.05);

                if (i == 0) {
                    hEta->Draw("HIST");
                } else {
                    hEta->Draw("HIST SAME");
                }
                legEta->AddEntry(hEta, titles[i].c_str(), "l");
            }
        }

        TLatex* texEta = new TLatex();
        texEta->SetNDC();
        texEta->SetTextSize(0.04);
        texEta->DrawLatex(0.15, 0.85, Form("%.1f < p_{T}^{trig} < %.1f GeV/c", ptTrigBins[trigBin], ptTrigBins[trigBin+1]));
        texEta->DrawLatex(0.15, 0.80, Form("%.1f < p_{T}^{assoc} < %.1f GeV/c", ptAssocBins[assocBin], ptAssocBins[assocBin+1]));

        legEta->Draw();
        cEta->SaveAs(Form("%s/projection_eta_trig%d_assoc%d.pdf", outputDir, trigBin, assocBin));
        cEta->SaveAs(Form("%s/projection_eta_trig%d_assoc%d.png", outputDir, trigBin, assocBin));
        delete cEta;

        // ========================================
        // Create comparison plot: Inclusive vs Components
        // ========================================
        TCanvas* cComp = new TCanvas(Form("c_comparison_trig%d_assoc%d", trigBin, assocBin), "", 1200, 500);
        cComp->Divide(2, 1);

        // Delta-phi comparison
        cComp->cd(1);
        gPad->SetLeftMargin(0.12);
        gPad->SetBottomMargin(0.12);

        // Calculate inclusive (sum of all jet categories)
        TH2F* hInclusive = nullptr;
        for (int i = 0; i < 3; i++) {
            if (hists[i] && hists[i]->GetEntries() > 0) {
                if (!hInclusive) {
                    hInclusive = (TH2F*)hists[i]->Clone(Form("hInclusive_trig%d_assoc%d", trigBin, assocBin));
                } else {
                    hInclusive->Add(hists[i]);
                }
            }
        }

        if (hInclusive) {
            TH1D* hPhiInclusive = hInclusive->ProjectionY(Form("hPhi_Inclusive_trig%d_assoc%d", trigBin, assocBin));
            hPhiInclusive->SetLineColor(kBlack);
            hPhiInclusive->SetLineWidth(3);
            hPhiInclusive->SetLineStyle(1);
            hPhiInclusive->SetTitle(";#Delta#phi;dN/d#Delta#phi");
            hPhiInclusive->GetXaxis()->SetTitleSize(0.05);
            hPhiInclusive->GetYaxis()->SetTitleSize(0.05);
            hPhiInclusive->Draw("HIST");

            // Draw components
            for (int i = 0; i < 3; i++) {
                if (hists[i] && hists[i]->GetEntries() > 0) {
                    TH1D* hPhi = hists[i]->ProjectionY(Form("hPhi_comp_%s_trig%d_assoc%d", titles[i].c_str(), trigBin, assocBin));
                    hPhi->SetLineColor(colors[i]);
                    hPhi->SetLineWidth(2);
                    hPhi->SetLineStyle(2);  // Dashed
                    hPhi->Draw("HIST SAME");
                }
            }

            // Legend
            TLegend* legComp1 = new TLegend(0.60, 0.60, 0.88, 0.88);
            legComp1->SetBorderSize(0);
            legComp1->SetFillStyle(0);
            legComp1->AddEntry(hPhiInclusive, "Inclusive", "l");
            legComp1->AddEntry((TObject*)0, "Components:", "");
            for (int i = 0; i < 3; i++) {
                if (hists[i] && hists[i]->GetEntries() > 0) {
                    TH1D* hPhi = (TH1D*)gDirectory->Get(Form("hPhi_comp_%s_trig%d_assoc%d", titles[i].c_str(), trigBin, assocBin));
                    legComp1->AddEntry(hPhi, titles[i].c_str(), "l");
                }
            }
            legComp1->Draw();

            // pT labels
            TLatex* texComp1 = new TLatex();
            texComp1->SetNDC();
            texComp1->SetTextSize(0.04);
            texComp1->DrawLatex(0.15, 0.85, Form("%.1f < p_{T}^{trig} < %.1f GeV/c", ptTrigBins[trigBin], ptTrigBins[trigBin+1]));
            texComp1->DrawLatex(0.15, 0.80, Form("%.1f < p_{T}^{assoc} < %.1f GeV/c", ptAssocBins[assocBin], ptAssocBins[assocBin+1]));
        }

        // Delta-eta comparison
        cComp->cd(2);
        gPad->SetLeftMargin(0.12);
        gPad->SetBottomMargin(0.12);

        if (hInclusive) {
            TH1D* hEtaInclusive = hInclusive->ProjectionX(Form("hEta_Inclusive_trig%d_assoc%d", trigBin, assocBin));
            hEtaInclusive->SetLineColor(kBlack);
            hEtaInclusive->SetLineWidth(3);
            hEtaInclusive->SetLineStyle(1);
            hEtaInclusive->SetTitle(";#Delta#eta;dN/d#Delta#eta");
            hEtaInclusive->GetXaxis()->SetTitleSize(0.05);
            hEtaInclusive->GetYaxis()->SetTitleSize(0.05);
            hEtaInclusive->Draw("HIST");

            // Draw components
            for (int i = 0; i < 3; i++) {
                if (hists[i] && hists[i]->GetEntries() > 0) {
                    TH1D* hEta = hists[i]->ProjectionX(Form("hEta_comp_%s_trig%d_assoc%d", titles[i].c_str(), trigBin, assocBin));
                    hEta->SetLineColor(colors[i]);
                    hEta->SetLineWidth(2);
                    hEta->SetLineStyle(2);  // Dashed
                    hEta->Draw("HIST SAME");
                }
            }

            // Legend
            TLegend* legComp2 = new TLegend(0.60, 0.60, 0.88, 0.88);
            legComp2->SetBorderSize(0);
            legComp2->SetFillStyle(0);
            legComp2->AddEntry(hEtaInclusive, "Inclusive", "l");
            legComp2->AddEntry((TObject*)0, "Components:", "");
            for (int i = 0; i < 3; i++) {
                if (hists[i] && hists[i]->GetEntries() > 0) {
                    TH1D* hEta = (TH1D*)gDirectory->Get(Form("hEta_comp_%s_trig%d_assoc%d", titles[i].c_str(), trigBin, assocBin));
                    legComp2->AddEntry(hEta, titles[i].c_str(), "l");
                }
            }
            legComp2->Draw();

            // pT labels
            TLatex* texComp2 = new TLatex();
            texComp2->SetNDC();
            texComp2->SetTextSize(0.04);
            texComp2->DrawLatex(0.15, 0.85, Form("%.1f < p_{T}^{trig} < %.1f GeV/c", ptTrigBins[trigBin], ptTrigBins[trigBin+1]));
            texComp2->DrawLatex(0.15, 0.80, Form("%.1f < p_{T}^{assoc} < %.1f GeV/c", ptAssocBins[assocBin], ptAssocBins[assocBin+1]));
        }

        cComp->SaveAs(Form("%s/comparison_inclusive_trig%d_assoc%d.pdf", outputDir, trigBin, assocBin));
        cComp->SaveAs(Form("%s/comparison_inclusive_trig%d_assoc%d.png", outputDir, trigBin, assocBin));
        delete cComp;
        if (hInclusive) delete hInclusive;
    }

    file->Close();

    cout << "\n========================================" << endl;
    cout << "Figures generated successfully!" << endl;
    cout << "Generated " << figureCount << " 2D correlation plots" << endl;
    cout << "Generated " << figureCount << " Delta-phi projections" << endl;
    cout << "Generated " << figureCount << " Delta-eta projections" << endl;
    cout << "Generated " << figureCount << " Inclusive vs Components comparisons" << endl;
    cout << "Output directory: " << outputDir << endl;
    cout << "========================================" << endl;
}
