//
//  processAsciiFile.hpp
//  analysisTransferFunction
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
  std::vector<double> ConvertStringToDouble(std::vector<std::string>& inputString, std::vector<double>& output);
  void SplitString(const std::string& inputString, std::string delimiterChar, std::vector<std::string> &outputVector);
  void ReadAsciiFile(std::fstream& myFile, std::vector<std::vector<double>> &outputImageVectorised);
  int GetNumberOfThresholds();
  int GetNumberOfPixels();
  static std::vector<double> GetPixelResponse(std::vector<std::vector<double>> inputVector, int pixelPosition);
  void SetDelimiterChar(std::string delimiterChar);
  
private:
  double m_threshold;
  double m_numberOfPixels;
  std::string m_delimiterChar;
};
#endif /* processAsciiFile_hpp */
