#ifndef TRIGGERINFO_H
#define TRIGGERINFO_H

#include <string>
#include <vector>
#include "TObject.h"

class TriggerInfo : public TObject {
	public:
		std::string name;
		unsigned version;
		int accept;
		int prescale;
		std::vector<float> TO_PT;
		std::vector<float> TO_ETA;
		std::vector<float> TO_PHI;
		TriggerInfo();
		TriggerInfo(const std::string & n, unsigned ver, int a, int pres,
				const std::vector<float> & TOPT,
				const std::vector<float> & TOETA,
				const std::vector<float> & TOPHI);
		~TriggerInfo();

		ClassDef(TriggerInfo,1);
};

//ClassImp(TriggerInfo);

#endif
