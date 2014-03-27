#include "photon.h"
#include "toolbox.h"
#include "jet.h"
#include "TLorentzVector.h"
#include <iostream>
#include <string>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::cin;

TLorentzVector Photon::lorentzVector() const {
	return TLorentzVector(getVarF("gn_px"), getVarF("gn_py"), getVarF("gn_pz"), getVarF("gn_en"));
}

bool Photon::isEB() const {
	return fabs(getVarF("egn_sceta")) < 1.479;
}

bool Photon::isEE() const {
	return fabs(getVarF("egn_sceta")) > 1.479 && fabs(getVarF("egn_sceta")) < 2.5;
}

bool Photon::isInCrack() const {
	double abseta = fabs(getVarF("egn_sceta"));
	return (abseta > 1.4442 && abseta < 1.566);
}

double Photon::EA_CH() const {
	double abseta = fabs(getVarF("egn_sceta"));
	if (abseta < 1.0)
		return 0.012;
	else if (abseta < 1.479)
		return 0.010;
	else if (abseta < 2.0)
		return 0.014;
	else if (abseta < 2.2)
		return 0.012;
	else if (abseta < 2.3)
		return 0.016;
	else if (abseta < 2.4)
		return 0.020;
	else
		return 0.012;
}

double Photon::EA_NH() const {
	double abseta = fabs(getVarF("egn_sceta"));
	if (abseta < 1.0)
		return 0.030;
	else if (abseta < 1.479)
		return 0.057;
	else if (abseta < 2.0)
		return 0.039;
	else if (abseta < 2.2)
		return 0.015;
	else if (abseta < 2.3)
		return 0.024;
	else if (abseta < 2.4)
		return 0.039;
	else
		return 0.072;
}

double Photon::EA_G() const {
	double abseta = fabs(getVarF("egn_sceta"));
	if (abseta < 1.0)
		return 0.148;
	else if (abseta < 1.479)
		return 0.130;
	else if (abseta < 2.0)
		return 0.112;
	else if (abseta < 2.2)
		return 0.216;
	else if (abseta < 2.3)
		return 0.262;
	else if (abseta < 2.4)
		return 0.260;
	else
		return 0.266;
}

double Photon::pfIsolation_CH(double rho) const {
	return std::max(0.0, getVarF("gn_chIso03") - rho * EA_CH());
}

double Photon::pfIsolation_NH(double rho) const {
	return std::max(0.0, getVarF("gn_nhIso03") - rho * EA_NH());
}

double Photon::pfIsolation_G(double rho) const {
	return std::max(0.0, getVarF("gn_gIso03") - rho * EA_G());
}

bool Photon::isSelected(double rho) {
//	bool passesID = false;
//	if (isEB()) {
//		if (
//				!getVarB("egn_isConv") &&
//				getVarF("egn_hoe") < 0.05 &&
//				getVarF("egn_sihih") < 0.011
//			)
//			passesID = true;
//	} else {
//		if (
//				!getVarB("egn_isConv") &&
//				getVarF("egn_hoe") < 0.05 &&
//				getVarF("egn_sihih") < 0.033
//			)
//			passesID = true;
//	}
	bool isMedium = (getVarI("gn_idbits") & (0x1 << 1));
//	if (passesID != isMedium) {
//		print();
//		throw string("ERROR Photon::isSelected(): ID bit doesn't not match!");
//	}

	double pt = lorentzVector().Pt();
	bool passesIso = false;
	if (isEB()) {
		if (
				pfIsolation_CH(rho) < 1.5 &&
				pfIsolation_NH(rho) < 1.0 + 0.04 * pt &&
				pfIsolation_G(rho) < 0.7 + 0.005 * pt
			)
			passesIso = true;
	} else {
		if (
				pfIsolation_CH(rho) < 1.2 &&
				pfIsolation_NH(rho) < 1.5 + 0.04 * pt &&
				pfIsolation_G(rho) < 1.0 + 0.005 * pt
			)
			passesIso = true;
	}

	if ( isEB() && !isInCrack() && isMedium && passesIso && !getVarB("egn_isConv") && getVarF("egn_r9") > 0.9 ) {
		return true;
	}

//	if (!isEB())
//		cout << "Fails isEB()" << endl;
//	else if (!passesID)
//		cout << "Fails ID" << endl;
//	else if (!passesIso)
//		cout << "Fails ISO" << endl;
//	else if (getVarB("egn_isConv"))
//		cout << "Fails conversion veto" << endl;
//	else if (getVarF("egn_r9") < 0.9)
//		cout << "Fails R9 cut" << endl;
//
	return false;
}
