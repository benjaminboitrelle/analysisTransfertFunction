//
//  processAsciiFile.cpp
//  analysisTransfertFunction
//
//  Created by Benjamin BOITRELLE on 09/11/2017.
//

#include "processAsciiFile.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <map>
#include <utility>

ProcessAsciiFile::ProcessAsciiFile() {
  // Constructor
  m_threshold = 0;
  m_numberOfPixels = 1000;
  m_delimiterChar = ";";
}

ProcessAsciiFile::~ProcessAsciiFile() {
  // Destructor
}

std::vector<double> ProcessAsciiFile::ConvertStringToDouble(std::vector<std::string>& inputString, std::vector<double>& output) {
  // Convert a vector of string into a vector of double

  output.resize(inputString.size());
  std::transform(inputString.begin(), inputString.end(), output.begin(), [](const std::string& val)
                 {
                   return std::stod(val);
                 });
  return output;
}

void ProcessAsciiFile::SplitString(const std::string& inputString, std::string delimiterChar, std::vector<std::string>& outputVector) {
  // Split a string according to a define delimiter and save it into a vector

  std::string::size_type i = 0;
  std::string::size_type j = inputString.find(delimiterChar);

  while (j != std::string::npos) {
    outputVector.push_back(inputString.substr(i, j - i));
    i = ++j;
    j = inputString.find(delimiterChar, j);
    if (j == std::string::npos) outputVector.push_back(inputString.substr(i, inputString.length()));
  }
}

void ProcessAsciiFile::ReadAsciiFile(std::fstream& myfile, std::vector<std::vector<double>> &outputImageVectorised) {
  // Open an Ascii file and store its element in a 2D vector of double

  std::string outputLine;
  while (std::getline(myfile, outputLine)) {
    std::vector<std::string> result;
    SplitString(outputLine, m_delimiterChar, result);
    std::vector<double> resultDouble(result.size());
    ConvertStringToDouble(result, resultDouble);
    outputImageVectorised.push_back(resultDouble);
    m_threshold++;
  }
  m_numberOfPixels = outputImageVectorised.at(0).size() - 1;
}

int ProcessAsciiFile::GetNumberOfThresholds() {
  return m_threshold;
}

std::vector<double> ProcessAsciiFile::GetPixelResponse(std::vector<std::vector<double>> inputImageVectorised, int pixelPosition) {
  // Return a vector containing the threshold (for pixelPosition = 0) or a pixel response (pixelPosition !=0)

  std::vector<double> pixelResponse;
  for (auto row = inputImageVectorised.begin(); row != inputImageVectorised.end(); row++) {
    pixelResponse.push_back(row->at(pixelPosition));
  }
  return pixelResponse;
}

int ProcessAsciiFile::GetNumberOfPixels() {
  return m_numberOfPixels;
}

void ProcessAsciiFile::SetDelimiterChar(std::string delimiterChar) {
  m_delimiterChar = delimiterChar;
}
