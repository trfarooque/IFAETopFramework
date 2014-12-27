import os
import string
import random
import re
import time, getpass
import socket
import sys
import datetime

from BatchTools import *
from Job import *
from Samples import *

##________________________________________________________
## OPTIONS
debug=True
nFilesSplit = 1
nMerge=1
useList = False
channels=["ELE","MUON"]
##........................................................

##________________________________________________________
## Defines some useful variables
platform = socket.gethostname()
now = datetime.datetime.now().strftime("%Y_%m_%d_%H%M")
here = os.getcwd()
##........................................................

##________________________________________________________
## Defining the paths and the tarball
inputDir="/nfs/pic.es/user/j/jmontejo/scratch/Histogrammer_files/V24_prod1_p15/"
listFolder=here+"/Lists_"+now
scriptFolder=here+"/Scripts_"+now
packageName="AnalysisCode"
tarballPath="/nfs/pic.es/user/l/lvalery/AnaCode.tgz"
outputDir="/nfs/pic.es/user/l/lvalery/TestJobs/"
##........................................................

##________________________________________________________
## Creating usefull repositories
os.system("mkdir -p " + listFolder) #list files folder
os.system("mkdir -p " + scriptFolder) #list files folder
##........................................................

##________________________________________________________
## Getting all samples and their associated weight/object systematics
ttbar       = getSampleUncertainties("ttbar","ttbar117050",ObjectSystematics,CommonWeightSystematics)
Dibosons    = getSampleUncertainties("Dibosons","Dibosons",ObjectSystematics,CommonWeightSystematics)
singleTop   = getSampleUncertainties("singleTop","singleTop_",ObjectSystematics,CommonWeightSystematics)
Wjets       = getSampleUncertainties("Wjets","Wjets",ObjectSystematics,CommonWeightSystematics)
Zjets       = getSampleUncertainties("Zjets","ZjetsPythia_HFOR_",ObjectSystematics,CommonWeightSystematics)
ttH         = getSampleUncertainties("ttH","ttH_125_",ObjectSystematics,CommonWeightSystematics)
tZ          = getSampleUncertainties("tZ","tZ_",ObjectSystematics,CommonWeightSystematics)
Data        = getSampleUncertainties("Data","Data",ObjectSystematics,CommonWeightSystematics)
printGoodNews("--> All samples recovered")
##........................................................

##________________________________________________________
## Loop over channels
printGoodNews("--> Performing the loop over samples and jobs")
nJobs = 0
for channel in channels:

    ##________________________________________________________
    ## Makes the list of files to run over (nominal and systematics) depending on the settings (splitting, ...)
    Samples = []
    Samples += [getSampleJobs(ttbar,InputDir=inputDir+"/"+channel+"/",NFiles=nFilesSplit,UseList=useList,ListFolder=listFolder)]
    '''Samples += [getSampleJobs(Data,InputDir=inputDir+"/"+channel+"/",NFiles=nFilesSplit,UseList=useList,ListFolder=listFolder)]
    Samples += [getSampleJobs(Dibosons,InputDir=inputDir+"/"+channel+"/",NFiles=nFilesSplit,UseList=useList,ListFolder=listFolder)]
    Samples += [getSampleJobs(singleTop,InputDir=inputDir+"/"+channel+"/",NFiles=nFilesSplit,UseList=useList,ListFolder=listFolder)]
    Samples += [getSampleJobs(Wjets,InputDir=inputDir+"/"+channel+"/",NFiles=nFilesSplit,UseList=useList,ListFolder=listFolder)]
    Samples += [getSampleJobs(Zjets,InputDir=inputDir+"/"+channel+"/",NFiles=nFilesSplit,UseList=useList,ListFolder=listFolder)]
    Samples += [getSampleJobs(ttH,InputDir=inputDir+"/"+channel+"/",NFiles=nFilesSplit,UseList=useList,ListFolder=listFolder)]
    Samples += [getSampleJobs(tZ,InputDir=inputDir+"/"+channel+"/",NFiles=nFilesSplit,UseList=useList,ListFolder=listFolder)]'''

    ##________________________________________________________
    ## Loop over samples and files
    for iSample in range(len(Samples)):
        SName = Samples[iSample][0]['name'] # sample name
    
        #Setting caracteristics for the JobSet object
        JOSet = JobSet(platform)
        JOSet.setScriptDir(scriptFolder)
        JOSet.setLogDir(outputDir)
    
        JOSet.setTarBall(tarballPath)#tarball sent to batch (contains all executables)
        JOSet.setPackageName(packageName)#name of the first repository in the tarball (containing all the packages)

        if(len(Samples[iSample])>0):
            JOSet.setScriptName(SName+"_"+Samples[iSample][0]['objSyst'])


        ##________________________________________________________
        ## Loop over jobs for this sample (multiple files or systematics)
        for iJob in range(len(Samples[iSample])):

            #Declare the Job object (one job = one code running once)
            jO = Job(platform)
        
            #Name of the executable you want to run
            jO.setExecutable("TestExecutable")
            jO.setDebug(debug)

            name=SName+"_"+channel+"_"+Samples[iSample][iJob]['objSyst']+"_"+`iJob` #name of the job

            jO.setName(name)
        
            jO.addOption("outputFile","out"+jO.execName+"_"+name+".root") #name of the output file
            jO.addOption("inputFile",Samples[iSample][iJob]['filelist']) #name of the input file (already got from ls)
            jO.addOption("textFileList","false")
            jO.addOption("sampleName",Samples[iSample][iJob]['sampleType'])

            if(channel=="MUON"):
                jO.addOption("inputTree","Muon/4jetin25252525_0elecex25topcommonetacommon_1muonex25topcommonetacommon/0btagin0.7892MV1/atree")
            elif(channel=="ELE"):
                jO.addOption("inputTree","Electron/4jetin25252525_1elecex25topcommonetacommon_0muonex25topcommonetacommon/0btagin0.7892MV1/atree")
                    
            jO.addOption("msgLevel","standard")
        
            if(Samples[iSample][iJob]['objSyst']=="nominal"):#we run on the nominal ... so we also compute the weight systematics
                jO.addOption("computeWeightSys","true")
                jO.addOption("sysName",Samples[iSample][iJob]['weightSyst'])
            else:
                jO.addOption("computeWeightSys","false")
        
            jO.addOption("useTRF","true")
        
            jO.setOutDir(outputDir)

            JOSet.addJob(jO)
    
            if JOSet.size()==nMerge:
                JOSet.writeScript()
                JOSet.submitSet()
                JOSet.clear()

        if(JOSet.size()>0):
            JOSet.writeScript()
            JOSet.submitSet()
            JOSet.clear()
##........................................................





