//
//  analysisTransfertFunction
//
//  Created by Benjamin BOITRELLE on 09/11/2017.
//

#include <stdio.h>

#include <TROOT.h>
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
  const std::string INPUT_FILE = "/Users/ben/PostDoc/analysisTransfertFunction/input/discrims_32_8_63_63_res.txt";
  const std::string OUTPUT_FILE = "sCurve_discrims_32_8_63_63_res.root";
  const int THRESHOLD_POSITION = 0;
  
  std::fstream myfile(INPUT_FILE);
  std::vector<std::vector<double>> outputImageVectorised;
  
  ProcessAsciiFile asciiFileToRead;
  asciiFileToRead.readAsciiFile(myfile, outputImageVectorised);
  std::vector<double> threshold = asciiFileToRead.getPixelResponse(outputImageVectorised, THRESHOLD_POSITION);
  const int ANALYSIS_RANGE = asciiFileToRead.getNumberOfPixels();
  //const int ANALYSIS_RANGE = 1000;
  auto outputRootFile = TFile::Open(OUTPUT_FILE.c_str(), "RECREATE");
  std::vector<double> temporalNoise, offset;
  
  PlotTransfertFunction transfertFunction;
  transfertFunction.plotTransfertFunction(ANALYSIS_RANGE, outputImageVectorised);
  
  temporalNoise = transfertFunction.getTemporalNoise();
  offset = transfertFunction.getOffset();
  
  auto temporalNoiseMax = std::max_element(std::begin(temporalNoise), std::end(temporalNoise));
  auto temporalNoiseMin = std::min_element(std::begin(temporalNoise), std::end(temporalNoise));
  auto temporalNoiseBin = std::ceil((*temporalNoiseMax + 1) - (*temporalNoiseMin -1)) * 100;

  auto offsetMax = std::max_element(std::begin(offset), std::end(offset));
  auto offsetMin = std::min_element(std::begin(offset), std::end(offset));
  auto offsetBin = std::ceil((*offsetMax + 1) - (*offsetMin - 1)) * 50;
  
  transfertFunction.plotHistogram("Noise distribution [uADC]", "Temporal Noise", temporalNoiseBin, std::ceil(*temporalNoiseMin) - 1 , std::ceil(*temporalNoiseMax) + 1, temporalNoise);
  transfertFunction.plotHistogram("Threshold distribution [uADC]", "Fixed pattern noise", offsetBin, std::ceil(*offsetMin) - 1 , std::ceil(*offsetMax) + 1, offset);
  
  outputRootFile->Close();
  
  std::cout << "Hello World!" << std::endl;

  
  return 0;
}
