#include "muon.h"
#include "toolbox.h"
#include "TLorentzVector.h"
#include <iostream>

bool Muon::isTrackerMuon() const {
	return (0x1 << 5) & getVarI("ln_idbits");
}

bool Muon::isGlobalMuon() const {
	return (0x1 << 6) & getVarI("ln_idbits");
}

bool Muon::isPFMuon() const {
	return (0x1 << 7) & getVarI("ln_idbits");
}

bool Muon::isTMOneStationTight() const {
	return (0x1 << 4) & getVarI("ln_idbits");
}

bool Muon::isLooseMuon() const {
//	bool passes = (isTrackerMuon() || isGlobalMuon()) && isPFMuon();
	bool idBit = getVarI("ln_idbits") & (0x1 << 8);
//	if (idBit != passes) {
//		print();
//		throw std::string("ERROR Muon::isLooseMuon(): ID bit does not match!");
//	}
	return idBit;
}

bool Muon::isSoftMuon() const {
//	bool passes = false;
//	if (
//			isTMOneStationTight() &&
//			getVarF("mn_trkLayersWithMeasurement") > 5 &&
//			getVarF("mn_pixelLayersWithMeasurement") > 1 &&
//			getVarF("mn_innerTrackChi2") < 1.8 &&
//			fabs(getVarF("ln_d0")) < 3.0 &&
//			fabs(getVarF("ln_dZ")) < 30.0
//		)
//		passes = true;
	bool idBit = getVarI("ln_idbits") & (0x1 << 9);
//	if (idBit != passes) {
//		print();
//		throw std::string("ERROR Muon::isSoftMuon(): ID bit does not match!");
//	}
	return idBit;
}

bool Muon::isTightMuon() const {
//	bool passes = false;
//	if (
//			isGlobalMuon() &&
//			isPFMuon() &&
//			getVarF("ln_trkchi2") < 10.0 &&
//			getVarF("mn_validMuonHits") > 0 &&
//			getVarF("mn_nMatchedStations") > 1 &&
//			fabs(getVarF("ln_d0")) < 0.2 &&
//			fabs(getVarF("ln_dZ")) < 0.5 &&
//			getVarF("ln_trkValidPixelHits") > 0 &&
//			getVarF("mn_trkLayersWithMeasurement") > 5
//		)
//		passes = true;
	bool idBit = getVarI("ln_idbits") & (0x1 << 10);
//	if (idBit != passes) {
//		print();
//		throw std::string("ERROR Muon::isTightMuon(): ID bit does not match!");
//	}
	return idBit;
}

double Muon::pfIsolation() const {
	return (getVarF("ln_chIso04") + max(0.0, getVarF("ln_nhIso04") + getVarF("ln_gIso04") - 0.5 * getVarF("ln_puchIso04"))) / lorentzVector().Pt();
}

bool Muon::isPFIsolatedLoose() const {
	double pfIsol = pfIsolation();
	return pfIsol < 0.2;
}

bool Muon::isPFIsolatedTight() const {
	double pfIsol = pfIsolation();
	return pfIsol < 0.12;
}
