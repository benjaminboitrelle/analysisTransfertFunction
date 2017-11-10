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

#include "TFile.h"

#include "processAsciiFile.hpp"
#include "plotTransfterFunction.hpp"

int main() {
  
  // These constant parameters have to be updated from a config file to avoid recompilation
  const std::string INPUT_FILE = "/Users/ben/PostDoc/sCurve/input/scans_testlab1.txt";
  const std::string OUTPUT_FILE = "sCurve.root";
  const int ANALYSIS_RANGE = 1000;
  const int THRESHOLD_POSITION = 0;
  
  std::fstream myfile(INPUT_FILE);
  std::vector<std::vector<double>> outputImageVectorised;
  
  ProcessAsciiFile asciiFileToRead;
  asciiFileToRead.readAsciiFile(myfile, outputImageVectorised);
  std::vector<double> threshold = asciiFileToRead.getPixelResponse(outputImageVectorised, THRESHOLD_POSITION);
  
  auto outputRootFile = TFile::Open(OUTPUT_FILE.c_str(), "RECREATE");
  std::vector<double> temporalNoise, offset;
  
  PlotTransfertFunction transfertFunction;
  transfertFunction.plotTransfertFunction(ANALYSIS_RANGE, outputImageVectorised);
  
  temporalNoise = transfertFunction.getTemporalNoise();
  offset = transfertFunction.getOffset();
  
  transfertFunction.plotHistogram("Noise distribution [uADC]", "Temporal Noise", 600, 0, 60, temporalNoise);
  transfertFunction.plotHistogram("Threshold distribution [mV]", "Fixed pattern noise", 500, -5, 5, offset);
  
  outputRootFile->Close();
  
  return 0;
}
