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
      edm::InputTag photonTag_; 
      edm::InputTag electronTag_; 
};

#endif //HLTPhotonConversionFilter_h
