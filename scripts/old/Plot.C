// Main script for the test Photoeffect
// pictures 2*2
// 

gROOT->Reset();
gROOT->SetStyle("Plain");
gStyle->SetOptStat(0);

string sInt(Int_t number)
  {
    stringstream ss;
    ss << number;
    return ss.str();
  }
string sDouble(Double_t number)
  {
    stringstream ss;
    ss << number;
    return ss.str();
  }
void AddMC(TFile& ff, Int_t key)
  {
    TLegend* leg = 0;
    if(1 == key) {
      leg = new TLegend(.2, .55, .5, .85);
      leg->SetTextSize(.03);
      leg->SetHeader("Photoeffect models");
    }
    TH1F* h16 = (TH1F*)ff.Get("h16");
    h16->SetLineColor(2);
    h16->SetMarkerColor(2);
    //    h16->SetLineWidth(3.);
    h16->Draw("SAME");
    if(leg) leg->AddEntry(h16, "Penelope", "l");
    
    TH1F* h17 = (TH1F*)ff.Get("h17");
    h17->SetLineColor(3);
    h17->SetMarkerColor(3);
    //    h17->SetLineWidth(2.5);
    h17->Draw("SAME");
    if(leg) leg->AddEntry(h17, "Livermore", "l");
    
    TH1F* h18 = (TH1F*)ff.Get("h18");
    h18->SetLineColor(4);
    h18->SetMarkerColor(4);
    //    h18->SetLineWidth(2.);
    h18->Draw("SAME");
    if(leg) leg->AddEntry(h18, "LivermoreOLD", "l");
    /*   
    TH1F* h19 = (TH1F*)ff.Get("h19");
    h19->SetLineColor(5);
    h19->SetMarkerColor(5);
    h19->SetLineWidth(1.5);
    h19->Draw("SAME");
    if(leg) leg->AddEntry(h19, "Polarized", "l");
    */
    TH1F* h20 = (TH1F*)ff.Get("h20");
    h20->SetLineColor(6);
    h20->SetMarkerColor(6);
    //    h20->SetLineWidth(1.);
    h20->Draw("SAME");
    if(1 == key) {
      leg->AddEntry(h20, "Standard", "l");
      leg->Draw("SAME");
    }
  }

void Plot()
{
  const int nlists = 5;
  string PL[nlists] = {"emstandard_opt0","emstandard_opt3","emstandard_opt4","emlivermore","empenelope"};  


  Double_t ee[5] = {0.0, 0.01, 0.05, 0.2, 1.0};
  Double_t E = 0;
  TString name;
  TString datafile;
  TString Name;
  double Xmin[5] = {-4,-1,-1,-1,-1};
  double Xmax[5] = {4,1,1,1,1};
  double Ymin[5] = {.00015, 1e-4, 1e-4, 1e-4, 1e-3};
  double Ymax[5] = {.015, 1e-2, 1e-2, 1e-2, 1e-1};
  TCanvas* c = new TCanvas(s1,"Backscattering",0, 1, 900, 600);

  Int_t I=0;
  for(Int_t Z=1; Z<100; Z++) {
    string s = sInt(Z);  
    cout << "" << endl;
    //cout << "Start with new Element "  << endl;
    //cout <<  "Energy spectra histogramms"  << endl;
    TString s1 = "c" + s;
    
    //cout << "Divide canvas "  << endl;	
    c->Divide(2,2);

    c->cd(1);
    I=1;
    E = ee[I];
    string e = sDouble(E);
    name = s + "_" + e;
    datafile="test/photo_" + name + ".root";
    Name = "Element Z=" + s + ", E=" + e + " MeV";
    TH1F* H1 = gPad->DrawFrame(-1, Ymin[I], 1, Ymax[I], Name);
    H1->GetXaxis()->SetTitle("cos#theta");
    gPad->SetLogy(1);
    cout <<  "File to open " << datafile  << endl;
    TFile f1(datafile);
    AddMC(f1,0);
    c->Update();

    c->cd(2);
    I=2;
    E = ee[I];
    e = sDouble(E);
    name = s + "_" + e;
    datafile="test/photo_" + name + ".root";
    Name = "Element Z=" + s + ", E=" + e + " MeV";
    TH1F* H2 = gPad->DrawFrame(-1, Ymin[I], 1, Ymax[I], Name);
    H2->GetXaxis()->SetTitle("cos#theta");
    gPad->SetLogy(1);
    cout <<  "File to open " << datafile  << endl;
    TFile f2(datafile);
    AddMC(f2,0);
    c->Update();

    c->cd(3);
    I=3;
    E = ee[I];
    e = sDouble(E);
    name = s + "_" + e;
    datafile="test/photo_" + name + ".root";
    Name = "Element Z=" + s + ", E=" + e + " MeV";
    TH1F* H3 = gPad->DrawFrame(-1, Ymin[I], 1, Ymax[I], Name);
    H3->GetXaxis()->SetTitle("cos#theta");
    gPad->SetLogy(1);
    cout <<  "File to open " << datafile  << endl;
    TFile f3(datafile);
    AddMC(f3,0);
    c->Update();

    c->cd(4);
    I=4;
    E = ee[I];
    e = sDouble(E);
    name = s + "_" + e;
    datafile="test/photo_" + name + ".root";
    Name = "Element Z=" + s + ", E=" + e + " MeV";
    TH1F* H4 = gPad->DrawFrame(-1, Ymin[I], 1, Ymax[I], Name);
    H4->GetXaxis()->SetTitle("cos#theta");
    gPad->SetLogy(1);
    cout <<  "File to open " << datafile  << endl;
    TFile f4(datafile);
    AddMC(f4,1);
    c->Update();
    TString S = s;
    c->Print("plot22/ApicA_" + S + ".gif");

    gPad->Close();
    c->Close();
    delete c;
  }
  cout << "END of macro" << endl;
}
