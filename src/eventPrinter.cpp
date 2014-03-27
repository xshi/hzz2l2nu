#include "eventPrinter.h"
#include "event.h"
#include "electron.h"
#include "toolbox.h"
#include "muon.h"
#include <fstream>
#include "TLorentzVector.h" 
#include <iomanip>
#include <sstream>
#include "jet.h"

using std::stringstream;
using std::setw;
using std::ofstream;
using std::string;
using std::cout;
using std::endl;

EventPrinter::EventPrinter(const Event & ev, PreselType t, const string & fN) {
	run = ev.getSVA<int>("run");
	lumi = ev.getSVA<int>("lumi");
	event = ev.getSVA<int>("event");
	if (fN != "") {
		outputFileName = fN;
		output = new ofstream(outputFileName.c_str());
	} else
		output = &cout;
	floatVars.push_back( ev.getSVA<float>("rho") );
//	intVars.push_back( ev.getSVA<int>("cat") );
	floatVarsNames.push_back( "rho" );
//	intVarsNames.push_back( "cat" );
	printEle = false;
	electrons = 0;
	printMu = false;
	muons = 0;
	printZ = false;
	printJets_ = false;
	jets = 0;
	useList = false;
	type_ = t;
}

EventPrinter::~EventPrinter() {
	if (outputFileName == "")
		delete output;
}

void EventPrinter::printHeader() const {
	(*output) << setw(15) << "RUN" << setw(15) << "LUMI" << setw(15) << "EVENT";
	(*output) << setw(10) << "BITS";
	(*output) << setw(10) << "nJets";
	(*output) << setw(10) << "MET";
	(*output) << setw(10) << "MT";
	(*output) << setw(15) << "CAT";

	for (unsigned i = 0; i < floatVarsNames.size(); ++i)
		(*output) << setw(10) << floatVarsNames[i];
	for (unsigned i = 0; i < intVarsNames.size(); ++i)
		(*output) << setw(10) << intVarsNames[i];
	if (printEle) {
		(*output) << setw(10) << "Ele Pt";
		(*output) << setw(10) << "Ele Eta";
		(*output) << setw(10) << "Ele E";
		(*output) << setw(10) << "Ele px";
		(*output) << setw(10) << "Ele py";
		(*output) << setw(10) << "Ele pz";
		(*output) << setw(10) << "sceta";
		//		(*output) << setw(10) << "isInCrack";
		(*output) << setw(10) << "MediumID";
		(*output) << setw(10) << "LooseID";
		(*output) << setw(10) << "pfIso";
		//		(*output) << setw(10) << "gIso";
		//		(*output) << setw(10) << "chIso";
		//		(*output) << setw(10) << "nhIso";
		(*output) << setw(10) << "isEB";
		//		(*output) << setw(10) << "detain";
		//		(*output) << setw(10) << "dphiin";
		//		(*output) << setw(10) << "sihih";
		//		(*output) << setw(10) << "hoe";
		//		(*output) << setw(10) << "d0";
		//		(*output) << setw(10) << "dZ";
		//		(*output) << setw(10) << "ooemoop";
		//		(*output) << setw(10) << "VFP";
		//		(*output) << setw(10) << "trkLostIn";
	}
	if (printMu) {
		(*output) << setw(10) << "Mu PT";
		(*output) << setw(10) << "Mu ETA";
		(*output) << setw(10) << "Mu PHI";
		(*output) << setw(10) << "nMatches";
		(*output) << setw(10) << "innerTrac";
		(*output) << setw(10) << "TightID";
		(*output) << setw(10) << "LooseID";
		(*output) << setw(10) << "SoftID";
		(*output) << setw(10) << "pfIso";
	}
	if (printZ) {
		(*output) << setw(10) << "Z MASS";
		(*output) << setw(10) << "Z PT";
		(*output) << setw(10) << "Z ETA";
	}
	if (printJets_) {
		(*output) << setw(10) << "J En";
		(*output) << setw(10) << "J PT";
		(*output) << setw(10) << "J ETA";
		(*output) << setw(10) << "J DETA";
		(*output) << setw(10) << "J PHI";
		(*output) << setw(10) << "J BTAG";
		(*output) << setw(10) << "J DPHI";
	}
	(*output) << endl;
	(*output) << string(200, '-') << endl;
}

bool EventPrinter::selectedEvent() const {
	if ( useList && selectedEvents.find( EventAddress(*run, *lumi, *event) ) != selectedEvents.end() )
		return true;
	return false;
}

void EventPrinter::print() const {
	if ( useList && selectedEvents.find( EventAddress(*run, *lumi, *event) ) == selectedEvents.end() )
		return;
	output->precision(4);
	(*output) << std::fixed;
	int lineLength = 0;
	(*output) << setw(15) << (*run) << setw(15) << (*lumi) << setw(15) << (*event);
	lineLength += 45;
	(*output) << setw(10) << bits << setw(10) << jets->size() << setw(10) << met.Pt() << setw(10) << mt << setw(15) << channel;
	lineLength += 55;
	for (unsigned i = 0; i < floatVars.size(); ++i) {
		(*output) << setw(10) << (*floatVars[i]);
		lineLength += 10;
	}
	for (unsigned i = 0; i < intVars.size(); ++i) {
		(*output) << setw(10) << (*intVars[i]);
		lineLength += 10;
	}
	unsigned maxObjects = 0;
	int electronsLength = 0;
	int muonsLength = 0;
	if (printEle) {
		if (!electrons)
			throw string("ERROR: Empty electrons collection!");
		maxObjects = max( maxObjects, electrons->size() );
		electronsLength = 230;
	}
	if (printMu) {
		if (!muons)
			throw string("ERROR: Empty muons collection!");
		maxObjects = max( maxObjects, muons->size() );
		muonsLength = 90;
	}
	if (printZ) {
		maxObjects = max( maxObjects, 1 );
	}
	if (printJets_) {
		if (!jets)
			throw string("ERROR: Empty jets collection!");
		maxObjects = max( maxObjects, jets->size() );
	}

	for (unsigned i = 0; i < maxObjects; ++i) {
		if (i > 0)
			(*output) << setw(lineLength) << "";
		if (printEle) {
			if (i < electrons->size() ) {
				(*output) << setw(10) << (*electrons)[i].lorentzVector().Pt();
				(*output) << setw(10) << (*electrons)[i].lorentzVector().Eta();
				(*output) << setw(10) << (*electrons)[i].getVarF("ln_en");
				(*output) << setw(10) << (*electrons)[i].getVarF("ln_px");
				(*output) << setw(10) << (*electrons)[i].getVarF("ln_py");
				(*output) << setw(10) << (*electrons)[i].getVarF("ln_pz");
				(*output) << setw(10) << (*electrons)[i].getVarF("ln_sceta");
				(*output) << setw(10) << (*electrons)[i].passesMediumID();
				(*output) << setw(10) << (*electrons)[i].passesLooseID();
				(*output) << setw(10) << (*electrons)[i].pfIsolation(*floatVars[1]);
				(*output) << setw(10) << (*electrons)[i].isEB();
			} else
				(*output) << setw(electronsLength) << "";
		}
		if (printMu) {
			if (i < muons->size() ) {
				(*output) << setw(10) << (*muons)[i].lorentzVector().Pt();
				(*output) << setw(10) << (*muons)[i].lorentzVector().Eta();
				(*output) << setw(10) << (*muons)[i].lorentzVector().Phi();
				(*output) << setw(10) << (*muons)[i].getVarF("ln_nMatches");
				(*output) << setw(10) << (*muons)[i].getVarF("ln_innerTrackChi2");
				(*output) << setw(10) << (*muons)[i].isTightMuon();
				(*output) << setw(10) << (*muons)[i].isLooseMuon();
				(*output) << setw(10) << (*muons)[i].isSoftMuon();
				(*output) << setw(10) << (*muons)[i].pfIsolation();
			} else
				(*output) << setw(muonsLength) << "";
		}
		if (printZ) {
			if (i == 0) {
				TLorentzVector lep1;
				TLorentzVector lep2;
				if (type_ == ELE) {
					lep1 = (*electrons)[0].lorentzVector();
					lep2 = (*electrons)[1].lorentzVector();
				} else if (type_ == MU) {
					lep1 = (*muons)[0].lorentzVector();
					lep2 = (*muons)[1].lorentzVector();
				}
				TLorentzVector Zcand = lep1 + lep2;
				(*output) << setw(10) << Zcand.M();
				(*output) << setw(10) << Zcand.Pt();
				(*output) << setw(10) << Zcand.Eta();
			} else
				(*output) << setw(30) << "";
		}
		if (printJets_) {
			if (i < jets->size() ) {
				(*output) << setw(10) << (*jets)[i].lorentzVector().E();
				(*output) << setw(10) << (*jets)[i].lorentzVector().Pt();
				(*output) << setw(10) << (*jets)[i].lorentzVector().Eta();
				if (i)
					(*output) << setw(10) << (*jets)[i].lorentzVector().Eta() - (*jets)[i - 1].lorentzVector().Eta();
				else
					(*output) << setw(10) << "";
				(*output) << setw(10) << (*jets)[i].lorentzVector().Phi();
				(*output) << setw(10) << (*jets)[i].getVarF("jn_jp");
				(*output) << setw(10) << deltaPhi((*jets)[i].lorentzVector().Phi(), met.Phi());
			}
		}
		if (i < maxObjects - 1)
			(*output) << endl;
	}
	(*output) << endl;
	//(*output) << string(200, '-') << endl;
}

void EventPrinter::readInEvents(const string & inFileName) {
	ifstream input(inFileName.c_str());

	if (!input.is_open())
		throw string("ERROR: Can't open input file: " + inFileName + "!");

	while (input.good()) {
		string tmpLine;
		getline(input, tmpLine);
		if (!tmpLine.size() || tmpLine[0] == '#')
			continue;
		stringstream ss;
		ss << tmpLine;
		unsigned r;
		ss >> r;
		unsigned l;
		ss >> l;
		unsigned e;
		ss >> e;
		selectedEvents.insert( EventAddress(r, l, e) );
	}
	useList = true;
}

void EventPrinter::printSelectedEvents() const {
	for (auto iter = selectedEvents.begin(); iter != selectedEvents.end(); ++iter)
		cout << *iter << endl;
}

EventPrinter::EventAddress::EventAddress() : run(0), lumi(0), event(0) {}

EventPrinter::EventAddress::EventAddress(unsigned r, unsigned l, unsigned e)
	: run(r), lumi(l), event(e) {}

bool EventPrinter::EventAddress::operator==(const EventAddress & ev) const {
	return run == ev.run && lumi == ev.lumi && event == ev.event;
}

bool EventPrinter::EventAddress::operator<(const EventAddress & ev) const {
	if (event == ev.event) {
		if (lumi == ev.lumi)
			return run < ev.run;
		else
			return lumi < ev.lumi;
	} else
		return event < ev.event;
	return run < ev.run && lumi == ev.lumi && event == ev.event;
}

ostream & operator<<(ostream & os, const EventPrinter::EventAddress & ev) {
	os << setw(10) << ev.run << setw(10) << ev.lumi << setw(10) << ev.event;
	return os;
}
