//
//  plotTransfterFunction.cpp
//  analysisTransfertFunction
//
//  Created by Benjamin BOITRELLE on 09/11/2017.
//

#include "plotTransfterFunction.hpp"
#include <string>
#include <vector>

#include "TStyle.h"
#include "TF1.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"

PlotTransfertFunction::PlotTransfertFunction(){
  // Constructor
}

PlotTransfertFunction::~PlotTransfertFunction(){
  // Destructor
}

TF1* PlotTransfertFunction::setFitErfc(int threshold_min, int threshold_max){
  // Create a TF1 containing the complementary error function and set some parameters
  
  std::unique_ptr<TF1> fitErfc{new TF1("fitErfc","(0.5 - 0.5 *  TMath::Erf(( [1]*x+[0]) ))",threshold_min, threshold_max)};
  fitErfc->SetParameter(0, -1.0);
  fitErfc->SetParameter(1, 0.5);
  return fitErfc.release();
}

TGraph* PlotTransfertFunction::transfertFunctionFitted(int numberOfEntries, double &variable1, double &variable2, TF1 *fitToUse, int markerColor){
  // Create and return a TGraph from variable1 and variable2 and fit it according to fitToUse
  
  std::unique_ptr<TGraph> graph{new TGraph(numberOfEntries, &variable1, &variable2)};
  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(0.5);
  graph->SetMarkerColor(markerColor);
  fitToUse->SetLineColor(markerColor);
  graph->Fit(fitToUse, "I");
  return graph.release();
}

void PlotTransfertFunction::plotHistogram(std::string title, std::string histoTitle, int bin, int rangeMin, int rangeMax, std::vector<double> parameterToPlot){
  // Plot an hitogramm from a vector of data set
  
  auto histoToPlot = new TH1F(title.c_str(), histoTitle.c_str(), bin, rangeMin, rangeMax);
  for(auto iterator : parameterToPlot){
    histoToPlot->Fill(iterator);
  }
  histoToPlot->Write();
}
