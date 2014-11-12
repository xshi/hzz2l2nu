#ifndef PRESELECTIONCMG_H
#define PRESELECTIONCMG_H

// Standard Libraries
#include <vector>
#include <string>
#include <tuple>
#include "event.h"
#include <cmath>

enum PreselType { ELE, MU, EMU, PHOT }; 

class Lepton;
class Electron;
class ElectronVariables;
class Event;
class Jet;
class Muon;
class Photon;
class TLorentzVector;
class RooWorkspace;

void LeptonPreselectionCMG( PreselType type, RooWorkspace * massPeak = nullptr );
Jet smearedJet(const Jet & origJet, unsigned mode);
TLorentzVector smearJets(std::vector<Jet> & jets, unsigned mode = 0);
std::vector<Photon> selectPhotonsCMG(const Event & ev);

template<typename T, int ID>
std::vector<T> buildLeptonCollection( const Event & ev,
		const std::vector< std::tuple<std::string, std::string> > & lepVars,
		const std::vector< std::tuple<std::string, std::string> > & addLepVars) {

	std::vector< const ArrayVariableContainer<float> * > lepArrPtrF;
	std::vector< const ArrayVariableContainer<int> * > lepArrPtrI;
	std::vector< const ArrayVariableContainer<bool> * > lepArrPtrB;
	for (unsigned i = 0; i < lepVars.size(); ++i) {
		std::string varType = std::get<1>(lepVars[i]);
		std::string varName = std::get<0>(lepVars[i]);
		if (varType == "F")
			lepArrPtrF.push_back( dynamic_cast<const ArrayVariableContainer<float> *>( ev.findVariable( varName ) ) );
		else if (varType == "I")
			lepArrPtrI.push_back( dynamic_cast<const ArrayVariableContainer<int> *>( ev.findVariable( varName ) ) );
		else if (varType == "B")
			lepArrPtrB.push_back( dynamic_cast<const ArrayVariableContainer<bool> *>( ev.findVariable( varName ) ) );
	}

	std::vector< const ArrayVariableContainer<float> * > addLepArrPtrF;
	std::vector< const ArrayVariableContainer<int> * > addLepArrPtrI;
	std::vector< const ArrayVariableContainer<bool> * > addLepArrPtrB;
	for (unsigned i = 0; i < addLepVars.size(); ++i) {
		std::string varType = std::get<1>(addLepVars[i]);
		std::string varName = std::get<0>(addLepVars[i]);
		if (varType == "F")
			addLepArrPtrF.push_back( dynamic_cast<const ArrayVariableContainer<float> *>( ev.findVariable( varName ) ) );
		else if (varType == "I")
			addLepArrPtrI.push_back( dynamic_cast<const ArrayVariableContainer<int> *>( ev.findVariable( varName ) ) );
		else if (varType == "B")
			addLepArrPtrB.push_back( dynamic_cast<const ArrayVariableContainer<bool> *>( ev.findVariable( varName ) ) );
	}
	
	std::vector<T> leptons;

	const SingleVariableContainer<int> * lnC = dynamic_cast<const SingleVariableContainer<int> *>(ev.findVariable("ln"));
	const ArrayVariableContainer<int> * id = dynamic_cast<const ArrayVariableContainer<int> *>(ev.findVariable("ln_id"));
	int ln = lnC->getVal();
	const ArrayVariableContainer<int> * pidC = dynamic_cast<const ArrayVariableContainer<int> *>(ev.findVariable("ln_pid"));
	for (int i = 0; i < ln; ++i) {
		if (std::fabs(id->getVal(i)) == ID) {
			T tmp;
			for (unsigned j = 0; j < lepArrPtrF.size(); j++) {
				tmp.addFloatVar( lepArrPtrF[j]->getName(), lepArrPtrF[j]->getVal(i) );
			}
			for (unsigned j = 0; j < lepArrPtrI.size(); j++) {
				tmp.addIntVar( lepArrPtrI[j]->getName(), lepArrPtrI[j]->getVal(i) );
			}
			for (unsigned j = 0; j < lepArrPtrB.size(); j++) {
				tmp.addBoolVar( lepArrPtrB[j]->getName(), lepArrPtrB[j]->getVal(i) );
			}

			int pid = pidC->getVal(i);

			for (unsigned j = 0; j < addLepArrPtrF.size(); j++) {
				tmp.addFloatVar( addLepArrPtrF[j]->getName(), addLepArrPtrF[j]->getVal(pid) );
			}
			for (unsigned j = 0; j < addLepArrPtrI.size(); j++) {
				tmp.addIntVar( addLepArrPtrI[j]->getName(), addLepArrPtrI[j]->getVal(pid) );
			}
			for (unsigned j = 0; j < addLepArrPtrB.size(); j++) {
				tmp.addBoolVar( addLepArrPtrB[j]->getName(), addLepArrPtrB[j]->getVal(pid) );
			}

			leptons.push_back(tmp);
		}
	}
	return leptons;
}

//#define CMSSWENV
#ifdef CMSSWENV

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
std::vector<Jet> selectJetsCMG(const Event & ev, const std::vector<Lepton> & leptons, JetCorrectionUncertainty  & jecUnc, TLorentzVector * diff = 0, unsigned mode = 0, double ptMin = 10, double etaMax = 4.7);

#else

std::vector<Jet> selectJetsCMG(const Event & ev, const std::vector<Lepton> & leptons, TLorentzVector * diff = 0, unsigned mode = 0, double ptMin = 10, double etaMax = 4.7);

#endif

#endif
