#include "TROOT.h" 
#include "TTree.h" 
#include "TCanvas.h" 
#include "TChain.h" 
#include "TBranch.h" 
#include <TFile.h>
#include "TH1F.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#include "TLegend.h"
#include "TLatex.h"
#include "TApplication.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLine.h"

using namespace std;

TCanvas* plot_Joy(TGraph *graph1, TGraph *graphData){

  string inputFileName = "exp_data/database_joy.txt";
  ifstream input(inputFileName); 
  int numberOfDatasets = 0;
  input >> numberOfDatasets;
  
  cout << "Reading C. Joy database, actually only for Al at normal incidence" << endl;
  cout << "In the file there are " << numberOfDatasets << " datasets." << endl;

  TCanvas *c3 = new TCanvas("c3","c3",200,10,700,500);
  c3->cd();

  TGraphErrors *myplotJ[numberOfDatasets];
  TString title = "Aluminium, low energy region";
  vector <TString> authors;

  for(int y=0; y<numberOfDatasets; y++){
    myplotJ[y] = new TGraphErrors();
    myplotJ[y]->SetTitle(title);
    myplotJ[y]->SetMarkerStyle(20); 
    myplotJ[y]->SetMarkerSize(0.7);
  }

  TLegend *legend = new TLegend(0.63,0.6,0.86,0.92);
  legend->SetBorderSize(0);
  legend->SetTextFont(42);
  legend->SetTextSize(0.03);
  legend->SetFillColor(0);
  legend->SetLineColor(0); 
  
  int mycolors[14] = {2, 3, 4, 42, 6, 7, 8, 33, 13, 41, 46, 30, 28};

  double energy, yield;
  int datasetNum = 0;
  int pointNum = 0;
  double percent_error = 0;
  while(input>>energy>>yield){
    if(energy==999){
      cout << "Database #" << yield << endl; 
      datasetNum++; 
      pointNum=0; 
      input>>percent_error;
      TString name;
      input>>name;
      authors.push_back(name);
      continue;
    }
    energy = energy/1000;
    cout << "Point: " << pointNum << ",  " << energy << ",  " << yield << endl;  
    int datasetNum_forVect = datasetNum-1;
    double total_error = percent_error * yield / 100;
    myplotJ[datasetNum_forVect]->SetPoint(pointNum,energy,yield);
    myplotJ[datasetNum_forVect]->SetPointError(pointNum,0,total_error);
    if(datasetNum<14){
      myplotJ[datasetNum_forVect]->SetMarkerColor( mycolors[datasetNum] );
      myplotJ[datasetNum_forVect]->SetLineColor( mycolors[datasetNum] );
    }else{
      myplotJ[datasetNum_forVect]->SetMarkerColor( 20+2*datasetNum );
      myplotJ[datasetNum_forVect]->SetLineColor( 20+2*datasetNum );   
    }
    /*
    if(datasetNum_forVect==6){
      myplotJ[datasetNum_forVect]->SetMarkerColor(kBlack);
      myplotJ[datasetNum_forVect]->SetMarkerStyle(26);
      myplotJ[datasetNum_forVect]->SetLineColor(kBlack);
    }
    */
    pointNum++;
  
  }

  c3->cd();

  TH1F *histoJ = myplotJ[0]->GetHistogram();
  histoJ->SetMarkerSize(0.7);
  histoJ->SetMarkerStyle(20); 
  histoJ->SetMarkerColor(myplotJ[0]->GetMarkerColor()); 

  cout << "Draw graphs..." << endl;
  TString mylegend = "Dataset #0";
  //legend->AddEntry(histoJ,mylegend,"p");

  //myplotJ[0]->GetXaxis()->SetRangeUser(0,70); 
  //myplotJ[0]->GetYaxis()->SetRangeUser(0.11,0.26); 

  //myplotJ[0]->Draw("apl");
  
  graph1->SetMarkerColor(kRed);
  graph1->SetLineColor(kRed);
  graph1->SetLineWidth(2);
  graphData->SetTitle(title);
  graphData->GetYaxis()->SetTitle("Electron backscattering coefficient");
  graphData->GetXaxis()->SetTitle("Energy (MeV)");

  //gPad->SetLogx();
  graphData->GetXaxis()->SetRangeUser(0.0001,0.07);
  graphData->GetYaxis()->SetRangeUser(0.11,0.26);

  graphData->Draw("ape");
  graph1->Draw("pe,same");
  myplotJ[0]->Draw("plsame");

  TH1F *histoGraph1 = graph1->GetHistogram();
  histoGraph1->SetMarkerColor(graph1->GetMarkerColor()); 
  TH1F *histoGraphData = graphData->GetHistogram();
  histoGraphData->SetMarkerColor(graphData->GetMarkerColor()); 
  legend->AddEntry(histoGraph1,"Geant4 Single Scattering","p");
  legend->AddEntry(histoGraphData,"#1 Sandia Lab. (1980)","pl");

  //legend->AddEntry(histoJ,mylegend,"p");

  TH1F *myHisto[numberOfDatasets];

  for(int y=0; y<numberOfDatasets; y++){
    myplotJ[y]->Draw("plsame");
    myHisto[y] = new TH1F();
    myHisto[y] = myplotJ[y]->GetHistogram();
    //TString mylegend = "Dataset #"+std::to_string(y);
    mylegend = "#" + std::to_string(y+2) + " " + authors.at(y);
    //cout << "Building legend..." << mylegend << endl;
    myHisto[y]->SetMarkerSize(0.7);
    myHisto[y]->SetMarkerStyle(20); 
    myHisto[y]->SetMarkerColor(myplotJ[y]->GetMarkerColor());
    myHisto[y]->SetLineColor(myplotJ[y]->GetMarkerColor());
    legend->AddEntry(myHisto[y],mylegend,"pl");
  }
  cout << "Draw legend..." << endl;
  legend->Draw();
  
  input.close();

  c3->Update();
  c3->Modified();

  return c3;
}


///***************************************************************************////

TCanvas* plot_Joy_all(TGraph *graphSS, TGraph *graphGS, TGraph *graphData){

  string exp_data_folder = "../data/";
  std::cout << "Reading experimental data from " << exp_data_folder << std::endl; 

  string inputFileName = exp_data_folder + "database_joy.txt";
  ifstream input(inputFileName); 
  int numberOfDatasets = 0;
  input >> numberOfDatasets;
  
  cout << "Reading C. Joy database, actually only for Al at normal incidence" << endl;
  cout << "In the file there are " << numberOfDatasets << " datasets." << endl;

  TCanvas *c3 = new TCanvas("c3","c3",200,10,700,500);
  c3->cd();

  TGraphErrors *myplotJ[numberOfDatasets];
  TString title = "Aluminium, low energy region";
  vector <TString> authors;

  for(int y=0; y<numberOfDatasets; y++){
    myplotJ[y] = new TGraphErrors();
    myplotJ[y]->SetTitle(title);
    myplotJ[y]->SetMarkerStyle(20); 
    myplotJ[y]->SetMarkerSize(0.7);
  }

  TLegend *legend = new TLegend(0.54,0.4,0.8,0.87);
  legend->SetBorderSize(0);
  legend->SetTextFont(42);
  legend->SetTextSize(0.03);
  legend->SetFillColor(0);
  legend->SetLineColor(0); 
  
  int mycolors[14] = {2, 3, 4, 42, 6, 7, 8, 33, 13, 41, 46, 30, 28};

  double energy, yield;
  int datasetNum = 0;
  int pointNum = 0;
  double percent_error = 0;
  while(input>>energy>>yield){
    if(energy==999){
      cout << "Database #" << yield << endl; 
      datasetNum++; 
      pointNum=0; 
      input>>percent_error;
      TString name;
      input>>name;
      authors.push_back(name);
      continue;
    }
    energy = energy/1000;
    //cout << "Point: " << pointNum << ",  " << energy << ",  " << yield << endl;  
    int datasetNum_forVect = datasetNum-1;
    double total_error = percent_error * yield / 100;
    myplotJ[datasetNum_forVect]->SetPoint(pointNum,energy,yield);
    myplotJ[datasetNum_forVect]->SetPointError(pointNum,0,total_error);
    if(datasetNum<14){
      myplotJ[datasetNum_forVect]->SetMarkerColor( mycolors[datasetNum] );
      myplotJ[datasetNum_forVect]->SetLineColor( mycolors[datasetNum] );
    }else{
      myplotJ[datasetNum_forVect]->SetMarkerColor( 20+2*datasetNum );
      myplotJ[datasetNum_forVect]->SetLineColor( 20+2*datasetNum );   
    }
    pointNum++;
  
  }

  c3->cd();

  TH1F *histoJ = myplotJ[0]->GetHistogram();
  histoJ->SetMarkerSize(0.7);
  histoJ->SetMarkerStyle(20); 
  histoJ->SetMarkerColor(myplotJ[0]->GetMarkerColor()); 

  cout << "Draw graphs..." << endl;
  TString mylegend = "Dataset #0";
  
  graphSS->SetMarkerColor(kRed);
  graphSS->SetLineColor(kRed);
  graphSS->SetLineWidth(2);
  
  graphGS->SetMarkerColor(kGreen);
  graphGS->SetLineColor(kGreen);
  graphGS->SetLineWidth(2);  
  
  graphData->SetTitle(title);
  graphData->GetYaxis()->SetTitle("Electron backscattering coefficient");
  graphData->GetXaxis()->SetTitle("Energy (MeV)");

  //gPad->SetLogx();
  graphData->GetXaxis()->SetRangeUser(0.0001,0.07);
  graphData->GetYaxis()->SetRangeUser(0.11,0.26);

  graphData->Draw("ape");
  graphSS->Draw("pe,same");
  graphGS->Draw("pe,same");
  myplotJ[0]->Draw("plsame");

  TH1F *histoGraphSS = graphSS->GetHistogram();
  histoGraphSS->SetMarkerColor(graphSS->GetMarkerColor()); 
  TH1F *histoGraphGS = graphGS->GetHistogram();
  histoGraphGS->SetMarkerColor(graphGS->GetMarkerColor());   
  TH1F *histoGraphData = graphData->GetHistogram();
  histoGraphData->SetMarkerColor(graphData->GetMarkerColor()); 
  legend->AddEntry(histoGraphSS,"Geant4 emstandardSS","p");
  legend->AddEntry(histoGraphGS,"Geant4 emstandardGS","p");
  legend->AddEntry(histoGraphData,"#1 Sandia Lab. (1980)","pl");

  TH1F *myHisto[numberOfDatasets];

  for(int y=0; y<numberOfDatasets; y++){
    myplotJ[y]->Draw("plsame");
    myHisto[y] = new TH1F();
    myHisto[y] = myplotJ[y]->GetHistogram();
    mylegend = "#" + std::to_string(y+2) + " " + authors.at(y);
    myHisto[y]->SetMarkerSize(0.7);
    myHisto[y]->SetMarkerStyle(20); 
    myHisto[y]->SetMarkerColor(myplotJ[y]->GetMarkerColor());
    myHisto[y]->SetLineColor(myplotJ[y]->GetMarkerColor());
    legend->AddEntry(myHisto[y],mylegend,"pl");
  }
  cout << "Draw legend..." << endl;
  legend->Draw();
  
  input.close();

  c3->Update();
  c3->Modified();

  return c3;
}
