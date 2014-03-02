/** \class HLTPhotonConversionFilter
 *
 *
 *  \author Xin Shi <Xin.Shi@cern.ch>
 *
 */

#include "HLTrigger/Egamma/interface/HLTPhotonConversionFilter.h"

#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"



//
// constructors and destructor
//
HLTPhotonConversionFilter::HLTPhotonConversionFilter(const edm::ParameterSet& iConfig) : HLTFilter(iConfig) 
{
  inputTag_ = iConfig.getParameter< edm::InputTag > ("inputTag");
  etcutEB_  = iConfig.getParameter<double> ("etcutEB");
  etcutEE_  = iConfig.getParameter<double> ("etcutEE");
  ncandcut_  = iConfig.getParameter<int> ("ncandcut");
  relaxed_ = iConfig.getUntrackedParameter<bool> ("relaxed",true) ;
  L1IsoCollTag_= iConfig.getParameter< edm::InputTag > ("L1IsoCand"); 
  L1NonIsoCollTag_= iConfig.getParameter< edm::InputTag > ("L1NonIsoCand"); 
  photonTag_ = iConfig.getParameter< edm::InputTag > ("photonSource");
}

HLTPhotonConversionFilter::~HLTPhotonConversionFilter(){}


// ------------ method called to produce the data  ------------
bool
HLTPhotonConversionFilter::hltFilter(edm::Event& iEvent, const edm::EventSetup& iSetup, trigger::TriggerFilterObjectWithRefs & filterproduct)
{
  using namespace trigger;

  bool matchesElectron(false); 
  bool matchesMuon(false); 

  edm::Handle<edm::View<reco::Candidate> > photonH; 
  iEvent.getByLabel( photonTag_, photonH );

  edm::Handle<reco::GsfElectronCollection> gsfEleH;
  iEvent.getByLabel( "gsfElectrons", gsfEleH );

  edm::Handle<reco::ConversionCollection> convH;
  edm::Handle<reco::BeamSpot> beamSpotH;

  for(size_t ipho=0; ipho<photonH->size(); ipho++)
    {
      const reco::Photon *pho = dynamic_cast<const reco::Photon *>( photonH->ptrAt(ipho).get() );

      if(pho==0) continue;

      matchesElectron = ConversionTools::hasMatchedPromptElectron(pho->superCluster(), gsfEleH, convH, beamSpotH->position());

    }


  // // The filter object
  // if (saveTags()) {
  //   filterproduct.addCollectionTag(L1IsoCollTag_);
  //   if (relaxed_) filterproduct.addCollectionTag(L1NonIsoCollTag_);
  // }

  // // Ref to Candidate object to be recorded in filter object
  //  edm::Ref<reco::RecoEcalCandidateCollection> ref;

  // // get hold of filtered candidates
  // //edm::Handle<reco::HLTFilterObjectWithRefs> recoecalcands;
  // edm::Handle<trigger::TriggerFilterObjectWithRefs> PrevFilterOutput;

  // iEvent.getByLabel (inputTag_,PrevFilterOutput);

  // std::vector<edm::Ref<reco::RecoEcalCandidateCollection> > recoecalcands;                // vref with your specific C++ collection type
  // PrevFilterOutput->getObjects(TriggerCluster, recoecalcands);
  // if(recoecalcands.empty()) PrevFilterOutput->getObjects(TriggerPhoton,recoecalcands);  //we dont know if its type trigger cluster or trigger photon
  
  // // look at all candidates,  check cuts and add to filter object
  // int n(0);

  // for (unsigned int i=0; i<recoecalcands.size(); i++) {
    
  //   ref = recoecalcands[i] ;
    
  //   // std::cout << "superCluster: " << ref->superCluster() << std::endl; 

  //   if( ( fabs(ref->eta()) < 1.479 &&  ref->et()  >= etcutEB_ ) || ( fabs(ref->eta()) >= 1.479 &&  ref->et()  >= etcutEE_ ) ){
  //     n++;
  //     // std::cout << "Passed eta: " << ref->eta() << std::endl;
  //     filterproduct.addObject(TriggerCluster, ref);
   
  //   }
  // }
  
  
  // filter decision
  bool accept (matchesElectron || matchesMuon); 
  // bool accept(n>=ncandcut_);
  
  return accept;
}
