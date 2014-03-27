#include "jet.h"
#include "TLorentzVector.h"

TLorentzVector Jet::lorentzVector() const {
	return TLorentzVector(getVarF("jn_px"), getVarF("jn_py"), getVarF("jn_pz"), getVarF("jn_en"));
}

bool Jet::passesPUID() const {
	return (getVarI("jn_idbits") & (0x1 << 9));
}

bool Jet::passesPFLooseID() const {
	return (getVarI("jn_idbits") & (0x1 << 0));
}
