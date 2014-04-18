#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include "UserCode/llvv_fwk/interface/MacroUtils.h"
#include "UserCode/llvv_fwk/interface/SmartSelectionMonitor.h"
#include "UserCode/llvv_fwk/interface/DataEventSummaryHandler.h"
#include "UserCode/llvv_fwk/interface/TMVAUtils.h"
#include "UserCode/llvv_fwk/interface/LeptonEfficiencySF.h"
#include "UserCode/llvv_fwk/interface/PDFInfo.h"
#include "UserCode/llvv_fwk/interface/MuScleFitCorrector.h"
#include "UserCode/llvv_fwk/interface/GammaWeightsHandler.h"
#include "UserCode/llvv_fwk/interface/HiggsUtils.h"
#include "UserCode/llvv_fwk/interface/BtagUncertaintyComputer.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TEventList.h"
#include "TROOT.h"
#include "TEfficiency.h" 

using namespace std;

TString getJetRegion(float eta)
{
  TString reg("TK");
  if(fabs(eta)>2.5)  reg="HEin";
  if(fabs(eta)>2.75) reg="HEout";
  if(fabs(eta)>3)    reg="HF";
  return reg;
}

int main(int argc, char* argv[])
{
  // ------------------------------------------------
  //    global initialization
  // ------------------------------------------------


  // check arguments
  if(argc<2){ std::cout << "Usage : " << argv[0] << " hzz2l2nu_cfg.py" << std::endl; exit(0); }
  
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  // configure the process
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");

  bool isMC       = runProcess.getParameter<bool>("isMC");
  // int mctruthmode = runProcess.getParameter<int>("mctruthmode");


  TString suffix=runProcess.getParameter<std::string>("suffix");
  std::vector<std::string> urls=runProcess.getParameter<std::vector<std::string> >("input");
  TString url=TString(urls[0]);
  TString outFileUrl(gSystem->BaseName(url)); 
  outFileUrl.ReplaceAll(".root",""); 

  outFileUrl+=suffix;
  // if(mctruthmode!=0) { outFileUrl += "_filt"; outFileUrl += mctruthmode; }
  TString outdir=runProcess.getParameter<std::string>("outdir");
  TString outUrl( outdir );
  
  gSystem->Exec("mkdir -p " + outUrl);

  // TString outTxtUrl= outUrl + "/" + outFileUrl + ".txt";
  // FILE* outTxtFile = NULL;
  //if(!isMC)
  // outTxtFile = fopen(outTxtUrl.Data(), "w");
  // printf("TextFile URL = %s\n",outTxtUrl.Data());

  //tree info
  TString dirname = runProcess.getParameter<std::string>("dirName");


  // --------------------------------------------------
  //  initiating histograms
  // --------------------------------------------------

  SmartSelectionMonitor mon;

  //generator level control : add an underflow entry to make sure the histo is kept
  mon.addHistogram( new TH1F( "wdecays",     ";W decay channel",5,0,5) );
  mon.addHistogram( new TH1F( "zdecays",     ";Z decay channel",6,0,6) );

  //event selection
  TH1F* Hcutflow  = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,6,0,6) ) ;
  TH1F *h=(TH1F*) mon.addHistogram( new TH1F ("eventflow", ";;Events", 8,0,8) );
  h->GetXaxis()->SetBinLabel(1,"#geq 2 iso leptons");
  h->GetXaxis()->SetBinLabel(2,"|M-91|<15");
  h->GetXaxis()->SetBinLabel(3,"p_{T}>55");
  h->GetXaxis()->SetBinLabel(4,"3^{rd}-lepton veto");
  h->GetXaxis()->SetBinLabel(5,"b-veto"); 
  h->GetXaxis()->SetBinLabel(6,"#Delta #phi(jet,E_{T}^{miss})>0.5");
  h->GetXaxis()->SetBinLabel(7,"E_{T}^{miss}>80");

  //pu control
  mon.addHistogram( new TH1F( "nvtx",";Vertices;Events",50,0,50) ); 
  mon.addHistogram( new TH1F( "nvtxraw",";Vertices;Events",50,0,50) ); 
  mon.addHistogram( new TH1F( "rho",";#rho;Events",50,0,25) ); 

  // trigger paths
  vector<string> trigs = runProcess.getParameter<vector<string> >("triggerPaths");

  // const int ntrigs = trigs.size(); 

  TH1D *h_total = new TH1D("h_total","total", trigs.size(), 0, trigs.size());   
  TH1D *h_pass = new TH1D("h_pass","pass", trigs.size(), 0, trigs.size());

  int n_trig_8_total = 0; 
  int n_trig_8_pass = 0; 

  //##############################################
  //######## GET READY FOR THE EVENT LOOP ########
  //##############################################

  //open the file and get events tree
  DataEventSummaryHandler evSummaryHandler;
  TFile *file = TFile::Open(url);
  printf("Looping on %s\n",url.Data());


  if(file==0) return -1;
  if(file->IsZombie()) return -1;
  if( !evSummaryHandler.attach( (TTree *) file->Get(dirname+"/data") , false) ) { file->Close();  return -1; }

  //check run range to compute scale factor (if not all entries are used)
  const Int_t totalEntries= evSummaryHandler.getEntries();
  
  cout << "total entries: " << totalEntries << endl; 

  //MC normalization (to 1/pb)
  float cnorm=1.0;
  if(isMC){
    TH1F* cutflowH = (TH1F *) file->Get(dirname+"/cutflow");
    if(cutflowH) cnorm=cutflowH->GetBinContent(1);
    printf("cnorm = %f\n",cnorm);
  }
  Hcutflow->SetBinContent(1,cnorm);


  //jet energy scale and uncertainties 
  // TString jecDir = runProcess.getParameter<std::string>("jecDir");
  // gSystem->ExpandPathName(jecDir);
  // FactorizedJetCorrector *jesCor        = utils::cmssw::getJetCorrector(jecDir,isMC);
  // JetCorrectionUncertainty *totalJESUnc = new JetCorrectionUncertainty((jecDir+"/MC_Uncertainty_AK5PFchs.txt").Data());
  
  //muon energy scale and uncertainties
  // MuScleFitCorrector *muCor=getMuonCorrector(jecDir,url);

  //lepton efficiencies
  //LeptonEfficiencySF lepEff;

  //b-tagging: beff and leff must be derived from the MC sample using the discriminator vs flavor
  //the scale factors are taken as average numbers from the pT dependent curves see:
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG#2012_Data_and_MC_EPS13_prescript
  BTagSFUtil btsfutil;
  float beff(0.68), sfb(0.99); // , sfbunc(0.015);
  float leff(0.13), sfl(1.05); // , sflunc(0.12);

  //pileup weighting
  // std::vector<double> dataPileupDistributionDouble = runProcess.getParameter< std::vector<double> >("datapileup");
  // std::vector<float> dataPileupDistribution; for(unsigned int i=0;i<dataPileupDistributionDouble.size();i++){dataPileupDistribution.push_back(dataPileupDistributionDouble[i]);}
  // std::vector<float> mcPileupDistribution;
  // if(isMC){
  //   TString puDist(dirname+"/pileup");
  //   TH1F* histo = (TH1F *) file->Get(puDist);
  //   if(!histo) std::cout<<"pileup histogram is null!!!\n";
  //   for(int i=1;i<=histo->GetNbinsX();i++){mcPileupDistribution.push_back(histo->GetBinContent(i));}
  //   delete histo;
  // }
  // while(mcPileupDistribution.size()<dataPileupDistribution.size())  mcPileupDistribution.push_back(0.0);
  // while(mcPileupDistribution.size()>dataPileupDistribution.size())dataPileupDistribution.push_back(0.0);
  
  gROOT->cd();  //THIS LINE IS NEEDED TO MAKE SURE THAT HISTOGRAM INTERNALLY PRODUCED IN LumiReWeighting ARE NOT DESTROYED WHEN CLOSING THE FILE
  // edm::LumiReWeighting *LumiWeights= isMC ? new edm::LumiReWeighting(mcPileupDistribution,dataPileupDistribution): 0;
  // utils::cmssw::PuShifter_t PuShifters;
  // if(isMC) { PuShifters=utils::cmssw::getPUshifters(dataPileupDistribution,0.05); }


  higgs::utils::EventCategory eventCategoryInst(higgs::utils::EventCategory::EXCLUSIVE2JETSVBF); //jet(0,>=1)+vbf binning

  
  
  //##############################################
  //########           EVENT LOOP         ########
  //##############################################
  //loop on all the events
  printf("Progressing Bar     :0%%       20%%       40%%       60%%       80%%       100%%\n");
  printf("Scanning the ntuple :");
  int treeStep(totalEntries/50);
  DuplicatesChecker duplicatesChecker;
  int nDuplicates(0);

  
  for( int iev=0; iev<totalEntries; iev++){
      if(iev%treeStep==0){printf(".");fflush(stdout);}

      //##############################################   EVENT LOOP STARTS   ##############################################
      //load the event content from tree
      evSummaryHandler.getEntry(iev);
      DataEventSummary &ev=evSummaryHandler.getEvent();
      if(!isMC && duplicatesChecker.isDuplicate( ev.run, ev.lumi, ev.event) ) { nDuplicates++; continue; }

      //physics objects
      data::PhysicsObjectCollection_t photons = evSummaryHandler.getPhysicsObject(DataEventSummaryHandler::PHOTONS);
      data::PhysicsObjectCollection_t leptons = evSummaryHandler.getPhysicsObject(DataEventSummaryHandler::LEPTONS);
      data::PhysicsObjectCollection_t jets    = evSummaryHandler.getPhysicsObject(DataEventSummaryHandler::JETS);
      data::PhysicsObjectCollection_t recoMet = evSummaryHandler.getPhysicsObject(DataEventSummaryHandler::MET);
      data::PhysicsObjectCollection_t gen     = evSummaryHandler.getPhysicsObject(DataEventSummaryHandler::GENPARTICLES);      
    
      //require compatibilitiy of the event with the PD
      bool eeTrigger          = ev.t_bits[0];
      // bool muTrigger          = ev.t_bits[6];
      bool mumuTrigger        = ev.t_bits[2] || ev.t_bits[3]; 
      bool emuTrigger         = ev.t_bits[4] || ev.t_bits[5];
      

      // fill the total triggers before selection
      //  'HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_v', itrig = 8 
      const int NTRIG = 8; 
      
      if (ev.t_bits[NTRIG]) {
	n_trig_8_total += 1; 
	h_total->Fill(NTRIG); 
      }
      
      bool hasPhotonTrigger(false);
      float triggerPrescale(1.0),triggerThreshold(0);
      // bool runPhotonSelection(mctruthmode==22 || mctruthmode==111);
      bool runPhotonSelection(true); 
      if(runPhotonSelection)
	{
	  eeTrigger=false; mumuTrigger=false;
	  for(size_t itrig=10; itrig>=7; itrig--)
	    {
	      if(!ev.t_bits[itrig]) continue;
	      hasPhotonTrigger=true;
	      triggerPrescale=ev.t_prescale[itrig];
	      if(itrig==10) triggerThreshold=92; //90
	      if(itrig==9)  triggerThreshold=77; //75
	      if(itrig==8)  triggerThreshold=50;
	      if(itrig==7)  triggerThreshold=36;
	      break;
	    }
	}


      
      //
      // DERIVE WEIGHTS TO APPLY TO SAMPLE
      //

      //pileup weight
      float weight = 1.0;
      double TotalWeight_plus = 1.0;
      double TotalWeight_minus = 1.0;
      float puWeight(1.0);
      // if(isMC){
      // 	puWeight          = LumiWeights->weight(ev.ngenITpu);
      // 	weight            = puWeight;
      //   TotalWeight_plus  = PuShifters[utils::cmssw::PUUP]->Eval(ev.ngenITpu);
      //   TotalWeight_minus = PuShifters[utils::cmssw::PUDOWN]->Eval(ev.ngenITpu);
      // }

      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
      Hcutflow->Fill(3,weight*TotalWeight_minus);
      Hcutflow->Fill(4,weight*TotalWeight_plus);
      Hcutflow->Fill(5,1.0);

      //
      //
      // BELOW FOLLOWS THE ANALYSIS OF THE MAIN SELECTION WITH N-1 PLOTS
      //
      //

      //
      // photon selection
      //
      data::PhysicsObjectCollection_t selPhotons;
      if(runPhotonSelection)
	{
	  //filter out number of prompt photons to avoid double counting
	 //  int ngenpho(0);
	  // 	  for(size_t igen=0; igen<gen.size(); igen++)
	  // 	    {
	  // 	      if(gen[igen].get("id")!=22 || gen[igen].get("status")!=1) continue;
	  // 	      float lxy=gen[igen].getVal("lxy");
	  // 	      if(lxy>0) continue;
	  // 	      ngenpho++;
	  //  }
	  //if(mctruthmode==111 && ngenpho>0) continue;
	  //if(mctruthmode==22 && ngenpho==0) continue;

	  //select the photons
	  for(size_t ipho=0; ipho<photons.size(); ipho++)
	    {
	      double pt=photons[ipho].pt();
	      double eta=photons[ipho].getVal("sceta");

	      //if systematics are active loosen the selection to the medium working point
	      Int_t idbits( photons[ipho].get("id") );
	      bool hasTightPhotonId( (idbits >> 2 ) & 0x1 );
	      double gIso    = photons[ipho].getVal("gIso03");
	      double gArea   = utils::cmssw::getEffectiveArea(22,eta,3,"gIso");	      
	      double chIso   = photons[ipho].getVal("chIso03");
	      double chArea  = utils::cmssw::getEffectiveArea(22,eta,3,"chIso");
	      double nhIso   = photons[ipho].getVal("nhIso03");
	      double nhArea  = utils::cmssw::getEffectiveArea(22,eta,3,"nhIso");
	      
	      //select the photon
	      if(pt<triggerThreshold || fabs(eta)>1.4442 ) continue;
	      bool passId(true);
	      if( photons[ipho].getVal("r9")<0.9 ) passId=false;
	      if(!hasTightPhotonId) passId=false;
	      if(!passId) continue;
	      bool passIso(true);
	      passIso &= (TMath::Max(chIso-chArea*ev.rho,0.0) < 0.7); 
	      passIso &= (TMath::Max(nhIso-nhArea*ev.rho,0.0) < 0.4+0.04*pt); 
	      passIso &= (TMath::Max(gIso-gArea*ev.rho,  0.0) < 0.5+0.005*pt); 
	      if(!passIso) continue; 
	      selPhotons.push_back(photons[ipho]);
	    }
	}



      //
      // LEPTON ANALYSIS
      //
      LorentzVector muDiff(0,0,0,0);
      data::PhysicsObjectCollection_t selLeptons, extraLeptons;
      for(size_t ilep=0; ilep<leptons.size(); ilep++)
	{
	  bool passKin(true),passId(true),passIso(true);
	  bool passLooseLepton(true), passSoftMuon(true);

	  int lid=leptons[ilep].get("id");

	  
	  // cout << "here: " << lid << endl; exit(0); 
	  //apply muon corrections
	  if(abs(lid)==13)
	    {
	      passSoftMuon=false;
	      // if(muCor){
	      // 	TLorentzVector p4(leptons[ilep].px(),leptons[ilep].py(),leptons[ilep].pz(),leptons[ilep].energy());
	      // 	muCor->applyPtCorrection(p4 , lid<0 ? -1 :1 );
	      // 	if(isMC) muCor->applyPtSmearing(p4, lid<0 ? -1 : 1, false);
	      // 	muDiff -= leptons[ilep];
	      // 	leptons[ilep].SetPxPyPzE(p4.Px(),p4.Py(),p4.Pz(),p4.E());
	      // 	muDiff += leptons[ilep];
	      //}
	    }

	  //no need for charge info any longer
	  lid=abs(lid);
	  TString lepStr( lid==13 ? "mu" : "e");

	  //veto nearby photon (loose electrons are many times photons...)
	  double minDRlg(9999.);
	  for(size_t ipho=0; ipho<selPhotons.size(); ipho++)
	    minDRlg=TMath::Min(minDRlg,deltaR(leptons[ilep],selPhotons[ipho]));
	  if(minDRlg<0.1) continue;
	  
	  //kinematics
	  float leta = lid==11 ? leptons[ilep].getVal("sceta") : leptons[ilep].eta();
	  if(leta> (lid==11 ? 2.5 : 2.4) )            passKin=false;
	  if(lid==11 && (leta>1.4442 && leta<1.5660)) passKin=false;
	  passLooseLepton &= passKin;
	  passSoftMuon    &= passKin;
	  if(lid==13){
	    if(leptons[ilep].pt()<10) passLooseLepton=false;
	    if(leptons[ilep].pt()<3)  passSoftMuon=false;
	  }
	  else if(lid==11){
	    if(leptons[ilep].pt()<10) passLooseLepton=false;
	  }
	  if(leptons[ilep].pt()<20) passKin=false;


	  //id
	  Int_t idbits = leptons[ilep].get("idbits");
	  if(lid==11){
	    if(leptons[ilep].getFlag("isconv"))            { passLooseLepton=false; passId=false; }
	    bool isLoose = ((idbits >> 4) & 0x1);
	    if(!isLoose)                                   passId=false; 
	    bool isVeto = ((idbits >> 4) & 0x1);
	    if(!isVeto)                                    passLooseLepton=false;
 	  }
	  else{
	    bool isTight    = ((idbits >> 10) & 0x1);
	    if(!isTight)                                   passId=false;
	    bool isLoose    = ((idbits >> 8) & 0x1);
	    if(!isLoose)                                   passLooseLepton=false;
	    bool isSoft  = ((idbits >> 9) & 0x1);
	    if(!isSoft)                                    passSoftMuon=false;
	  }

	  //isolation
	  Float_t gIso    = leptons[ilep].getVal("gIso04");
	  Float_t chIso   = leptons[ilep].getVal("chIso04");
	  Float_t puchIso = leptons[ilep].getVal("puchIso04");
	  Float_t nhIso   = leptons[ilep].getVal("nhIso04");
	  float relIso= lid==11 ?
	    (TMath::Max(nhIso+gIso-ev.rho*utils::cmssw::getEffectiveArea(11,leptons[ilep].getVal("sceta")),Float_t(0.))+chIso)/leptons[ilep].pt() :
	    (TMath::Max(nhIso+gIso-0.5*puchIso,0.)+chIso)/leptons[ilep].pt()
	    ;
	  if(lid==11){
	    if(relIso>0.15) { 
	      passIso=false;
	      passLooseLepton=false;
	    }
	  }
	  else{
	    if(relIso>0.12) passIso=false;
	    if(relIso>0.20) passLooseLepton=false;
	  }
	  
	  if(passId && passIso && passKin)          selLeptons.push_back(leptons[ilep]);
	  else if(passLooseLepton || passSoftMuon)  extraLeptons.push_back(leptons[ilep]);
	}
      std::sort(selLeptons.begin(),   selLeptons.end(), data::PhysicsObject_t::sortByPt);
      std::sort(extraLeptons.begin(), extraLeptons.end(), data::PhysicsObject_t::sortByPt);
      recoMet[0] -= muDiff;

      //
      //JET/MET ANALYSIS
      //
      //add scale/resolution uncertainties and propagate to the MET
      //utils::cmssw::updateJEC(jets,jesCor,totalJESUnc,ev.rho,ev.nvtx,isMC);
      std::vector<LorentzVector> met=utils::cmssw::getMETvariations(recoMet[0],jets,selLeptons,isMC);

      //select the jets
      data::PhysicsObjectCollection_t selJets;
      int njets(0),nbtags(0),nbtagsJP(0);
      float mindphijmet(9999.);
      for(size_t ijet=0; ijet<jets.size(); ijet++) 
	{
	  if(jets[ijet].pt()<15 || fabs(jets[ijet].eta())>4.7 ) continue;

	  //mc truth for this jet
	  const data::PhysicsObject_t &genJet=jets[ijet].getObject("genJet");
	  TString jetType( genJet.pt()>0 ? "truejetsid" : "pujetsid" );
	  
	  //cross-clean with selected leptons and photons
	  double minDRlj(9999.),minDRlg(9999.);
          for(size_t ilep=0; ilep<selLeptons.size(); ilep++)
            minDRlj = TMath::Min( minDRlj, deltaR(jets[ijet],selLeptons[ilep]) );
	  for(size_t ipho=0; ipho<selPhotons.size(); ipho++)
	    minDRlg = TMath::Min( minDRlg, deltaR(jets[ijet],selPhotons[ipho]) );
	  if(minDRlj<0.4 || minDRlg<0.4) continue;
	  
	  //jet id
	  Int_t idbits=jets[ijet].get("idbits");
	  bool passPFloose( ((idbits>>0) & 0x1));
	  int simplePuId( ( idbits >>7 ) & 0xf );
	  bool passLooseSimplePuId(  ( simplePuId >> 2) & 0x1);
	  if(jets[ijet].pt()>30)
	    {
	      mon.fillHisto(jetType,"",fabs(jets[ijet].eta()),0);
	      if(passPFloose)                        mon.fillHisto(jetType,"",fabs(jets[ijet].eta()),1);
	      if(passLooseSimplePuId)                mon.fillHisto(jetType,"",fabs(jets[ijet].eta()),2);
	      if(passPFloose && passLooseSimplePuId) mon.fillHisto(jetType,"",fabs(jets[ijet].eta()),3);
	    }
	  if(!passPFloose || !passLooseSimplePuId) continue;
	  selJets.push_back(jets[ijet]);
	  if(jets[ijet].pt()>30) {
	    njets++;
	    float dphijmet=fabs(deltaPhi(jets[ijet].phi(),met[0].phi()));
	    if(dphijmet<mindphijmet) mindphijmet=dphijmet;
	    if(fabs(jets[ijet].eta())<2.5){
	      nbtagsJP += (jets[ijet].getVal("jp")>0.264);

	      bool hasCSVtag(jets[ijet].getVal("csv")>0.405);
	      //update according to the SF measured by BTV
	      if(isMC)
		{
		  int flavId=genJet.info.find("id")->second;
		  if(abs(flavId)==5)        btsfutil.modifyBTagsWithSF(hasCSVtag,sfb,beff);
		  else if(abs(flavId)==4)   btsfutil.modifyBTagsWithSF(hasCSVtag,sfb/5,beff);
		  else		            btsfutil.modifyBTagsWithSF(hasCSVtag,sfl,leff);
		}
	      nbtags   += hasCSVtag;
	    }
	  }
	}
      std::sort(selJets.begin(), selJets.end(), data::PhysicsObject_t::sortByPt);

      //
      // ASSIGN CHANNEL
      //
      std::vector<TString> chTags;
      int dilId(1);
      LorentzVector boson(0,0,0,0);
      if(!runPhotonSelection && selLeptons.size()==2)
	{
 	  // for(size_t ilep=0; ilep<2; ilep++)
	  //   {
	  //     dilId *= selLeptons[ilep].get("id");
	  //     int id(abs(selLeptons[ilep].get("id")));
	  // weight *= isMC ? lepEff.getLeptonEfficiency( selLeptons[ilep].pt(), selLeptons[ilep].eta(), id,  id ==11 ? "loose" : "loose" ).first : 1.0;
	  //     boson += selLeptons[ilep];
	  //   }
     	  // //check the channel
	  // if( abs(dilId)==121 && eeTrigger)   chTags.push_back("ee");
	  // if( abs(dilId)==169 && mumuTrigger) chTags.push_back("mumu"); 
	  // if( abs(dilId)==143 && emuTrigger) chTags.push_back("emu"); 
	}
      else{
	if(hasPhotonTrigger && selPhotons.size()) {
	  dilId=22;
	  chTags.push_back("ee");
	  chTags.push_back("mumu");
	  boson = selPhotons[0];
	  weight *= triggerPrescale;
	}
      }
      if(chTags.size()==0) continue;

      TString evCat=eventCategoryInst.GetCategory(selJets,boson);
      std::vector<TString> tags(1,"all");
      for(size_t ich=0; ich<chTags.size(); ich++){
	tags.push_back( chTags[ich] );
	tags.push_back( chTags[ich]+evCat );
      }

      //
      // BASELINE SELECTION
      //
      bool passMass(fabs(boson.mass()-91)<15);
      bool passQt(boson.pt()>55);
      bool passThirdLeptonVeto( selLeptons.size()==2 && extraLeptons.size()==0 );
      bool passBtags(nbtags==0);
      bool passMinDphijmet( njets==0 || mindphijmet>0.5);


      // passed photon + jet trigger study

      // if ( passQt && passThirdLeptonVeto && passBtags && passMinDphijmet )
      if ( passQt) { //  && passThirdLeptonVeto ) // && passBtags && passMinDphijmet )
	n_trig_8_pass += 1; 
	h_pass->Fill(NTRIG); 
      }

      if(runPhotonSelection)
	{
	  passMass=hasPhotonTrigger;
	  passThirdLeptonVeto=(selLeptons.size()==0 && extraLeptons.size()==0);
	}

      mon.fillHisto("eventflow",  tags,0,weight);
      mon.fillHisto("nvtxraw",  tags,ev.nvtx,weight/puWeight);
      mon.fillHisto("nvtx",  tags,ev.nvtx,weight);
      mon.fillHisto("rho",  tags,ev.rho,weight);
      if(!runPhotonSelection){
	mon.fillHisto("leadpt",      tags,selLeptons[0].pt(),weight); 
	mon.fillHisto("trailerpt",   tags,selLeptons[1].pt(),weight); 
	mon.fillHisto("leadeta",     tags,fabs(selLeptons[0].eta()),weight); 
	mon.fillHisto("trailereta",  tags,fabs(selLeptons[1].eta()),weight); 
      }
      mon.fillHisto("zmass", tags,boson.mass(),weight); 
      mon.fillHisto("zy",    tags,fabs(boson.Rapidity()),weight); 

      if(passMass){

	mon.fillHisto("eventflow",tags, 1,weight);
	mon.fillHisto("zpt",      tags, boson.pt(),weight);

	//these two are used to reweight photon -> Z, the 3rd is a control
	mon.fillHisto("qt",       tags, boson.pt(),weight,true); 
	mon.fillHisto("qtraw",    tags, boson.pt(),weight/triggerPrescale,true); 

	if(passQt){

	  mon.fillHisto("eventflow",tags,2,weight);
	  int nExtraLeptons((selLeptons.size()-2)+extraLeptons.size());
	  mon.fillHisto("nextraleptons",tags,nExtraLeptons,weight);
	  if(nExtraLeptons>0){
	    LorentzVector thirdLepton(selLeptons.size()>2 ?  selLeptons[1] : extraLeptons[0]);
	    double dphi=fabs(deltaPhi(thirdLepton.phi(),met[0].phi()));
	    double mt=TMath::Sqrt(2*thirdLepton.pt()*met[0].pt()*(1-TMath::Cos(dphi)));
	    mon.fillHisto("thirdleptonpt",tags,thirdLepton.pt(),weight);
	    mon.fillHisto("thirdleptoneta",tags,fabs(thirdLepton.eta()),weight);
	    mon.fillHisto("thirdleptonmt",tags,mt,weight);
	  }

	  if(passThirdLeptonVeto){
	    
	    mon.fillHisto("eventflow",tags,3,weight);
	    for(size_t ijet=0; ijet<selJets.size(); ijet++){
	      if(selJets[ijet].pt()<30 || fabs(selJets[ijet].eta())>2.5) continue;
	      float jp(selJets[ijet].getVal("jp"));
	      float csv(selJets[ijet].getVal("csv"));
	      mon.fillHisto( "csv",tags,csv,weight);

	      if(!isMC) continue;
	      const data::PhysicsObject_t &genJet=jets[ijet].getObject("genJet");
	      int flavId=genJet.info.find("id")->second;
	      TString jetFlav("others");
	      if(abs(flavId)==5)      jetFlav="b";
	      else if(abs(flavId)==4) jetFlav="c";
	      mon.fillHisto( "csv"+jetFlav,tags,csv,weight);
	      mon.fillHisto( "jp"+jetFlav,tags,jp,weight);
	    }
	    mon.fillHisto( "nbtags",tags,nbtags,weight);
	    mon.fillHisto( "nbtagsJP",tags,nbtagsJP,weight);
	    
	    if(passBtags){
	      mon.fillHisto("eventflow",tags,4,weight);

	      //include photon prediction from this point forward
	      //requires looping tag by tag as weights are category-specific
	      //the following relies on the hypothesis that the tags are ordered as follows: all, ch, ch+subtag, ch, ch+subtag, etc...
	      //so that the ch will be assigned the weight of its subtag and all will be the summ of all ch+subtag weights
	      std::vector<LorentzVector> massiveBoson(tags.size(),boson);
	      std::vector<float> photonWeights(tags.size(),1.0);

	      for(size_t itag=0; itag<tags.size(); itag++){
		
		//update the weight
		TString icat=tags[itag];
		float iweight(weight*photonWeights[itag]);
		
		LorentzVector iboson=massiveBoson[itag];

		mon.fillHisto( "mindphijmet",icat,mindphijmet,iweight);
		if(met[0].pt()>80) mon.fillHisto( "mindphijmetNM1",icat,mindphijmet,iweight);
		if(passMinDphijmet){
		  mon.fillHisto("eventflow",icat,5,iweight);
		  
		  //this one is used to sample the boson mass: cuts may shape Z lineshape
		  mon.fillHisto("qmass",       tags, boson.mass(),weight); 

		  mon.fillHisto( "njets",icat,njets,iweight);
		  mon.fillHisto( "met",icat,met[0].pt(),iweight,true);
		  mon.fillHisto( "balance",icat,met[0].pt()/iboson.pt(),iweight);
		  TVector2 met2(met[0].px(),met[0].py());
		  TVector2 boson2(iboson.px(), iboson.py());
		  double axialMet(boson2*met2); axialMet/=-iboson.pt();
		  mon.fillHisto( "axialmet",icat,axialMet,iweight);
		  double mt=higgs::utils::transverseMass(iboson,met[0],true);
		  mon.fillHisto( "mt",icat,mt,iweight,true);
		  
		  // if(met[0].pt()>optim_Cuts1_met[0]) 
		  //   {
		  //     mon.fillHisto( "mtcheckpoint",  icat, mt,          iweight, true);
		  //     mon.fillHisto( "metcheckpoint", icat, met[0].pt(), iweight, true);
		  //   }

		  if(met[0].pt()>80){
		    mon.fillHisto("eventflow",icat,6,iweight);
		    mon.fillHisto( "mtNM1",icat,mt,iweight,true);
		    mon.fillHisto( "balanceNM1",icat,met[0].pt()/iboson.pt(),iweight);
		    mon.fillHisto( "axialmetNM1",icat,axialMet,iweight);



		  }

		  if(mt>500){
		    mon.fillHisto( "metNM1",icat,met[0].pt(),iweight,true);
		  }

		  //pre-VBF control
		  if(njets>=2){
		    LorentzVector dijet=selJets[0]+selJets[1];
		    float deta=fabs(selJets[0].eta()-selJets[1].eta());
		    float dphi=fabs(deltaPhi(selJets[0].phi(),selJets[1].phi()));
		    float pt1(selJets[0].pt()),pt2(selJets[1].pt());
		    mon.fillHisto( "leadjetpt",icat,pt1,iweight);
		    mon.fillHisto( "trailerjetpt",icat,pt2,iweight);
		    if(pt1>30 && pt2>30){
		      float eta1(selJets[0].eta()),eta2(selJets[1].eta());
		      float fwdEta( fabs(eta1)>fabs(eta2) ? eta1 : eta2);
		      float cenEta( fabs(eta1)>fabs(eta2) ? eta2 : eta1);
		      mon.fillHisto("fwdjeteta",icat,fabs(fwdEta),  iweight);
		      mon.fillHisto("cenjeteta",icat,fabs(cenEta),  iweight);
		      mon.fillHisto("vbfdetajj",icat,deta,        iweight);
		      if(deta>4.0){
			mon.fillHisto("vbfmjj",   icat,dijet.mass(),iweight,true);
			if(dijet.mass()>500){
			  mon.fillHisto("vbfdphijj",icat,dphi,        iweight);
			}
		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
            
      //
      // HISTOS FOR STATISTICAL ANALYSIS (include systematic variations)
      //
      //Fill histogram for posterior optimization, or for control regions
      // for(size_t ivar=0; ivar<nvarsToInclude; ivar++){
      // 	float iweight = weight;                            //nominal
	
      // 	//energy scale/resolution
      // 	bool varyJesUp( varNames[ivar]=="_jesup" );
      // 	bool varyJesDown( varNames[ivar]=="_jesdown" );
      // 	bool varyJerUp( varNames[ivar]=="_jerup" );
      // 	bool varyJerDown( varNames[ivar]=="_jerdown" );
      // 	bool varyUmetUp( varNames[ivar]=="_umetup" );
      // 	bool varyUmetDown( varNames[ivar]=="_umetdown" );
      // 	bool varyLesUp( varNames[ivar]=="_lesup" );
      // 	bool varyLesDown( varNames[ivar]=="_lesdown" );
		
      // 	//pileup variations
      // 	if(varNames[ivar]=="_puup") iweight *=TotalWeight_plus;
      // 	if(varNames[ivar]=="_pudown") iweight *=TotalWeight_minus;
	
      // 	//btag
      // 	bool varyBtagUp( varNames[ivar]=="_btagup" );
      // 	bool varyBtagDown( varNames[ivar]=="_btagdown" );
	

      // 	//recompute MET/MT if JES/JER was varied
      // 	LorentzVector    zvv = met[utils::cmssw::NOMINAL];
      // 	if(varyJesUp)    zvv = met[utils::cmssw::JESUP];
      // 	if(varyJesDown)  zvv = met[utils::cmssw::JESDOWN];
      // 	if(varyJerUp)    zvv = met[utils::cmssw::JERUP];
      // 	if(varyJerDown)  zvv = met[utils::cmssw::JERDOWN];
      // 	if(varyUmetUp)   zvv = met[utils::cmssw::UMETUP];
      // 	if(varyUmetDown) zvv = met[utils::cmssw::UMETDOWN];
      // 	if(varyLesUp)    zvv = met[utils::cmssw::LESUP];
      // 	if(varyLesDown)  zvv = met[utils::cmssw::LESDOWN];
	
      // 	data::PhysicsObjectCollection_t tightVarJets;
      // 	bool passLocalBveto(passBtags);
      // 	for(size_t ijet=0; ijet<jets.size(); ijet++){

      // 	  float eta=jets[ijet].eta();
      // 	  if( fabs(eta)>4.7 ) continue;
      // 	  float pt=jets[ijet].pt();
      // 	  if(varyJesUp)    pt=jets[ijet].getVal("jesup");
      // 	  if(varyJesDown)  pt=jets[ijet].getVal("jesdown");
      // 	  if(varyJerUp)    pt=jets[ijet].getVal("jerup");
      // 	  if(varyJerDown)  pt=jets[ijet].getVal("jerdown");
      // 	  if(pt<30) continue;

      // 	  //cross-clean with selected leptons and photons
      // 	  double minDRlj(9999.),minDRlg(9999.);
      //     for(size_t ilep=0; ilep<selLeptons.size(); ilep++)
      //       minDRlj = TMath::Min( minDRlj, deltaR(jets[ijet],selLeptons[ilep]) );
      // 	  for(size_t ipho=0; ipho<selPhotons.size(); ipho++)
      // 	    minDRlg = TMath::Min( minDRlg, deltaR(jets[ijet],selPhotons[ipho]) );
      // 	  if(minDRlj<0.4 || minDRlg<0.4) continue;
	  
      // 	  //jet id
      // 	  Int_t idbits=jets[ijet].get("idbits");
      // 	  bool passPFloose( (idbits>>0) & 0x1 );
      // 	  int simplePuId( (idbits >>7) & 0xf );
      // 	  bool passLooseSimplePuId(  ( simplePuId >> 2) & 0x1);
      // 	  if(!passPFloose || !passLooseSimplePuId) continue;
	 
      // 	  //jet is selected
      // 	  tightVarJets.push_back(jets[ijet]);

      // 	  //check b-tag
      // 	  if(pt<30 || fabs(eta)>2.5) continue;
      // 	  if(!isMC) continue;
      // 	  if(!varyBtagUp && !varyBtagDown) continue;
      // 	  const data::PhysicsObject_t &genJet=jets[ijet].getObject("genJet");
      // 	  int flavId=genJet.info.find("id")->second;
      // 	  bool hasCSVtag(jets[ijet].getVal("csv")>0.405);
      // 	  if(varyBtagUp) {
      // 	    if(abs(flavId)==5)        btsfutil.modifyBTagsWithSF(hasCSVtag,sfb+sfbunc,beff);
      // 	    else if(abs(flavId)==4)   btsfutil.modifyBTagsWithSF(hasCSVtag,sfb/5+2*sfbunc,beff);
      // 	    else		            btsfutil.modifyBTagsWithSF(hasCSVtag,sfl+sflunc,leff);
      // 	  }
      // 	  else if(varyBtagDown) {
      // 	    if(abs(flavId)==5)        btsfutil.modifyBTagsWithSF(hasCSVtag,sfb-sfbunc,beff);
      // 	    else if(abs(flavId)==4)   btsfutil.modifyBTagsWithSF(hasCSVtag,sfb/5-2*sfbunc,beff);
      // 	    else		            btsfutil.modifyBTagsWithSF(hasCSVtag,sfl-sflunc,leff);
      // 	  }
      // 	  passLocalBveto |= hasCSVtag;
      // 	}
	
      // 	bool isZsideBand    ( (boson.mass()>40  && boson.mass()<70) || (boson.mass()>110 && boson.mass()<200) );
      // 	bool isZsideBandPlus( (boson.mass()>110 && boson.mass()<200) );
      // 	bool passPreselection                 (passMass && passQt && passThirdLeptonVeto && passMinDphijmet && passLocalBveto);
      // 	bool passPreselectionMbvetoMzmass     (            passQt && passThirdLeptonVeto && passMinDphijmet                  );          
	
      // 	//re-assign the event category to take migrations into account
      // 	TString evCat  = eventCategoryInst.GetCategory(tightVarJets,boson);
      // 	for(size_t ich=0; ich<chTags.size(); ich++){
	  
      // 	  TString tags_full=chTags[ich]+evCat;
      // 	  float chWeight(iweight);

      // 	  //update weight and mass for photons
      // 	  LorentzVector iboson(boson);
      // 	  if(gammaWgtHandler!=0)
      // 	    {
      // 	      std::vector<Float_t> photonVars;
      // 	      photonVars.push_back(iboson.pt());
      // 	      //photonVars.push_back(met[0].pt()/iboson.pt());
      // 	      float photonWeight=gammaWgtHandler->getWeightFor(photonVars,tags_full);
      // 	      chWeight *= photonWeight;
      // 	      iboson   = gammaWgtHandler->getMassiveP4(iboson,tags_full);
      // 	    }
	  
      // 	  //updet the transverse mass
      // 	  float mt =higgs::utils::transverseMass(iboson,zvv,true);

      // 	  //scan the MET cut and fill the shapes
      // 	  for(unsigned int index=0;index<optim_Cuts1_met.size();index++){             
	    
      // 	    if(zvv.pt()>optim_Cuts1_met[index]){
      // 	      for(unsigned int nri=0;nri<NRparams.size();nri++){
		
      // 		float nrweight=chWeight*NRweights[nri];
      // 		if(nri>0)
      // 		  {
      // 		    nrweight=chWeight*NRweights[nri];
      // 		    if(lShapeWeights[0]==0) nrweight=0;
      // 		    else                    nrweight/=lShapeWeights[0];
      // 		  }
		
      // 		if(passPreselection && ivar==0 && nri==0                                    )   mon.fillHisto("metcount", tags_full, index, nrweight);
      // 		if(passPreselection                                                         )   mon.fillHisto(TString("mt_shapes")+NRsuffix[nri]+varNames[ivar],tags_full,index, mt,nrweight);
      // 		if(passPreselection                                                         )   mon.fillHisto(TString("met_shapes")+NRsuffix[nri]+varNames[ivar],tags_full,index, zvv.pt(),nrweight);
      // 		if(passPreselectionMbvetoMzmass && passMass          && passLocalBveto      )   mon.fillHisto("mt_shapes_NRBctrl"+NRsuffix[nri]+varNames[ivar],tags_full,index,0,nrweight);
      // 		if(passPreselectionMbvetoMzmass && isZsideBand       && passLocalBveto      )   mon.fillHisto("mt_shapes_NRBctrl"+NRsuffix[nri]+varNames[ivar],tags_full,index,1,nrweight);
      // 		if(passPreselectionMbvetoMzmass && isZsideBandPlus   && passLocalBveto      )   mon.fillHisto("mt_shapes_NRBctrl"+NRsuffix[nri]+varNames[ivar],tags_full,index,2,nrweight);
      // 		if(passPreselectionMbvetoMzmass && passMass          && !passLocalBveto     )   mon.fillHisto("mt_shapes_NRBctrl"+NRsuffix[nri]+varNames[ivar],tags_full,index,3,nrweight);
      // 		if(passPreselectionMbvetoMzmass && isZsideBand       && !passLocalBveto     )   mon.fillHisto("mt_shapes_NRBctrl"+NRsuffix[nri]+varNames[ivar],tags_full,index,4,nrweight);
      // 		if(passPreselectionMbvetoMzmass && isZsideBandPlus   && !passLocalBveto     )   mon.fillHisto("mt_shapes_NRBctrl"+NRsuffix[nri]+varNames[ivar],tags_full,index,5,nrweight);
      // 	      }
      // 	    }
      // 	  }
      // 	}
      // }
  }
  printf("\n"); 
  file->Close();
  
  // Make efficiency histo

  TEfficiency* trig_eff = new TEfficiency("eff", ";Trigger Paths;Efficiency", 
					  trigs.size(), 0, trigs.size());
  for (unsigned int bin=1; bin<=trigs.size(); bin++) {
    int iTotal = h_total->GetBinContent(bin); 
    int iPass = h_pass->GetBinContent(bin); 
    
    if (iPass  == 0 ) continue; 

    trig_eff->SetTotalEvents(bin, iTotal); 
    trig_eff->SetPassedEvents(bin, iPass); 
  }

  //##############################################
  //########     SAVING HISTO TO FILE     ########
  //##############################################
  //save control plots to file
  outUrl += "/";
  outUrl += outFileUrl + ".root";
  printf("Results save in %s\n", outUrl.Data());
  
  //save all to the file
  TFile *ofile=TFile::Open(outUrl, "recreate");
  mon.Write();

  h_total->Write(); 
  h_pass->Write(); 
  trig_eff->Write(); 

  ofile->Close();

  cout << "N tot = " << n_trig_8_total 
       << " ,  N pass = " << n_trig_8_pass << endl; 
  
  // if(outTxtFile)fclose(outTxtFile);
}  





