/***********************************************************************
	This is the main program to process the CBNT Root Ntuple from
Athena with SUSYtup. See SUSYtup.h for more information.
	For version 7.0.0++ using name susy for Ntuple.
***********************************************************************/
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
#include <iomanip>
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
TApplication myapp("app",NULL,NULL); 

// Hard coded errors from Sandia article
double stat = 0.0025;
double syst_Al = 0.014;
double syst_C = 0.02;
double syst_Be = 0.016;
double syst_Mo = 0.016;
double syst_Ta = 0.012;
double syst_U = 0.014;

/////////////////////////// Functions in read_data.C ///////////////////////////
TGraphErrors read_data(string inputAngle, string inputMaterial, string inputCoeff, TGraphErrors data) ;
TGraphErrors read_data_Neubert(string inputAngle, string inputMaterial, string inputCoeff, TGraphErrors data) ;
TGraphErrors read_theo(string inputAngle, string inputMaterial, string inputCoeff, TGraphErrors data) ;
TCanvas* DrawCanvas(TGraph *graph1, TGraph *graphData, TString title, TString ylab, TString labelYrat, double minimum, double maximum) ;
////////////////////////////////////////////////////////////////////////////////

/////////////////////////// Functions in plot_Joy.C ///////////////////////////
TCanvas* plot_Joy(TGraph *graph1, TGraph *graphData) ;
TCanvas* plot_Joy_all(TGraph *graphSS, TGraph *graphGS, TGraph *graphData) ;
///////////////////////////////////////////////////////////////////////////////


int main(int argc, char **argv) { 

  cout << "-------------------------------------------" << std::endl;
  cout << " N arguments " << argc << std::endl;
  string inputMaterial=argv[1];
  std::cout << " Material: " << std::endl;
  std::cout << "   " << inputMaterial << std::endl;
  string inputAngle=argv[2];
  std::cout << " Angle: " << std::endl;
  double inputangle = std::stod(inputAngle);
  std::cout << "   " << inputangle << std::endl;
  string inputCoeff=argv[3];
  std::cout << " Plot energy or number coeff.? (e/n) " << std::endl;
  std::cout << "   " << inputCoeff << std::endl;
  string theo=argv[4];
  int displayTheory = std::stod(theo);
  std::cout << " Display theory points from paper? (0/1) " << std::endl;
  std::cout << "   " << displayTheory << std::endl;
  int displayJoy = 0;
  string joy=argv[5];
  if(joy!="") displayJoy = std::stod(joy);
  std::cout << " Display C. Joy database? (0/1) " << std::endl;
  std::cout << "   " << displayJoy << std::endl;
  int drawCanvas = 1;
  string canvas=argv[6];
  if(canvas!="") drawCanvas = std::stod(canvas);
  std::cout << " Display canvas with plot? (0/1) " << std::endl;
  std::cout << "   " << drawCanvas << std::endl;

  // Display low energy results only for Al at 0 degrees.
  if(displayJoy && ( !(inputMaterial == "Aluminium" || inputMaterial == "Al" || inputMaterial == "G4_Al") || 
    ((inputMaterial == "Aluminium" || inputMaterial == "Al" || inputMaterial == "G4_Al") && inputangle!=0) ) ){
   cout << "The low energy datasets are available only for Al at normal incidence (0 deg.) at the moment. Change the input flag." << endl;
   return 1;
  }
  
  TString sim_data_folder = "../results/";
  std::cout << "Looking for simulated results (ASCII format) into " << sim_data_folder << std::endl;

  string inputFileName;  // e.g. res_00deg_emstandatdGS.dat
  if( (inputMaterial == "Aluminium" || inputMaterial == "Al" || inputMaterial == "G4_Al") && !displayJoy) inputFileName = sim_data_folder + "G4_Al";
  if( (inputMaterial == "Aluminium" || inputMaterial == "Al" || inputMaterial == "G4_Al") && displayJoy ) inputFileName = sim_data_folder + "G4_Al.lowen";
  if(inputMaterial == "Berillium" || inputMaterial == "Be" || inputMaterial == "G4_Be") inputFileName = sim_data_folder + "G4_Be";
  if(inputMaterial == "Carbon" || inputMaterial == "C" || inputMaterial == "G4_C") inputFileName = sim_data_folder + "G4_C";
  if(inputMaterial == "Molibdenum" || inputMaterial == "Mo" || inputMaterial == "G4_Mo") inputFileName = sim_data_folder + "G4_Mo";
  if(inputMaterial == "Tantalium" || inputMaterial == "Ta" || inputMaterial == "G4_Ta") inputFileName = sim_data_folder + "G4_Ta"; 
  if(inputMaterial == "Titanium" || inputMaterial == "Ti" || inputMaterial == "G4_Ti") inputFileName = sim_data_folder + "G4_Ti";
  if(inputMaterial == "Uranium" || inputMaterial == "U" || inputMaterial == "G4_U") inputFileName = sim_data_folder + "G4_U";

  string ASCIIinputAngle;
  if (inputAngle=="0") ASCIIinputAngle="00";
  else ASCIIinputAngle=inputAngle;

  // ******* Define graphs and histos **********
  TCanvas *c = new TCanvas("c","Histogram Drawing Options",200,10,700,500);
  // simulated points GS
  TGraphErrors myplotGS;
  TString new_angle = inputAngle;
  // Be has different angles...
  if(inputMaterial == "Berillium" || inputMaterial == "Be" || inputMaterial == "G4_Be"){
    string original_angle = inputAngle;
    double d_inputAngle = stod(inputAngle);
    d_inputAngle++;
    std::ostringstream s;
    s << d_inputAngle;
    new_angle = s.str();
  }

  TString title = "Target material: " + inputMaterial + ", incident angle: " + new_angle + " deg.";
  cout << "Plotting canvas with title: " << title << endl;
  myplotGS.SetTitle(title);
  myplotGS.SetMarkerColor(3);
  myplotGS.SetLineColor(3);
  myplotGS.SetMarkerStyle(11); 
  // simulated points Opt0
  TGraphErrors myplotOpt0;
  myplotOpt0.SetMarkerColor(43);
  myplotOpt0.SetLineColor(43);
  myplotOpt0.SetMarkerStyle(11); 
  // simulated points Opt3
  TGraphErrors myplotOpt3;
  myplotOpt3.SetMarkerColor(4);
  myplotOpt3.SetLineColor(4);
  myplotOpt3.SetMarkerStyle(11); 
  // simulated points SS
  TGraphErrors myplotSS;
  myplotSS.SetName("SS");
  myplotSS.SetMarkerColor(kRed);  
  myplotSS.SetLineColor(kRed);
  myplotSS.SetMarkerStyle(11); 

  string file = "/res_" + ASCIIinputAngle + "deg_";

  // fill GS graph
  string folderGS = "/" + ASCIIinputAngle + "deg_emstandatdGS/";
  string inputFileNameGS = inputFileName + "/" + folderGS + file + "emstandatdGS.dat";
  ifstream inputGS(inputFileNameGS);
  cout << "Opening input file " << inputFileNameGS << endl;
  if (!inputGS.is_open()){cout <<"Can't open file...abort" << endl; return 1;};
  string materialGS; string energyGS_s, angleGS_s, numCoeffGS_s, numCoeffErrGS_s, enCoeffGS_s, enCoeffErrGS_s; 
  double energyGS, angleGS, numCoeffGS, numCoeffErrGS, enCoeffGS, enCoeffErrGS;
  int iGS = 0;
  while (inputGS>>materialGS>>energyGS_s>>angleGS_s>>enCoeffGS_s>>enCoeffErrGS_s>>numCoeffGS_s>>numCoeffErrGS_s){
    numCoeffGS = std::stod (numCoeffGS_s);
    if(std::isnan(numCoeffGS)) numCoeffGS = 0;
    numCoeffErrGS = std::stod (numCoeffErrGS_s);
    if(std::isnan(numCoeffErrGS)) numCoeffErrGS = 0;
    energyGS = std::stod (energyGS_s);
    angleGS = std::stod (angleGS_s);
    //cout << materialGS << " " << energyGS << " " << angleGS << " " << numCoeffGS << " " << numCoeffErrGS << endl;
    myplotGS.SetPoint(iGS,energyGS, numCoeffGS);
    myplotGS.SetPointError(iGS, 0, numCoeffErrGS);
    iGS++;
  }
  // fill SS graph
  string folderSS = "/" + ASCIIinputAngle + "deg_emstandatdSS/";
  string inputFileNameSS = inputFileName + "/" + folderSS + file + "emstandatdSS.dat";
  ifstream inputSS(inputFileNameSS);
  cout << "Opening input file " << inputFileNameSS << endl;
  if (!inputSS.is_open()){cout <<"Can't open file...abort" << endl; return 1;};
  string materialSS; string energySS_s, angleSS_s, numCoeffSS_s, numCoeffErrSS_s, enCoeffSS_s, enCoeffErrSS_s; 
  double energySS, angleSS, numCoeffSS, numCoeffErrSS, enCoeffSS, enCoeffErrSS;
  int iSS = 0;
  while (inputSS>>materialSS>>energySS_s>>angleSS_s>>enCoeffSS_s>>enCoeffErrSS_s>>numCoeffSS_s>>numCoeffErrSS_s){
    numCoeffSS = std::stod (numCoeffSS_s);
    if(std::isnan(numCoeffSS)) numCoeffSS = 0;
    numCoeffErrSS = std::stod (numCoeffErrSS_s);
    if(std::isnan(numCoeffErrSS)) numCoeffErrSS = 0;
    energySS = std::stod (energySS_s);
    angleSS = std::stod (angleSS_s);
    //cout << materialSS << " " << energySS << " " << angleSS << " " << numCoeffSS << " " << numCoeffErrSS << endl;
    myplotSS.SetPoint(iSS,energySS, numCoeffSS);
    myplotSS.SetPointError(iSS, 0, numCoeffErrSS);
    iSS++;
  }
  // fill Opt0 graph
  string folderOpt0 = "/" + ASCIIinputAngle + "deg_emstandatdOpt0/";
  string inputFileNameOpt0 = inputFileName + "/" + folderOpt0 + file + "emstandatdOpt0.dat";
  ifstream inputOpt0(inputFileNameOpt0);
  cout << "Opening input file " << inputFileNameOpt0 << endl;
  if (!inputOpt0.is_open()){cout <<"Can't open file...abort" << endl; return 1;};
  string materialOpt0; string energyOpt0_s, angleOpt0_s, numCoeffOpt0_s, numCoeffErrOpt0_s, enCoeffOpt0_s, enCoeffErrOpt0_s; 
  double energyOpt0, angleOpt0, numCoeffOpt0, numCoeffErrOpt0, enCoeffOpt0, enCoeffErrOpt0;
  int iOpt0 = 0;
  while (inputOpt0>>materialOpt0>>energyOpt0_s>>angleOpt0_s>>enCoeffOpt0_s>>enCoeffErrOpt0_s>>numCoeffOpt0_s>>numCoeffErrOpt0_s){
    numCoeffOpt0 = std::stod (numCoeffOpt0_s);
    if(std::isnan(numCoeffOpt0)) numCoeffOpt0 = 0;
    numCoeffErrOpt0 = std::stod (numCoeffErrOpt0_s);
    if(std::isnan(numCoeffErrOpt0)) numCoeffErrOpt0 = 0;
    energyOpt0 = std::stod (energyOpt0_s);
    angleOpt0 = std::stod (angleOpt0_s);
    //cout << materialOpt0 << " " << energyOpt0 << " " << angleOpt0 << " " << numCoeffOpt0 << " " << numCoeffErrOpt0 << endl;
    myplotOpt0.SetPoint(iOpt0,energyOpt0, numCoeffOpt0);
    myplotOpt0.SetPointError(iOpt0, 0, numCoeffErrOpt0);
    iOpt0++;
  }
 // fill Opt3 graph
  string folderOpt3 = "/" + ASCIIinputAngle + "deg_emstandatdOpt3/";
  string inputFileNameOpt3 = inputFileName + "/" + folderOpt3 + file + "emstandatdOpt3.dat";
  ifstream inputOpt3(inputFileNameOpt3);
  cout << "Opening input file " << inputFileNameOpt3 << endl;
  if (!inputOpt3.is_open()){cout <<"Can't open file...abort" << endl; return 1;};
  string materialOpt3; string energyOpt3_s, angleOpt3_s, numCoeffOpt3_s, numCoeffErrOpt3_s, enCoeffOpt3_s, enCoeffErrOpt3_s; 
  double energyOpt3, angleOpt3, numCoeffOpt3, numCoeffErrOpt3, enCoeffOpt3, enCoeffErrOpt3;
  int iOpt3 = 0;
  while (inputOpt3>>materialOpt3>>energyOpt3_s>>angleOpt3_s>>enCoeffOpt3_s>>enCoeffErrOpt3_s>>numCoeffOpt3_s>>numCoeffErrOpt3_s){
    numCoeffOpt3 = std::stod (numCoeffOpt3_s);
    if(std::isnan(numCoeffOpt3)) numCoeffOpt3 = 0;
    numCoeffErrOpt3 = std::stod (numCoeffErrOpt3_s);
    if(std::isnan(numCoeffErrOpt3)) numCoeffErrOpt3 = 0;
    energyOpt3 = std::stod (energyOpt3_s);
    angleOpt3 = std::stod (angleOpt3_s);
    //cout << materialOpt3 << " " << energyOpt3 << " " << angleOpt3 << " " << numCoeffOpt3 << " " << numCoeffErrOpt3 << endl;
    myplotOpt3.SetPoint(iOpt3,energyOpt3, numCoeffOpt3);
    myplotOpt3.SetPointError(iOpt3, 0, numCoeffErrOpt3);
    iOpt3++;
  }


  // Read data
  TGraphErrors data;
  data = read_data(inputAngle, inputMaterial, inputCoeff, data);
  data.SetMarkerColor(1);
  data.SetMarkerStyle(20); 
  data.SetMarkerSize(0.7); 
  data.SetName("data");
  TH1F *histodata = data.GetHistogram();
  histodata->SetMarkerColor(1);
  histodata->SetMarkerStyle(20);

  // Read theory points from article
  TGraphErrors data_theo;
  TH1F *histodata_theo;
  if(displayTheory){
  data_theo = read_theo(inputAngle, inputMaterial, inputCoeff, data_theo);
  data_theo.SetMarkerColor(1);
  data_theo.SetMarkerStyle(4); 
  data_theo.SetMarkerSize(0.7); 
  data_theo.SetName("Theory");
  histodata_theo = data_theo.GetHistogram();
  histodata_theo->SetMarkerColor(1);
  histodata_theo->SetMarkerStyle(4);
  }

  // Read data
  TGraphErrors data_Neubert;
  data_Neubert = read_data_Neubert(inputAngle, inputMaterial, inputCoeff, data_Neubert);
  data_Neubert.SetMarkerColor(1);
  data_Neubert.SetMarkerStyle(26); 
  data_Neubert.SetMarkerSize(0.7); 
  data_Neubert.SetName("data_Neubert");
  TH1F *histodata_Neubert = data_Neubert.GetHistogram();
  histodata_Neubert->SetMarkerColor(1);
  histodata_Neubert->SetMarkerStyle(26);
  

  // ******* Create the  Legend ***********
  // Auxiliary histos for legend
  TH1F *histoGS = myplotGS.GetHistogram();
  histoGS->SetMarkerColor(3);
  histoGS->SetLineColor(3);
  histoGS->SetMarkerStyle(2);
  if(inputCoeff=="e") histoGS->GetYaxis()->SetTitle("Backscattered energy coefficient");
  if(inputCoeff=="n") histoGS->GetYaxis()->SetTitle("Backscattered electron number coefficient");
  histoGS->GetXaxis()->SetTitle("Energy (MeV)");

  TH1F *histoOpt0 = myplotOpt0.GetHistogram();
  histoOpt0->SetMarkerColor(43);
  histoOpt0->SetLineColor(43);
  histoOpt0->SetMarkerStyle(2); 

  TH1F *histoOpt3 = myplotOpt3.GetHistogram();
  histoOpt3->SetMarkerColor(4);
  histoOpt3->SetLineColor(4);
  histoOpt3->SetMarkerStyle(2);
  
  TH1F *histoSS = myplotSS.GetHistogram();
  histoSS->SetMarkerColor(kRed);
  histoSS->SetLineColor(kRed);
  histoSS->SetMarkerStyle(2);

  TLegend *legend = new TLegend(0.65,0.6,0.8,0.87);
  legend->SetBorderSize(0);
  legend->SetTextFont(42);
  legend->SetTextSize(0.04);
  legend->SetFillColor(0);
  legend->SetLineColor(0); 
  legend->AddEntry(histoOpt0,"emstandard_opt0","p");
  legend->AddEntry(histoGS,"emstandardGS","p");
  legend->AddEntry(histoOpt3,"emstandard_opt3","p");
  legend->AddEntry(histoSS,"standardSS","p");
  legend->AddEntry(histodata,"Sandia data","p");
  if( (inputMaterial == "Aluminium" || inputMaterial == "Al" || inputMaterial == "G4_Al") && (inputangle==0 || inputangle==30 || inputangle==60 || inputangle==75) )legend->AddEntry(histodata_Neubert,"Neubert data","p"); 
  if(displayTheory) legend->AddEntry(histodata_theo,"Theory","p");

  c->cd();

  std::vector<double> ymax;
  ymax.push_back( histoGS->GetMaximum() ); //cout << "MaxGS " << histoGS->GetMaximum() << endl;
  ymax.push_back( histoSS->GetMaximum() ); //cout << "MaxSS " << histoSS->GetMaximum() << endl;
  ymax.push_back( histoOpt0->GetMaximum() ); //cout << "MaxOpt0 " << histoOpt0->GetMaximum() << endl;
  ymax.push_back( histoOpt3->GetMaximum() ); //cout << "MaxOpt3 " << histoOpt3->GetMaximum() << endl;
  double maximum = *std::max_element(ymax.begin(), ymax.end());
  std::vector<double> ymin;
  ymin.push_back( histoGS->GetMinimum() ); //cout << "MaxGS " << histoGS->GetMaximum() << endl;
  ymin.push_back( histoSS->GetMinimum() ); //cout << "MaxSS " << histoSS->GetMaximum() << endl;
  ymin.push_back( histoOpt0->GetMinimum() ); //cout << "MaxOpt0 " << histoOpt0->GetMaximum() << endl;
  ymin.push_back( histoOpt3->GetMinimum() ); //cout << "MaxOpt3 " << histoOpt3->GetMaximum() << endl;
  double minimum = *std::min_element(ymin.begin(), ymin.end());
  cout << "Maximum Y value: " << maximum << endl;
  cout << "Minimum Y value: " << minimum << endl;
  maximum = maximum + maximum*0.1;
  minimum = minimum - minimum*0.5;
  myplotGS.GetYaxis()->SetRangeUser(minimum,maximum);  

  if(displayJoy){
    //myplotGS.GetXaxis()->SetRangeUser(0.0001,0.07); 
    //myplotGS.Draw("ap");
    myplotSS.Draw("ap");
    myplotSS.GetYaxis()->SetRangeUser(0.02,0.2);
    myplotOpt0.Draw("psame");
    myplotOpt3.Draw("psame");
    myplotGS.Draw("psame");
    data.Draw("psame");
    if( (inputMaterial == "Aluminium" || inputMaterial == "Al" || inputMaterial == "G4_Al") && (inputangle==0 || inputangle==30 || inputangle==60 || inputangle==75) ) data_Neubert.Draw("psame");
    if(displayTheory){data_theo.Draw("psame");}
    legend->Draw(); 
  }else{
    myplotGS.Draw("ap");
    myplotSS.Draw("psame");
    myplotOpt0.Draw("psame");
    myplotOpt3.Draw("psame");
    data.Draw("psame");
    if( (inputMaterial == "Aluminium" || inputMaterial == "Al" || inputMaterial == "G4_Al") && (inputangle==0 || inputangle==30 || inputangle==60 || inputangle==75) )
    data_Neubert.Draw("psame");
    if(displayTheory){data_theo.Draw("psame");}
    legend->Draw();
  }

  TString outputFig = "plot/"+inputMaterial+inputAngle;
  cout << "Output pdf saved in: " << outputFig << endl; 

  TString myYtitle;
  if(inputCoeff=="e") myYtitle = "Backscattered energy coefficient";
  if(inputCoeff=="n") myYtitle = "Backscattered electron number coefficient";
  TCanvas *c2 = DrawCanvas(&myplotSS, &data, "SS vs data", myYtitle, "SS/data", minimum, maximum);

  if(displayJoy==0){
    c->Print(outputFig+".png","png");
    c2->Print(outputFig+"_rat.png","png");
  }

  if(displayJoy==1 && (inputMaterial == "Aluminium" || inputMaterial == "Al" || inputMaterial == "G4_Al") && inputangle==0){
    cout << " " << endl;
    cout << " *** Joy database *** " << endl;
    TCanvas *c3 = plot_Joy_all(&myplotSS, &myplotGS, &data);
    c3->Print(outputFig+"_lowEn.png","png");
  }
  

  if(drawCanvas) myapp.Run(true);

  return 0;

}


