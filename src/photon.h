#ifndef PHOTON_H
#define PHOTON_H

#include "lepton.h"
#include <vector>


class TLorentzVector;
class Jet;

class Photon : public Lepton {
	public :
		virtual TLorentzVector lorentzVector() const;
		bool isEB() const;
		bool isEE() const;
		bool isInCrack() const;
		double EA_CH() const;
		double EA_NH() const;
		double EA_G() const;
		double pfIsolation_CH(double rho) const;
		double pfIsolation_NH(double rho) const;
		double pfIsolation_G(double rho) const;
		bool isSelected(double rho);
};

#endif // PHOTON_H
