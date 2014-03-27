#include <algorithm>
#include "electron.h"
#include "event.h"
#include "eventPrinter.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "jet.h"
#include "jet.h"
#include "muon.h"
#include "options.h"
#include "photon.h"
#include "photonPrescale.h"
#include "preselectionCMG.h"
#include <RooAbsPdf.h>
#include <RooDataSet.h>
#include <RooRealVar.h>
#include <RooWorkspace.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include "toolbox.h"
#include <TRandom.h>
#include <unordered_set>
#include <tuple>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;
using std::setw;
using std::ofstream;
using std::unordered_set;
using std::tuple;
using std::make_tuple;
using std::get;

void LeptonPreselectionCMG( PreselType type, RooWorkspace * w ) {
	const Options & opt = Options::getInstance(); 
	if (type == ELE)
		cout << "Running Electron Preselection :" << endl;
	else if (type == MU)
		cout << "Running Muon Preselection :" << endl;
	else if (type == EMU)
		cout << "Running Electron-Muon Preselection() ..." << endl;
	else if (type == PHOT)
		cout << "Running Photon Preselection :" << endl;

	string systVar;
	try {
		systVar = opt.checkStringOption("SYSTEMATIC_VAR");
	} catch (const std::string & exc) {
		cout << exc << endl;
	}
	if (systVar == "NONE")
		systVar.clear();

#ifdef CMSSWENV
	JetCorrectionUncertainty jecUnc("Summer13_V4_MC_Uncertainty_AK5PFchs.txt");
#endif

	string inputDir = opt.checkStringOption("INPUT_DIR");
	string outputDir = opt.checkStringOption("OUTPUT_DIR");
	string sampleName = opt.checkStringOption("SAMPLE_NAME");
	string inputFile = inputDir + '/' + sampleName + ".root";
	cout << "\tInput file: " << inputFile << endl;

	bool isSignal = opt.checkBoolOption("SIGNAL");
	TGraph * higgsW = 0;
	TGraph * higgsI = 0;
	if (isSignal) {
		double higgsM = opt.checkDoubleOption("HIGGS_MASS");
		if (higgsM >= 400) {
			string dirName = "H" + double2string(higgsM);
			bool isVBF = opt.checkBoolOption("VBF");
			string lshapeHistName = "cps";
			string intHistName = "nominal";
			
			if (systVar == "LSHAPE_UP") {
				intHistName = "up";
			} else if (systVar == "LSHAPE_DOWN") {
				intHistName = "down";
			}

			if (isVBF) {
				TFile weightFile("VBF_LineShapes.root");
				higgsW = (TGraph *) ( (TDirectory *) weightFile.Get(dirName.c_str()))->Get( lshapeHistName.c_str() )->Clone();

			} else {
				TFile weightFile("GG_LineShapes.root");
				higgsW = (TGraph *) ( (TDirectory *) weightFile.Get(dirName.c_str()))->Get( lshapeHistName.c_str() )->Clone();
				TFile interfFile("newwgts_interf.root");
				higgsI = (TGraph *) ( (TDirectory *) interfFile.Get(dirName.c_str()))->Get( intHistName.c_str() )->Clone();
			}
		}
	}

	TFile * file = new TFile( inputFile.c_str() );
	if (!file->IsOpen())
		throw string("ERROR: Can't open the file: " + inputFile + "!");
	TDirectory * dir = (TDirectory *) file->Get("dataAnalyzer");
	TH1D * nEvHisto = (TH1D *) dir->Get("cutflow");
	TH1D * puHisto = (TH1D *) dir->Get("pileup");
	TTree * tree = ( TTree * ) dir->Get( "data" );
	Event ev( tree );
	const int * runP = ev.getSVA<int>("run"); 
	const int * lumiP = ev.getSVA<int>("lumi"); 
	const int * eventP = ev.getSVA<int>("event"); 
	const bool * trigBits = ev.getAVA<bool>("t_bits");
	const int * trigPres = ev.getAVA<int>("t_prescale");
	const float * metPtA = ev.getAVA<float>("met_pt");
	const float * metPhiA = ev.getAVA<float>("met_phi");
	const float * rhoP = ev.getSVA<float>("rho");
	const float * rho25P = ev.getSVA<float>("rho25");
	const int * nvtxP = ev.getSVA<int>("nvtx"); 
	const int * niP = ev.getSVA<int>("ngenITpu"); 
	
#ifdef PRINTEVENTS
	string eventFileName;
	if (type == ELE)
		eventFileName = "events_ele.txt";
	else if (type == MU)
		eventFileName = "events_mu.txt";
	else if (type == EMU)
		eventFileName = "events_emu.txt";

	EventPrinter evPrint(ev, type, eventFileName);
	evPrint.readInEvents("diff.txt");
	evPrint.printElectrons();
	evPrint.printMuons();
	evPrint.printZboson();
	evPrint.printJets();
	evPrint.printHeader();
#endif

	string outputFile = outputDir + '/' + sampleName;

	if (systVar.size())
		outputFile += ('_' + systVar);

	if (type == ELE)
		outputFile += "_elePresel.root";
	else if (type == MU)
		outputFile += "_muPresel.root";
	else if (type == EMU)
		outputFile += "_emuPresel.root";
	else if (type == PHOT)
		outputFile += "_phPresel.root";
	cout << "\tOutput file: " << outputFile << endl;

	TFile * out = new TFile( outputFile.c_str(), "recreate" );
	TH1D * outNEvHisto = new TH1D("nevt", "nevt", 1, 0, 1);
	outNEvHisto->SetBinContent(1, nEvHisto->GetBinContent(1));
	outNEvHisto->Write("nevt");

	TH1D * outPuHisto = new TH1D( *puHisto );
	outPuHisto->Write("pileup");

	std::vector< std::tuple<std::string, std::string> > eleVars;
	eleVars.push_back( std::make_tuple("ln_px", "F") );
	eleVars.push_back( std::make_tuple("ln_py", "F") );
	eleVars.push_back( std::make_tuple("ln_pz", "F") );
	eleVars.push_back( std::make_tuple("ln_en", "F") );
	eleVars.push_back( std::make_tuple("ln_idbits", "I") );
	eleVars.push_back( std::make_tuple("ln_d0", "F") );
	eleVars.push_back( std::make_tuple("ln_dZ", "F") );
	eleVars.push_back( std::make_tuple("ln_nhIso03", "F") );
	eleVars.push_back( std::make_tuple("ln_gIso03", "F") );
	eleVars.push_back( std::make_tuple("ln_chIso03", "F") );
	eleVars.push_back( std::make_tuple("ln_trkLostInnerHits", "F") );

	std::vector< std::tuple<std::string, std::string> > addEleVars;
	addEleVars.push_back( std::make_tuple("egn_sceta", "F") );
	addEleVars.push_back( std::make_tuple("egn_detain", "F") );
	addEleVars.push_back( std::make_tuple("egn_dphiin", "F") );
	addEleVars.push_back( std::make_tuple("egn_sihih", "F") );
	addEleVars.push_back( std::make_tuple("egn_hoe", "F") );
	addEleVars.push_back( std::make_tuple("egn_ooemoop", "F") );
	addEleVars.push_back( std::make_tuple("egn_isConv", "B") );

	std::vector< std::tuple<std::string, std::string> > muVars;
	muVars.push_back( std::make_tuple("ln_px", "F") );
	muVars.push_back( std::make_tuple("ln_py", "F") );
	muVars.push_back( std::make_tuple("ln_pz", "F") );
	muVars.push_back( std::make_tuple("ln_en", "F") );
	muVars.push_back( std::make_tuple("ln_idbits", "I") );
	muVars.push_back( std::make_tuple("ln_d0", "F") );
	muVars.push_back( std::make_tuple("ln_dZ", "F") );
	muVars.push_back( std::make_tuple("ln_nhIso04", "F") );
	muVars.push_back( std::make_tuple("ln_gIso04", "F") );
	muVars.push_back( std::make_tuple("ln_chIso04", "F") );
	muVars.push_back( std::make_tuple("ln_puchIso04", "F") );
	muVars.push_back( std::make_tuple("ln_trkchi2", "F") );
	muVars.push_back( std::make_tuple("ln_trkValidPixelHits", "F") );

	std::vector< std::tuple<std::string, std::string> > addMuVars;
	addMuVars.push_back( std::make_tuple("mn_trkLayersWithMeasurement", "F") );
	addMuVars.push_back( std::make_tuple("mn_pixelLayersWithMeasurement", "F") );
	addMuVars.push_back( std::make_tuple("mn_innerTrackChi2", "F") );
	addMuVars.push_back( std::make_tuple("mn_validMuonHits", "F") );
	addMuVars.push_back( std::make_tuple("mn_nMatchedStations", "F") );

	unsigned run;
	unsigned lumi;
	unsigned event;
	double pfmet;
	int nele;
	int nmu;
	int nsoftmu;
	double l1pt;
	double l1eta;
	double l1phi;
	double l2pt;
	double l2eta;
	double l2phi;
	double zmass;
	double zpt;
	double zeta;
	double mt;
	int nsoftjet;
	int nhardjet;
	double maxJetBTag;
	double minDeltaPhiJetMet;
	double detajj;
	double mjj;
	int nvtx;
	int ni;
	int category;
	double weight;
	double hmass;
	double hweight;

	TTree * smallTree = new TTree("HZZ2l2nuAnalysis", "HZZ2l2nu Analysis Tree");
	smallTree->Branch( "Run", &run, "Run/i" );
	smallTree->Branch( "Lumi", &lumi, "Lumi/i" );
	smallTree->Branch( "Event", &event, "Event/i" );
	smallTree->Branch( "PFMET", &pfmet, "PFMET/D" );
	smallTree->Branch( "NELE", &nele, "NELE/I" );
	smallTree->Branch( "NMU", &nmu, "NMU/I" );
	smallTree->Branch( "NSOFTMU", &nsoftmu, "NSOFTMU/I" );
	smallTree->Branch( "L1PT", &l1pt, "L1PT/D" );
	smallTree->Branch( "L1ETA", &l1eta, "L1ETA/D" );
	smallTree->Branch( "L1PHI", &l1phi, "L1PHI/D" );
	smallTree->Branch( "L2PT", &l2pt, "L2PT/D" );
	smallTree->Branch( "L2ETA", &l2eta, "L2ETA/D" );
	smallTree->Branch( "L2PHI", &l2phi, "L2PHI/D" );
	smallTree->Branch( "ZMASS", &zmass, "ZMASS/D" );
	smallTree->Branch( "ZPT", &zpt, "ZPT/D" );
	smallTree->Branch( "ZETA", &zeta, "ZETA/D" );
	smallTree->Branch( "MT", &mt, "MT/D" );
	smallTree->Branch( "NSOFTJET", &nsoftjet, "NSOFTJET/I" );
	smallTree->Branch( "NHARDJET", &nhardjet, "NHARDJET/I" );
	smallTree->Branch( "MAXJETBTAG", &maxJetBTag, "MAXJETBTAG/D" );
	smallTree->Branch( "MINDPJETMET", &minDeltaPhiJetMet, "MINDPJETMET/D" );
	smallTree->Branch( "DETAJJ", &detajj, "DETAJJ/D" );
	smallTree->Branch( "MJJ", &mjj, "MJJ/D" );
	smallTree->Branch( "NVTX", &nvtx, "NVTX/I" );
	smallTree->Branch( "nInter" , &ni, "nInter/I" );
	smallTree->Branch( "CATEGORY", &category, "CATEGORY/I" );
	smallTree->Branch( "Weight" , &weight, "Weight/D" );
	smallTree->Branch( "HMASS", &hmass, "HMASS/D" );
	smallTree->Branch( "HWEIGHT", &hweight, "HWEIGHT/D" );

	bool isData = opt.checkBoolOption("DATA");

	unsigned long nentries = tree->GetEntries();

	RooDataSet * events = nullptr;

	PhotonPrescale photonPrescales;

	vector<int> thresholds;
	if (type == PHOT) {
		if (w == nullptr)
			throw string("ERROR: No mass peak pdf!");
		RooRealVar * zmass = w->var("mass");
		zmass->setRange(76.0, 106.0);
		RooAbsPdf * pdf = w->pdf("massPDF");
		events = pdf->generate(*zmass, nentries);

		photonPrescales.addTrigger("HLT_Photon36_R9Id90_HE10_Iso40_EBOnly", 36, 3, 7);
		photonPrescales.addTrigger("HLT_Photon50_R9Id90_HE10_Iso40_EBOnly", 50, 5, 8);
		photonPrescales.addTrigger("HLT_Photon75_R9Id90_HE10_Iso40_EBOnly", 75, 7, 9);
		photonPrescales.addTrigger("HLT_Photon90_R9Id90_HE10_Iso40_EBOnly", 90, 10, 10);
	}

	TH1D ptSpectrum("ptSpectrum", "ptSpectrum", 200, 55, 755);
	ptSpectrum.Sumw2();

	unordered_set<EventAdr> eventsSet;
	for ( unsigned long iEvent = 0; iEvent < nentries; iEvent++ ) {
//		if (iEvent < 6060000)
//			continue;

		if ( iEvent % 10000 == 0) {
			cout << string(40, '\b');
			cout << setw(10) << iEvent << " / " << setw(10) << nentries << " done ..." << std::flush;
		}

		tree->GetEntry( iEvent );

		run = -999;
		lumi = -999;
		event = -999;
		pfmet = -999;
		nele = -999;
		nmu = -999;
		nsoftmu = -999;
		l1pt = -999;
		l1eta = -999;
		l1phi = -999;
		l2pt = -999;
		l2eta = -999;
		l2phi = -999;
		zmass = -999;
		zpt = -999;
		zeta = -999;
		mt = -999;
		nsoftjet = -999;
		nhardjet = -999;
		maxJetBTag = -999;
		minDeltaPhiJetMet = -999;
		detajj = -999;
		mjj = -999;
		nvtx = -999;
		ni = -999;
		weight = -999;
		category = -1;
		hmass = -999;
		hweight = -999;

		run = *runP;
		lumi = *lumiP;
		event = *eventP;

		EventAdr tmp(run, lumi, event);
		if (eventsSet.find( tmp ) != eventsSet.end()) {
			continue;
		}
		eventsSet.insert( tmp );

		if (type == ELE && isData) {
			if (trigBits[0] != 1 || trigPres[0] != 1)
				continue;
		}

		if (type == MU && isData) {
			if ( (trigBits[2] != 1 || trigPres[2] != 1)
				&& (trigBits[3] != 1 || trigPres[3] != 1)
				&& (trigBits[6] != 1 || trigPres[6] != 1)
			   )
				continue;
		}

		if (type == EMU && isData) {
			if ( (trigBits[4] != 1 || trigPres[4] != 1)
				&& (trigBits[5] != 1 || trigPres[5] != 1)
			   )
				continue;
		}

		vector<Electron> electrons = buildLeptonCollection<Electron, 11>(ev, eleVars, addEleVars);
		vector<Muon> muons = buildLeptonCollection<Muon, 13>(ev, muVars, addMuVars);

		float rho = *rhoP;
		float rho25 = *rho25P;

		vector<Electron> looseElectrons;
		vector<Electron> selectedElectrons;
		for (unsigned j = 0; j < electrons.size(); ++j) {
			try {
			TLorentzVector lv = electrons[j].lorentzVector();
			if (
					lv.Pt() > 10 &&
					fabs(lv.Eta()) < 2.5 &&
					!electrons[j].isInCrack() &&
					electrons[j].passesVetoID() &&
					electrons[j].isPFIsolatedLoose(rho25)
				) {
				looseElectrons.push_back(electrons[j]);
			}

			if (
					lv.Pt() > 20 &&
					fabs(lv.Eta()) < 2.5 &&
					!electrons[j].isInCrack() &&
					electrons[j].passesMediumID() &&
					electrons[j].isPFIsolatedMedium(rho25)
				) {
				selectedElectrons.push_back(electrons[j]);
			}
			} catch (const string & exc) {
				cout << exc << endl;
				cout << "run = " << run << endl;
				cout << "lumi = " << lumi << endl;
				cout << "event = " << event << endl;
			}
		}

		vector<Muon> looseMuons;
		vector<Muon> softMuons;
		vector<Muon> selectedMuons;
		for (unsigned j = 0; j < muons.size(); ++j) {
			TLorentzVector lv = muons[j].lorentzVector();
			if (
					lv.Pt() > 10 &&
					fabs(lv.Eta()) < 2.4 &&
					muons[j].isLooseMuon() &&
					muons[j].isPFIsolatedLoose()
				) {
				looseMuons.push_back(muons[j]);
			} else if (
					lv.Pt() > 3 &&
					fabs(lv.Eta()) < 2.4 &&
					muons[j].isSoftMuon()
				) {
				softMuons.push_back(muons[j]);
			}
			if (
					lv.Pt() > 20 &&
					fabs(lv.Eta()) < 2.4 &&
					muons[j].isTightMuon() &&
					muons[j].isPFIsolatedTight()
				) {
				selectedMuons.push_back(muons[j]);
			}
		}

		vector<Lepton> looseLeptons;
		for (unsigned i = 0; i < looseElectrons.size(); ++i)
			looseLeptons.push_back(looseElectrons[i]);
		for (unsigned i = 0; i < looseMuons.size(); ++i)
			looseLeptons.push_back(looseMuons[i]);
		for (unsigned i = 0; i < softMuons.size(); ++i)
			looseLeptons.push_back(softMuons[i]);

#ifdef PRINTEVENTS
		evPrint.setElectronCollection(selectedElectrons);
		evPrint.setMuonCollection(selectedMuons);
#endif

		vector<Photon> photons = selectPhotonsCMG( ev );
		vector<Photon> selectedPhotons;
		for (unsigned i = 0; i < photons.size(); ++i) {
			if (photons[i].isSelected(rho) && photons[i].lorentzVector().Pt() > 55)
				selectedPhotons.push_back( photons[i] );
		}

		if (type == PHOT) {
			vector<Electron> tmpElectrons;
			for (unsigned i = 0; i < selectedPhotons.size(); ++i) {
				TLorentzVector phVec = selectedPhotons[i].lorentzVector();
				for (unsigned j = 0; j < looseElectrons.size(); ++j) {
					TLorentzVector elVec = looseElectrons[j].lorentzVector();
					double dR = deltaR(phVec.Eta(), phVec.Phi(), elVec.Eta(), elVec.Phi());
					if ( dR > 0.05 )
						tmpElectrons.push_back( looseElectrons[j] );
				}
			}
			looseElectrons = tmpElectrons;
		}

		string leptonsType;
		Lepton * selectedLeptons[2] = {0};
		if (type == ELE) {
			if (selectedElectrons.size() < 2) {
				continue;
			} else {
				selectedLeptons[0] = &selectedElectrons[0];
				selectedLeptons[1] = &selectedElectrons[1];
			}
		} else if (type == MU) {
			if (selectedMuons.size() < 2) {
				continue;
			} else {
				selectedLeptons[0] = &selectedMuons[0];
				selectedLeptons[1] = &selectedMuons[1];
			}
		} else if (type == EMU) {
			if (selectedElectrons.size() < 1 || selectedMuons.size() < 1) {
				continue;
			} else {
				selectedLeptons[0] = &selectedElectrons[0];
				selectedLeptons[1] = &selectedMuons[0];
			}
		} else if (type == PHOT) {
			if (selectedPhotons.size() != 1) {
				continue;
			}
		}

		nele = looseElectrons.size();
		nmu = looseMuons.size();
		nsoftmu = softMuons.size();

		TLorentzVector Zcand;

		if (type == ELE || type == MU || type == EMU) {
			TLorentzVector lep1 = selectedLeptons[0]->lorentzVector();
			TLorentzVector lep2 = selectedLeptons[1]->lorentzVector();

			if (lep2.Pt() > lep1.Pt() && type != EMU) {
				TLorentzVector temp = lep1;
				lep1 = lep2;
				lep2 = temp;
			}

			l1pt = lep1.Pt();
			l1eta = lep1.Eta();
			l1phi = lep1.Phi();

			l2pt = lep2.Pt();
			l2eta = lep2.Eta();
			l2phi = lep2.Phi();

			Zcand = lep1 + lep2;
			zmass = Zcand.M();
		} else if (type == PHOT) {
			Zcand = selectedPhotons[0].lorentzVector();
			zmass = events->get(iEvent)->getRealValue("mass");
		}

		zpt = Zcand.Pt();
		zeta = Zcand.Eta();

		if (type == PHOT) {
			unsigned idx = photonPrescales.getIndex(zpt);
			if (trigBits[idx])
				weight = trigPres[idx];
			else
				continue;
			ptSpectrum.Fill(zpt, weight);
		}

		TLorentzVector met;
		met.SetPtEtaPhiM(metPtA[0], 0.0, metPhiA[0], 0.0);
		TLorentzVector clusteredFlux;

		unsigned mode = 0;
		if (systVar == "JES_UP")
			mode = 1;
		else if (systVar == "JES_DOWN")
			mode = 2;
		TLorentzVector jecCorr;

#ifdef CMSSWENV
		vector<Jet> jetsAll = selectJetsCMG( ev, looseLeptons, jecUnc, &jecCorr, mode );
#else
		vector<Jet> jetsAll = selectJetsCMG( ev, looseLeptons, &jecCorr, mode );
#endif

		met -= jecCorr;

		mode = 0;
		if (systVar == "JER_UP")
			mode = 1;
		else if (systVar == "JER_DOWN")
			mode = 2;
		TLorentzVector smearCorr = smearJets( jetsAll, mode );
		if (isData && smearCorr != TLorentzVector())
			throw std::string("Jet smearing corrections different from zero in DATA!");
		met -= smearCorr;

		vector<Jet> selectedJets;
		for (unsigned i = 0; i < jetsAll.size(); ++i) {
			if (
					jetsAll[i].lorentzVector().Pt() > 10
					&& fabs(jetsAll[i].lorentzVector().Eta()) < 4.7
					&& jetsAll[i].passesPUID() &&
					jetsAll[i].passesPFLooseID()
				)
				selectedJets.push_back( jetsAll[i] );
		}
		if (type == PHOT) {
			vector<Jet> tmpJets;
			for (unsigned i = 0; i < selectedPhotons.size(); ++i) {
				TLorentzVector phVec = selectedPhotons[i].lorentzVector();
				for (unsigned j = 0; j < selectedJets.size(); ++j) {
					TLorentzVector jVec = selectedJets[j].lorentzVector();
					double dR = deltaR(phVec.Eta(), phVec.Phi(), jVec.Eta(), jVec.Phi());
					if ( dR > 0.4 )
						tmpJets.push_back( selectedJets[j] );
				}
			}
			selectedJets = tmpJets;
		}

		if (systVar == "UMET_UP" || systVar == "UMET_DOWN") {
			for (unsigned i = 0; i < jetsAll.size(); ++i)
				clusteredFlux += jetsAll[i].lorentzVector();
			for (unsigned i = 0; i < looseElectrons.size(); ++i)
				clusteredFlux += looseElectrons[i].lorentzVector();
			for (unsigned i = 0; i < looseMuons.size(); ++i)
				clusteredFlux += looseMuons[i].lorentzVector();

			TLorentzVector unclusteredFlux = -(met + clusteredFlux);
			if (systVar == "UMET_UP")
				unclusteredFlux *= 1.1;
			else
				unclusteredFlux *= 0.9;
			met = -(clusteredFlux + unclusteredFlux);
		}

		if (systVar == "LES_UP" || systVar == "LES_DOWN") {
			TLorentzVector diff;
			double sign = 1.0;
			if (systVar == "LES_DOWN")
				sign = -1.0;
			for (unsigned i = 0; i < looseElectrons.size(); ++i) {
				TLorentzVector tempEle = looseElectrons[i].lorentzVector();
				if (looseElectrons[i].isEB())
					diff += sign * 0.02 * tempEle;
				else
					diff += sign * 0.05 * tempEle;
			}
			for (unsigned i = 0; i < looseMuons.size(); ++i)
				diff += sign * 0.01 * looseMuons[i].lorentzVector();

			met -= diff;
		}

		pfmet = met.Pt();

		double px = met.Px() + Zcand.Px();
		double py = met.Py() + Zcand.Py();
		double pt2 = px * px + py * py;
		double e = sqrt(zpt * zpt + zmass * zmass) + sqrt(pfmet * pfmet + zmass * zmass);
		double mt2 = e * e - pt2;
		mt = (mt2 > 0) ? sqrt(mt2) : 0;

		vector<Jet> hardjets;
		vector<Jet> softjets;
		maxJetBTag = -999;
		minDeltaPhiJetMet = 999;
		for ( unsigned j = 0; j < selectedJets.size(); ++j ) {
			TLorentzVector jet = selectedJets[j].lorentzVector();

			if ( jet.Pt() > 30 ) {
				hardjets.push_back( selectedJets[j] );
			}
			if ( jet.Pt() > 15 )
				softjets.push_back( selectedJets[j] );
		}
		nhardjet = hardjets.size();
		nsoftjet = softjets.size();
//		if ( type == PHOT && nsoftjet == 0 )
//			continue;

		if (nhardjet > 1) {
			sort(hardjets.begin(), hardjets.end(), [](const Jet & a, const Jet & b) {
					return a.lorentzVector().Pt() > b.lorentzVector().Pt();
				});
			TLorentzVector jet1 = hardjets[0].lorentzVector();
			TLorentzVector jet2 = hardjets[1].lorentzVector();
			const double maxEta = max( jet1.Eta(), jet2.Eta() );
			const double minEta = min( jet1.Eta(), jet2.Eta() );
			bool passCJV = true;
			for (unsigned j = 2; j < hardjets.size(); ++j) {
				double tmpEta = hardjets[j].lorentzVector().Eta();
				if ( tmpEta > minEta && tmpEta < maxEta )
					passCJV = false;
			}
			const double tmpDelEta = std::fabs(jet2.Eta() - jet1.Eta());
			TLorentzVector diJetSystem = jet1 + jet2;
			const double tmpMass = diJetSystem.M();
			if ( type == PHOT) {
				if (passCJV && tmpDelEta > 4.0 && tmpMass > 500 && zeta > minEta && maxEta > zeta) {
					detajj = tmpDelEta;
					mjj = tmpMass;
				}
			} else {
				if (passCJV && tmpDelEta > 4.0 && tmpMass > 500 && l1eta > minEta && l2eta > minEta && maxEta > l1eta && maxEta > l2eta) {
					detajj = tmpDelEta;
					mjj = tmpMass;
				}
			}
		}

		category = evCategory(nhardjet, nsoftjet, detajj, mjj, type == PHOT);

		minDeltaPhiJetMet = 10;
		for ( unsigned j = 0; j < hardjets.size(); ++j ) {
			TLorentzVector jet = hardjets[j].lorentzVector();
			if ( hardjets[j].getVarF("jn_jp") > maxJetBTag && fabs(jet.Eta()) < 2.5 )
				maxJetBTag = hardjets[j].getVarF("jn_jp");
			double tempDelPhiJetMet = deltaPhi(met.Phi(), jet.Phi());
			if ( tempDelPhiJetMet < minDeltaPhiJetMet )
				minDeltaPhiJetMet = tempDelPhiJetMet;
		}

		nvtx = *nvtxP;

		if (isData)
			ni = -1;
		else
			ni = *niP;

		if (isSignal) {
			const int nMC = ev.getSVV<int>("mcn");
			const int * mcID = ev.getAVA<int>("mc_id");
			int hIdx = 0;
			for (; hIdx < nMC; ++hIdx)
				if (fabs(mcID[hIdx]) == 25)
					break;
			if (hIdx == nMC)
				throw string("ERROR: Higgs not found in signal sample!");

			float Hpx = ev.getAVV<float>("mc_px", hIdx);
			float Hpy = ev.getAVV<float>("mc_py", hIdx);
			float Hpz = ev.getAVV<float>("mc_pz", hIdx);
			float Hen = ev.getAVV<float>("mc_en", hIdx);
			TLorentzVector higgs;
			higgs.SetPxPyPzE( Hpx, Hpy, Hpz, Hen );
			hmass = higgs.M();

			if (higgsW) {
				hweight = higgsW->Eval(hmass);
				if (higgsI)
					hweight *= higgsI->Eval(hmass);
			} else
				hweight = 1;
		}

		if ( opt.checkBoolOption("ADDITIONAL_LEPTON_VETO") && (type == ELE || type == MU || type == EMU) && ((nele + nmu + nsoftmu) > 2) )
			continue;
		if ( opt.checkBoolOption("ADDITIONAL_LEPTON_VETO") && (type == PHOT) && ((nele + nmu + nsoftmu) > 0) )
			continue;
		if ( opt.checkBoolOption("ZPT_CUT") && zpt < 55 )
			continue;
		// for different background estimation methods different window should be applied:
		// * sample for photons should have 76.0 < zmass < 106.0
		// * sample for non-resonant background should not have this cut applied
		if ( opt.checkBoolOption("TIGHT_ZMASS_CUT") && (type == ELE || type == MU) && (zmass < 76.0 || zmass > 106.0))
			continue;
		if ( opt.checkBoolOption("WIDE_ZMASS_CUT") && (type == ELE || type == MU) && (zmass < 76.0 || zmass > 106.0))
			continue;
		if ( opt.checkBoolOption("BTAG_CUT") && ( maxJetBTag > 0.264) )
			continue;
		if ( opt.checkBoolOption("DPHI_CUT") && ( minDeltaPhiJetMet < 0.5) )
			continue;


#ifdef PRINTEVENTS
		evPrint.setJetCollection(hardjets);
		evPrint.setMET(met);
		evPrint.setMT(mt);
		string channelType;
		if (type == ELE)
			channelType = "ee";
		else if (type == MU)
			channelType = "mumu";
		else if (type == EMU)
			channelType = "emu";
		if (category == 1)
			channelType += "eq0jets";
		else if (category == 2)
			channelType += "geq1jets";
		else
			channelType += "vbf";
		evPrint.setChannel(channelType);
		unsigned bits = 0;
		bits |= (0x7);
		bits |= ((zmass > 76.0 && zmass < 106.0) << 3);
		bits |= ((zpt > 55) << 4);
		bits |= (((nele + nmu + nsoftmu) == 2) << 5);
		bits |= ((maxJetBTag < 0.275) << 6);
		bits |= ((minDeltaPhiJetMet > 0.5) << 7);
		evPrint.setBits(bits);
		evPrint.print();
#endif
		
		smallTree->Fill();
	}
	cout << endl;
	
	TCanvas canv("canv", "canv", 800, 600);
	//effNum.Sumw2();
	//effDen.Sumw2();
	//effNum.Divide(&effDen);
	//effNum.Draw();
	canv.SetGridy();
	canv.SetGridx();
	//canv.SaveAs("triggEff.ps");
	//canv.Clear();
	ptSpectrum.SetMarkerStyle(20);
	ptSpectrum.SetMarkerSize(0.5);
	ptSpectrum.Draw("P0E");
	//ptSpectrum.Draw("COLZ");
	canv.SetLogy();
	canv.SaveAs("ptSpectrum.ps");

	delete file;
	smallTree->Write("", TObject::kOverwrite);
	delete smallTree;
	delete out;
}

#ifdef CMSSWENV
vector<Jet> selectJetsCMG(const Event & ev, const vector<Lepton> & leptons, JetCorrectionUncertainty  & jecUnc, TLorentzVector * diff, unsigned mode, double ptMin, double etaMax) {
#else
vector<Jet> selectJetsCMG(const Event & ev, const vector<Lepton> & leptons, TLorentzVector * diff, unsigned mode, double ptMin, double etaMax) {
#endif

	const SingleVariableContainer<int> * jn = dynamic_cast<const SingleVariableContainer<int> *>(ev.findVariable("jn"));
	const ArrayVariableContainer<float> * jn_px = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("jn_px"));
	const ArrayVariableContainer<float> * jn_py = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("jn_py"));
	const ArrayVariableContainer<float> * jn_pz = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("jn_pz"));
	const ArrayVariableContainer<float> * jn_en = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("jn_en"));
	const ArrayVariableContainer<float> * jn_jp = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("jn_jp"));
	const ArrayVariableContainer<float> * jn_genpx = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("jn_genjpx"));
	const ArrayVariableContainer<float> * jn_genpy = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("jn_genjpy"));
	const ArrayVariableContainer<float> * jn_genpz = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("jn_genjpz"));
	const ArrayVariableContainer<float> * jn_genen = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("jn_genjen"));
	const ArrayVariableContainer<int> * jn_idbits = dynamic_cast<const ArrayVariableContainer<int> *>(ev.findVariable("jn_idbits"));

	if (mode == 1 || mode == 2) {
		if (diff == 0)
			throw string("ERROR - selectJetsCMG(): NULL pointer!");
		diff->SetPxPyPzE(0, 0, 0, 0);
	}

	vector<Jet> jets;
	for ( int i = 0; i < jn->getVal(); ++i ) {
		TLorentzVector jet(jn_px->getVal(i), jn_py->getVal(i), jn_pz->getVal(i), jn_en->getVal(i));
		bool matchesLepton = false;
		for (unsigned k = 0; k < leptons.size(); ++k) {
			TLorentzVector lepton = leptons[k].lorentzVector();
			double delR = deltaR(lepton.Eta(), lepton.Phi(), jet.Eta(), jet.Phi());
			if (delR < 0.15) {
				matchesLepton = true;
				break;
			}
		}
		if (matchesLepton)
			continue;
		if (mode == 1 || mode == 2) {
#ifdef CMSSWENV
			jecUnc.setJetEta(jet.Eta());
			jecUnc.setJetPt(jet.Pt());
			double sF = fabs(jecUnc.getUncertainty(true));
			if ( sF > 0.3 ) {
				cout << setw(10) << jet.Pt() << setw(10) << jet.Eta() << setw(10) << sF << endl;	
				throw string("ERROR!");	
			}
			if (mode == 1)
				sF = 1 + sF;
			else
				sF = 1 - sF;
			TLorentzVector newJet = sF * jet;
			(*diff) += (newJet - jet);
			jet = newJet;
#else
			throw string("ERROR: Jet Uncertainty Systematic variation not supported!");
#endif
		}
		Jet tmp;
		tmp.addFloatVar( jn_px->getName(), jet.Px() );
		tmp.addFloatVar( jn_py->getName(), jet.Py() );
		tmp.addFloatVar( jn_pz->getName(), jet.Pz() );
		tmp.addFloatVar( jn_en->getName(), jet.E() );
		tmp.addIntVar( jn_idbits->getName(), jn_idbits->getVal(i) );
		tmp.addFloatVar( jn_jp->getName(), jn_jp->getVal(i) );
		TLorentzVector genJet( jn_genpx->getVal(i), jn_genpy->getVal(i), jn_genpz->getVal(i), jn_genen->getVal(i) );
		tmp.addFloatVar( "jn_genpt", genJet.Pt() );

		jets.push_back(tmp);
	}
	return jets;
}

TLorentzVector smearJets(vector<Jet> & jets, unsigned mode) {
	TLorentzVector jetDiff;
	for ( unsigned j = 0; j < jets.size(); ++j ) {
		Jet smJet = smearedJet( jets[j], mode );
		jetDiff += (smJet.lorentzVector() - jets[j].lorentzVector());
		jets[j] = smJet;
	}
	return jetDiff;
}

Jet smearedJet(const Jet & origJet, unsigned mode) {
	if (origJet.getVarF("jn_genpt") <= 0)
		return origJet;

	//smearing factors are described in https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution
	double eta = fabs(origJet.lorentzVector().Eta());
	double pt = origJet.lorentzVector().Pt();
	double ptSF = 1.0;
	double ptSF_err = 0.06;
	if (eta < 0.5) {
		ptSF = 1.052;
		ptSF_err = sqrt(pow(0.012, 2) + pow(0.5 * (0.062 + 0.061), 2));
	} else if (eta >= 0.5 && eta < 1.1) {
		ptSF = 1.057;
		ptSF_err = sqrt(pow(0.012, 2) + pow(0.5 * (0.056 + 0.055), 2));
	} else if (eta >= 1.1 && eta < 1.7) {
		ptSF = 1.096;
		ptSF_err = sqrt(pow(0.017, 2) + pow(0.5 * (0.063 + 0.062), 2));
	} else if (eta >= 1.7 && eta < 2.3) {
		ptSF = 1.134;
		ptSF_err = sqrt(pow(0.035, 2) + pow(0.5 * (0.087 + 0.085),2));
	} else if (eta >= 2.3 && eta < 5.0) {
		ptSF = 1.288;
		ptSF_err = sqrt(pow(0.127, 2) + pow(0.5 * (0.155 + 0.153), 2));
	}

	if (mode == 1)
		ptSF += ptSF_err;
	else if (mode == 2)
		ptSF -= ptSF_err;
	
	ptSF = max(0., (origJet.getVarF("jn_genpt") + ptSF * (pt - origJet.getVarF("jn_genpt")))) / pt;  //deterministic version
	if (ptSF <= 0)
		return origJet;

	double px = origJet.getVarF("jn_px") * ptSF;
	double py = origJet.getVarF("jn_py") * ptSF;
	double pz = origJet.getVarF("jn_pz");
	double mass = origJet.lorentzVector().M();
	double en = sqrt(mass * mass + px * px + py * py + pz * pz);

	Jet smearedJet(origJet);
	smearedJet.addFloatVar( "jn_px", px );
	smearedJet.addFloatVar( "jn_py", py );
	smearedJet.addFloatVar( "jn_pz", pz );
	smearedJet.addFloatVar( "jn_en", en );
	return smearedJet;
}


vector<Photon> selectPhotonsCMG(const Event & ev) {
	const SingleVariableContainer<int> * gn = dynamic_cast<const SingleVariableContainer<int> *>(ev.findVariable("gn"));
	const ArrayVariableContainer<float> * gn_px = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("gn_px"));
	const ArrayVariableContainer<float> * gn_py = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("gn_py"));
	const ArrayVariableContainer<float> * gn_pz = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("gn_pz"));
	const ArrayVariableContainer<float> * gn_en = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("gn_en"));
	const ArrayVariableContainer<int> * gn_idbits = dynamic_cast<const ArrayVariableContainer<int> *>(ev.findVariable("gn_idbits"));
	const ArrayVariableContainer<float> * gn_chIso03 = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("gn_chIso03"));
	const ArrayVariableContainer<float> * gn_nhIso03 = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("gn_nhIso03"));
	const ArrayVariableContainer<float> * gn_gIso03 = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("gn_gIso03"));

	const ArrayVariableContainer<float> * egn_sceta = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("egn_sceta"));
	const ArrayVariableContainer<bool> * egn_isConv = dynamic_cast<const ArrayVariableContainer<bool> *>(ev.findVariable("egn_isConv"));
	const ArrayVariableContainer<float> * egn_hoe = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("egn_hoe"));
	const ArrayVariableContainer<float> * egn_sihih = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("egn_sihih"));
	const ArrayVariableContainer<float> * egn_r9 = dynamic_cast<const ArrayVariableContainer<float> *>(ev.findVariable("egn_r9"));

	const ArrayVariableContainer<int> * pidC = dynamic_cast<const ArrayVariableContainer<int> *>(ev.findVariable("gn_pid"));

	vector<Photon> photons;
	for ( int i = 0; i < gn->getVal(); ++i ) {
		Photon tmpPhoton;

		tmpPhoton.addFloatVar( gn_px->getName(), gn_px->getVal(i) );
		tmpPhoton.addFloatVar( gn_py->getName(), gn_py->getVal(i) );
		tmpPhoton.addFloatVar( gn_pz->getName(), gn_pz->getVal(i) );
		tmpPhoton.addFloatVar( gn_en->getName(), gn_en->getVal(i) );
		tmpPhoton.addIntVar( gn_idbits->getName(), gn_idbits->getVal(i) );
		tmpPhoton.addFloatVar( gn_chIso03->getName(), gn_chIso03->getVal(i) );
		tmpPhoton.addFloatVar( gn_nhIso03->getName(), gn_nhIso03->getVal(i) );
		tmpPhoton.addFloatVar( gn_gIso03->getName(), gn_gIso03->getVal(i) );

		int pid = pidC->getVal(i);

		tmpPhoton.addFloatVar( egn_sceta->getName(), egn_sceta->getVal(pid) );
		tmpPhoton.addBoolVar( egn_isConv->getName(), egn_isConv->getVal(pid) );
		tmpPhoton.addFloatVar( egn_hoe->getName(), egn_hoe->getVal(pid) );
		tmpPhoton.addFloatVar( egn_sihih->getName(), egn_sihih->getVal(pid) );
		tmpPhoton.addFloatVar( egn_r9->getName(), egn_r9->getVal(pid) );

		photons.push_back(tmpPhoton);
	}
	return photons;
}
