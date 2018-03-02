//
//  plotTransfterFunction.hpp
//  analysisTransferFunction
//
//  Created by Benjamin BOITRELLE on 09/11/2017.
//

#ifndef SRC_PLOTTRANSFTERFUNCTION_HPP_
#define SRC_PLOTTRANSFTERFUNCTION_HPP_

#include <stdio.h>
#include <string>
#include <vector>

#include "TF1.h"
#include "TGraph.h"

class PlotTransferFunction{
 public:
  PlotTransferFunction();
  ~PlotTransferFunction();

  TGraph* PrepareTransferFunctionFitted(int numberOfEntries, double& variable1, double& variable2, TF1* fitToUse, int markerColor);
  void PlotHistogram(std::string title, std::string histoTitle, int bin, int rangeMin, int rangeMax, std::vector<double> parameterToPlot);
  void PlotTransfer(int pixelRange, std::vector<std::vector<double>> inputVectorToAnalyse);
  void PlotBadPixelsMask(std::string title, std::string histoTitle, int binColumn, int columnMin, int columnMax, int binRow, int rowMin, int rowMax, std::vector<std::vector<double>> input, int threshold);
  std::vector<double> GetTemporalNoise();
  std::vector<double> GetOffset();
  void SetNumberOfEvents(int numberOfEvents);
  TF1* SetFitErfc(double threshold_min, double threshold_max);
  void SetFitRange(double fitStart, double fitEnd);
  void SetConversionFactor(double conversionFactor);
  void SetUnits(std::string units);
  void SetNumberOfRows(int row);
  void SetNumberOfColumns(int column);
  void SetOutputDirectory(std::string outputDirectory);

 private:
  std::vector<double> m_temporalNoise;
  std::vector<double> m_offset;
  int m_numberOfEvents;
  double m_fitStart;
  double m_fitEnd;
  double m_conversionFactor;
  std::string m_units;
  int m_column;
  int m_row;
  std::string m_outputDirectory;
};

#endif  // SRC_PLOTTRANSFTERFUNCTION_HPP_
