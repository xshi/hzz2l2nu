#ifndef EVENTPRINTER_H
#define EVENTPRINTER_H

#include<string>
#include<iostream>
#include<memory>
#include<vector>
#include<set>
#include "preselectionCMG.h"
#include <TLorentzVector.h>

class Event;
class Electron;
class Muon;
class Jet;

class EventPrinter {
	private :
		class EventAddress {
			private :
				unsigned run;
				unsigned lumi;
				unsigned event;
			public :
				EventAddress();
				EventAddress(unsigned r, unsigned l, unsigned e);
				bool operator==(const EventAddress & ev) const;
				bool operator<(const EventAddress & ev) const;
				friend std::ostream & operator<<(std::ostream & os, const EventAddress & ev);
		};
		const int * run;
		const int * lumi;
		const int * event;
		std::string outputFileName;
		std::ostream * output;
		std::vector<const float *> floatVars;
		std::vector<const int *> intVars;
		std::vector<std::string> floatVarsNames;
		std::vector<std::string> intVarsNames;
		bool printEle;
		const std::vector<Electron> * electrons;
		bool printMu;
		const std::vector<Muon> * muons;
		bool printZ;
		bool useList;
		bool printJets_;
		const std::vector<Jet> * jets;
		PreselType type_;	
		std::set<EventAddress> selectedEvents;
		EventPrinter(const EventPrinter &);
		bool operator=(const EventPrinter &);
		friend std::ostream & operator<<(std::ostream & os, const EventAddress & ev);
		unsigned bits;
		TLorentzVector met;
		double mt;
		std::string channel;
	public :
		EventPrinter(const Event & ev, PreselType t, const std::string & fN = "");
		~EventPrinter();
		void print() const;
		void printHeader() const;
		void printElectrons() {
			printEle = true;
		}
		void setElectronCollection(const std::vector<Electron> & ele) {
			electrons = &ele;
		}
		void printMuons() {
			printMu = true;
		}
		void setMuonCollection(const std::vector<Muon> & mu) {
			muons = &mu;
		}
		void printZboson() {
			printZ = true;
		}
		void printJets() {
			printJets_ = true;
		}
		void setJetCollection(const std::vector<Jet> & j) {
			jets = &j;
		}
		void readInEvents(const std::string & inFileName);
		void printSelectedEvents() const;
		void setMET(const TLorentzVector & met_) {
			met = met_;
		}
		void setBits(unsigned bits_) {
			bits = bits_;
		}
		void setMT(double mt_) {
			mt = mt_;
		}
		void setChannel(const std::string & channel_) {
			channel = channel_;
		}
		bool selectedEvent() const;
};

#endif // EVENTPRINTER_H
