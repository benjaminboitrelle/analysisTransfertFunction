//
//  plotTransfterFunction.cpp
//  analysisTransfertFunction
//
//  Created by Benjamin BOITRELLE on 09/11/2017.
//

#include "plotTransfterFunction.hpp"
#include "processAsciiFile.hpp"

#include <string>
#include <vector>

#include "TStyle.h"
#include "TF1.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TMultiGraph.h"
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

TGraph* PlotTransfertFunction::prepareTransfertFunctionFitted(int numberOfEntries, double &variable1, double &variable2, TF1 *fitToUse, int markerColor){
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

void PlotTransfertFunction::plotTransfertFunction(int pixelRange, std::vector<std::vector<double> > inputVectorToAnalyse){
  
  std::unique_ptr<TMultiGraph> multiGraph{new TMultiGraph()};
  std::vector<double> threshold =  ProcessAsciiFile::getPixelResponse(inputVectorToAnalyse, 0);
  int numberOfEvents = 1000;
  ProcessAsciiFile readAsciiFile;
  int thresholdMax = readAsciiFile.getNumberOfThresholds();
  
  for (auto numberOfPixels = 1; numberOfPixels < pixelRange; numberOfPixels++){
    std::vector<double> response = ProcessAsciiFile::getPixelResponse(inputVectorToAnalyse, numberOfPixels);
    std::transform(response.begin(), response.end(), response.begin(), std::bind2nd(std::divides<double>(), numberOfEvents));
    TF1* fit = PlotTransfertFunction::setFitErfc(0, thresholdMax);
    multiGraph->Add(PlotTransfertFunction::prepareTransfertFunctionFitted(threshold.size(), threshold[0], response[0], fit, numberOfPixels));
    m_temporalNoise.push_back(-1. * (fit->GetParameter(0) / fit->GetParameter(1)));
    m_offset.push_back(1. / (fit->GetParameter(1) * sqrt(2)));
  }
  
  multiGraph->SetTitle("Transfert function fitted");
  multiGraph->Write();
}

std::vector<double> PlotTransfertFunction::getTemporalNoise(){
  return m_temporalNoise;
}

std::vector<double> PlotTransfertFunction::getOffset(){
  return m_offset;
}
