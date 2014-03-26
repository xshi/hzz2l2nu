// 
// Author: Xin Shi <Xin.Shi@cern.ch> 
// 2014.03.13
// 

#include <iostream>
#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>
#include <TH1F.h> 
#include <TH2D.h> 
#include <TFile.h> 
#include <TCanvas.h> 
#include <TCanvas.h> 
#include <TStyle.h> 
#include <TProfile.h> 
#include <TPaveText.h> 

using namespace std; 
void setTDRStyle() {

  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(800); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

// For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

// For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

// For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

  tdrStyle->SetEndErrorSize(2);
  // tdrStyle->SetErrorMarker(20);
  tdrStyle->SetMarkerStyle(20);
  tdrStyle->SetErrorX(0.);
  //tdrStyle->SetCenterTitle(); 

// For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);


//For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);


// For the statistics box:
  tdrStyle->SetOptFile(1);
  // tdrStyle->SetOptStat("mr"); // To display the mean and RMS:   SetOptStat("mr");

  tdrStyle->SetOptStat(1111111); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

// Margins:
  tdrStyle->SetPadTopMargin(0.08);
  tdrStyle->SetPadBottomMargin(0.1);
  tdrStyle->SetPadLeftMargin(0.1);
  tdrStyle->SetPadRightMargin(0.3);

// For the Global title:
  // tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.04);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  tdrStyle->SetTitleBorderSize(0);
  
  // tdrStyle->SetTitleAlign(13);
  // tdrStyle->SetTitleX(.99); 

  // For the axis titles:
  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.01, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);

  //  tdrStyle->SetTitleXOffset(0.9);
  // tdrStyle->SetTitleYOffset(1.05);

  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset


// For the axis labels:
  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  //  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.02, "XYZ");

// For the axis:
  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

// Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  tdrStyle->cd();
}


void fill_hlt_path(int i, TString label, TProfile *prf_paths_active_time, TH1F *h1f_path) {
  int iBin = prf_paths_active_time->GetXaxis()->FindBin(label.Data()); 
  double content = prf_paths_active_time->GetBinContent(iBin); 
  double error = prf_paths_active_time->GetBinError(iBin); 

  h1f_path->SetBinContent(i+1, content); 
  h1f_path->SetBinError(i+1, error); 
  h1f_path->GetXaxis()->SetBinLabel(i+1, label.Data()); 
 
  // Char_t message[80];
  // sprintf(message, "T(%s) = %.4f#pm%.4f[ms] ", label.Data(), content, error); 
  cout << label << ": " << content << " +/- " << error << " [ms]" << endl; 
} 

void print_module_average(TString hname){
  TH1F *h1f_path = NULL; 
  gDirectory->GetObject(hname.Data(), h1f_path); 
  if (! h1f_path) {
    cerr << "Can't find object: " << hname << endl; 
    return; 
  }
  
  cout << "found it" << endl; 

  
}


void draw(TString inputFile, TString outFile) {
  setTDRStyle(); 
  TFile::Open(inputFile.Data()); 
  // int run_number = get_run_number(inputFile)
  int run_number = 207515; 

  TString TimerServicePath = Form("DQMData/Run %d/HLT/Run summary/TimerService", run_number); 
  
  TCanvas *c = new TCanvas(); 
  c->Print(Form("%s[", outFile.Data()));

  // draw all_paths processing time
  TH1F * h1f_all_paths = NULL; 
  TString hname = "all_paths"; 
  gDirectory->GetObject(Form("%s/%s", TimerServicePath.Data(), hname.Data()), h1f_all_paths); 
  
  if (! h1f_all_paths) {
    cerr << "Can't find object: " << hname << endl; 
    return; 
  }

  h1f_all_paths->Draw();
  c->Print(outFile);
  c->Clear(); 

  // draw paths_active_time
  TProfile * prf_paths_active_time = NULL; 
  hname = "paths_active_time"; 
  gDirectory->GetObject(Form("%s/%s", TimerServicePath.Data(), hname.Data()), prf_paths_active_time); 
    
  if (! prf_paths_active_time) {
    cerr << "Can't find object: " << hname << endl; 
    return ; 
  }
  
  prf_paths_active_time->Draw();
  c->Print(outFile);
  c->Clear(); 
  
  // draw zoom-in PATH time

  vector<TString> labels; 

  // labels.push_back("HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_v5");

  labels.push_back("HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1");
  labels.push_back("HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1");
  labels.push_back("HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1");
  labels.push_back("HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1");
  labels.push_back("HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1");
  labels.push_back("HLT_Photon135_PFMET40_v1"); 
  labels.push_back("HLT_Photon150_PFMET40_v1"); 
  labels.push_back("HLT_Photon160_PFMET40_v1"); 
  labels.push_back("HLT_Photon250_NoHE_PFMET40_v1"); 
  labels.push_back("HLT_Photon300_NoHE_PFMET40_v1"); 

  // TH1F * h1f_path = new TH1F("h", "HLT Paths time;;processing time [ms]", 5, 0, 5); 

  TH1F * h1f_path = new TH1F("h", "HLT Paths time;;processing time [ms]",
			     labels.size(), 0, labels.size()); 

  for (vector<TString>::size_type i=0; i!= labels.size(); i++){
    fill_hlt_path(i, labels[i], prf_paths_active_time, h1f_path); 
  }
  
  h1f_path->GetXaxis()->SetLabelSize(0.025); 
  h1f_path->GetYaxis()->SetTitleSize(0.04); 
  h1f_path->Draw("E"); 

  // TPaveText *pt = new TPaveText(0.7,0.5, 0.99, 0.75, "NDC");
  // Char_t message[80];
  // sprintf(message, "T(%s) = %.4f#pm%.4f[ms] ", label.Data(), content, error); 
  // cout << label << ": " << content << " +/- " << error << " [ms]" << endl; 
  // pt->AddText(message);  

  // pt->SetBorderSize(0);
  // pt->SetFillColor(0); 
  // pt->Draw(); 

  // List module time larger than 0.1ms
  // for (vector<TString>::size_type i=0; i!= labels.size(); i++){
  //   print_module_average(i, labels[i], prf_paths_active_time, h1f_path); 
  // }
  int i = 0; 
  TString module_average_path_name = Form("%s/Paths/%s_module_active",
					  TimerServicePath.Data(), 
					  labels[i].Data()); 
  // cout << module_average_path_name << endl; 
  print_module_average(module_average_path_name);   


  c->Print(outFile);
  c->Clear(); 
  h1f_path->Delete(); 
  // pt->Delete(); 

  




  c->Print(Form("%s]", outFile.Data()));
  delete c; 
}



#ifndef __CINT__ 
#include <iostream>
#include <algorithm>

char* get_option(char ** begin, char ** end, const std::string & option){
  char ** itr = std::find(begin, end, option);
  if (itr != end && ++itr != end)  return *itr;
  return 0;
}

bool option_exists(char** begin, char** end, const std::string& option){
  return std::find(begin, end, option) != end;
}


void print_usage(){
  cerr << "Usage: drawTime HLT_Path_name  inputFile [test.pdf]\n" 
       << endl; 
}


int main(int argc, char** argv) {
  if (argc < 2) {
    print_usage() ;  
    return -1; 
  }
  
  TString inputFile = argv[1]; 
  TString outFile = "test.pdf"; 
  if (argc >= 3) outFile = argv[2]; 
  draw(inputFile, outFile);

  gSystem->Exit(0);
  
  return 0 ;
}

#endif


