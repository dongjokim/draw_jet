#include <algorithm>
Double_t GenGaussian(const Double_t *x, const Double_t *par) {
  double beta = par[0];
  double alpha = par[1];
  double f = par[2];
  double zyam = par[3];
  double factor = (f * beta) / (2.0 * alpha * TMath::Gamma(1.0 / beta));
  double term = TMath::Exp(-TMath::Power(TMath::Abs(x[0]) / alpha, beta));
  return zyam + (factor * term);
}


std::tuple<double, double> GenGaussianSigma(double jetBeta, double jetBetaError,
                                            double jetAlpha,
                                            double jetAlphaError) {
  double invJetBeta1 = 1.0 / jetBeta;
  double invJetBeta3 = 3.0 * invJetBeta1;
  double gammaInvJetBeta1 = TMath::Gamma(invJetBeta1);
  double gammaInvJetBeta3 = TMath::Gamma(invJetBeta3);
  double gammaFactor = gammaInvJetBeta3 / gammaInvJetBeta1;
  double jetSigma = jetAlpha * TMath::Sqrt(gammaFactor);
  double polyGamma1 =
      (TMath::Gamma(invJetBeta1 + 1e-6) - TMath::Gamma(invJetBeta1 - 1e-6)) /
      (2e-6 * gammaInvJetBeta1);
  double polyGamma3 =
      (TMath::Gamma(invJetBeta3 + 1e-6) - TMath::Gamma(invJetBeta3 - 1e-6)) /
      (2e-6 * gammaInvJetBeta3);
  double polyExp = polyGamma1 - 3.0 * polyGamma3;
  double jetSigmaError =
      TMath::Sqrt(gammaFactor * jetAlphaError * jetAlphaError +
                  0.5 * jetAlpha * jetAlpha * jetBetaError * jetBetaError *
                      gammaFactor * polyExp * polyExp / (jetBeta * jetBeta));
  return {jetSigma, jetSigmaError};
}

void fitYield(const TString input_file, const TString output_file);

TString Path = "../Data/";
TString runSys = "NeNe"; //OO or NeNe
const bool useMultBins = true;  // this replaces centrality to multiplicity bins -- cent2Mult folder has the dat files
ofstream fit_status_summary("fit_status_summary_" + runSys + ".txt", ios::out);

void fitYield_all() {
	TString input_file;
	TString output_file;

  input_file = Path + "JetShape_" + runSys + ".root";
  output_file = Path + "fitJetShape_" + runSys + ".root";
  cout<<"Processing: "<<input_file<<" -> "<<output_file<<endl;
  fitYield(input_file.Data(), output_file.Data());

}

void fitYield(const TString input_file, const TString output_file) {
  
  TFile *infile = new TFile(input_file, "read");
  if (!infile) {
    cout << "Input file missing" << endl;
    return;
  }
  TFile *outfile = new TFile(output_file, "recreate");
  outfile->cd();

  double effCorr[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  std::vector<double> xBins;
  if(useMultBins){
    if (runSys == "OO") {
      xBins = {6.75728, 30.5803, 64.4195, 93.3647, 140.792};
    } else if (runSys == "NeNe") {
      xBins = {7.49835, 36.7439, 80.4162, 119.344, 181.104};
    }
  }
  else xBins= {80, 45, 25, 15, 5};
  
  // double xBins[] = {80, 50, 30, 10};
  // double xBins[] = {85.12597662, 78.64028817, 69.29416772, 56.18524629, 42.57332625, 32.09453017, 23.94454905, 17.59803041, 12.69011614,  8.85295212,  4.21740219};
  int nBins = end(xBins) - begin(xBins);

  double pTTrig[] = {1,2,3,4,8};
  double pTAssoc[] = {1,2,3,4,8};
  int nTrigBins = std::size(pTTrig)-1;
  int nAssocBins = std::size(pTAssoc)-1;

  for (int itrig = 0; itrig < nTrigBins; itrig++) {
    for (int iassoc = 0; iassoc < nAssocBins; iassoc++) {
      if(iassoc > itrig) continue;
      // Create graphs for each pTassoc bin
      TGraphErrors *pjetSigma = new TGraphErrors(nBins);
      TGraphErrors *pjetSigmadphi = new TGraphErrors(nBins);
      TGraphErrors *pjetYield = new TGraphErrors(nBins);
      TGraphErrors *pjetYielddphi = new TGraphErrors(nBins);
      TGraph *chi2byndf = new TGraph(nBins);
      TGraph *chi2byndfdphi = new TGraph(nBins);
      TH2F *chi2vsFitStatus = new TH2F("chi2vsFitStatus", "chi2vsFitStatus", 10, 0.0, 10.0, 10, 0, 10);

      for (int ib = 0; ib < nBins; ib++) {
        TH1D *heta = (TH1D *)infile->Get(
            Form("proj_deta_%u_%u_%u", itrig, iassoc, ib));
            
        // heta->Rebin(2);
        // heta->Scale(0.5);
        TH1D *hphi = (TH1D *)infile->Get(
            Form("proj_dphi_SR_%u_%u_%u", itrig, iassoc, ib));

        if (!heta || !hphi) {
          cout << "Error reading deta or dphi histogram " << itrig << "\t" << iassoc << "\t" << ib << endl;
          continue;
        }

        // delta eta
        double minEta = -1.3, maxEta = 1.3;
        TF1 fitJet(Form("fitJet_%u_%u_%u", itrig, iassoc, ib), GenGaussian, minEta,
                   maxEta, 4);
        // double etaMin =
        // phe1->GetMinimum(1e-5);//pfit->GetMinimumX(-TMath::Pi()/2.0,3.0/2.0*TMath::Pi());
        // if (itrig==1 && ib == 2) {
        //   fitJet.SetParLimits(0, 2.0,3.0);
        //   fitJet.SetParLimits(1, 0.01,1.0);
        //   fitJet.SetParameter(2, 0.01);
        //   fitJet.SetParameter(3, heta->GetMinimum(1e-5));
        // }
        // fitJet.SetParLimits(0, 0.01,3.0);
        // fitJet.SetParLimits(1, 0.01,1.0);
        // fitJet.SetParameter(2, 0.01);
        // fitJet.SetParameter(3, heta->GetMinimum(1e-5));
        
        fitJet.SetParameter(0, 1.0);
        fitJet.SetParameter(1, 0.5);
        fitJet.SetParameter(2, 0.01);
        fitJet.SetParameter(3, heta->GetMinimum(1e-5));

        fitJet.SetParLimits(0, 0.1, 7.5);
        fitJet.SetParLimits(1, 0.01, 1.5);

        cout << "Fitting with " << minEta << "\t" << maxEta << endl;
        double fit_status = heta->Fit(&fitJet, "SE", "", minEta, maxEta);
        double fchi2byndf = fitJet.GetChisquare() / fitJet.GetNDF();

        if(fit_status != 0){
          fit_status_summary << output_file << "\t" << fit_status << "\t" << itrig << "\t" << iassoc << "\t" << ib << endl;
        }

        chi2vsFitStatus->Fill(fit_status, fchi2byndf);

        double jetBeta = fitJet.GetParameter(0);
        double jetBetaError = fitJet.GetParError(0);
        double jetAlpha = fitJet.GetParameter(1);
        double jetAlphaError = fitJet.GetParError(1);
        auto [jetSigma, jetSigmaError] =
            GenGaussianSigma(jetBeta, jetBetaError, jetAlpha, jetAlphaError);
        // printf("%f %f %f %f %f %f", jetSigma, jetSigmaError, jetBeta,
        // jetBetaError,
        //  jetAlpha, jetAlphaError);

        // Extract yield from the fit
        // Extract yield from the deta fit
        TH1D *h_deta = (TH1D *)heta->Clone(Form("%s_ZYAM_subtracted", heta->GetName()));
        double dzyam_deta = fitJet.GetParameter(3);
        for(uint i = 1; i <= h_deta->GetXaxis()->GetNbins(); ++i){
          double y = h_deta->GetBinContent(i);
          double yerr = h_deta->GetBinError(i);
          h_deta->SetBinContent(i,y-dzyam_deta);
        }

        double nearSideJet_phi = 1.4; // Eta cut on jet
        int a = heta->GetXaxis()->FindBin(-nearSideJet_phi); //|dphi| range < 1.4
        int b = heta->GetXaxis()->FindBin(+nearSideJet_phi);
        double Yfrag_deta_Err;
        double Yfrag_deta = h_deta->IntegralAndError(a,b,Yfrag_deta_Err,"width");

        // Set values in graphs
        pjetSigma->SetPoint(ib, xBins[ib], jetSigma);
        pjetSigma->SetPointError(ib, 0.0, jetSigmaError);
        pjetYield->SetPoint(ib, xBins[ib], Yfrag_deta/effCorr[ib]);
        pjetYield->SetPointError(ib, 0.0, Yfrag_deta_Err/effCorr[ib]);
        chi2byndf->SetPoint(ib, xBins[ib], fchi2byndf);
        
        heta->Write(heta->GetName());
        h_deta->Write(h_deta->GetName());
        fitJet.Write();

        // delta phi
        double deltaPhi = TMath::Pi()/2.0;
        TF1 fitJetdphi(Form("fitJetdphi_%u_%u_%u", itrig, iassoc, ib),
                       GenGaussian, -deltaPhi, deltaPhi, 4);
        // double etaMin =
        // phe1->GetMinimum(1e-5);//pfit->GetMinimumX(-TMath::Pi()/2.0,3.0/2.0*TMath::Pi());
        fitJetdphi.SetParameter(0, 1);
        fitJetdphi.SetParameter(1, 1);
        fitJetdphi.SetParameter(2, 0.01);
        fitJetdphi.SetParameter(3, hphi->GetMinimum(1e-5));

        fitJetdphi.SetParLimits(0, 0.1, 2.0);
        fitJetdphi.SetParLimits(1, 0.01, 2.0);
    
        cout << "Fitting delta phi peak in " << -deltaPhi << "\t" << deltaPhi << endl;
        int fit_status_dphi = hphi->Fit(&fitJetdphi, "SE", "", -deltaPhi, deltaPhi);
        double fchi2byndfdphi = fitJetdphi.GetChisquare() / fitJetdphi.GetNDF();
        // if(fit_status_dphi != 0){
        //   fit_status_summary << output_file << "\t dphi" << fit_status_dphi << "\t" << itrig << "\t" << iassoc << "\t" << ib << endl;
        // }
        // heta->Draw();
        // fitJet.Draw("same");

        jetBeta = fitJetdphi.GetParameter(0);
        jetBetaError = fitJetdphi.GetParError(0);
        jetAlpha = fitJetdphi.GetParameter(1);
        jetAlphaError = fitJetdphi.GetParError(1);
        auto [jetSigmadphi, jetSigmaErrordphi] =
            GenGaussianSigma(jetBeta, jetBetaError, jetAlpha, jetAlphaError);
            
        // Extract yield from the dphi fit
        TH1D *h_dphi = (TH1D *)hphi->Clone(Form("%s_ZYAM_subtracted", hphi->GetName()));
        double dzyam_dphi = fitJetdphi.GetParameter(3);
        for(uint i = 1; i <= h_dphi->GetXaxis()->GetNbins(); ++i){
          double y = h_dphi->GetBinContent(i);
          double yerr = h_dphi->GetBinError(i);
          h_dphi->SetBinContent(i,y-dzyam_dphi);
        }

        // Fragmentation yield
        double nearSideJet_eta = 1.4; // Eta cut on jet
        int a_dp = hphi->GetXaxis()->FindBin(-nearSideJet_eta); //|dphi| range < 1.4
        int b_dp = hphi->GetXaxis()->FindBin(+nearSideJet_eta);
        double Yfrag_dphi_Err;
        double Yfrag_dphi = h_dphi->IntegralAndError(a_dp,b_dp,Yfrag_dphi_Err,"width");

        pjetSigmadphi->SetPoint(ib, xBins[ib], jetSigmadphi);
        pjetSigmadphi->SetPointError(ib, 0.0, jetSigmaErrordphi);
        pjetYielddphi->SetPoint(ib, xBins[ib], Yfrag_dphi/effCorr[ib]);
        pjetYielddphi->SetPointError(ib, 0.0, Yfrag_dphi_Err/effCorr[ib]);
        chi2byndfdphi->SetPoint(ib, xBins[ib], fchi2byndfdphi);

        hphi->Write(hphi->GetName());
        h_dphi->Write(h_dphi->GetName());
        fitJetdphi.Write();

        heta->Reset();
        hphi->Reset();

        cout << "Eta: sigma = " << jetSigma << " +/- " << jetSigmaError 
             << ", yield = " << Yfrag_deta << " +/- " << Yfrag_deta_Err << endl;
        cout << "Phi: sigma = " << jetSigmadphi << " +/- " << jetSigmaErrordphi 
             << ", yield = " << Yfrag_dphi << " +/- " << Yfrag_dphi_Err << endl;
      }

      
      pjetSigma->SetTitle(Form("%.1f < p_{T}^{trig} < %.1f GeV, %.1f < p_{T}^{assoc} < %.1f GeV", pTTrig[itrig], pTTrig[itrig+1], pTAssoc[iassoc], pTAssoc[iassoc+1]));
      pjetSigmadphi->SetTitle(Form("%.1f < p_{T}^{trig} < %.1f GeV, %.1f < p_{T}^{assoc} < %.1f GeV", pTTrig[itrig], pTTrig[itrig+1], pTAssoc[iassoc], pTAssoc[iassoc+1]));
      pjetYield->SetTitle(Form("%.1f < p_{T}^{trig} < %.1f GeV, %.1f < p_{T}^{assoc} < %.1f GeV", pTTrig[itrig], pTTrig[itrig+1], pTAssoc[iassoc], pTAssoc[iassoc+1]));
      pjetYielddphi->SetTitle(Form("%.1f < p_{T}^{trig} < %.1f GeV, %.1f < p_{T}^{assoc} < %.1f GeV", pTTrig[itrig], pTTrig[itrig+1], pTAssoc[iassoc], pTAssoc[iassoc+1]));
      chi2byndf->SetTitle(Form("%.1f < p_{T}^{trig} < %.1f GeV, %.1f < p_{T}^{assoc} < %.1f GeV", pTTrig[itrig], pTTrig[itrig+1], pTAssoc[iassoc], pTAssoc[iassoc+1]));
      chi2byndfdphi->SetTitle(Form("%.1f < p_{T}^{trig} < %.1f GeV, %.1f < p_{T}^{assoc} < %.1f GeV", pTTrig[itrig], pTTrig[itrig+1], pTAssoc[iassoc], pTAssoc[iassoc+1]));
      chi2vsFitStatus->SetTitle(Form("%.1f < p_{T}^{trig} < %.1f GeV, %.1f < p_{T}^{assoc} < %.1f GeV", pTTrig[itrig], pTTrig[itrig+1], pTAssoc[iassoc], pTAssoc[iassoc+1]));
      
      pjetSigma->Write(Form("jetSigma_%u_%u", itrig, iassoc));
      chi2byndf->Write(Form("chi2byndf_%u_%u", itrig, iassoc));
      chi2vsFitStatus->Write(Form("chi2vsFitStatus_%u_%u", itrig, iassoc));
      pjetSigmadphi->Write(Form("jetSigma_dphi_%u_%u", itrig, iassoc));
      chi2byndfdphi->Write(Form("chi2byndf_dphi_%u_%u", itrig, iassoc));
      
      // Write yield graphs
      pjetYield->Write(Form("jetYield_%u_%u", itrig, iassoc));
      pjetYielddphi->Write(Form("jetYield_dphi_%u_%u", itrig, iassoc));
    }
  }

  outfile->Close();
}