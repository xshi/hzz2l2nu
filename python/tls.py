#!/usr/bin/env python
"""
General tools for analysis. 

"""

__author__ = "Xin Shi <Xin.Shi@cern.ch>"
__copyright__ = "Copyright (c) Xin Shi"


import os
import sys
import subprocess
import json
import math 
import tls

def print_usage():
    print('''
Usage: 
    ./tls.py [arg]

Arguments: 
    -h                     Print this help message and exit.  
    hlt_get_passed_evts    Get the passed event number for HLT paths.
    hlt_get_rates          Get the rates for the HLT paths.
''')

    
def main():
    args = sys.argv[1:]
    if len(args) == 0 or '-h' in args:
        return print_usage()
        
    function = getattr(tls, args[0])
    function(args[1:])

    
def hlt_get_passed_evts(args):
    from dat import hltpaths 
    for sample in args:
        resdir = os.path.join(os.getcwd(), sample, 'res')
        files = get_files_in_dir(resdir, pattern='.stdout') 
        print '\n  %s \n' % sample
        hltpaths_dict = get_total_passed_evts(resdir, files, hltpaths)
        print 'HLT Path                                      Run     Passed'
        print '------------------------------------------------------------'
        for hltpath in hltpaths:
            Run = hltpaths_dict[hltpath]['Run']
            Passed = hltpaths_dict[hltpath]['Passed']
            print '%s  %s  %s' %(hltpath, Run, Passed)
        print '------------------------------------------------------------'
        dbfile = '%s.json' %sample
        db = open(dbfile, 'w')
        json.dump(hltpaths_dict, db)
        db.close()
        sys.stdout.write('Saved as %s \n' %dbfile)


def hlt_get_rates(args):
    from dat import hltpaths, crossSections13TeV, photon_jet_samples
    ilumi = 1.4e34
    sys.stdout.write('Calculating ilumi = %s \n' %ilumi )
    for hltpath in hltpaths:
        rates = []
        err = [] 
        tabfile = '%s.csv' % hltpath
        fo = open(tabfile, 'w') 
        sys.stdout.write('\n %s \n' % hltpath )
        fo.write('\n %s \n' % hltpath )
        sys.stdout.write('Sample, nevts, count, xsec, rate \n')
        fo.write('Sample, nevts, count, xsec, rate, err \n')
        for sample in photon_jet_samples:
            xsec = crossSections13TeV[sample][0]*1e-36 
            dbfile = '%s.json' % crossSections13TeV[sample][1]
            db = open(dbfile)
            hltpaths_dict = json.load(db)
            db.close()
            nevts = hltpaths_dict[hltpath]['Run']
            count = hltpaths_dict[hltpath]['Passed']
            rate, rateerr = calc_rate(ilumi, count, xsec, nevts)
            rateerrsq = pow(rateerr,2)
            line = '%s, %s, %s, %.2e, %.2f +- %.2f \n' % (
                sample, nevts, count, xsec, rate, rateerr)
            sys.stdout.write(line)
            fo.write(line) 
            rates.append(rate)
            err.append(rateerrsq)
        tot_rate = sum(rates)
        tot_err = math.sqrt(sum(err))
        tot_line = 'The total rate is %.f +- %.f\n' %(tot_rate, tot_err)
        sys.stdout.write(tot_line)
        fo.write(tot_line)
        fo.close()
        sys.stdout.write('Saved as %s\n' %tabfile)

    
#----------------------------------------------------------------
#   Supporting function 
#----------------------------------------------------------------

def calc_rate(ilumi, count, xsec, nevts):
    rate = xsec*ilumi*(count/float(nevts))
    rateerr = ((xsec * ilumi)/float(nevts)) * math.sqrt(count)
    return rate, rateerr 


def get_files_in_dir(path, pattern=None):
    files = []
    cmd = 'ls %s' %path
    output = proc_cmd(cmd)
    if 'No such file or directory' in output:
        sys.stdout.write(output)
        sys.stdout.write('Please check cmd: \n')
        sys.stdout.write(cmd)
        sys.stdout.write('\n')
        sys.exit()

    content = output.strip()
    lines = content.split('\n')
    for l in lines:
        if pattern is None or pattern in l:
            files.append(l)
    return files


def get_total_passed_evts(path, files, hltpaths):
    hltpaths_dict = {}
    for hltpath in hltpaths: 
        hltpaths_dict[hltpath] = {'Run': 0, 'Passed': 0}

    for fi in files:
        f = StdoutFile(os.path.join(path, fi))
        f.get_total_passed_evts(hltpaths_dict)

    return hltpaths_dict


def proc_cmd(cmd, test=False, verbose=1, procdir=None, shell=False):
    if test:
        sys.stdout.write(cmd+'\n')
        return 

    # No need to add quote in the cmd! 
    cwd = os.getcwd()
    if procdir is not None:
        os.chdir(procdir)
    
    args = cmd.split()

    if isinstance(cmd, list) or shell:
        args = cmd 

    process = subprocess.Popen(args, stdout=subprocess.PIPE,
                               stderr=subprocess.STDOUT, shell=shell)
    stdout = process.communicate()[0]

    if 'error' in stdout:
        sys.stdout.write(stdout)
        
    if procdir is not None:
        os.chdir(cwd)

    return stdout



# ------------------------------------------------------------
# Classes 
# ------------------------------------------------------------

class StdoutFile(object):
    "Handle CMSSW.stdout file"

    def __init__(self, filename):
        self.data = []
        fi = open(filename, 'r')
        for line in fi:
            self.data.append(line)
        fi.close()
        
    def get_total_passed_evts(self, hltpaths_dict):
        "Get the total and passed evetns for HLT paths"

        for line in self.data:
            line = line.strip()
            for hltpath in hltpaths_dict.keys(): 
                if hltpath in line:
                    items = line.split()
                    try: 
                        nrun = int(items[3])
                    except ValueError:
                        continue

                    npassed = int(items[4])
                    Run = hltpaths_dict[hltpath]['Run']
                    Passed = hltpaths_dict[hltpath]['Passed']

                    hltpaths_dict[hltpath]['Run'] = Run + nrun
                    hltpaths_dict[hltpath]['Passed'] = Passed + npassed

        return hltpaths_dict 


if __name__ == '__main__':
    main()
    
