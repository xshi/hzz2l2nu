# Installation within CMSSW environement

 * Install CMSSW
	    cmsrel CMSSW_6_2_6
	    cd CMSSW_6_2_6/src
        cmsenv

*  Set up preselection code
        git clone https://github.com/xshi/hzz2l2nu.git HiggsAnalysis/hzz2l2nu
    	scram b
	    hash -r
		
*   Run test preselection job

        cd HiggsAnalysis/hzz2l2nu/test
        mkdir OutputSamples
	    preselHZZ2l2nu MC8TeV_ZZ.config

