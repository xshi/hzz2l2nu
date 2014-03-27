#include "lepton.h"
#include "TLorentzVector.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

using std::string;

TLorentzVector Lepton::lorentzVector() const {
	return TLorentzVector(getVarF("ln_px"), getVarF("ln_py"), getVarF("ln_pz"), getVarF("ln_en"));
}

void Lepton::addFloatVar(const string & name, float value) {
	varsF[name] = value;
}

void Lepton::addIntVar(const string & name, int value) {
	varsI[name] = value;
}

void Lepton::addBoolVar(const string & name, bool value) {
	varsB[name] = value;
}

float Lepton::getVarF(const std::string & name) const {
	try {
		return varsF.at(name);
	} catch (const std::out_of_range & exc){
		throw string("ERROR Lepton::getVarF(const std::string &): Can't find variable " + name + "!");
	}
}

int Lepton::getVarI(const std::string & name) const {
	try {
		return varsI.at(name);
	} catch (const std::out_of_range & exc){
		throw string("ERROR Lepton::getVarI(const std::string &): Can't find variable " + name + "!");
	}
}

bool Lepton::getVarB(const std::string & name) const {
	try {
		return varsB.at(name);
	} catch (const std::out_of_range & exc){
		throw string("ERROR Lepton::getVarB(const std::string &): Can't find variable " + name + "!");
	}
}

void Lepton::print() const {
	for (auto iter = varsF.begin(); iter != varsF.end(); ++iter)
		std::cout <<  (*iter).first << " = " << (*iter).second << std::endl;
	for (auto iter = varsI.begin(); iter != varsI.end(); ++iter)
		std::cout <<  (*iter).first << " = " << (*iter).second << std::endl;
	for (auto iter = varsB.begin(); iter != varsB.end(); ++iter)
		std::cout <<  (*iter).first << " = " << (*iter).second << std::endl;
}
