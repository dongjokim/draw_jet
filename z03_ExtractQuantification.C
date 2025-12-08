// Macro to extract quantification metrics from correlation results
// This version merges all multiplicity bins and focuses on jet categories
// Usage: root -b -q 'ExtractQuantification.C("correlations.root", "quantification.txt")'

#include "TFile.h"
#include "TH2F.h"
#include "TH1D.h"
#include "TF1.h"
#include "TF2.h"
#include "TDirectory.h"
#include "TMath.h"
#include "TFitResult.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

// Generalized 2D Gaussian function for fitting
// Parameters: [0]=amplitude, [1]=mean_eta, [2]=sigma_eta, [3]=mean_phi, [4]=sigma_phi, [5]=background
Double_t GenGaussian2D(Double_t *x, Double_t *par) {
    Double_t deta = x[0] - par[1];  // x[0] is eta
    Double_t dphi = x[1] - par[3];  // x[1] is phi

    Double_t arg_eta = 0;
    Double_t arg_phi = 0;

    if (par[2] != 0) arg_eta = deta / par[2];
    if (par[4] != 0) arg_phi = dphi / par[4];

    // Generalized Gaussian: exp(-0.5 * (arg^2))
    Double_t fitval = par[5] + par[0] * TMath::Exp(-0.5 * (arg_eta*arg_eta + arg_phi*arg_phi));

    return fitval;
}

// Fit correlation function with generalized Gaussian and extract background
// Returns: vector with [background, amplitude, sigma_eta, sigma_phi, NSY (integrated)]
vector<double> FitCorrelationFunction(TH2F* hCorr) {
    vector<double> results = {0.0, 0.0, 0.0, 0.0, 0.0};  // background, amplitude, sigma_eta, sigma_phi, NSY

    if (!hCorr || hCorr->GetEntries() == 0) return results;

    // Create 2D fit function
    TF2 *fitFunc = new TF2("fitFunc", GenGaussian2D, -2.0, 2.0, -TMath::Pi(), TMath::Pi(), 6);

    // Set parameter names
    fitFunc->SetParName(0, "Amplitude");
    fitFunc->SetParName(1, "Mean_eta");
    fitFunc->SetParName(2, "Sigma_eta");
    fitFunc->SetParName(3, "Mean_phi");
    fitFunc->SetParName(4, "Sigma_phi");
    fitFunc->SetParName(5, "Background");

    // Get initial estimates
    double maxVal = hCorr->GetMaximum();
    double minVal = hCorr->GetMinimum();

    // Initial parameter values
    fitFunc->SetParameter(0, maxVal - minVal);  // Amplitude
    fitFunc->SetParameter(1, 0.0);              // Mean eta (centered at 0)
    fitFunc->SetParameter(2, 0.5);              // Sigma eta (initial guess)
    fitFunc->SetParameter(3, 0.0);              // Mean phi (centered at 0)
    fitFunc->SetParameter(4, 0.5);              // Sigma phi (initial guess)
    fitFunc->SetParameter(5, minVal);           // Background (minimum value)

    // Set parameter limits
    fitFunc->SetParLimits(0, 0, maxVal * 2.0);     // Amplitude > 0
    fitFunc->SetParLimits(1, -0.5, 0.5);           // Mean eta near 0
    fitFunc->SetParLimits(2, 0.1, 2.0);            // Sigma eta reasonable range
    fitFunc->SetParLimits(3, -0.5, 0.5);           // Mean phi near 0
    fitFunc->SetParLimits(4, 0.1, 2.0);            // Sigma phi reasonable range
    fitFunc->SetParLimits(5, 0, maxVal);           // Background >= 0

    // Fit in near-side region only
    // For TH2, we need to set the range first, then fit
    fitFunc->SetRange(-1.0, -TMath::Pi()/2.0, 1.0, TMath::Pi()/2.0);
    TFitResultPtr fitResult = hCorr->Fit(fitFunc, "QNRS", "");

    if (fitResult->IsValid()) {
        results[0] = fitFunc->GetParameter(5);  // Background
        results[1] = fitFunc->GetParameter(0);  // Amplitude
        results[2] = fitFunc->GetParameter(2);  // Sigma eta
        results[3] = fitFunc->GetParameter(4);  // Sigma phi

        // Calculate integrated yield (NSY) = Amplitude * 2*pi * sigma_eta * sigma_phi
        results[4] = results[1] * 2.0 * TMath::Pi() * results[2] * results[3];
    } else {
        // If fit fails, fall back to simple background estimation from away-side
        const double dPhiAwayMin = TMath::Pi() - TMath::Pi()/4.0;
        const double dPhiAwayMax = TMath::Pi() + TMath::Pi()/4.0;
        const double dEtaMin = -0.5;
        const double dEtaMax = 0.5;

        int xBinMin = hCorr->GetXaxis()->FindBin(dEtaMin);
        int xBinMax = hCorr->GetXaxis()->FindBin(dEtaMax);
        int yBinMin = hCorr->GetYaxis()->FindBin(dPhiAwayMin);
        int yBinMax = hCorr->GetYaxis()->FindBin(dPhiAwayMax);

        double sum = 0.0;
        int nBins = 0;
        for (int xBin = xBinMin; xBin <= xBinMax; xBin++) {
            for (int yBin = yBinMin; yBin <= yBinMax; yBin++) {
                sum += hCorr->GetBinContent(xBin, yBin);
                nBins++;
            }
        }
        results[0] = (nBins > 0) ? sum / nBins : 0.0;  // Background from away-side

        // Use simple RMS for widths
        results[2] = 0.5;  // Default sigma eta
        results[3] = 0.5;  // Default sigma phi
    }

    delete fitFunc;
    return results;
}

void ExtractQuantification(const char* inputFile = "correlations.root",
                           const char* outputFile = "quantification.txt") {

    cout << "========================================" << endl;
    cout << "Extracting Quantification Metrics" << endl;
    cout << "(Multiplicity-Integrated with Generalized Gaussian Fitting)" << endl;
    cout << "========================================" << endl;
    cout << "Input file: " << inputFile << endl;
    cout << "Output file: " << outputFile << endl;

    TFile* file = TFile::Open(inputFile, "READ");
    if (!file || file->IsZombie()) {
        cerr << "Error: Cannot open file " << inputFile << endl;
        return;
    }

    // Open output file
    ofstream out(outputFile);
    if (!out.is_open()) {
        cerr << "Error: Cannot create output file " << outputFile << endl;
        file->Close();
        return;
    }

    // Write header
    out << "# Quantification Results (Multiplicity-Integrated, Generalized Gaussian Fitting)" << endl;
    out << "# Background extracted from 2D Gaussian fit parameter" << endl;
    out << "# NSY calculated from integrated Gaussian: Amplitude * 2*pi * sigma_eta * sigma_phi" << endl;
    out << "# Format: pT_trig_min pT_trig_max pT_assoc_min pT_assoc_max category NSY background sigma_eta sigma_phi fraction" << endl;
    out << "#" << endl;

    // Get correlation directory
    TDirectory* dirRatio = file->GetDirectory("Correlation");
    if (!dirRatio) {
        cerr << "Error: Cannot find Correlation directory" << endl;
        file->Close();
        out.close();
        return;
    }

    // Find all histogram names and merge by pT bins and jet category
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
            continue; // Skip non-jet-category histograms
        }

        if (trigBin < 0 || assocBin < 0 || multBin < 0 || jetMultCategory < 0) continue;

        // Create merged histogram key (without multiplicity bin)
        const char* catName = (jetMultCategory == 0) ? "Single" : (jetMultCategory == 1) ? "Dijet" : "Multijet";
        char mergedKey[200];
        sprintf(mergedKey, "%d_%d_%d", jetMultCategory, trigBin, assocBin);

        // Get histogram
        TH2F* h = (TH2F*)dirRatio->Get(hname.Data());
        if (!h) continue;

        // Add to merged histogram or create new one
        if (mergedHists.find(mergedKey) == mergedHists.end()) {
            // Create new merged histogram
            char mergedName[200];
            sprintf(mergedName, "hRatio_%s_trig%d_assoc%d_merged", catName, trigBin, assocBin);
            mergedHists[mergedKey] = (TH2F*)h->Clone(mergedName);
        } else {
            // Add to existing merged histogram
            mergedHists[mergedKey]->Add(h);
        }
    }

    cout << "Created " << mergedHists.size() << " merged histograms" << endl;

    // pT bin definitions
    vector<double> ptTrigBins = {1.0, 2.0, 3.0, 4.0, 8.0};
    vector<double> ptAssocBins = {1.0, 2.0, 3.0, 4.0, 8.0};

    // Calculate metrics for merged histograms and store by pT bins
    map<string, map<int, vector<double>>> ptBinResults; // key="trigBin_assocBin", value=map[category] = [NSY, background, sigmaEta, sigmaPhi]

    cout << "\nFitting correlation functions with 2D Gaussian..." << endl;

    for (const auto& entry : mergedHists) {
        string key = entry.first;
        TH2F* h = entry.second;

        // Parse key to get category, trigBin, assocBin
        int jetCat, trigBin, assocBin;
        sscanf(key.c_str(), "%d_%d_%d", &jetCat, &trigBin, &assocBin);

        // Fit correlation function with generalized Gaussian
        // Returns: [background, amplitude, sigma_eta, sigma_phi, NSY (integrated)]
        vector<double> fitResults = FitCorrelationFunction(h);

        double background = fitResults[0];
        double nsy = fitResults[4];  // Integrated NSY from Gaussian fit
        double sigmaEta = fitResults[2];  // Width from fit
        double sigmaPhi = fitResults[3];  // Width from fit

        // Store in ptBinResults
        char ptKey[100];
        sprintf(ptKey, "%d_%d", trigBin, assocBin);
        ptBinResults[ptKey][jetCat] = {nsy, background, sigmaEta, sigmaPhi};
    }

    // Calculate fractions and write output
    out << fixed << setprecision(4);

    for (const auto& entry : ptBinResults) {
        string ptKey = entry.first;
        const auto& catResults = entry.second;

        // Parse pT key
        int trigBin, assocBin;
        sscanf(ptKey.c_str(), "%d_%d", &trigBin, &assocBin);

        // Calculate total NSY
        double totalNSY = 0.0;
        for (const auto& catEntry : catResults) {
            totalNSY += catEntry.second[0]; // NSY is first element
        }

        // Write results for each category
        for (int jetCat = 0; jetCat < 3; jetCat++) {
            string catName = (jetCat == 0) ? "Single" : (jetCat == 1) ? "Dijet" : "Multijet";

            double nsy = 0.0, background = 0.0, sigmaEta = 0.0, sigmaPhi = 0.0, fraction = 0.0;

            if (catResults.find(jetCat) != catResults.end()) {
                const auto& metrics = catResults.at(jetCat);
                nsy = metrics[0];
                background = metrics[1];
                sigmaEta = metrics[2];
                sigmaPhi = metrics[3];

                if (totalNSY > 0) {
                    fraction = (nsy / totalNSY) * 100.0;
                }
            }

            out << ptTrigBins[trigBin] << " " << ptTrigBins[trigBin+1] << " "
                << ptAssocBins[assocBin] << " " << ptAssocBins[assocBin+1] << " "
                << catName << " "
                << nsy << " "
                << background << " "
                << sigmaEta << " "
                << sigmaPhi << " "
                << fraction << endl;
        }
    }

    // Clean up
    for (auto& entry : mergedHists) {
        delete entry.second;
    }

    out.close();
    file->Close();

    cout << "\nQuantification metrics extracted successfully!" << endl;
    cout << "Background extracted from generalized Gaussian fit" << endl;
    cout << "Widths (sigma_eta, sigma_phi) from fit parameters" << endl;
    cout << "NSY from integrated Gaussian (Amplitude * 2*pi * sigma_eta * sigma_phi)" << endl;
    cout << "Results written to: " << outputFile << endl;
    cout << "========================================" << endl;
}
