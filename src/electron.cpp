#include "electron.h"
#include <cmath>
#include "TLorentzVector.h"
#include <string>

using std::string;
using std::max;

bool Electron::isEB() const {
	return fabs(getVarF("egn_sceta")) < 1.479;
}

bool Electron::isEE() const {
	return fabs(getVarF("egn_sceta")) > 1.479 && fabs(getVarF("egn_sceta")) < 2.5;
}

bool Electron::isInCrack() const {
	double abseta = fabs(getVarF("egn_sceta"));
	return (abseta > 1.4442 && abseta < 1.566);
}

double Electron::EA() const {
	double abseta = fabs(getVarF("egn_sceta"));
	if (abseta < 1.0)
		return 0.13;
	else if (abseta < 1.479)
		return 0.14;
	else if (abseta < 2.0)
		return 0.07;
	else if (abseta < 2.2)
		return 0.09;
	else if (abseta < 2.3)
		return 0.11;
	else if (abseta < 2.4)
		return 0.11;
	else
		return 0.14;
}

double Electron::pfIsolation(double rho) const {
	double rhoPr = max(rho, 0.0);
	double nIso = max(getVarF("ln_nhIso03") + getVarF("ln_gIso03") - rhoPr * EA(), 0.0);
	return (nIso + getVarF("ln_chIso03")) / lorentzVector().Pt();
}

bool Electron::isPFIsolatedVeto(double rho) const {
	double pfIsol = pfIsolation(rho);
	if (isEB())
		return pfIsol < 0.15;
	else
		return pfIsol < 0.15;
}

bool Electron::isPFIsolatedLoose(double rho) const {
	TLorentzVector lv = lorentzVector();
	double pfIsol = pfIsolation(rho);
	if (isEB())
		return pfIsol < 0.15;
	else {
		if (lv.Pt() > 20)
			return pfIsol < 0.15;
		else
			return pfIsol < 0.10;
	}
}

bool Electron::isPFIsolatedMedium(double rho) const {
	TLorentzVector lv = lorentzVector();
	double pfIsol = pfIsolation(rho);
	if (isEB())
		return pfIsol < 0.15;
	else {
		if (lv.Pt() > 20)
			return pfIsol < 0.15;
		else
			return pfIsol < 0.10;
	}
}

bool Electron::isPFIsolatedTight(double rho) const {
	TLorentzVector lv = lorentzVector();
	double pfIsol = pfIsolation(rho);
	if (isEB())
		return pfIsol < 0.10;
	else {
		if (lv.Pt() > 20)
			return pfIsol < 0.10;
		else
			return pfIsol < 0.07;
	}
}

bool Electron::passesVetoID() const {
//	bool passes = false;
//	if (isEB()) {
//		if (
//				fabs(getVarF("egn_detain")) < 0.007 &&
//				fabs(getVarF("egn_dphiin")) < 0.8 &&
//				getVarF("egn_sihih") < 0.01 &&
//				getVarF("egn_hoe") < 0.15 &&
//				fabs(getVarF("ln_d0")) < 0.04 &&
//				fabs(getVarF("ln_dZ")) < 0.2
//			)
//			passes = true;
//	} else {
//		if (
//				fabs(getVarF("egn_detain")) < 0.01 &&
//				fabs(getVarF("egn_dphiin")) < 0.7 &&
//				getVarF("egn_sihih") < 0.03 &&
//				fabs(getVarF("ln_d0")) < 0.04 &&
//				fabs(getVarF("ln_dZ")) < 0.2
//			)
//			passes = true;
//	}
	bool idBit = getVarI("ln_idbits") & (0x1 << 3);
//	if (passes != idBit) {
//		print();
//		throw string("ERROR Electron::passesVetoID(): ID bit doesn't not match!");
//	}
	return idBit;
}

bool Electron::passesLooseID() const {
//	bool passes = false;
//	if (isEB()) {
//		if (
//				fabs(getVarF("egn_detain")) < 0.007 &&
//				fabs(getVarF("egn_dphiin")) < 0.15 &&
//				getVarF("egn_sihih") < 0.01 &&
//				getVarF("egn_hoe") < 0.12 &&
//				fabs(getVarF("ln_d0")) < 0.02 &&
//				fabs(getVarF("ln_dZ")) < 0.2 &&
//				fabs( getVarF("egn_ooemoop") ) < 0.05 &&
//				!getVarB("egn_isConv") &&
//				getVarF("ln_trkLostInnerHits") <= 1
//			)
//			passes = true;
//	} else {
//		if (
//				fabs(getVarF("egn_detain")) < 0.009 &&
//				fabs(getVarF("egn_dphiin")) < 0.10 &&
//				getVarF("egn_sihih") < 0.03 &&
//				getVarF("egn_hoe") < 0.10 &&
//				fabs(getVarF("ln_d0")) < 0.02 &&
//				fabs(getVarF("ln_dZ")) < 0.2 &&
//				fabs( getVarF("egn_ooemoop") ) < 0.05 &&
//				!getVarB("egn_isConv") &&
//				getVarF("ln_trkLostInnerHits") <= 1
//			)
//			passes = true;
//	}
	bool idBit = getVarI("ln_idbits") & (0x1 << 4);
//	if (passes != idBit) {
//		print();
//		throw string("ERROR Electron::passesLooseID(): ID bit doesn't not match!");
//	}
	return idBit;
}

bool Electron::passesMediumID() const {
//	bool passes = false;
//	if (isEB()) {
//		if (
//				fabs(getVarF("egn_detain")) < 0.004 &&
//				fabs(getVarF("egn_dphiin")) < 0.06 &&
//				getVarF("egn_sihih") < 0.01 &&
//				getVarF("egn_hoe") < 0.12 &&
//				fabs(getVarF("ln_d0")) < 0.02 &&
//				fabs(getVarF("ln_dZ")) < 0.1 &&
//				fabs( getVarF("egn_ooemoop") ) < 0.05 &&
//				!getVarB("egn_isConv") &&
//				getVarF("ln_trkLostInnerHits") <= 1
//			)
//			passes = true;
//	} else {
//		if (
//				fabs(getVarF("egn_detain")) < 0.007 &&
//				fabs(getVarF("egn_dphiin")) < 0.03 &&
//				getVarF("egn_sihih") < 0.03 &&
//				getVarF("egn_hoe") < 0.10 &&
//				fabs(getVarF("ln_d0")) < 0.02 &&
//				fabs(getVarF("ln_dZ")) < 0.1 &&
//				fabs( getVarF("egn_ooemoop") ) < 0.05 &&
//				!getVarB("egn_isConv") &&
//				getVarF("ln_trkLostInnerHits") <= 1
//			)
//			passes = true;
//	}
	bool idBit = getVarI("ln_idbits") & (0x1 << 5);
//	if (passes != idBit) {
//		print();
//		throw string("ERROR Electron::passesMediumID(): ID bit doesn't not match!");
//	}
	return idBit;
}
