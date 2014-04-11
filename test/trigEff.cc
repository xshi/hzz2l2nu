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
#include "tdrStyle.h"

using namespace std;


void trigEff(TString inputFile, TString outFile) {
  setTDRStyle(); 

  cout << "inputFile: " << inputFile << endl; 

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
  cerr << "Usage: trigEff HLT_Path_name inputFile [test.pdf]\n" 
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
  trigEff(inputFile, outFile);

  gSystem->Exit(0);
  
  return 0 ;
}

#endif


