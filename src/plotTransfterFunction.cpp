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
  m_numberOfEvents = 1;
  m_fitStart = 0.;
  m_fitEnd = 255.;
  m_conversionFactor = 1.0;
}

PlotTransfertFunction::~PlotTransfertFunction(){
  // Destructor
}

TF1* PlotTransfertFunction::SetFitErfc(double threshold_min, double threshold_max){
  // Create a TF1 containing the complementary error function and set some parameters
  
  std::unique_ptr<TF1> fitErfc{new TF1("fitErfc","(0.5 - 0.5 *  TMath::Erf(( [1]*x+[0]) ))",threshold_min, threshold_max)};
  fitErfc->SetParameter(0, -1.0);
  fitErfc->SetParameter(1, 0.5);
  return fitErfc.release();
}

TGraph* PlotTransfertFunction::PrepareTransfertFunctionFitted(int numberOfEntries, double &variable1, double &variable2, TF1 *fitToUse, int markerColor){
  // Create and return a TGraph from variable1 and variable2 and fit it according to fitToUse
  
  std::unique_ptr<TGraph> graph{new TGraph(numberOfEntries, &variable1, &variable2)};
  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(0.5);
  graph->SetMarkerColor(markerColor);
  fitToUse->SetLineColor(markerColor);
  graph->Fit(fitToUse, "IR");
  return graph.release();
}

void PlotTransfertFunction::PlotHistogram(std::string title, std::string histoTitle, int bin, int rangeMin, int rangeMax, std::vector<double> parameterToPlot){
  // Plot an hitogramm from a vector of data set
  
  std::unique_ptr<TCanvas> canvas (new TCanvas("canvas", "canvas", 1200, 1100));
  gStyle->SetOptStat(1111);
  gStyle->SetOptFit(1011);
  auto histoToPlot = new TH1F(title.c_str(), histoTitle.c_str(), bin, rangeMin, rangeMax);
  for(auto iterator : parameterToPlot){
    histoToPlot->Fill(iterator);
  }
  std::string xTitle = "Threhold [" + m_units + "]";
  std::string yTitle = "Entries";
  histoToPlot->Fit("gaus");
  histoToPlot->GetXaxis()->SetTitle(xTitle.c_str());
  histoToPlot->GetYaxis()->SetTitle(yTitle.c_str());
  histoToPlot->Write();
}

void PlotTransfertFunction::PlotTransfert(int pixelRange, std::vector<std::vector<double> > inputVectorToAnalyse){
  
  std::unique_ptr<TMultiGraph> multiGraph{new TMultiGraph()};
  std::vector<double> threshold =  ProcessAsciiFile::GetPixelResponse(inputVectorToAnalyse, 0);
  ProcessAsciiFile readAsciiFile;
  //int thresholdMax = readAsciiFile.GetNumberOfThresholds();
  
  for (auto numberOfPixels = 1; numberOfPixels < pixelRange; numberOfPixels++){
    std::vector<double> response = ProcessAsciiFile::GetPixelResponse(inputVectorToAnalyse, numberOfPixels);
    std::transform(response.begin(), response.end(), response.begin(), std::bind2nd(std::divides<double>(), m_numberOfEvents));
    TF1* fit = PlotTransfertFunction::SetFitErfc(m_fitStart, m_fitEnd);
    multiGraph->Add(PlotTransfertFunction::PrepareTransfertFunctionFitted(threshold.size(), threshold[0], response[0], fit, numberOfPixels));
//    m_temporalNoise.push_back(1. / (fit->GetParameter(1) * sqrt(2))); // CVF * 1. / (fit->GetParameter(1) * sqrt(2))
//    m_offset.push_back(-1. * (fit->GetParameter(0) / fit->GetParameter(1))); // -1. * CVF * (fit->GetParameter(0) / fit->GetParameter(1))
    m_temporalNoise.push_back( (m_conversionFactor *  1.) / (fit->GetParameter(1) * sqrt(2)));
    m_offset.push_back( (-1. * m_conversionFactor) * (fit->GetParameter(0) / fit->GetParameter(1))); 
  }
  std::string title = "Transfer function fitted; threshold [" + m_units + "]; Entries";
  multiGraph->SetTitle(title.c_str());
//  multiGraph->Draw("AP")
  multiGraph->Write();
}

std::vector<double> PlotTransfertFunction::GetTemporalNoise(){
  return m_temporalNoise;
}

std::vector<double> PlotTransfertFunction::GetOffset(){
  return m_offset;
}

void PlotTransfertFunction::SetNumberOfEvents(int numberOfEvents){
  m_numberOfEvents = numberOfEvents;  
}

void PlotTransfertFunction::SetFitRange(double fitStart, double fitEnd){
  m_fitStart = fitStart;
  m_fitEnd = fitEnd;  
}

void PlotTransfertFunction::SetConversionFactor(double conversionFactor){
  m_conversionFactor = conversionFactor;
}

void PlotTransfertFunction::SetUnits(std::string units){
  m_units = units;
}
