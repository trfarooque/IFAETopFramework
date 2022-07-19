#!/bin/python
import time
from BatchTools import *
##
##
## CLASS JOBSET
##
##
class JobSet:
    ##_________________________________________________________________________
    ##
    def __init__(self,platform):        
        if(platform.find("lxplus")>-1):
            self.platform = "lxplus"
        elif(platform.find("pic")>-1):
            self.platform = "pic"
        elif(platform.find("aglt2")>-1):
            self.platform = "aglt2"
        else:
            printError("<!> In JobSet class constructor ... Sorry guy I do not know the platform you run on ... yet ... " + platform)
            self.platform = "pic"
        self.jobs=[]
        self.logDir=""
        self.scriptDir=""
        self.scriptName=""
        self.tarballPath=""
        self.batch="condor"
        self.queue=""
        self.jobRecoveryFileName="JobCheck.chk"
        self.writeSubmissionCommandsToFileOnly=False
        self.setUpCommand=""

    ##_________________________________________________________________________
    ##
    def addJob(self,job):
        self.jobs += [job]

    ##_________________________________________________________________________
    ##
    def size(self):
        return len(self.jobs)

    ##_________________________________________________________________________
    ##
    def clear(self):
        self.jobs=[]
        self.scriptName=""

    ##_________________________________________________________________________
    ##
    def setBatch(self,batch):
	self.batch=batch

    ##_________________________________________________________________________
    ##
    def setQueue(self,queue):
	self.queue=queue

   ##_________________________________________________________________________
    ##
    def setSubmissionCommandsFile(self,submissioncommandsfile):
        self.writeSubmissionCommandsToFileOnly=True
        self.submissioncommandsfile=submissioncommandsfile

    ##_________________________________________________________________________
    ##
    def setLogDir(self,logDir):
        self.logDir=logDir
    
    ##_________________________________________________________________________
    ##
    def setScriptDir(self,scriptDir):
        self.scriptDir=scriptDir

    ##_________________________________________________________________________
    ##
    def setScriptName(self,name):
        self.scriptName=name

    ##_________________________________________________________________________
    ##
    def setTarBall(self,tarBall):
        self.tarballPath=tarBall
    
    ##_________________________________________________________________________
    ##
    def setJobRecoveryFile(self,name):
        self.jobRecoveryFileName = name

    ##_________________________________________________________________________
    ##
    def setSetUpCommand(self,name):
        self.setUpCommand = name

    ##_________________________________________________________________________
    ##
    def Initialize(self,f):
        f.write("#!/bin/bash \n")
        f.write("export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase \n")
        f.write("source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh \n")
        f.write("echo 'Changing to directory' $TMPDIR \n")
        f.write("cd $TMPDIR \n")
        f.write("echo 'In directory' $PWD \n")
        f.write("\n")
        f.write("echo '==> Before copying tarball'\n")
        f.write("ls -lrth\n")
        f.write("\n")
        f.write("cp -r "+self.tarballPath+" AnaCode.tgz \n")
        f.write("\n")
        f.write("echo '==> After copying the tarball'\n")
        f.write("ls -lrth\n")
        f.write("\n")
        f.write("tar xf AnaCode.tgz \n")
        f.write("echo '==> After extracting the tarball'\n")
        f.write("ls -lrth\n")
        f.write("\n")
        #if self.setUpCommand=="" :
        #    f.write("rcSetup \n")
        #else :
            #recompiling the package with the given release passed as input
        #    CommandSetUp=self.setUpCommand+" \n"
        #    f.write("rm -rvf RootCoreBin \n")
        #    f.write("rm  rcSetup* \n")
            #f.write(CommandSetUp)
        #    f.write("rc find_packages \n")
        #    f.write("rc compile \n")

        f.write("cd build && asetup --restore && make && source */setup.sh \n")
        #f.write("source compileScript.sh \n")
        f.write("\n")
        f.write("echo '==> After the setup' \n")
        f.write("ls -lrth \n")
        f.write("\n")
        f.write("\n")
        f.write("\n")

    ##_________________________________________________________________________
    ##
    def Running(self,f,job):

        f.write("export OUTDIR='"+job.outDir+"' \n")
        f.write("\n")
        if(job.execName.find("python") != -1):
            f.write("cd x86_64-centos7-gcc8-opt/python/VLQAnalysis \n")
            f.write("\n")
        f.write(job.execName + " ")
        for iOption in range(len(job.jobOptions)):
            f.write("--"+job.jobOptions[iOption][0]+"="+job.jobOptions[iOption][1]+" ")
        f.write(">& "+job.jobName+".log \n")
        f.write("\n")
        f.write("echo '==> After running the code'\n")
        if self.batch == "pbs":
            f.write("mkdir -p $OUTDIR \n")
        f.write("\n")
        f.write("mv *.root $OUTDIR \n")
        f.write("mv *.log $OUTDIR \n")
        f.write("\n")
        f.write("\n")
        f.write("\n")

    ##_________________________________________________________________________
    ##
    def Terminate(self,f):

        if self.batch == "pbs":
            f.write("rm -rf $TMPDIR/*")
        f.write("\n")

    ##_________________________________________________________________________
    ##
    def writeCondorSubmitScript(self):
        f = open(self.scriptDir+"/"+self.scriptName+".sub","w")
        f.write("#Basic configuration \n")
        f.write("executable              = "+self.scriptDir+"/"+self.scriptName+" \n")
        f.write("output                  = "+self.logDir+"/VLQAna.$(ClusterId).$(ProcId).out \n")
        f.write("error                   = "+self.logDir+"/VLQAna.$(ClusterId).$(ProcId).err \n")
        f.write("log                     = "+self.logDir+"/VLQAna.$(ClusterId).log \n")
        f.write("\n")
        f.write("\n")
        if(self.platform == "aglt2"):
            f.write("#Duration of job \n")
            if(self.queue.upper() == "MEDIUM"):
                f.write("+IsMediumJob = true \n")
            elif(self.queue.upper() == "LONG"):
                f.write("+IsLongJob = true \n")
            else:
                f.write("+IsShortJob = true \n")
            f.write("\n")
            f.write("\n")
        f.write("#Script options \n")
        if(self.platform != "aglt2"):
            f.write("request_memory           = 8 GB \n")
        f.write("nJobs                   = 1 \n")
        f.write("\n")
        f.write("\n")
        #f.write("include command : mkdir "+self.logDir+"/output \n")
        #f.write("include command : mkdir "+self.logDir+"/error \n")
        #f.write("include command : mkdir "+self.logDir+"/log \n")
        f.write("#Queue subjobs \n")
        f.write("queue $(nJobs) \n")
        f.close()


    ##_________________________________________________________________________
    ##
    def writeScript(self):
        if(self.scriptName==""):
            self.scriptName = self.jobs[len(self.jobs)-1].jobName
    
        #Declaration of the output recovery tool
        f_reco_file = 0
        if not self.jobRecoveryFileName == "":
            f_reco_file = open(self.jobRecoveryFileName,"a")

        #Writting the scripts
        current_merged_script_name = self.scriptDir+"/"+self.scriptName
        f = open(current_merged_script_name,"w")
        self.Initialize(f)
        for iJob in range(len(self.jobs)):
            temp_job = self.jobs[iJob]
            self.Running(f,temp_job)
            
            #Also write a sub-script to make easier the resubmission of the jobs
            current_sub_script_name = self.scriptDir+"/sub_"+self.scriptName+"_"+temp_job.jobName
            f_sub = open(current_sub_script_name,"w")
            self.Initialize(f_sub)
            self.Running(f_sub,temp_job)
            self.Terminate(f_sub)
            f_sub.close()
        
            #Write the file to possibly relaunch the missing jobs
            if not self.jobRecoveryFileName == "":
                for iOption in range(len(temp_job.jobOptions)):
                    if temp_job.jobOptions[iOption][0].upper()=="OUTPUTFILE":
                        if self.batch == "pbs":
                            f_reco_file.write(temp_job.outDir+"/"+temp_job.jobOptions[iOption][1]+" "+current_sub_script_name+"\n")
                        else:
                            f_reco_file.write(temp_job.outDir+"/"+temp_job.jobOptions[iOption][1]+" "+self.scriptDir+"/"+self.scriptName+".sub \n")
                        break

        self.Terminate(f)
        f_reco_file.close()
        f.close()
        if self.batch != "pbs":
            self.writeCondorSubmitScript()
        os.chmod(current_merged_script_name, 0755)

    ##_________________________________________________________________________
    ##
    def submitSet(self):
        if self.batch != "pbs":
            com="condor_submit " + self.scriptDir + "/" + self.scriptName + ".sub"
            time.sleep(2)
            os.system(com)
        else:
            com=""
            if(self.platform == "lxplus"):
                com += "bsub -q "
            elif(self.platform == "pic"):
                com += "qsub -q "
            else:
                printError("The system you are running on is not supported yet ... Please move to lxplus or PIC")
            if(self.queue==""):
                self.queue="at3_short"
            com += self.queue
            com += " " + self.scriptDir+"/"+self.scriptName+" "
            if not(self.logDir==""):
                if(self.platform == "lxplus"):
                    com += " -o "+self.logDir+"/"+self.scriptName+"_batch.log -e "+self.logDir+"/"+self.scriptName+"_batch.log"
                else:
                    com += " -o "+self.logDir+" -e "+self.logDir
            if self.writeSubmissionCommandsToFileOnly:
                self.submissioncommandsfile.write("echo "+self.scriptName+"\n")
                self.submissioncommandsfile.write(com+"\n")
            else:
                os.system(com)


##
##
## CLASS JOB
##
##
class Job:
    """
    Class allowing to deal transparently with all batch systems. The interface
    is done in the class (by the means of the platform variable), and not in 
    the script itself
    """
    ##_________________________________________________________________________
    ##
    def __init__(self,platform):
        if(platform.find("pic")>-1):
            self.platform = "pic"
        elif(platform.find("lxplus")>-1):
            self.platform = "lxplus"
        elif(platform.find("aglt2")>-1):
            self.platform = "aglt2"
        else:
            printError("<!> In Job class constructor ... Sorry guy I do not know the platform you run on ... " + platform)
            self.platform = "pic"
        self.debug = False
        self.jobOptions=[]
        self.jobName=""
        self.execName=""
        self.outDir=""

    ##_________________________________________________________________________
    ##
    def setDebug(self,debug):
        self.debug = debug

    ##_________________________________________________________________________
    ##
    def addOption(self,option,value):
        self.jobOptions += [[option,value]]

    ##_________________________________________________________________________
    ##
    def setName(self,name):
        self.jobName=name
    
    ##_________________________________________________________________________
    ##
    def setExecutable(self,exe):
        self.execName = exe
    
    ##_________________________________________________________________________
    ##
    def setOutDir(self,outDir):
        self.outDir = outDir
