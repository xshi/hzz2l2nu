CMSSW_MacroLibrary
==================

Library

Installation on lxplus:

ssh <username>@lx64slc5.cern.ch
setenv SCRAM_ARCH slc5_amd64_gcc472
cmsrel CMSSW_6_2_6
cd CMSSW_6_2_6/src
cmsenv
cd -
git clone https://github.com/qbaza/CMSSW_MacroLibrary.git MacroLibrary
cd MacroLibrary
make
