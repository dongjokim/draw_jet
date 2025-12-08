// Macro to generate LaTeX tables from quantification results (merged multiplicity bins)
// Usage: root -b -q 'GenerateTables.C("quantification.txt", "tables")'

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

struct QuantData {
    double ptTrigMin, ptTrigMax;
    double ptAssocMin, ptAssocMax;
    string category;
    double nsy;
    double sigmaEta;
    double sigmaPhi;
    double fraction;
};

void GenerateTables(const char* inputFile = "results/quantification.txt",
                    const char* outputDir = "results/tables") {

    cout << "========================================" << endl;
    cout << "Generating LaTeX Tables" << endl;
    cout << "(Multiplicity-Integrated)" << endl;
    cout << "========================================" << endl;
    cout << "Input file: " << inputFile << endl;
    cout << "Output directory: " << outputDir << endl;

    // Create output directory
    gSystem->Exec(Form("mkdir -p %s", outputDir));

    // Read input file
    ifstream infile(inputFile);
    if (!infile.is_open()) {
        cerr << "Error: Cannot open input file " << inputFile << endl;
        return;
    }

    vector<QuantData> data;
    string line;

    while (getline(infile, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') continue;

        QuantData entry;
        istringstream iss(line);
        iss >> entry.ptTrigMin >> entry.ptTrigMax
            >> entry.ptAssocMin >> entry.ptAssocMax
            >> entry.category
            >> entry.nsy >> entry.sigmaEta >> entry.sigmaPhi >> entry.fraction;

        data.push_back(entry);
    }
    infile.close();

    cout << "Read " << data.size() << " data points" << endl;

    // Group data by pT bins: map[ptKey] -> map[category] -> QuantData
    map<string, map<string, QuantData>> groupedData;

    for (const auto& entry : data) {
        char ptKey[100];
        sprintf(ptKey, "%.1f_%.1f_%.1f_%.1f",
                entry.ptTrigMin, entry.ptTrigMax, entry.ptAssocMin, entry.ptAssocMax);
        groupedData[ptKey][entry.category] = entry;
    }

    // ========================================
    // Table 1: Near-Side Yield (NSY)
    // ========================================
    {
        string filename = string(outputDir) + "/table_nsy.tex";
        ofstream out(filename);
        if (!out.is_open()) {
            cerr << "Error: Cannot create " << filename << endl;
            return;
        }

        out << "\\begin{table}[htbp]" << endl;
        out << "\\centering" << endl;
        out << "\\caption{Near-Side Yield for different jet multiplicities (multiplicity-integrated)}" << endl;
        out << "\\label{tab:nsy_merged}" << endl;
        out << "\\begin{tabular}{cccccc}" << endl;
        out << "\\hline\\hline" << endl;
        out << "$p_T^{\\rm trig}$ (GeV/$c$) & $p_T^{\\rm assoc}$ (GeV/$c$) & Single-jet & Dijet & Multi-jet & Fraction (\\%) \\\\" << endl;
        out << "\\hline" << endl;

        for (const auto& entry : groupedData) {
            const auto& catMap = entry.second;

            // Get the first entry to extract pT ranges
            auto it = catMap.begin();
            if (it == catMap.end()) continue;

            double ptTrigMin = it->second.ptTrigMin;
            double ptTrigMax = it->second.ptTrigMax;
            double ptAssocMin = it->second.ptAssocMin;
            double ptAssocMax = it->second.ptAssocMax;

            // Get values for each category
            double nsySingle = catMap.count("Single") ? catMap.at("Single").nsy : 0.0;
            double nsyDijet = catMap.count("Dijet") ? catMap.at("Dijet").nsy : 0.0;
            double nsyMulti = catMap.count("Multijet") ? catMap.at("Multijet").nsy : 0.0;

            double fracSingle = catMap.count("Single") ? catMap.at("Single").fraction : 0.0;
            double fracDijet = catMap.count("Dijet") ? catMap.at("Dijet").fraction : 0.0;
            double fracMulti = catMap.count("Multijet") ? catMap.at("Multijet").fraction : 0.0;

            out << fixed << setprecision(1);
            out << ptTrigMin << "--" << ptTrigMax << " & ";
            out << ptAssocMin << "--" << ptAssocMax << " & ";

            out << setprecision(2);
            out << nsySingle << " (" << setprecision(1) << fracSingle << "\\%) & ";
            out << setprecision(2) << nsyDijet << " (" << setprecision(1) << fracDijet << "\\%) & ";
            out << setprecision(2) << nsyMulti << " (" << setprecision(1) << fracMulti << "\\%) & ";
            out << setprecision(1) << (fracSingle + fracDijet + fracMulti) << " \\\\" << endl;
        }

        out << "\\hline\\hline" << endl;
        out << "\\end{tabular}" << endl;
        out << "\\end{table}" << endl;
        out.close();

        cout << "Created: " << filename << endl;
    }

    // ========================================
    // Table 2: RMS Widths (sigma_eta)
    // ========================================
    {
        string filename = string(outputDir) + "/table_sigma_eta.tex";
        ofstream out(filename);
        if (!out.is_open()) {
            cerr << "Error: Cannot create " << filename << endl;
            return;
        }

        out << "\\begin{table}[htbp]" << endl;
        out << "\\centering" << endl;
        out << "\\caption{RMS width in $\\Delta\\eta$ for different jet multiplicities (multiplicity-integrated)}" << endl;
        out << "\\label{tab:sigma_eta_merged}" << endl;
        out << "\\begin{tabular}{ccccc}" << endl;
        out << "\\hline\\hline" << endl;
        out << "$p_T^{\\rm trig}$ (GeV/$c$) & $p_T^{\\rm assoc}$ (GeV/$c$) & Single-jet & Dijet & Multi-jet \\\\" << endl;
        out << "\\hline" << endl;

        for (const auto& entry : groupedData) {
            const auto& catMap = entry.second;

            auto it = catMap.begin();
            if (it == catMap.end()) continue;

            double ptTrigMin = it->second.ptTrigMin;
            double ptTrigMax = it->second.ptTrigMax;
            double ptAssocMin = it->second.ptAssocMin;
            double ptAssocMax = it->second.ptAssocMax;

            double sigmaSingle = catMap.count("Single") ? catMap.at("Single").sigmaEta : 0.0;
            double sigmaDijet = catMap.count("Dijet") ? catMap.at("Dijet").sigmaEta : 0.0;
            double sigmaMulti = catMap.count("Multijet") ? catMap.at("Multijet").sigmaEta : 0.0;

            out << fixed << setprecision(1);
            out << ptTrigMin << "--" << ptTrigMax << " & ";
            out << ptAssocMin << "--" << ptAssocMax << " & ";

            out << setprecision(4);
            out << sigmaSingle << " & " << sigmaDijet << " & " << sigmaMulti << " \\\\" << endl;
        }

        out << "\\hline\\hline" << endl;
        out << "\\end{tabular}" << endl;
        out << "\\end{table}" << endl;
        out.close();

        cout << "Created: " << filename << endl;
    }

    // ========================================
    // Table 3: RMS Widths (sigma_phi)
    // ========================================
    {
        string filename = string(outputDir) + "/table_sigma_phi.tex";
        ofstream out(filename);
        if (!out.is_open()) {
            cerr << "Error: Cannot create " << filename << endl;
            return;
        }

        out << "\\begin{table}[htbp]" << endl;
        out << "\\centering" << endl;
        out << "\\caption{RMS width in $\\Delta\\phi$ for different jet multiplicities (multiplicity-integrated)}" << endl;
        out << "\\label{tab:sigma_phi_merged}" << endl;
        out << "\\begin{tabular}{ccccc}" << endl;
        out << "\\hline\\hline" << endl;
        out << "$p_T^{\\rm trig}$ (GeV/$c$) & $p_T^{\\rm assoc}$ (GeV/$c$) & Single-jet & Dijet & Multi-jet \\\\" << endl;
        out << "\\hline" << endl;

        for (const auto& entry : groupedData) {
            const auto& catMap = entry.second;

            auto it = catMap.begin();
            if (it == catMap.end()) continue;

            double ptTrigMin = it->second.ptTrigMin;
            double ptTrigMax = it->second.ptTrigMax;
            double ptAssocMin = it->second.ptAssocMin;
            double ptAssocMax = it->second.ptAssocMax;

            double sigmaSingle = catMap.count("Single") ? catMap.at("Single").sigmaPhi : 0.0;
            double sigmaDijet = catMap.count("Dijet") ? catMap.at("Dijet").sigmaPhi : 0.0;
            double sigmaMulti = catMap.count("Multijet") ? catMap.at("Multijet").sigmaPhi : 0.0;

            out << fixed << setprecision(1);
            out << ptTrigMin << "--" << ptTrigMax << " & ";
            out << ptAssocMin << "--" << ptAssocMax << " & ";

            out << setprecision(4);
            out << sigmaSingle << " & " << sigmaDijet << " & " << sigmaMulti << " \\\\" << endl;
        }

        out << "\\hline\\hline" << endl;
        out << "\\end{tabular}" << endl;
        out << "\\end{table}" << endl;
        out.close();

        cout << "Created: " << filename << endl;
    }

    // ========================================
    // Table 4: Fraction Summary
    // ========================================
    {
        string filename = string(outputDir) + "/table_fractions.tex";
        ofstream out(filename);
        if (!out.is_open()) {
            cerr << "Error: Cannot create " << filename << endl;
            return;
        }

        out << "\\begin{table}[htbp]" << endl;
        out << "\\centering" << endl;
        out << "\\caption{Jet multiplicity fractions (\\%) for different $p_T$ bins (multiplicity-integrated)}" << endl;
        out << "\\label{tab:fractions_merged}" << endl;
        out << "\\begin{tabular}{ccccc}" << endl;
        out << "\\hline\\hline" << endl;
        out << "$p_T^{\\rm trig}$ (GeV/$c$) & $p_T^{\\rm assoc}$ (GeV/$c$) & Single-jet & Dijet & Multi-jet \\\\" << endl;
        out << "\\hline" << endl;

        for (const auto& entry : groupedData) {
            const auto& catMap = entry.second;

            auto it = catMap.begin();
            if (it == catMap.end()) continue;

            double ptTrigMin = it->second.ptTrigMin;
            double ptTrigMax = it->second.ptTrigMax;
            double ptAssocMin = it->second.ptAssocMin;
            double ptAssocMax = it->second.ptAssocMax;

            double fracSingle = catMap.count("Single") ? catMap.at("Single").fraction : 0.0;
            double fracDijet = catMap.count("Dijet") ? catMap.at("Dijet").fraction : 0.0;
            double fracMulti = catMap.count("Multijet") ? catMap.at("Multijet").fraction : 0.0;

            out << fixed << setprecision(1);
            out << ptTrigMin << "--" << ptTrigMax << " & ";
            out << ptAssocMin << "--" << ptAssocMax << " & ";
            out << fracSingle << " & " << fracDijet << " & " << fracMulti << " \\\\" << endl;
        }

        out << "\\hline\\hline" << endl;
        out << "\\end{tabular}" << endl;
        out << "\\end{table}" << endl;
        out.close();

        cout << "Created: " << filename << endl;
    }

    cout << "\\n========================================" << endl;
    cout << "LaTeX tables generated successfully!" << endl;
    cout << "Output directory: " << outputDir << endl;
    cout << "========================================" << endl;
}
