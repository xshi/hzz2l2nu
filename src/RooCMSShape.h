/*****************************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: PhysicsTools/TagAndProbe/RooCMSShape
 *
 *
 * Authors:
 *   Nadia Adam, Princeton - neadam@princeton.edu
 *   Adam Hunt, Princeton  - ahunt@princeton.edu
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *
 * Description:
 *   Defines a probability density function which has exponential decay 
 *   distribution at high mass beyond the pole position (say, Z peak)  
 *   but turns over (i.e., error function) at low mass due to threshold 
 *   effect. We use this to model the background shape in Z->ll invariant 
 *   mass.
 * History:
 *   
 *
 *****************************************************************************/

#ifndef ROOCMSSHAPE
#define ROOCMSSHAPE

#include <RooAbsPdf.h>
#include <RooRealProxy.h>
#include <RooCategoryProxy.h>
#include <RooAbsReal.h>
#include <RooAbsCategory.h>

class RooCMSShape : public RooAbsPdf {
	public:
		RooCMSShape() {};
		RooCMSShape(const char *name, const char *title,
				RooAbsReal & _x,
				RooAbsReal & _alpha,
				RooAbsReal & _beta,
				RooAbsReal & _gamma,
				RooAbsReal & _peak
				);
		RooCMSShape(const RooCMSShape & other, const char * name);
		inline virtual TObject* clone(const char* newname) const { return new RooCMSShape(*this,newname); }
		inline virtual ~RooCMSShape() {}
		Double_t evaluate() const;
	protected:
		RooRealProxy x;
		RooRealProxy alpha;
		RooRealProxy beta;
		RooRealProxy gamma;
		RooRealProxy peak;
	private:
		ClassDef(RooCMSShape,3);
};

#endif
