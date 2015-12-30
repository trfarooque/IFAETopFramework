import os
import os.path
import string
import random
import re
import time, getpass
import socket
import sys
import datetime
from ROOT import *

sys.path.append("../../IFAETopFramework/python/")

from BatchTools import *

if(len(sys.argv)!=2):
    printWarning("Output checker ==> Wrong input arguments")
    print "    python "+sys.argv[0]+" <name of .chk file>"
    sys.exit()

f = open(sys.argv[1],"read")
fNew = open(sys.argv[1]+".new","write")

nMissing = 0
nZombie = 0
nGood = 0
filesChecked = 0
for line in f:
    filesChecked += 1
    
    if(filesChecked%100==0):
        print "==> "+`filesChecked`+" files checked"
    
    fileToCheck = line.replace("\n","")

    rootFile = TFile(fileToCheck,"read")

    if not(os.path.isfile(fileToCheck)):
        printError("ABSENT: "+fileToCheck)
        nMissing += 1
        fNew.write(fileToCheck+"\n")
        continue

    if rootFile.IsZombie():
        printError("ZOMBIE: "+fileToCheck)
        fNew.write(fileToCheck+"\n")
        nZombie += 1
        continue

    rootFile.Close()
    nGood += 1

f.close()
fNew.close()

print "============================="
print "SUMMARY"
print "============================="
print "Analysed files:   "+`filesChecked`
print "Good files :      "+`nGood`
print "Absent files :    "+`nMissing`
print "Corrupted files : "+`nZombie`
print "============================="
