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
  TGraph* transfertFunctionFitted(int numberOfEntries, double &variable1, double &variable2, TF1* fitToUse, int markerColor );
  void plotHistogram(std::string title, std::string histoTitle, int bin, int rangeMin, int rangeMax, std::vector<double> parameterToPlot );
  
  
};

#endif /* plotTransfterFunction_hpp */
