import FWCore.ParameterSet.Config as cms

runProcess = cms.PSet(
    input = cms.vstring("../data/ZZJetsTo2L2Nu.root"),
    outdir = cms.string("tmp"),
    suffix = cms.string("_test"),
    dirName = cms.string("dataAnalyzer"),
    isMC = cms.bool(True),
    debug = cms.bool(True)	
)

