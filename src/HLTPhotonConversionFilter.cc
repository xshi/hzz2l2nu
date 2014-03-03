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
  photonTag_ = iConfig.getParameter< edm::InputTag > ("photonTag");
  electronTag_ = iConfig.getParameter< edm::InputTag > ("electronTag");
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
  iEvent.getByLabel( electronTag_, gsfEleH );

  edm::Handle<reco::ConversionCollection> convH;
  edm::Handle<reco::BeamSpot> beamSpotH;

  for(size_t ipho=0; ipho<photonH->size(); ipho++)
    {
      const reco::Photon *pho = dynamic_cast<const reco::Photon *>( photonH->ptrAt(ipho).get() );

      if(pho==0) continue;

      matchesElectron = ConversionTools::hasMatchedPromptElectron(pho->superCluster(), gsfEleH, convH, beamSpotH->position());

    }
  // filter decision
  bool accept (matchesElectron || matchesMuon); 
  return accept;
}

