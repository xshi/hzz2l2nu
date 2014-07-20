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

  std::string line;
  int nlines = 0;

  while (getline(in, line)) {
    std::istringstream iss(line);
    if ( line.find("#") == 0 ) continue; 
    if (!(iss >> path >> rate >> prescale )) break; 
    if (!in.good()) break;
    std::cout << path << ": " << rate*prescale << std::endl; 
    nlines ++; 
  }

  std::cout << "Total of " << nlines << " found" << std::endl;

  return 0 ;
}



