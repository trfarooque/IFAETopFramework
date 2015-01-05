from BatchTools import *


class JobSet:
    ##_________________________________________________________________________
    def __init__(self,platform):
        
        if(platform.find("lxplus")>-1):
            self.platform = "lxplus"
        elif(platform.find("pic")>-1):
            self.platform = "pic"
        else:
            printError("<!> In Job class constructor ... Sorry guy I do not know the platform you run on ... " + platform)
            self.platform = "lxplus"
        self.jobs=[]
        self.logDir=""
        self.scriptDir=""
        self.scriptName=""
        self.tarballPath=""
        self.packageName=""
        self.queue=""

    ##_________________________________________________________________________
    def addJob(self,job):
        self.jobs += [job]

    ##_________________________________________________________________________
    def size(self):
        return len(self.jobs)

    ##_________________________________________________________________________
    def clear(self):
        self.jobs=[]
        self.scriptName=""

    ##_________________________________________________________________________
    def setLogDir(self,logDir):
        self.logDir=logDir
    
    ##_________________________________________________________________________
    def setScriptDir(self,scriptDir):
        self.scriptDir=scriptDir

    ##_________________________________________________________________________
    def setScriptName(self,name):
        self.scriptName=name

    ##_________________________________________________________________________
    def setTarBall(self,tarBall):
        self.tarballPath=tarBall
    
    ##_________________________________________________________________________
    def setPackageName(self,package):
        self.packageName=package

    ##_________________________________________________________________________
    def Initialize(self,f):
        f.write("#!/bin/bash \n")
        if(self.platform=="pic"):
            f.write("export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase \n")
            f.write("source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh \n")
            f.write("cd $TMPDIR \n")
            f.write("\n")
        f.write("echo '==> Before copying tarball'\n")
        f.write("ls -lrth\n")
        f.write("\n")
        f.write("cp -r "+self.tarballPath+" AnaCode.tgz \n")
        f.write("tar -xf AnaCode.tgz \n")
        f.write("cd "+self.packageName+" \n")
        f.write("rcSetup \n")
        f.write("\n")
        f.write("echo '==>After the download' \n")
        f.write("ls -lrth \n")
        f.write("\n")
        f.write("\n")
        f.write("\n")

    ##_________________________________________________________________________
    def Running(self,f,job):

        f.write("export OUTDIR='"+job.outDir+"' \n")
        f.write("\n")
        f.write(job.execName + " ")
        for iOption in range(len(job.jobOptions)):
            f.write("--"+job.jobOptions[iOption][0]+"="+job.jobOptions[iOption][1]+" ")
        f.write(">& "+job.jobName+".log \n")
        f.write("\n")
        f.write("echo '==> After running the code'\n")
        f.write("mkdir -p $OUTDIR\n")
        f.write("\n")
        f.write("mv *.root $OUTDIR \n")
        f.write("mv *.log $OUTDIR \n")
        f.write("\n")
        f.write("\n")
        f.write("\n")

    ##_________________________________________________________________________
    def Terminate(self,f):
        
        if(self.platform=="pic"):
            f.write("rm -rf $TMPDIR/*")
        f.write("\n")

    ##_________________________________________________________________________
    def writeScript(self):
        if(self.scriptName==""):
            self.scriptName = self.jobs[len(self.jobs)-1].jobName
        
        f = open(self.scriptDir+"/"+self.scriptName,"w")

        self.Initialize(f)
        for iJob in range(len(self.jobs)):
            self.Running(f,self.jobs[iJob])

        self.Terminate(f)
        f.close()
        
    ##_________________________________________________________________________
    def submitSet(self):
        com=""
        com += "qsub -q "
        if(self.queue==""):
            self.queue="at3_short"
        com += self.queue
        com += " " + self.scriptDir+"/"+self.scriptName+" "
        if not(self.logDir==""):
            com += " -o "+self.logDir+" -e "+self.logDir
        os.system(com)

class Job:
    """
    Class allowing to deal transparently with all batch systems. The interface
    is done in the class (by the means of the platform variable), and not in 
    the script itself
    """

    ##_________________________________________________________________________
    def __init__(self,platform):
        
        if(platform.find("pic")>-1):
            self.platform = "pic"
        else:
            printError("<!> In Job class constructor ... Sorry guy I do not know the platform you run on ... " + platform)
            self.platform = "lxplus"
        self.debug = False
        self.jobOptions=[]
        self.jobName=""
        self.execName=""
        self.outDir=""

    ##_________________________________________________________________________
    def setDebug(self,debug):
        self.debug = debug

    ##_________________________________________________________________________
    def addOption(self,option,value):
        self.jobOptions += [[option,value]]

    ##_________________________________________________________________________
    def setName(self,name):
        self.jobName=name
    
    ##_________________________________________________________________________
    def setExecutable(self,exe):
        self.execName = exe
    
    ##_________________________________________________________________________
    def setOutDir(self,outDir):
        self.outDir = outDir
    
