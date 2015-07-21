#!/usr/bin/env python
import os
import string
import random
import re
import time
import socket
import getpass
import sys
import glob

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

#___________________________________________________________________
#
def getSampleJobs(sample,InputDir="",NFiles="1",UseList=False,ListFolder="./",exclusions=[],splitWeightSyst=False):
    
    # Return a library containing the necessary informations
    if (UseList and ListFolder==""):
        printError("No ListFolder given ... returns ...")
        return

    SampleName = sample['name']

    printGoodNews("--> Generating the object for '"+SampleName+"' sample")

    # Creates the file lists (always needed even when using list files in command line)
    Result = []

    # Creates the list of object systematics (different instances of the code)
    Systs = []
    for iterator_sys in sample['objSyst']:
        if iterator_sys['oneSided']:#one-sided systematics
            Systs += [iterator_sys['nameUp']]
        else:
            Systs += [iterator_sys['nameUp']]
            Systs += [iterator_sys['nameDown']]

    if(splitWeightSyst):
    	for iterator_wgt in sample['weightSyst']:
        	if iterator_wgt['oneSided']:#one-sided systematics
            		Systs += [iterator_wgt['nameUp']+"_WeightSys"]
        	else:
            		Systs += [iterator_wgt['nameUp']+"_WeightSys"]
            		Systs += [iterator_wgt['nameDown']+"_WeightSys"]

    # Loop over all the object systematics to be processed (including the nominal)
    for iSys in range(len(Systs)):
        # Producing the list of all files corresponding to the expected ones
        ListName = ListFolder + "/" + SampleName + "_" + Systs[iSys]

        failed = 1
        
        if(Systs[iSys].find("_WeightSys")==-1):#if obj systs
            failed = produceList([sample['name'],Systs[iSys]],InputDir,ListName,exclusions)
        else:#if weight sys, take nom file
            failed = produceList([sample['name'],"nominal"],InputDir,ListName,exclusions)

        if(failed>=1):#in case there are no files, skip this systematic
            printWarning("I didn't find any files for the systematic *" + Systs[iSys] + "*. Sure it's expected ? I continue with the next one.")
            continue

        # Split the list according to the number of input files to be merged
        nListFiles = splitList(ListName,ListName+"_",NFiles)

        # Get the weight systematics in case we run over the nimnal object
        listWeight=""
        if(Systs[iSys]=="nominal" and splitWeightSyst==False):
            for iWS in sample['weightSyst']:
                if iWS['oneSided']:
                    listWeight+=iWS['name']+","
                else:
                    listWeight+=iWS['nameUp']+","
                    listWeight+=iWS['nameDown']+","
        elif(splitWeightSyst and Systs[iSys].find("_WeightSys")>-1):
            listWeight+=Systs[iSys].replace("_WeightSys","")

        # For each file/syst, creates a dictionnary
        for i in range(nListFiles):
            listFileName = ListName + "_"
            if(i<1000): listFileName = listFileName + "0"
            if(i<100): listFileName = listFileName + "0"
            if(i<10): listFileName = listFileName + "0"
            listFileName = listFileName + `i`
        
            fileListCommandLine=""
            if not(UseList):
                fileListCommandLine = getCommandLineFromFile(listFileName)
            else:
                fileListCommandLine = listFileName

            # Builds a dictionnary with all interesting informations for the loop
            sample = {
                        'sampleType':sample['sampleType'],
                        'name':SampleName,
                        'filelist':fileListCommandLine,
                        'objSyst':Systs[iSys],
                        'weightSyst':listWeight
                    }
            Result += [sample]
    return Result

#___________________________________________________________________
#
def produceList(Patterns, InputDirectory, listName,exclusions=[]):
    com = "ls "+InputDirectory+"{*,*/*}.root*"
    for iPattern in range(len(Patterns)):
	if Patterns[iPattern]=="": continue
        com += " | grep "+Patterns[iPattern]
    for iExclusion in range(len(exclusions)):
        com += " | grep -v "+exclusions[iExclusion]

    com += " | grep -v \":\""
    com += " > "+listName
    result = os.system(com)
    return result

#___________________________________________________________________
#
def splitList(name, outName, Nlines):
    os.system("rm -f "+outName+"*")
    com = "split -d -a 4 -l "+`Nlines`+" "+name+" "+outName
    os.system(com)
    return int(os.popen("ls "+outName+"* |wc ").readlines()[0].split()[0])

#___________________________________________________________________
#
def getCommandLineFromFile(listFileName):
    f = open(listFileName,"r")
    command_line = ""
    for line in f:
        line=line.replace("\n","")
        command_line += line
        command_line += ","
    command_line=command_line[:-1]#removing the last coma
    return command_line
