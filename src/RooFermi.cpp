#include <Riostream.h>
#include "RooFermi.h"
#include <TMath.h>

ClassImp(RooFermi) 

RooFermi::RooFermi(const char *name, const char *title, RooAbsReal& _x, RooAbsReal& _lowcut,
		RooAbsReal& _highcut, RooAbsReal& _lowalpha, RooAbsReal& _highalpha, RooAbsReal& _c) :
	RooAbsPdf(name,title), x("x","x",this,_x), lowcut("lowcut","lowcut",this,_lowcut),	highcut("highcut","highcut",this,_highcut),
	lowalpha("lowalpha","lowalpha",this,_lowalpha),	highalpha("highalpha","highalpha",this,_highalpha),	c("c","c",this,_c) {}

RooFermi::RooFermi(const RooFermi& other, const char* name) : RooAbsPdf(other,name), x("x",this,other.x),
	lowcut("lowcut",this,other.lowcut), highcut("highcut",this,other.highcut), lowalpha("lowalpha",this,other.lowalpha),
	highalpha("highalpha",this,other.highalpha), c("c",this,other.c) {} 

Double_t RooFermi::evaluate() const { 
	return 1.0 + c / ( (TMath::Exp((x - highcut) / highalpha) + 1) * (TMath::Exp((lowcut - x) / lowalpha) + 1) );
} 
