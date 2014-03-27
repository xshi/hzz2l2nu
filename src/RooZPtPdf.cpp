#include <Riostream.h> 
#include "RooZPtPdf.h" 
#include "toolbox.h"
#include <sstream>
#include <cmath>

ClassImp(RooZPtPdf)

RooZPtPdf::RooZPtPdf(const char *name, const char *title,
		RooAbsReal & _zpt,
		RooAbsReal & _p1,
		RooAbsReal & _p2,
		RooAbsReal & _p3,
		RooAbsReal & _p4,
		RooAbsReal & _p5) :
	RooAbsPdf(name,title),
	zpt("zpt", "zpt", this, _zpt),
	p1("p1", "p1", this, _p1),
	p2("p2", "p2", this, _p2),
	p3("p3", "p3", this, _p3),
	p4("p4", "p4", this, _p4),
	p5("p5", "p5", this, _p5) {}

RooZPtPdf::RooZPtPdf(const RooZPtPdf& other, const char* name) : RooAbsPdf(other,name),
	zpt("zpt",this,other.zpt),
	p1("p1", this, other.p1),
	p2("p2", this, other.p2),
	p3("p3", this, other.p3),
	p4("p4", this, other.p4),
	p5("p5", this, other.p5) {}

Double_t RooZPtPdf::evaluate() const {
	unsigned nPar = 5;
	double params[nPar];
	params[0] = p1;
	params[1] = p2;
	params[2] = p3;
	params[3] = p4;
	params[4] = p5;
	return ptFunc(zpt, nPar, params);
}

Int_t RooZPtPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const {
	if (matchArgs(allVars,analVars,zpt))
		return 1;
	return 0;
}

Double_t RooZPtPdf::analyticalIntegral(Int_t code, const char* rangeName) const 
{
	assert(code == 1);
	
	//double c = std::pow(1 + 210.0 / p1, -p2) / std::pow(1 + 210.0 / p3, -p4);
	//if (zpt.max(rangeName) < 210) {
	//	return ptFuncIntegral(zpt.max(rangeName), p1, p2) - ptFuncIntegral(zpt.min(rangeName), p1, p2);
	//} else if (zpt.min(rangeName) >= 210) {
	//	return c * (ptFuncIntegral(zpt.max(rangeName), p3, p4) - ptFuncIntegral(zpt.min(rangeName), p3, p4));
	//} else {
	//	return (ptFuncIntegral(210, p1, p2) - ptFuncIntegral(zpt.min(rangeName), p1, p2)) + ( c * ( ptFuncIntegral(zpt.max(rangeName), p3, p4) - ptFuncIntegral(210, p3, p4) ) );
	//}
	return ptFuncIntegral(zpt.max(rangeName), p1, p2) - ptFuncIntegral(zpt.min(rangeName), p1, p2) + p5 * (ptFuncIntegral(zpt.max(rangeName), p3, p4) - ptFuncIntegral(zpt.min(rangeName), p3, p4));
	
}
