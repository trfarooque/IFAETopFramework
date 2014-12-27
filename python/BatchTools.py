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
def getSampleJobs(sample,InputDir="",NFiles="",UseList=False,ListFolder=""):
    #return a library containing the necessary informations
    if (UseList and ListFolder==""):
        printError("No ListFolder given ... returns ...")
        return

    SampleName = sample['name']
    
    printGoodNews("--> Generating the object for '"+SampleName+"' sample")

    # Creates the file lists (always needed even when using list files in command line)
    Result = []
    systs = sample['objSyst']
    for iSys in range(len(systs)):
        
        ListName = ListFolder + "/" + SampleName + "_" + systs[iSys]
        failed = produceList([sample['name'],systs[iSys]],InputDir,ListName)
        if(failed):#in case there are no files, skip this systematic
            continue
        nListFiles = splitList(ListName,ListName+"_",NFiles)

        listWeight=""
        if(systs[iSys]=="nominal"):
            for iWS in sample['weightSyst']:   listWeight+=iWS+","

        #for each file, creates a dictionnary
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

            ## builds a dictionnary with all interesting informations for the loop
            sample = {
                        'sampleType':sample['sampleType'],
                        'name':SampleName,
                        'filelist':fileListCommandLine,
                        'objSyst':systs[iSys],
                        'weightSyst':listWeight
                    }
            Result += [sample]
    return Result

#___________________________________________________________________
#
def produceList(Patterns, InputDirectory, listName):
    com = ""
    
    if(InputDirectory.find("eos")>-1):#if you use the eos system, you have to change a bit the commands
        com = "eos find " + InputDirectory
    else:
        com = "ls "+InputDirectory+"*.root*"
        
    for iPattern in range(len(Patterns)):
        com += " | grep "+Patterns[iPattern]
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
