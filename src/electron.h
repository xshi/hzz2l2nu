#include "lepton.h"

#ifndef ELECTRON_H
#define ELECTRON_H

class Electron : public Lepton {
	public :
		bool isEB() const;
		bool isEE() const;
		bool isInCrack() const;
		double EA() const;
		double pfIsolation(double rho) const;
		bool isPFIsolatedVeto(double rho) const;
		bool isPFIsolatedLoose(double rho) const;
		bool isPFIsolatedMedium(double rho) const;
		bool isPFIsolatedTight(double rho) const;
		bool passesVetoID() const;
		bool passesLooseID() const;
		bool passesMediumID() const;
//		bool passesTightID() const;
};

#endif // ELECTRON_H
