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

// Hard coded errors
extern double stat;
extern double syst_Al;
extern double syst_C;
extern double syst_Be;
extern double syst_Mo;
extern double syst_Ta;
extern double syst_U;


///////////////////////////////////////////////////
// Read data points from file

TGraphErrors read_data(std::string inputAngle, std::string inputMaterial, std::string inputCoeff, TGraphErrors data){

  typedef vector<vector<string> > Rows;
  Rows rows;
  TString inputFileName;
  double syst_error = 0;

  TString exp_data_folder = "../data/";
  std::cout << "Reading experimental data from " << exp_data_folder << std::endl; 

  if(inputMaterial == "Aluminium" || inputMaterial == "Al" || inputMaterial == "G4_Al"){ inputFileName = exp_data_folder + "Al_"; syst_error = syst_Al; }
  if(inputMaterial == "Berillium" || inputMaterial == "Be" || inputMaterial == "G4_Be"){ inputFileName = exp_data_folder + "Be_"; syst_error = syst_Be; }
  if(inputMaterial == "Carbon" || inputMaterial == "C" || inputMaterial == "G4_C"){ inputFileName = exp_data_folder + "C_"; syst_error = syst_C; }
  if(inputMaterial == "Molibdenum" || inputMaterial == "Mo" || inputMaterial == "G4_Mo"){ inputFileName = exp_data_folder + "Mo_"; syst_error = syst_Mo; }
  if(inputMaterial == "Tantalium" || inputMaterial == "Ta" || inputMaterial == "G4_Ta"){ inputFileName = exp_data_folder + "Ta_"; syst_error = syst_Ta; }
  if(inputMaterial == "Titanium" || inputMaterial == "Ti" || inputMaterial == "G4_Ti"){ inputFileName = exp_data_folder + "Ti_"; syst_error = 0.01; }
  if(inputMaterial == "Uranium" || inputMaterial == "U" || inputMaterial == "G4_U"){ inputFileName = exp_data_folder + "U_"; syst_error = syst_U; }

  double tot_error = syst_error + stat;

  if(inputCoeff == "e") inputFileName = inputFileName + "energy.tsv";
  if(inputCoeff == "n") inputFileName = inputFileName + "number.tsv";

  cout << "Opening data file: " << inputFileName << endl;

  // Be has different angles...
  if(inputMaterial == "Berillium" || inputMaterial == "Be" || inputMaterial == "G4_Be"){
    string original_angle = inputAngle;
    double d_inputAngle = stod(inputAngle);
    d_inputAngle++;
    std::ostringstream s;
    s << d_inputAngle;
    inputAngle =  s.str();
    cout << "For Be change the input angle: " << original_angle << " --> " << inputAngle << endl;
  }

  ifstream input(inputFileName);
  char const row_delim = '\n';
  char const field_delim = '\t';
  std::vector<double> myValue;
  std::vector<double> myEnergy;
  int row_num = 0;
  int field_num = 0;
  int myColumn = -1;
  std::vector<string> angles; 
  for (string row; getline(input, row, row_delim); ) {
    rows.push_back(Rows::value_type());
    istringstream ss(row);
    row_num++;
    bool isExp = false;
    for (string field; getline(ss, field, field_delim); ) {
      if(row_num == 1){
        angles.push_back(field);
        if(field==inputAngle){
          myColumn = field_num;
          cout << "Look column " << field_num << endl;
        }
      }else{
         // escape in case no angle match
         if(myColumn==-1){ 
           cout << endl;
           cout << endl;
           cout << "------> Invalid angle selected. Available angles:" << endl;
           for(std::vector<double>::size_type i = 0; i != angles.size(); i++){ cout << angles.at(i) << ", ";} 
           cout << endl;
           cout << endl;
           return 0;
         }
        // if you found the angle, continue 
        rows.back().push_back(field);
        if(field_num == 0 && (field=="Exp" || field=="Exp ")) isExp = true;
        if(field_num == 1 && isExp) myEnergy.push_back(std::stod(field));  
        if(field_num == myColumn && isExp) myValue.push_back(std::stod(field)); 
      }
      field_num++;  
    }
    // reset the flags
    field_num = 0;
    isExp = false; 
  }


  cout << "Reading data from .tsv: " << endl;
  for(std::vector<double>::size_type i = 0; i != myValue.size(); i++){
    data.SetPoint(i,myEnergy.at(i), myValue.at(i));
    double error = myValue.at(i)*tot_error;
    data.SetPointError(i, 0, error);
    cout << myEnergy.at(i) << " " << myValue.at(i) << "+-" << myValue.at(i)*tot_error << endl;
  }
  cout << endl;

  return data;
}




///////////////////////////////////////////////////////////////////////

// Same function for theory points

TGraphErrors read_theo(std::string inputAngle, std::string inputMaterial, std::string inputCoeff, TGraphErrors data){

  typedef vector<vector<string> > Rows;
  Rows rows;
  TString inputFileName;

  TString exp_data_folder = "../data/";
  std::cout << "Reading theoretical data from literature in folder " << exp_data_folder << std::endl; 

  double syst_error = 0;
  if(inputMaterial == "Aluminium" || inputMaterial == "Al" || inputMaterial == "G4_Al"){ inputFileName = exp_data_folder + "Al_"; syst_error = syst_Al; }
  if(inputMaterial == "Berillium" || inputMaterial == "Be" || inputMaterial == "G4_Be"){ inputFileName = exp_data_folder + "Be_"; syst_error = syst_Be; }
  if(inputMaterial == "Carbon" || inputMaterial == "C" || inputMaterial == "G4_C"){ inputFileName = exp_data_folder + "C_"; syst_error = syst_C; }
  if(inputMaterial == "Molibdenum" || inputMaterial == "Mo" || inputMaterial == "G4_Mo"){ inputFileName = exp_data_folder + "Mo_"; syst_error = syst_Mo; }
  if(inputMaterial == "Tantalium" || inputMaterial == "Ta" || inputMaterial == "G4_Ta"){ inputFileName = exp_data_folder + "Ta_"; syst_error = syst_Ta; }
  if(inputMaterial == "Titanium" || inputMaterial == "Ti" || inputMaterial == "G4_Ti"){ inputFileName = exp_data_folder + "Ti_"; syst_error = 0.01; }
  if(inputMaterial == "Uranium" || inputMaterial == "U" || inputMaterial == "G4_U"){ inputFileName = exp_data_folder + "U_"; syst_error = syst_U; }

  double tot_error = syst_error + stat;

  if(inputCoeff == "e") inputFileName = inputFileName + "energy.tsv";
  if(inputCoeff == "n") inputFileName = inputFileName + "number.tsv";

  cout << "Opening data file: " << inputFileName << endl;

  // Be has different angles...
  if(inputMaterial == "Berillium" || inputMaterial == "Be" || inputMaterial == "G4_Be"){
    string original_angle = inputAngle;
    double d_inputAngle = stod(inputAngle);
    d_inputAngle++;
    std::ostringstream s;
    s << d_inputAngle;
    inputAngle =  s.str();
    cout << "For Be change the input angle: " << original_angle << " --> " << inputAngle << endl;
  }

  ifstream input(inputFileName);
  char const row_delim = '\n';
  char const field_delim = '\t';
  std::vector<double> myValue;
  std::vector<double> myEnergy;
  int row_num = 0;
  int field_num = 0;
  int myColumn = -1;
  std::vector<string> angles; 
  for (string row; getline(input, row, row_delim); ) {
    rows.push_back(Rows::value_type());
    istringstream ss(row);
    row_num++;
    bool isExp = false;
    for (string field; getline(ss, field, field_delim); ) {
      if(row_num == 1){
        angles.push_back(field);
        if(field==inputAngle){
          myColumn = field_num;
          cout << "Look column " << field_num << endl;
        }
      }else{
         // escape in case no angle match
         if(myColumn==-1){ 
           cout << endl;
           cout << endl;
           cout << "------> Invalid angle selected. Available angles:" << endl;
           for(std::vector<double>::size_type i = 0; i != angles.size(); i++){ cout << angles.at(i) << ", ";} 
           cout << endl;
           cout << endl;
           return 0;
         }
        // if you found the angle, continue 
        rows.back().push_back(field);
        if(field_num == 0 && (field=="Theory" || field=="Theory ")) isExp = true;
        if(field_num == 1 && isExp) myEnergy.push_back(std::stod(field));  
        if(field_num == myColumn && isExp) myValue.push_back(std::stod(field)); 
      }
      field_num++;  
    }
    // reset the flags
    field_num = 0;
    isExp = false; 
  }


  cout << "Reading theory from .tsv: " << endl;
  for(std::vector<double>::size_type i = 0; i != myValue.size(); i++){
    data.SetPoint(i,myEnergy.at(i), myValue.at(i));
    double error = myValue.at(i)*tot_error;
    data.SetPointError(i, 0, 0);
    cout << myEnergy.at(i) << " " << myValue.at(i) << endl;
  }
  cout << endl;

  return data;
}


///////////////////////////////////////////
///////// Draw canvas

Double_t  erat(Double_t a, Double_t b, Double_t ea, Double_t eb) {
       Double_t e2=pow(ea,2)/pow(b,2)+pow(a,2)*pow(eb,2)/pow(b,4);
       return sqrt(e2);
}


TCanvas* DrawCanvas(TGraph *graph1, TGraph *graphData, TString title, TString ylab, TString labelYrat, double minimum, double maximum){

  std::cout << "Draw canvas..." << std::endl;

  TCanvas *c = new TCanvas("crat","Plot ratio",200,10,800,600); 
  c->SetFillColor(kWhite);  
  TPad *pad1 = new TPad("pad1","pad1",0.00,0.30,1.0,1.0,21);  
  TPad *pad2 = new TPad("pad2","pad2",0.00,0.00,1.0,0.3,21); 
  pad1->SetFillColor(kWhite);
  pad2->SetFillColor(kWhite); 
  pad1->SetRightMargin(0.1);
  pad2->SetRightMargin(0.1);
  pad1->SetBottomMargin(0.01);
  pad2->SetTopMargin(0.05);
  pad2->SetBottomMargin(0.3);
  pad1->Draw();  
  pad2->Draw();  

  pad1->cd();
  TLegend *legend = new TLegend(0.65,0.72,0.8,0.87);
  legend->SetBorderSize(0);
  legend->SetTextFont(42);
  legend->SetTextSize(0.04);
  legend->SetFillColor(0);
  legend->SetLineColor(0);
  TH1F *hgraph1 = graph1->GetHistogram(); 
  legend->AddEntry(hgraph1,"emstandardSS","pe");
  legend->AddEntry(graphData,"Sandia80 data","pe");
  graphData->GetYaxis()->SetRangeUser(minimum,maximum); 
  graphData->SetTitle(title);
  graphData->GetYaxis()->SetTitle(ylab);
  graphData->Draw("ape");
  graph1->SetTitle(title);
  graph1->Draw("pe,same");
  legend->Draw();

  pad2->cd();

  int nbins = graph1->GetN();
  double miny = 0.7;
  double maxy = 1.3;
  
  TGraphErrors *myplotRat = new TGraphErrors();
  //myplotRat->SetName("Ratio");
  //myplotRat->SetTitle(title);
  //myplotRat->SetMarkerColor(1);
  //myplotRat->SetLineColor(1);
  //myplotRat->SetMarkerStyle(11); 
  myplotRat->SetMinimum(miny);
  myplotRat->SetMaximum(maxy);

  for (int i=0;i<nbins;i++){
    double x1, y1, x2, y2;
    double x1temp, y1temp;
    graphData->GetPoint(i, x2, y2);
    for (int j=0;j<nbins;j++){
      graph1->GetPoint(j, x1temp, y1temp);
      if(x1temp==x2) break;
    }
    x1 = x1temp;
    y1 = y1temp;  
    double ey1 = graph1->GetErrorY(i);
    double ey2 = graphData->GetErrorY(i);
    double error=erat(y2, y1, ey2, ey1);
    double value = y2/y1;

    myplotRat->SetPoint(i, x1, value);
    myplotRat->SetPointError(i, 0, error);
    //cout << "Point " << i << ", energy: " << x1 << ", ratio value: " << value << " +- " << error << endl; 
  }

  TH1F *histoRat = myplotRat->GetHistogram();
  double xmax = histoRat->GetXaxis()->GetXmax();
  //histoRat->GetYaxis()->SetTitleOffset(1.3);
  histoRat->GetXaxis()->SetTitleSize(0.12);
  histoRat->GetXaxis()->SetLabelSize(0.09);
  histoRat->GetYaxis()->SetTitleSize(0.1);
  histoRat->GetYaxis()->SetLabelSize(0.08);
  TString titlex = "Incident energy [MeV]";
  histoRat->GetXaxis()->SetTitle(titlex); 

  histoRat->GetYaxis()->SetTitle(labelYrat);
  histoRat->GetYaxis()->SetTitleOffset(0.4);
  histoRat->GetYaxis()->SetTitleSize(0.09);
  histoRat->GetYaxis()->SetTitle(labelYrat);

  TLine *line = new TLine(0,1,xmax,1);
  line->SetLineColor(kBlack);
  TLine *line2 = new TLine(0,1.1,xmax,1.1);
  line2->SetLineColor(kBlack);
  line2->SetLineStyle(3);
  TLine *line3 = new TLine(0,0.9,xmax,0.9);
  line3->SetLineColor(kBlack);
  line3->SetLineStyle(3);

  myplotRat->SetMarkerColor(kRed);
  myplotRat->SetLineColor(kRed);
  myplotRat->GetYaxis()->SetTitle(labelYrat); 
  myplotRat->Draw("ape");
  line->Draw();
  line2->Draw("same");
  line3->Draw("same");
  myplotRat->Draw("pe,same");
  //graphData->Draw("ape");
  //myplotRat.Draw("pe,same");

  c->Update();
  c->Modified();

  return c;
}

///////////////////////////////////////////////////////////////////////////////

///////////////// Read Neubert data ///////////////////////

TGraphErrors read_data_Neubert(std::string inputAngle, std::string inputMaterial, std::string inputCoeff, TGraphErrors data){

  typedef vector<vector<string> > Rows;
  Rows rows;
  TString inputFileName;
  double syst_error = 0;

  TString exp_data_folder = "../data/";
  std::cout << "Reading Neubert data from " << exp_data_folder << std::endl; 

  if(inputMaterial == "Aluminium" || inputMaterial == "Al" || inputMaterial == "G4_Al"){ inputFileName = exp_data_folder + "Al_"; syst_error = syst_Al; }
  if(inputMaterial == "Berillium" || inputMaterial == "Be" || inputMaterial == "G4_Be"){ inputFileName = exp_data_folder + "Be_"; syst_error = syst_Be; }
  if(inputMaterial == "Carbon" || inputMaterial == "C" || inputMaterial == "G4_C"){ inputFileName = exp_data_folder + "C_"; syst_error = syst_C; }
  if(inputMaterial == "Molibdenum" || inputMaterial == "Mo" || inputMaterial == "G4_Mo"){ inputFileName = exp_data_folder + "Mo_"; syst_error = syst_Mo; }
  if(inputMaterial == "Tantalium" || inputMaterial == "Ta" || inputMaterial == "G4_Ta"){ inputFileName = exp_data_folder + "Ta_"; syst_error = syst_Ta; }
  if(inputMaterial == "Titanium" || inputMaterial == "Ti" || inputMaterial == "G4_Ti"){ inputFileName = exp_data_folder + "Ti_"; syst_error = 0.01; }
  if(inputMaterial == "Uranium" || inputMaterial == "U" || inputMaterial == "G4_U"){ inputFileName = exp_data_folder + "U_"; syst_error = syst_U; }

  double tot_error = syst_error + stat;

  if(inputCoeff == "e") inputFileName = inputFileName + "energy_Neubert.tsv";
  if(inputCoeff == "n") inputFileName = inputFileName + "number_Neubert.tsv";

  cout << "Opening data file: " << inputFileName << endl;

  // Be has different angles...
  if(inputMaterial == "Berillium" || inputMaterial == "Be" || inputMaterial == "G4_Be"){
    string original_angle = inputAngle;
    double d_inputAngle = stod(inputAngle);
    d_inputAngle++;
    std::ostringstream s;
    s << d_inputAngle;
    inputAngle =  s.str();
    cout << "For Be change the input angle: " << original_angle << " --> " << inputAngle << endl;
  }

  ifstream input(inputFileName);
  char const row_delim = '\n';
  char const field_delim = '\t';
  std::vector<double> myValue;
  std::vector<double> myEnergy;
  int row_num = 0;
  int field_num = 0;
  int myColumn = -1;
  std::vector<string> angles; 
  for (string row; getline(input, row, row_delim); ) {
    rows.push_back(Rows::value_type());
    istringstream ss(row);
    row_num++;
    bool isExp = false;
    for (string field; getline(ss, field, field_delim); ) {
      if(row_num == 1){
        angles.push_back(field);
        if(field==inputAngle){
          myColumn = field_num;
          cout << "Look column " << field_num << endl;
        }
      }else{
         // escape in case no angle match
         if(myColumn==-1){ 
           cout << endl;
           cout << endl;
           cout << "------> Invalid angle selected. Available angles:" << endl;
           for(std::vector<double>::size_type i = 0; i != angles.size(); i++){ cout << angles.at(i) << ", ";} 
           cout << endl;
           cout << endl;
           return 0;
         }
        // if you found the angle, continue 
        rows.back().push_back(field);
        if(field_num == 0 && (field=="Exp" || field=="Exp ")) isExp = true;
        if(field_num == 1 && isExp) myEnergy.push_back(std::stod(field));  
        if(field_num == myColumn && isExp) myValue.push_back(std::stod(field)); 
      }
      field_num++;  
    }
    // reset the flags
    field_num = 0;
    isExp = false; 
  }


  cout << "Reading data from .tsv ..." << endl;
  for(std::vector<double>::size_type i = 0; i != myValue.size(); i++){
    data.SetPoint(i,myEnergy.at(i), myValue.at(i));
    //double error = myValue.at(i)*tot_error;
    double error = 0.006 * (1 - myValue.at(i));
    data.SetPointError(i, 0, error);
    //cout << myEnergy.at(i) << " " << myValue.at(i) << "+-" << myValue.at(i)*tot_error << endl;
  }
  //cout << endl;

  return data;
}
