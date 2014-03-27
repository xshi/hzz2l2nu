#include "lepton.h"

#ifndef MUON_H
#define MUON_H

class Muon : public Lepton {
	public :
		bool isTrackerMuon() const;
		bool isGlobalMuon() const;
		bool isPFMuon() const;
		bool isTMOneStationTight() const;
		bool isLooseMuon() const;
		bool isSoftMuon() const;
		bool isTightMuon() const;
		double pfIsolation() const;
		bool isPFIsolatedLoose() const;
		bool isPFIsolatedTight() const;
};

#endif // MUON_H
