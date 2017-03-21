#! /usr/bin/env python

import sys
from scapy.all import *

p = IP(dst=sys.argv[1])/UDP()
ps = str(p)
hs = ' ' .join("{:02x}".format(ord(x)) for x in ps)
lens = [2,2,5,5,5,2,2,5,11,5,5,5,5,5,5]
c = 0

for i in lens:
        print hs[c:c+i]
        c += (i+1)

