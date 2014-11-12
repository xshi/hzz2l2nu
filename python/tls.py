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
    print("./tls.py -h")

    
def main():
    args = sys.argv[1:]
    if len(args) == 0 or '-h' in args:
        return print_usage()
        
    function = getattr(tls, args[0])
    function(args[1:])




if __name__ == '__main__':
    main()
    
