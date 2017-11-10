//
//  plotTransfterFunction.hpp
//  analysisTransfertFunction
//
//  Created by Benjamin BOITRELLE on 09/11/2017.
//

#ifndef plotTransfterFunction_hpp
#define plotTransfterFunction_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include "TF1.h"
#include "TGraph.h"

class PlotTransfertFunction{
public:
  PlotTransfertFunction();
  ~PlotTransfertFunction();
  
  TF1* setFitErfc(int threshold_min, int threshold_max);
  TGraph* prepareTransfertFunctionFitted(int numberOfEntries, double &variable1, double &variable2, TF1* fitToUse, int markerColor );
  void plotHistogram(std::string title, std::string histoTitle, int bin, int rangeMin, int rangeMax, std::vector<double> parameterToPlot );
  void plotTransfertFunction(int pixelRange, std::vector<std::vector<double>> inputVectorToAnalyse);
  std::vector<double> getTemporalNoise();
  std::vector<double> getOffset();
  
private:
  std::vector<double> m_temporalNoise;
  std::vector<double> m_offset;
  
};

#endif /* plotTransfterFunction_hpp */
