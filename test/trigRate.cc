// 
// Author: Xin Shi <Xin.Shi@cern.ch> 
// 2014.07.20
// 

#include <iostream>
#include <string> 
#include <fstream>
#include <sstream>

int main(int argc, char** argv) {
  std::cout << "Calculatine trigger rate ... " << std::endl;

  std::ifstream in;
  in.open("ref_rate.dat"); 

  std::string path;
  double rate;
  double prescale;

  double factor_pfmet, factor_vbf; 
  std::string line;
  int nlines = 0;

  while (getline(in, line)) {
    if (!in.good()) break;
    std::istringstream iss(line);
    if ( line.find("#") == 0 ) continue; 
    if (!(iss >> path >> rate >> prescale >> factor_pfmet >> factor_vbf )) break; 

    // std::cout << path << ": " << rate*prescale << " Hz, " 
    // 	      << factor_pfmet << " (PFMET) " 
    // 	      << factor_vbf << " (VBF)" << std::endl; 

    printf("%s : %.2f Hz (PFMET) %.2f Hz (VBF)\n",
	   path.c_str(),
	   rate*prescale/factor_pfmet,
	   rate*prescale/factor_vbf); 

    // std::cout << path << ": " // << rate*prescale 
    // 	      << rate*prescale/factor_pfmet << " (PFMET) "
    // 	      << rate*prescale/factor_vbf << " (VBF)" << std::endl; 
    
    nlines ++; 
  }

  std::cout << "Total of " << nlines << " found" << std::endl;

  return 0 ;
}



