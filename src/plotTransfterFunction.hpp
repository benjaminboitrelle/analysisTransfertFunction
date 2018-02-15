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
  
  TGraph* PrepareTransfertFunctionFitted(int numberOfEntries, double &variable1, double &variable2, TF1* fitToUse, int markerColor );
  void PlotHistogram(std::string title, std::string histoTitle, int bin, int rangeMin, int rangeMax, std::vector<double> parameterToPlot );
  void PlotTransfert(int pixelRange, std::vector<std::vector<double>> inputVectorToAnalyse);
  std::vector<double> GetTemporalNoise();
  std::vector<double> GetOffset();
  void SetNumberOfEvents(int numberOfEvents);
  TF1* SetFitErfc(double threshold_min, double threshold_max);
  void SetFitRange(double fitStart, double fitEnd);
  void SetConversionFactor(double conversionFactor);
  void SetUnits(std::string units);

  
private:
  std::vector<double> m_temporalNoise;
  std::vector<double> m_offset;
  int m_numberOfEvents;
  double m_fitStart;
  double m_fitEnd;
  double m_conversionFactor;
  std::string m_units;

};

#endif /* plotTransfterFunction_hpp */
