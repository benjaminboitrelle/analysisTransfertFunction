//
//  analysisTransferFunction
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

#include "xmlReader/tinyxml2.h"

using namespace tinyxml2;

int main(int argc, char **argv) {
  std::string xmlPath;
  if (argc < 3 || argc > 4) {
    std::cerr << "Usage: " << argv[0] << " <option(s)> SOURCES "
    << "Options:\n"
    << "\t -h,--help\t\tShow this help message\n"
    << "\t -cfg,--config CONFIGURATION \t"
    << " Specify path and name of XML configuration file to use"
    << std::endl;
    return 1;
  }
  if (strcmp(argv[1], "-cfg") == 0 || strcmp(argv[1], "--config") == 0) {
    xmlPath = argv[2];
  }

  XMLDocument configFileXML;
  XMLError errorResult = configFileXML.LoadFile(xmlPath.c_str());
  if (errorResult != XML_SUCCESS) {
    std::cerr << "ERROR! Could not find XML file." << std::endl;
    return 1;
  }

  const std::string INPUT_FILE = configFileXML.FirstChildElement("FILE")->FirstChildElement("INPUT")->GetText();
  if (INPUT_FILE == nullptr) {
      std::cerr << "Input file field empty..." << std::endl;
      return 1;
  }

  const std::string DELIMITER = configFileXML.FirstChildElement("FILE")->FirstChildElement("DELIMITER")->GetText();
  if (DELIMITER == nullptr) {
      std::cerr << "DELIMITER field empty..." << std::endl;
      return 1;
  }
  std::string delimiterChar;
  if (DELIMITER == "tab") {
    delimiterChar = "\t";
  } else if (DELIMITER == "whitespace") {
    delimiterChar = " ";
  } else {
    delimiterChar = DELIMITER;
  }

  const std::string OUTPUT_FILE = configFileXML.FirstChildElement("FILE")->FirstChildElement("OUTPUT")->GetText();
  if (OUTPUT_FILE == nullptr) {
      std::cerr << "OUTPUT file field empty..." << std::endl;
      return 1;
  }
  const std::string OUTPUT_DIR = configFileXML.FirstChildElement("FILE")->FirstChildElement("OUTPUT_DIR")->GetText();
  if (OUTPUT_DIR == nullptr) {
    std::cerr << "OUTPUT_DIR field empty..." << std::endl;
    return 1;
  }

  const std::string NUMBER_EVENTS = configFileXML.FirstChildElement("ANALYSIS")->FirstChildElement("NUMBER_EVENTS")->GetText();
  if (NUMBER_EVENTS == nullptr) {
    std::cerr << "NUMBER_EVENTS field is empty..." << std::endl;
    return 1;
  }

  const std::string FIT_START = configFileXML.FirstChildElement("ANALYSIS")->FirstChildElement("FIT_START")->GetText();
  if (FIT_START == nullptr) {
    std::cerr << "FIT_START field is empty..." << std::endl;
    return 1;
  }

  const std::string FIT_END = configFileXML.FirstChildElement("ANALYSIS")->FirstChildElement("FIT_END")->GetText();
  if (FIT_END == nullptr) {
    std::cerr << "FIT_END field is empty..." << std::endl;
    return 1;
  }

  const std::string RANGE = configFileXML.FirstChildElement("ANALYSIS")->FirstChildElement("RANGE")->GetText();
  if (RANGE == nullptr) {
    std::cerr << "RANGE field is empty..." << std::endl;
    return 1;
  }

  const std::string CONVERSION_FACTOR = configFileXML.FirstChildElement("ANALYSIS")->FirstChildElement("CVF")->GetText();
  if (CONVERSION_FACTOR == nullptr) {
    std::cerr << "CONVERSION_FACTOR field is empty..." << std::endl;
    return 1;
  }

  const std::string UNITS = configFileXML.FirstChildElement("ANALYSIS")->FirstChildElement("UNITS")->GetText();
  if (UNITS == nullptr) {
    std::cerr << "UNITS field is empty..." << std::endl;
    return 1;
  }
  
  const std::string THRESHOLD = configFileXML.FirstChildElement("ANALYSIS")->FirstChildElement("THRESHOLD")->GetText();
  if (THRESHOLD == nullptr) {
    std::cerr << "THRESHOLD field is empty..." << std::endl;
    return 1;
  }
  
  const std::string COLUMNS = configFileXML.FirstChildElement("SENSOR")->FirstChildElement("COLUMNS")->GetText();
  if (COLUMNS == nullptr) {
    std::cerr << "COLUMNS field is empty..." << std::endl;
    return 1;
  }
  
  const std::string ROWS = configFileXML.FirstChildElement("SENSOR")->FirstChildElement("ROWS")->GetText();
  if (ROWS == nullptr) {
    std::cerr << "ROWS field is empty..." << std::endl;
    return 1;
  }
  

  const int THRESHOLD_POSITION = 0;

  std::fstream myfile(INPUT_FILE);
  std::vector<std::vector<double>> outputImageVectorised;

  ProcessAsciiFile asciiFileToRead;
  asciiFileToRead.SetDelimiterChar(delimiterChar);
  asciiFileToRead.ReadAsciiFile(myfile, outputImageVectorised);
  std::vector<double> threshold = asciiFileToRead.GetPixelResponse(outputImageVectorised, THRESHOLD_POSITION);
  auto outputRootFile = TFile::Open(OUTPUT_FILE.c_str(), "RECREATE");
  std::vector<double> temporalNoise, offset;

  PlotTransferFunction TransferFunction;
  TransferFunction.SetConversionFactor(stod(CONVERSION_FACTOR));
  TransferFunction.SetUnits(UNITS);
  TransferFunction.SetNumberOfEvents(stoi(NUMBER_EVENTS));
  TransferFunction.SetNumberOfColumns(stoi(COLUMNS));
  TransferFunction.SetNumberOfRows(stoi(ROWS));
  TransferFunction.SetOutputDirectory(OUTPUT_DIR);
  
  (FIT_START == "none" || FIT_END == "none")? TransferFunction.SetFitRange(threshold.front(), threshold.back()) : TransferFunction.SetFitRange(stoi(FIT_START), stoi(FIT_END));
  (RANGE == "none")? TransferFunction.PlotTransfer(asciiFileToRead.GetNumberOfPixels(), outputImageVectorised) : TransferFunction.PlotTransfer(stoi(RANGE), outputImageVectorised);

  temporalNoise = TransferFunction.GetTemporalNoise();
  offset = TransferFunction.GetOffset();

  auto temporalNoiseMax = std::max_element(std::begin(temporalNoise), std::end(temporalNoise));
  auto temporalNoiseMin = std::min_element(std::begin(temporalNoise), std::end(temporalNoise));
  auto temporalNoiseBin = std::ceil((*temporalNoiseMax + 1) - (*temporalNoiseMin -1)) * 100;

  auto offsetMax = std::max_element(std::begin(offset), std::end(offset));
  auto offsetMin = std::min_element(std::begin(offset), std::end(offset));
  auto offsetBin = std::ceil((*offsetMax + 1) - (*offsetMin - 1)) * 50;

  std::string temporalNoiseTitle = "Noise distribution [" + UNITS + "]";
  std::string fixedPatternNoiseTitle = "Threshold distribution [" + UNITS + "]";
  std::string badPixelsMaskTitle = "Bad Pixel Mask at threshold: " + THRESHOLD;

  TransferFunction.PlotHistogram(temporalNoiseTitle.c_str(), "Temporal Noise", temporalNoiseBin, std::ceil(*temporalNoiseMin) - 1 , std::ceil(*temporalNoiseMax) + 1, temporalNoise);
  TransferFunction.PlotHistogram(fixedPatternNoiseTitle.c_str(), "Fixed pattern noise", offsetBin, std::ceil(*offsetMin) - 1 , std::ceil(*offsetMax) + 1, offset);
  TransferFunction.PlotBadPixelsMask(badPixelsMaskTitle.c_str(), badPixelsMaskTitle.c_str(), stoi(COLUMNS), 0, stoi(COLUMNS), stoi(ROWS), 0, stoi(ROWS), outputImageVectorised, stoi(THRESHOLD));
  
  outputRootFile->Close();

  return 0;
}
