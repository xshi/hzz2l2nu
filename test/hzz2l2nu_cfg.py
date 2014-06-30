import FWCore.ParameterSet.Config as cms

runProcess = cms.PSet(
    #input = cms.vstring("../data/ZZJetsTo2L2Nu.root"),
    input = cms.vstring("../data/G_Photon.root"),
    outdir = cms.string("tmp"),
    suffix = cms.string("_test"),
    dirName = cms.string("dataAnalyzer"),
    isMC = cms.bool(True),
    debug = cms.bool(True), 
    triggerPaths = cms.vstring(
        'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v',
        'HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v',
        'HLT_Mu17_Mu8_v',
        'HLT_Mu17_TkMu8_v',
        'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v',
        'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v',
        'HLT_IsoMu24_eta2p1_v',
        'HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_v',
        'HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_v',
        'HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_v',
        'HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_v',
        'HLT_Photon250_NoHE_v1_v',
        'HLT_Photon300_NoHE_v1_v',
        'HLT_Ele27_WP80_v',
        'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v',
        'HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_PFMET40_v'
    ),	
)

