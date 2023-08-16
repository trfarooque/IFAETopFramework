#!/usr/bin/env python

'''
Place where are stored the object and weight systematics for all samples.
They are recorded into list of Objects that are declared in the following way:

getSystematics(name="BJesUnc",nameUp="BJesUnc_up",nameDown="BJesUnc_down",oneSided=False)

--> name: name of the nuisance parameter
--> nameUp: pattern used to recognise the systematic variation
    . for the weight systematic: names recored in SystManager
    . for the object systematics: pattern to recognise the corresponding files
--> nameDown: same
--> oneSided: is the systematic one-sided (e.g. JER)

NOTE: for the one-sided systematics, only the "name" is required

getSystematics(name="JER",oneSided=True)
'''

##____________________________________________________________
## Defining the samples
def getSystematics(name="",nameUp="",nameDown="",oneSided=False,type="O"):
    Syst =  {
            'name':name,
            'nameUp':nameUp,
            'nameDown':nameDown,
            'oneSided':oneSided,
            'type':type
        }
    return Syst

##____________________________________________________________
## Defining the samples
def getSampleUncertainties(type="",name="",objSyst=[]#,weightSyst=[]
                           ,isSignal=False):
    Sample = {
                'sampleType':type,
                'name':name,
                'objSyst':objSyst,
                #'weightSyst':weightSyst,
                'isSignal':isSignal
            }
    return Sample
