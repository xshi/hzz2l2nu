#include "photonPrescale.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

using std::map;
using std::ifstream;
using std::stringstream;
using std::string;
using std::ostream;

void PhotonPrescale::addTrigger(const std::string & tN, double th, double offs, unsigned idx) {
	triggers.push_back( PhotonTrigger(tN, th, offs, idx) );
	sort( triggers.begin(), triggers.end(), [](const PhotonTrigger & trg1, const PhotonTrigger & trg2) {
			return trg1.getThreshold() < trg2.getThreshold();
			});
}

double PhotonPrescale::nextThreshold(double pt) const {
	for (unsigned i = 0; i < triggers.size(); ++i) {
		if (triggers[i].getThreshold() > pt)
			return triggers[i].getThreshold();
	}
	return 10e6;
}

unsigned PhotonPrescale::getIndex(double pt) {
	for (unsigned i = 0; i < triggers.size(); ++i) {
		if ( pt > triggers[i].getThreshold() + triggers[i].getOffset() ) {
			if (i < triggers.size() - 1) {
				if (pt < triggers[i + 1].getThreshold() + triggers[i + 1].getOffset())
					return triggers[i].getIndex();
			} else
				return triggers[i].getIndex();

		}
	}
	std::cout << pt << std::endl;
	throw string("ERROR: Can't find proper index!");
}
