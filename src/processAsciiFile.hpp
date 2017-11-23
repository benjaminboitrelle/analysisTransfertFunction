//
//  processAsciiFile.hpp
//  analysisTransfertFunction
//
//  Created by Benjamin BOITRELLE on 09/11/2017.
//

#ifndef processAsciiFile_hpp
#define processAsciiFile_hpp

#include <stdio.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <map>
#include <utility>
#include <fstream>

class ProcessAsciiFile{
  
public:
  ProcessAsciiFile();
  ~ProcessAsciiFile();
  std::vector<double> convertStringToDouble(std::vector<std::string>& inputString, std::vector<double>& output);
  void splitString(const std::string& inputString, char delimiterChar, std::vector<std::string> &outputVector);
  void readAsciiFile(std::fstream& myFile, std::vector<std::vector<double>> &outputImageVectorised);
  int getNumberOfThresholds();
  int getNumberOfPixels();
  static std::vector<double> getPixelResponse(std::vector<std::vector<double>> inputVector, int pixelPosition);
  
private:
  double m_threshold;
  double m_numberOfPixels;
};
#endif /* processAsciiFile_hpp */
