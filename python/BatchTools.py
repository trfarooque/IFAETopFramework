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

##___________________________________________________________________
##
def printError(text):
    #prints text in red
    os.system("echo -e '\\033[41;1;37m "+text+" \\033[0m'")
##___________________________________________________________________
##
def printWarning(text):
    #prints text in orange
    os.system("echo -e '\\033[43;1;37m "+text+" \\033[0m'")
##___________________________________________________________________
##
def printGoodNews(text):
    #prints text in green
    os.system("echo -e '\\033[42;1;37m "+text+" \\033[0m'")

##___________________________________________________________________
##
def getSampleJobs(sample,InputDir="",NFiles="1",UseList=False,ListFolder="./",exclusions=[], useDiffFilesForObjSyst=False):

    # Return a library containing the necessary informations
    if (UseList and ListFolder==""):
        printError("No ListFolder given ... returns ...")
        return

    SampleName = sample['name']

    printGoodNews("--> Generating the object for '"+SampleName+"' sample")

    # Output of function
    Result = []

    # Creates the list of object systematics (different instances of the code)
    Systs = []
    for iterator_sys in sample['objSyst']:
        if iterator_sys['oneSided']:#one-sided systematics
            Systs += [iterator_sys['name']]
        else:
            Systs += [iterator_sys['nameUp']]
            Systs += [iterator_sys['nameDown']]

    # In case the systematics are in different trees, no list to list the files multiple times
    listCreated = False
    ListName = ""

    # Produce the total files list if the option useTotalFileFirst is set to True (avoids doing ls multiple times)
    totalFileName = "AllFiles_"+InputDir.replace("/","_")
    if not os.path.exists(ListFolder + "/" + totalFileName ) :
        failed = produceList(["",],InputDir,ListFolder + "/" + totalFileName)
        if failed>=1 :
            printWarning("I didn't find any files for sample "+sample['name']+". Sure it's expected ? I continue with the next one.")
            return

    # do the exclusion here so that they are not stored in the file (the filename does not include the exclusions, so storing in file is problematic)
    allfiles=[]
    with open(ListFolder + "/" + totalFileName) as f:
        for x in f.read().splitlines():
            exclude=False
            for excl in exclusions:
                if excl in x:
                    exclude=True
            if not exclude:
                allfiles.append(x)

    # Loop over all the object systematics to be processed (including the nominal)
    for iSys in range(len(Systs)):

        # Producing the list of all files corresponding to the template
        filelist=[]

        if(useDiffFilesForObjSyst):
            for filename in allfiles:
                if Systs[iSys] in filename and SampleName in filename:
                    filelist.append(filename)
        else:
            for filename in allfiles:
                if SampleName in filename:
                    filelist.append(filename)

        if(len(filelist)<=0): #in case there are no files, skip this systematic
            if Systs[iSys]=="":
                printWarning("I didn't find any files for the sample. Sure it's expected ? I continue with the next one.")
            else:
                printWarning("I didn't find any files for the systematic *" + Systs[iSys] + "*. Sure it's expected ? I continue with the next one.")
            continue

        # Split the list according to the number of input files to be merged
        nListFiles=len(filelist)/int(NFiles)
        if len(filelist) % NFiles>0:
            nListFiles=nListFiles+1

        # Get the weight systematics in case we run over the nominal object
        listWeight=""
        if( Systs[iSys].upper()=="NOMINAL" ):
            for iWS in sample['weightSyst']:
                if iWS['oneSided']:
                    listWeight+=iWS['name']+","
                else:
                    listWeight+=iWS['nameUp']+","
                    listWeight+=iWS['nameDown']+","

        # For each file/syst, creates a dictionnary
        for i in range(nListFiles):
            thisJobFileList=[]
            for ifile in range(i*NFiles,min(len(filelist),(i+1)*NFiles)):
                thisJobFileList.append(filelist[ifile])

            # Builds a dictionnary with all interesting informations for the loop
            sample = {
                        'sampleType':sample['sampleType'],
                        'name':SampleName,
                        'filelist':",".join(thisJobFileList),
                        'objSyst':Systs[iSys],
                        'weightSyst':listWeight,
                        'objectTree':Systs[iSys]
                    }
            Result += [sample]

    return Result

##___________________________________________________________________
##
def filterListWithTemplate( originalTotalFile, templates, filterFile):
    com = "less "+originalTotalFile
    if len(templates):
        for temp in templates:
            com += " | grep "+temp
    com += " > "+filterFile
    result = os.system(com)
    return result

##___________________________________________________________________
##
def produceList(Patterns, InputDirectory, listName):
    com = "ls "+InputDirectory+"{*,*/*}.root*"
    if "/eos/atlas/" in InputDirectory:
        com = "/afs/cern.ch/project/eos/installation/atlas/bin/eos.select find -f "+InputDirectory+" | grep \"\\.root\""
    for iPattern in range(len(Patterns)):
	if Patterns[iPattern]=="": continue
        com += " | grep "+Patterns[iPattern]

    com += " | grep -v \":\""
    if "/eos/atlas/" in InputDirectory:
        com+=" | sed \'s/\/eos\/atlas\//root:\/\/eosatlas\/\/eos\/atlas\//g\'"
    com += " > "+listName
    result = os.system(com)

    return result

##___________________________________________________________________
##
def getCommandLineFromFile(listFileName):
    f = open(listFileName,"r")
    command_line = ""
    for line in f:
        line=line.replace("\n","")
        command_line += line
        command_line += ","
    command_line=command_line[:-1]#removing the last coma
    return command_line

##___________________________________________________________________
##
def prepareTarBall(pathToPackage,pathToTarball):
    current_folder = os.getcwd()
    os.chdir(pathToPackage)
    
    printGoodNews("=> Creating tarball !")
    print "   -> ", pathToTarball
    com = "tar czf " + pathToTarball + " * .RootCoreBin"
    os.system(com)
    printGoodNews("=> Tarball done :-)")
    
    os.chdir(current_folder)

