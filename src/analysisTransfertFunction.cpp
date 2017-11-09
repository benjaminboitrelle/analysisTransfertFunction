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

#include "processAsciiFile.hpp"

int main() {
  const std::string INPUT_FILE = "/Users/ben/PostDoc/sCurve/input/scans_testlab1.txt";

  std::fstream myfile(INPUT_FILE);
  std::vector<std::vector<double>> outputImageVectorised;

  ProcessAsciiFile asciiFileToRead;
  asciiFileToRead.readAsciiFile(myfile, outputImageVectorised);
  
  std::cout << "Hello World!" << std::endl;
  
  return 0;
}
