#!/usr/bin/env python
"""
General tools for analysis. 

"""

__author__ = "Xin Shi <Xin.Shi@cern.ch>"
__copyright__ = "Copyright (c) Xin Shi"


import os
import sys
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
    print args 


if __name__ == '__main__':
    main()
    
