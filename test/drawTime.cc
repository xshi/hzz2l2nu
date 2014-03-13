// 
// Author: Xin Shi <Xin.Shi@cern.ch> 
// 2014.03.13
// 

#include <iostream>
#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>
#include <TH2D.h> 
#include <TFile.h> 
#include <TCanvas.h> 
#include <TCanvas.h> 
#include <TStyle.h> 

using namespace std; 


void draw(TFile * inputFile, TString outFile) {

  cout << outFile << endl; 

  // TH2D *h3 = new TH2D("h3", "", 416, 0., 416., 160, 0., 160.);
  // TH2D *h2d;
  // for (int chip = 0; chip < 16 ; chip++) { 
  //   gDirectory->GetObject(Form("DAQ/Hits_C%d_V0", chip), h2d); 
  //   if (!h2d){
  //     cerr << "No object name found: " << endl;
  //     return ;
  //   }
    
  //   for (int icol = 0; icol < 52; icol++) {
  //     for (int irow = 0; irow < 80; irow++)  {
  // 	double value = h2d->GetBinContent(icol, irow); 
  // 	if (chip < 8) {h3->SetBinContent(415-(chip*52+icol)+1, 159-irow+1, value);}
  // 	if (chip > 7) {h3->SetBinContent((chip-8)*52+icol+1, irow+1, value);}
  //     }
  //   }
  // }
  
  // TCanvas *c = new TCanvas("c", "DAQ module", 800, 200); 
  // h3->DrawCopy("colz");

  // gROOT->SetStyle("Plain");
  
  // gStyle->SetPalette(1);
  // gStyle->SetOptStat(0);
  // gStyle->SetTitle(0);

  // c->SaveAs(outFile);
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


