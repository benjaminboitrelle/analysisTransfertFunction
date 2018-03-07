//
//  plotTransfterFunction.cpp
//  analysisTransferFunction
//
//  Created by Benjamin BOITRELLE on 09/11/2017.
//

#include "plotTransfterFunction.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <functional>

#include "TStyle.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"

#include "processAsciiFile.hpp"


PlotTransferFunction::PlotTransferFunction() {
  // Constructor
  m_numberOfEvents = 1;
  m_fitStart = 0.;
  m_fitEnd = 255.;
  m_conversionFactor = 1.0;
  m_column = 128;
  m_row = 128;
}

PlotTransferFunction::~PlotTransferFunction() {
  // Destructor
}

TF1* PlotTransferFunction::SetFitErfc(double threshold_min, double threshold_max) {
  // Create a TF1 containing the complementary error function and set some parameters

  std::unique_ptr<TF1> fitErfc{new TF1("fitErfc", "(0.5 - 0.5 *  TMath::Erf(( [1]*x+[0]) ))", threshold_min, threshold_max)};
  fitErfc->SetParameter(0, -1.0);
  fitErfc->SetParameter(1, 0.5);
  return fitErfc.release();
}

TGraph* PlotTransferFunction::PrepareTransferFunctionFitted(int numberOfEntries, double &variable1, double &variable2, TF1 *fitToUse, int markerColor) {
  // Create and return a TGraph from variable1 and variable2 and fit it according to fitToUse

  std::unique_ptr<TGraph> graph{new TGraph(numberOfEntries, &variable1, &variable2)};
  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(0.5);
  graph->SetMarkerColor(markerColor);
  fitToUse->SetLineColor(markerColor);
  graph->Fit(fitToUse, "IR");
  return graph.release();
}

void PlotTransferFunction::PlotHistogram(std::string title, std::string histoTitle, int bin, int rangeMin, int rangeMax, std::vector<double> parameterToPlot) {
  // Plot an hitogramm from a vector of data set

  std::unique_ptr<TCanvas> canvas (new TCanvas("canvas", "canvas", 1200, 1100));
  gStyle->SetOptStat(1111);
  gStyle->SetOptFit(1011);
  auto histoToPlot = new TH1F(title.c_str(), histoTitle.c_str(), bin, rangeMin, rangeMax);
  for (auto iterator : parameterToPlot) {
    histoToPlot->Fill(iterator);
  }
  std::string xTitle = "Threhold [" + m_units + "]";
  std::string yTitle = "Entries";
  histoToPlot->Fit("gaus");
  histoToPlot->GetXaxis()->SetTitle(xTitle.c_str());
  histoToPlot->GetYaxis()->SetTitle(yTitle.c_str());
  histoToPlot->Write();
}

void PlotTransferFunction::PlotTransfer(int pixelRange, std::vector<std::vector<double> > inputVectorToAnalyse) {
  std::unique_ptr<TMultiGraph> multiGraph {new TMultiGraph()};
  std::vector<double> threshold =  ProcessAsciiFile::GetPixelResponse(inputVectorToAnalyse, 0);
  ProcessAsciiFile readAsciiFile;
  for (auto numberOfPixels = 1; numberOfPixels < pixelRange; numberOfPixels++) {
    std::vector<double> response = ProcessAsciiFile::GetPixelResponse(inputVectorToAnalyse, numberOfPixels);
    std::transform(response.begin(), response.end(), response.begin(), std::bind2nd(std::divides<double>(), m_numberOfEvents));
    TF1* fit = PlotTransferFunction::SetFitErfc(m_fitStart, m_fitEnd);
    multiGraph->Add(PlotTransferFunction::PrepareTransferFunctionFitted(threshold.size(), threshold[0], response[0], fit, numberOfPixels));
    m_temporalNoise.push_back((m_conversionFactor *  1.) / (fit->GetParameter(1) * sqrt(2)));
    m_offset.push_back((-1. * m_conversionFactor) * (fit->GetParameter(0) / fit->GetParameter(1)));
  }
  std::string title = "Transfer function fitted; threshold [" + m_units + "]; Entries";
  multiGraph->SetTitle(title.c_str());
  multiGraph->Write();
}

void PlotTransferFunction::PlotBadPixelsMask(std::string title, std::string histoTitle, int binColumn, int columnMin, int columnMax, int binRow, int rowMin, int rowMax, std::vector<std::vector<double> > input, int threshold) {
  // For a defined threshold, plot response of each pixels on a 2D matrix
  
  std::unique_ptr<TCanvas> canvas (new TCanvas("canvas", "canvas", 1200, 1000));
  TH2F *histoPixelsMask = new TH2F(title.c_str(), histoTitle.c_str(), binColumn, columnMin, columnMax, binRow, rowMin, rowMax);
  int column(columnMin), row(rowMin);
  for (unsigned int iteratorPixel = 1; iteratorPixel < input.at(threshold).size(); iteratorPixel++){
    histoPixelsMask->Fill(column, row, input[threshold][iteratorPixel]/m_numberOfEvents);
    (iteratorPixel % m_column != (unsigned int)columnMin)? column++ : (column = columnMin, row++);
    if (row == m_row) row = rowMin;
  }
  std::string xTitle = "Columns";
  std::string yTitle = "Rows";
  std::string zTitle = "Normalised response";
  std::string savePlot = m_outputDirectory + "/badPixelsMask_threshold_" + std::to_string(threshold) + ".png";
  histoPixelsMask->GetXaxis()->SetTitle(xTitle.c_str());
  histoPixelsMask->GetYaxis()->SetTitle(yTitle.c_str());
  histoPixelsMask->GetZaxis()->SetTitle(zTitle.c_str());
  gStyle->SetOptStat(kFALSE);
  gStyle->SetPalette(kRainBow);
  histoPixelsMask->Draw("COLZ");
  histoPixelsMask->Write();
  canvas->Update();
  canvas->SaveAs(savePlot.c_str());
}

std::vector<double> PlotTransferFunction::GetTemporalNoise() {
  return m_temporalNoise;
}

std::vector<double> PlotTransferFunction::GetOffset() {
  return m_offset;
}

void PlotTransferFunction::SetNumberOfEvents(int numberOfEvents) {
  m_numberOfEvents = numberOfEvents;
}

void PlotTransferFunction::SetFitRange(double fitStart, double fitEnd) {
  m_fitStart = fitStart;
  m_fitEnd = fitEnd;
}

void PlotTransferFunction::SetConversionFactor(double conversionFactor) {
  m_conversionFactor = conversionFactor;
}

void PlotTransferFunction::SetUnits(std::string units) {
  m_units = units;
}

void PlotTransferFunction::SetNumberOfRows(int row) {
  m_row = row;
}

void PlotTransferFunction::SetNumberOfColumns(int column){
  m_column = column;
}

void PlotTransferFunction::SetOutputDirectory(std::string outputDirectory){
  m_outputDirectory = outputDirectory;
}
