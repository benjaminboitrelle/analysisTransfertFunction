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
  std::vector<double> getPixelResponse(std::vector<std::vector<double>> inputVector, int pixelPosition);
  
private:
  int m_threshold;
  std::vector<double> m_pixelResponse;
};
#endif /* processAsciiFile_hpp */