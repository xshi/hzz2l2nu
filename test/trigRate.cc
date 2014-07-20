// 
// Author: Xin Shi <Xin.Shi@cern.ch> 
// 2014.07.20
// 

#include <iostream>
#include <map> 
#include <string> 
// #include <algorithm>

int main(int argc, char** argv) {
  std::cout << "Calculatine trigger rate ... " << std::endl;

  std::map <std::string, double> path_rate;  

  // Rates taken from run 1917188, LS32-40
  path_rate["HLT_Photon135"]=7.3;
  path_rate["HLT_Photon22_R9Id90_HE10_Iso40_EBOnly"]= 1.97;   
  path_rate["HLT_Photon36_R9Id90_HE10_Iso40_EBOnly"]= 0.95;
  path_rate["HLT_Photon50_R9Id90_HE10_Iso40_EBOnly"]= 0.58;
  path_rate["HLT_Photon75_R9Id90_HE10_Iso40_EBOnly"]= 0.33;
  path_rate["HLT_Photon90_R9Id90_HE10_Iso40_EBOnly"]= 0.39;
  path_rate["HLT_Photon135"]= 7.34;
  path_rate["HLT_Photon150"]= 4.59;
  path_rate["HLT_Photon160"]= 3.33;
  path_rate["HLT_Photon250_NoHE"]= 0.80;
  path_rate["HLT_Photon300_NoHE"]= 0.80;

  return 0 ;
}



