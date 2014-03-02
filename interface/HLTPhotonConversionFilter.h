#ifndef HLTPhotonConversionFilter_h
#define HLTPhotonConversionFilter_h

/** \class HLTPhotonConversionFilter
 *
 *  \author Xin Shi <Xin.Shi@cern.ch> 
 *
 */

#include "HLTrigger/HLTcore/interface/HLTFilter.h"

//
// class decleration
//

class HLTPhotonConversionFilter : public HLTFilter {

   public:
      explicit HLTPhotonConversionFilter(const edm::ParameterSet&);
      ~HLTPhotonConversionFilter();
      virtual bool hltFilter(edm::Event&, const edm::EventSetup&, trigger::TriggerFilterObjectWithRefs & filterproduct);
      

   private:
      edm::InputTag inputTag_; // input tag identifying product contains egammas
      double etcutEB_;           // Barrel Et threshold in GeV 
      double etcutEE_;           // Endcap Et threshold in GeV 
      int    ncandcut_;        // number of egammas required
      bool   relaxed_;
      edm::InputTag L1IsoCollTag_; 
      edm::InputTag L1NonIsoCollTag_; 
      edm::InputTag photonTag_; // input photonSource tag

};

#endif //HLTPhotonConversionFilter_h
