#include "triggerinfo.h"

TriggerInfo::TriggerInfo() {}
TriggerInfo::TriggerInfo(const std::string & n, unsigned ver, int a, int pres,
		const std::vector<float> & TOPT,
		const std::vector<float> & TOETA,
		const std::vector<float> & TOPHI) : name(n), version(ver), prescale(pres),
	TO_PT(TOPT),
	TO_ETA(TOETA),
	TO_PHI(TOPHI) {}
TriggerInfo::~TriggerInfo() {}
