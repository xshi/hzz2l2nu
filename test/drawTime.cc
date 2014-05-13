// 
// Author: Xin Shi <Xin.Shi@cern.ch> 
// Date: 2014.03.13
// Version: 1.1 

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
#include "tdrStyle.h"
using namespace std; 


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


void print_module_average(TString hname, double minContent=0.2){
  cout << "hanme: " << hname << endl ;

  TH1F *h1f_path = NULL; 
  gDirectory->GetObject(hname.Data(), h1f_path); 
  if (! h1f_path) {
    cerr << "Can't find object: " << hname << endl; 
    return; 
  }
  int nbins = h1f_path->GetXaxis()->GetNbins(); 

  cout << "PATH valute larger than " << minContent << " : " << endl; 

  for ( int bin=1; bin!= nbins; bin++) {
    double content = h1f_path->GetBinContent(bin); 
   
    if ( content > minContent) {
      cout << h1f_path->GetXaxis()->GetBinLabel(bin)
	   << "  " << content << endl; 
    }
  }
}


void draw(TString inputFile, TString outFile) {
  setTDRStyle(); 
  TFile::Open(inputFile.Data()); 
  // int run_number = get_run_number(inputFile)
  int run_number = 207515; 

  // TString TimerServicePath = Form("DQMData/Run %d/HLT/Run summary/TimerService", run_number); 
  TString TimerServicePath = Form("DQMData/Run %d/HLT/Run summary/TimerService/process HLTX", run_number); 
  
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

 
  labels.push_back("HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_VBF_v1");
  labels.push_back("HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_VBF_v1");
  labels.push_back("HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_VBF_v1");
  labels.push_back("HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_VBF_v1");
  labels.push_back("HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_VBF_v1");
  labels.push_back("HLT_Photon135_VBF_v1"); 
  labels.push_back("HLT_Photon150_VBF_v1"); 
  labels.push_back("HLT_Photon160_VBF_v1"); 
  labels.push_back("HLT_Photon250_NoHE_VBF_v1"); 
  labels.push_back("HLT_Photon300_NoHE_VBF_v1"); 

 
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
  TString module_average_path_name = Form("%s/Paths/%s_module_average",
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


