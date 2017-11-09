//
//  analysisTransfertFunction
//
//  Created by Benjamin BOITRELLE on 09/11/2017.
//

#include <stdio.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <map>
#include <utility>

#include "TGraph.h"
#include "TMultiGraph.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1F.h"
#include "TCanvas.h"

#include "processAsciiFile.hpp"
#include "plotTransfterFunction.hpp"


int main() {
  const std::string INPUT_FILE = "/Users/ben/PostDoc/sCurve/input/scans_testlab1.txt";
  const int THRESHOLD_POSITION = 0;
  const int THRESHOLD_MIN = 0;
  const int PIXEL_RESPONSE_BEGIN = 1;
  const double NUMBER_OF_ENTRIES = 1000. ;
  
  std::fstream myfile(INPUT_FILE);
  std::vector<std::vector<double>> outputImageVectorised;
  
  ProcessAsciiFile asciiFileToRead;
  asciiFileToRead.readAsciiFile(myfile, outputImageVectorised);
  int  numberOfThresholds = asciiFileToRead.getNumberOfThresholds();
  std::vector<double> threshold = asciiFileToRead.getPixelResponse(outputImageVectorised, THRESHOLD_POSITION);
  
  std::unique_ptr<TCanvas> canvas (new TCanvas("canvas", "canvas", 1200, 1100));
  auto outputRootFile = TFile::Open("sCurve.root", "RECREATE");
  std::unique_ptr<TMultiGraph> multiGraph{new TMultiGraph()};
  std::vector<double> temporalNoise, offset;
  
  PlotTransfertFunction transfertFunction;
  
  for (auto numberOfPixels = PIXEL_RESPONSE_BEGIN; numberOfPixels < 10; numberOfPixels++){
    std::vector<double> response = asciiFileToRead.getPixelResponse(outputImageVectorised, numberOfPixels);
    std::transform(response.begin(), response.end(), response.begin(), std::bind2nd(std::divides<double>(), NUMBER_OF_ENTRIES));
    TF1* fit = transfertFunction.setFitErfc(THRESHOLD_MIN, numberOfThresholds);
    multiGraph->Add(transfertFunction.transfertFunctionFitted(numberOfThresholds, threshold[0], response[0], fit, numberOfPixels));
    temporalNoise.push_back(-1. * (fit->GetParameter(0) / fit->GetParameter(1)));
    offset.push_back(1. / (fit->GetParameter(1) * sqrt(2)));
  }
  multiGraph->SetTitle("Transfert function fitted");
  multiGraph->Write();

  transfertFunction.plotHistogram("Noise distribution [uADC]", "Temporal Noise", 600, 0, 60, temporalNoise);
  transfertFunction.plotHistogram("Threshold distribution [mV]", "Fixed pattern noise", 500, -5, 5, offset);

  
  outputRootFile->Close();
  
  return 0;
}
