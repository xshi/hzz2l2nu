// 
// Author: Xin Shi <Xin.Shi@cern.ch> 
// 2014.04.11
// 

#include <iostream>
#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>
#include <TH1F.h> 
#include <TH2D.h> 
#include <TFile.h> 
#include <TCanvas.h> 
#include <TStyle.h> 
#include <TProfile.h> 
#include <TPaveText.h> 
// #include "tdrStyle.h"
#include <TApplication.h> 
#include <TEfficiency.h> 


using namespace std;

void set_root_style(int stat=1110, int grid=0){
  gROOT->Reset();

  gStyle->SetTitleFillColor(0) ; 
  gStyle->SetTitleBorderSize(0); 
    
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetCanvasDefX(0); 
  gStyle->SetCanvasDefY(0); 
  gStyle->SetFrameBorderMode(0); 
  gStyle->SetFrameBorderSize(1); 
  gStyle->SetFrameFillColor(0); 
  gStyle->SetFrameFillStyle(0); 
  gStyle->SetFrameLineColor(1); 
  gStyle->SetFrameLineStyle(1); 
  gStyle->SetFrameLineWidth(1); 

  // gStyle->SetPadTopMargin(PadTopMargin);  
  gStyle->SetPadLeftMargin(0.10);  
  gStyle->SetPadRightMargin(0.05);  

  gStyle->SetLabelSize(0.03, "XYZ");  
  gStyle->SetTitleSize(0.04, "XYZ");  
  gStyle->SetTitleOffset(1.2, "Y");  

  gStyle->SetPadBorderMode(0);  
  gStyle->SetPadColor(0);  
  gStyle->SetPadTickX(1); 
  gStyle->SetPadTickY(1); 
  gStyle->SetPadGridX(grid); 
  gStyle->SetPadGridY(grid); 

  gStyle->SetOptStat(stat); 
  gStyle->SetStatColor(0); 
  gStyle->SetStatBorderSize(1); 
}

//void trigEff(TString inputFile, TString outFile) {
void trigEff(TString inputFile) {
  // setTDRStyle(); 
  set_root_style(); 

  cout << "inputFile: " << inputFile << endl; 

  TFile *f = new TFile(inputFile);
  
  TEfficiency *eff = (TEfficiency*)f->Get("eff");

  TCanvas *c = new TCanvas("c", "c", 800, 800);

  eff->Draw();
  c->Update();
  
  //open the file and get events tree
  // DataEventSummaryHandler evSummaryHandler;
  

}


// #include <algorithm>

char* get_option(char ** begin, char ** end, const std::string & option){
  char ** itr = std::find(begin, end, option);
  if (itr != end && ++itr != end)  return *itr;
  return 0;
}

bool option_exists(char** begin, char** end, const std::string& option){
  return std::find(begin, end, option) != end;
}


void print_usage(){
  cerr << "Usage: trigEff inputFile \n" 
       << endl; 
}


int main(int argc, char** argv) {
  if (argc < 2) {
    print_usage() ;  
    return -1; 
  }
  
  TString inputFile = argv[1]; 
  // TString outFile = "test.pdf"; 
  // if (argc >= 3) outFile = argv[2]; 
  TApplication theApp("App", 0, 0);
  theApp.SetReturnFromRun(true);
  // trigEff(inputFile, outFile);
  trigEff(inputFile);
  theApp.Run();
 
  // return 0 ;
}



