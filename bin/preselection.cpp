#include "TROOT.h"
#include "TFile.h"
#include "TH1D.h"
#include "../src/options.h"
#include "../src/preselectionCMG.h"
#include "../src/event.h"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

int main(int argc, const char * argv[]) {
	if (argc != 2) {
		cout << "USAGE: " << argv[0] << " CONFIG_FILE" << endl;
	}
	gROOT->ProcessLine("#include <vector>");

	try {
		Options::getInstance().readInOptions(argv[1]);
		bool isData = Options::getInstance().checkBoolOption("DATA");
		bool isMC = Options::getInstance().checkBoolOption("MC");
		if (isData == isMC)
			throw std::string("ERROR: Problem with isData and isMC flags!");
		if (Options::getInstance().checkBoolOption("ELECTRON_PRESELECTION"))
			LeptonPreselectionCMG(ELE);
		if (Options::getInstance().checkBoolOption("MUON_PRESELECTION"))
			LeptonPreselectionCMG(MU);
		if (Options::getInstance().checkBoolOption("EMU_PRESELECTION"))
			LeptonPreselectionCMG(EMU);
	} catch (string & e) {
		cout << e << endl;
	}
}
