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
  // candTag_ = iConfig.getParameter< edm::InputTag > ("candTag");
  photonTag_ = iConfig.getParameter< edm::InputTag > ("photonTag");
  electronTag_ = iConfig.getParameter< edm::InputTag > ("electronTag");
}

HLTPhotonConversionFilter::~HLTPhotonConversionFilter(){}


// ------------ method called to produce the data  ------------
bool
HLTPhotonConversionFilter::hltFilter(edm::Event& iEvent, const edm::EventSetup& iSetup, trigger::TriggerFilterObjectWithRefs & filterproduct)
{
  using namespace trigger;

 // Ref to Candidate object to be recorded in filter object
  edm::Ref<reco::RecoEcalCandidateCollection> ref;

  edm::Handle<trigger::TriggerFilterObjectWithRefs> PrevFilterOutput;

  // iEvent.getByLabel (candTag_,PrevFilterOutput);
  iEvent.getByLabel (photonTag_,PrevFilterOutput);

  std::vector<edm::Ref<reco::RecoEcalCandidateCollection> > recoecalcands;
  PrevFilterOutput->getObjects(TriggerCluster, recoecalcands);
  if(recoecalcands.empty()) PrevFilterOutput->getObjects(TriggerPhoton,recoecalcands);  //we dont know if its type trigger cluster or trigger photon

 // look at all photons, check cuts and add to filter object
  // int n = 0;
  
  bool matches(false);
  edm::Handle<reco::ConversionCollection> convH;
  edm::Handle<reco::BeamSpot> beamSpotH;

  // edm::Handle<reco::GsfElectronCollection> gsfEleH;
  // iEvent.getByLabel( electronTag_, gsfEleH );


  for (unsigned int i=0; i<recoecalcands.size(); i++) {
    
    ref = recoecalcands[i];
    matches = ConversionTools::hasMatchedConversion(*(ref->superCluster()), convH, beamSpotH->position()); 
    // matches = ConversionTools::hasMatchedPromptElectron(ref->superCluster(),							gsfEleH, convH, beamSpotH->position()); 
  }

  // look at all electrons
  // std::vector<edm::Ref<reco::ElectronCollection> > elecands;
  // PrevFilterOutput->getObjects(TriggerElectron, elecands);


 

  // bool matchesElectron(false); 
  // bool matchesMuon(false); 

  // edm::Handle<edm::View<reco::Candidate> > photonH; 
  // iEvent.getByLabel( photonTag_, photonH );

  // edm::Handle<reco::GsfElectronCollection> gsfEleH;
  // iEvent.getByLabel( electronTag_, gsfEleH );

  // edm::Handle<reco::ConversionCollection> convH;
  // edm::Handle<reco::BeamSpot> beamSpotH;

  // for(size_t ipho=0; ipho<photonH->size(); ipho++)
  //   {
  //     const reco::Photon *pho = dynamic_cast<const reco::Photon *>( photonH->ptrAt(ipho).get() );

  //     if(pho==0) continue;

  //     matchesElectron = ConversionTools::hasMatchedPromptElectron(pho->superCluster(), gsfEleH, convH, beamSpotH->position());

  //   }
  // filter decision
  // bool accept (matchesElectron || matchesMuon); 
  return matches; 
}

