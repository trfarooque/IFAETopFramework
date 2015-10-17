########################################################
#
# CheckFiles.py
# -------------
#   -> Checks if the number of local files corresponds
#      to the one on the grid
#   -> Check the potential corruption of the files
#   -> In case you want to check if all the files with template are there
#
#   EXECUTION:
#       python CheckFiles.py --path=<path> [--check_corruption=true]
#       []: optional arguments
#
#   For template checking (e.g. check that all files corresponding to "rucio list-files user.toto.*.bla.root")
#       python CheckFiles.py --path=<path> --template=<template> [--check_corruption=true]
#       []: optional arguments
#
########################################################

#!/bin/python

import glob
import os
import subprocess
import sys
from ROOT import *

#___________________________________________________________________
#
def printError(text):
    #prints text in red
    os.system("echo -e '\\033[41;1;37m "+text+" \\033[0m'")
#___________________________________________________________________
#
def printWarning(text):
    #prints text in orange
    os.system("echo -e '\\033[43;1;37m "+text+" \\033[0m'")
#___________________________________________________________________
#
def printGoodNews(text):
    #prints text in green
    os.system("echo -e '\\033[42;1;37m "+text+" \\033[0m'")

##________________________________________________________________
##
def CheckNumberOfFilesForDownloadedSamples( folder ):
    #Check number in AMI
    sample = folder.split("/")
    sample = sample[len(sample)-1]
    comRucio = "rucio list-files \""+sample+"\""
    command = comRucio + " | grep \"Total files :\" | awk '{print $4}'"
    out = os.popen(command).read()
    if(out==""):
        print "<!> Cannot find dataset: "+sample
        return

    n_files_rucio = int(out)

    #Check number of local files in folder
    localfiles = glob.glob(folder+"*/*.root*")
    n_local_files = len(localfiles)

    #Check corruction if needed
    corrupted = []
    if(checkCorruption):
        for _file in localfiles:
            f = TFile(_file,"read")
            if(f.IsZombie()):
                corrupted += [_file]
            f.Close()

    #Comparing number of files
    if(n_local_files==n_files_rucio):
        if(len(corrupted)==0):
            printGoodNews("=> Fine for this sample : " + sample)
        else:
            printWarning("=> Corrupted files detected. List is below:")
            for corr in corrupted:
                print "        -> "+corr
    else:
        printError( "=> ERROR for this sample : " + sample)
        print "       - local : "+`n_local_files`
        print "       - rucio : "+`n_files_rucio`

##________________________________________________________________
##
def CheckFiles(inputFolder):
    list_of_datasets = glob.glob(inputFolder+"/*")
    for dataset in list_of_datasets:
        CheckNumberOfFilesForDownloadedSamples(dataset)

##________________________________________________________________
##
def CheckFilesFromTemplate(inputFolder,templateFile):
    #Get the expected results from rucio
    comRucio = "rucio ls \""+templateFile+"\""
    command = comRucio + " | grep \"COLLECTION\" | awk '{print $2}'"
    out = os.popen(command).read()
    if(out==""):
        printError("<!> Template "+templateFile+" has no results. Please check !")
        return
    exp_datasets = []
    for x in out.split("\n"):
        if(x!=""):
            exp_datasets += [inputFolder + "/" + x.split(":")[1] ]

    for dataset in exp_datasets:
        CheckNumberOfFilesForDownloadedSamples(dataset)

##________________________________________________________________
##
## Decoding arguments
##________________________________________________________________
##
bigInputFolder = ""
templateFile = ""
checkCorruption = False
if(len(sys.argv)>1):
    for iArg in range(1,len(sys.argv)):
        splitted = sys.argv[iArg].split("=")
        if(splitted[0]=="--path"):
            bigInputFolder = splitted[1]
        elif(splitted[0]=="--check_corruption"):
            if(splitted[1].upper()=="TRUE" or splitted[1]=="1"):
                checkCorruption = True
        elif(splitted[0]=="--template"):
            templateFile = splitted[1]
        else:
            printError("Please check your arguments !")
            exit(-1)
else:
    exit(-1)

##________________________________________________________________
##
if bigInputFolder!="" and templateFile=="":
    CheckFiles(bigInputFolder)
elif bigInputFolder!="" and templateFile!="":
    CheckFilesFromTemplate(bigInputFolder,templateFile)
