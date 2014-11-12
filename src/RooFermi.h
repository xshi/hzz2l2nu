#ifndef ROOFERMI
#define ROOFERMI

#include <RooAbsPdf.h>
#include <RooRealProxy.h>
#include <RooCategoryProxy.h>
#include <RooAbsReal.h>
#include <RooAbsCategory.h>

class RooFermi : public RooAbsPdf {
	public:
		RooFermi() {};
		RooFermi(const char *name, const char *title,
				RooAbsReal & _x,
				RooAbsReal & _lowcut,
				RooAbsReal & _highcut,
				RooAbsReal & _lowalpha,
				RooAbsReal & _highalpha,
				RooAbsReal & _c
				);
		RooFermi(const RooFermi & other, const char * name);
		inline virtual TObject* clone(const char* newname) const { return new RooFermi(*this,newname); }
		inline virtual ~RooFermi() {}
		Double_t evaluate() const;
	protected:
		RooRealProxy x;
		RooRealProxy lowcut;
		RooRealProxy highcut;
		RooRealProxy lowalpha;
		RooRealProxy highalpha;
		RooRealProxy c;
	private:
		ClassDef(RooFermi,4);
};

#endif
