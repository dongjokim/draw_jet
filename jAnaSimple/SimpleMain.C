#include <iostream>
#include "TROOT.h"
#include "TSystem.h"
#include "TString.h"

int SimpleCorrelation(TString inputfile, TString outputfile);

int main(int argc, char** argv) {
    TString inputFile = "input_trees.txt";
    TString outputFile = "simple_correlation.root";

    if (argc > 1) inputFile = argv[1];
    if (argc > 2) outputFile = argv[2];

    std::cout << "Running SimpleCorrelation with:" << std::endl;
    std::cout << "  Input file: " << inputFile << std::endl;
    std::cout << "  Output file: " << outputFile << std::endl;

    gSystem->Load("libSimpleCorr.so");
    return SimpleCorrelation(inputFile, outputFile);
}
