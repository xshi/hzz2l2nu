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

using namespace std; 


void draw(TFile * inputFile, TString outFile) {
  // int run_number = get_run_number(inputFile)
  int run_number = 207515; 

  TH1F * h1f_all_paths = NULL; 
  gDirectory->GetObject(Form("DQMData/Run %d/HLT/Run summary/TimerService/all_paths", run_number), h1f_all_paths); 
  
  TCanvas *c = new TCanvas("c", "Time", 600, 600); 
  h1f_all_paths->Draw();

  // gROOT->SetStyle("Plain");
  
  // gStyle->SetPalette(1);
  // gStyle->SetOptStat(0);
  // gStyle->SetTitle(0);

  c->SaveAs(outFile);
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
  
  TString hlt_name = argv[1]; 

  TFile * inputFile = TFile::Open(argv[2]);
  
  if ( inputFile ) {
    TString outFile = "test.pdf"; 
    if (argc == 4) outFile = argv[3]; 
    draw(inputFile, outFile);
  } else {
    cerr << "Unable to open file: " << argv[2] << endl; 
    print_usage(); 
  }
  
  gSystem->Exit(0);
  
  return 0 ;
}

#endif


