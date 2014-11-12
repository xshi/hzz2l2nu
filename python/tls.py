#!/usr/bin/env python
"""
General tools for analysis. 

"""

__author__ = "Xin Shi <Xin.Shi@cern.ch>"
__copyright__ = "Copyright (c) Xin Shi"


import os
import sys
import subprocess
import tls


def print_usage():
    print('''
Usage: 
    ./tls.py [arg]

Arguments: 
    -h                     Print this help message and exit.  
    hlt_get_passed_evts    Get the passed event number for HLT menus.''')

    
def main():
    args = sys.argv[1:]
    if len(args) == 0 or '-h' in args:
        return print_usage()
        
    function = getattr(tls, args[0])
    function(args[1:])

    
def hlt_get_passed_evts(args):
    resdir = '%s/res' %args[0]
    files = get_files_in_dir(resdir, pattern='.stdout') 
    print files
    print len(files)
    
    
#----------------------------------------------------------------
#   Supporting function 
#----------------------------------------------------------------

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



if __name__ == '__main__':
    main()
    
