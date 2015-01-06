#!/usr/bin/env python
from BatchTools import *

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
##
## OBJECT SYSTEMATICS
##
ObjectSystematics =  []
ObjectSystematics += [getSystematics(name="nominal",oneSided=True)] # the nominal is considered as a systematic variation
ObjectSystematics += [getSystematics(name="BJesUnc",nameUp="BJesUnc_up",nameDown="BJesUnc_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="eer",nameUp="eer_up",nameDown="eer_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="ees",nameUp="ees_up",nameDown="ees_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="muid",nameUp="muid_up",nameDown="muid_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="mums",nameUp="mums_up",nameDown="mums_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="musc",nameUp="musc_up",nameDown="musc_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="EtaIntercalibrationModel",nameUp="EtaIntercalibrationModel_up",nameDown="EtaIntercalibrationModel_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="EtaIntercalibrationStatMethod",nameUp="EtaIntercalibrationStatMethod_up",nameDown="EtaIntercalibrationStatMethod_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="flavor_comp",nameUp="flavor_comp_up",nameDown="flavor_comp_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="flavor_response",nameUp="flavor_response_up",nameDown="flavor_response_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="jeff",nameUp="jeff",nameDown="jeff",oneSided=True)]
ObjectSystematics += [getSystematics(name="jer",nameUp="jer",nameDown="jer",oneSided=True)]
ObjectSystematics += [getSystematics(name="JesEffectiveDet1",nameUp="JesEffectiveDet1_up",nameDown="JesEffectiveDet1_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="JesEffectiveDet2",nameUp="JesEffectiveDet2_up",nameDown="JesEffectiveDet2_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="JesEffectiveDet3",nameUp="JesEffectiveDet3_up",nameDown="JesEffectiveDet3_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="JesEffectiveMix1",nameUp="JesEffectiveMix1_up",nameDown="JesEffectiveMix1_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="JesEffectiveMix2",nameUp="JesEffectiveMix2_up",nameDown="JesEffectiveMix2_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="JesEffectiveModel1",nameUp="JesEffectiveModel1_up",nameDown="JesEffectiveModel1_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="JesEffectiveModel2",nameUp="JesEffectiveModel2_up",nameDown="JesEffectiveModel2_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="JesEffectiveModel3",nameUp="JesEffectiveModel3_up",nameDown="JesEffectiveModel3_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="JesEffectiveModel4",nameUp="JesEffectiveModel4_up",nameDown="JesEffectiveModel4_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="JesEffectiveStat1",nameUp="JesEffectiveStat1_up",nameDown="JesEffectiveStat1_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="JesEffectiveStat2",nameUp="JesEffectiveStat2_up",nameDown="JesEffectiveStat2_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="JesEffectiveStat3",nameUp="JesEffectiveStat3_up",nameDown="JesEffectiveStat3_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="jvf",nameUp="jvf_up",nameDown="jvf_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="Pileup_OffsetMu",nameUp="Pileup_OffsetMu_up",nameDown="Pileup_OffsetMu_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="Pileup_OffsetNPV",nameUp="Pileup_OffsetNPV_up",nameDown="Pileup_OffsetNPV_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="Pileup_Pt",nameUp="Pileup_Pt_up",nameDown="Pileup_Pt_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="Pileup_Rho",nameUp="Pileup_Rho_up",nameDown="Pileup_Rho_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="res_soft",nameUp="res_soft_up",nameDown="res_soft_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="sc_soft",nameUp="sc_soft_up",nameDown="sc_soft_down",oneSided=False)]
ObjectSystematics += [getSystematics(name="SinglePart",nameUp="SinglePart_up",nameDown="SinglePart_down",oneSided=False)]

##____________________________________________________________
##
## WEIGHT SYSTEMATICS
##
CommonWeightSystematics =  []
CommonWeightSystematics += [getSystematics(name="BTAGBREAK1",nameUp="BTAGBREAK1UP",nameDown="BTAGBREAK1DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="BTAGBREAK2",nameUp="BTAGBREAK2UP",nameDown="BTAGBREAK2DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="BTAGBREAK3",nameUp="BTAGBREAK3UP",nameDown="BTAGBREAK3DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="BTAGBREAK4",nameUp="BTAGBREAK4UP",nameDown="BTAGBREAK4DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="BTAGBREAK5",nameUp="BTAGBREAK5UP",nameDown="BTAGBREAK5DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="BTAGBREAK6",nameUp="BTAGBREAK6UP",nameDown="BTAGBREAK6DW",oneSided=False,type="W")]

CommonWeightSystematics += [getSystematics(name="CTAGBREAK1",nameUp="CTAGBREAK1UP",nameDown="CTAGBREAK1DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="CTAGBREAK2",nameUp="CTAGBREAK2UP",nameDown="CTAGBREAK2DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="CTAGBREAK3",nameUp="CTAGBREAK3UP",nameDown="CTAGBREAK3DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="CTAGBREAK4",nameUp="CTAGBREAK4UP",nameDown="CTAGBREAK4DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="CTAGBREAK5",nameUp="CTAGBREAK5UP",nameDown="CTAGBREAK5DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="CTAGBREAK6",nameUp="CTAGBREAK6UP",nameDown="CTAGBREAK6DW",oneSided=False,type="W")]

CommonWeightSystematics += [getSystematics(name="LTAGBREAK1",nameUp="LTAGBREAK1UP",nameDown="LTAGBREAK1DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="LTAGBREAK2",nameUp="LTAGBREAK2UP",nameDown="LTAGBREAK2DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="LTAGBREAK3",nameUp="LTAGBREAK3UP",nameDown="LTAGBREAK3DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="LTAGBREAK4",nameUp="LTAGBREAK4UP",nameDown="LTAGBREAK4DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="LTAGBREAK5",nameUp="LTAGBREAK5UP",nameDown="LTAGBREAK5DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="LTAGBREAK6",nameUp="LTAGBREAK6UP",nameDown="LTAGBREAK6DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="LTAGBREAK7",nameUp="LTAGBREAK7UP",nameDown="LTAGBREAK7DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="LTAGBREAK8",nameUp="LTAGBREAK8UP",nameDown="LTAGBREAK8DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="LTAGBREAK9",nameUp="LTAGBREAK9UP",nameDown="LTAGBREAK9DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="LTAGBREAK10",nameUp="LTAGBREAK10UP",nameDown="LTAGBREAK10DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="LTAGBREAK11",nameUp="LTAGBREAK11UP",nameDown="LTAGBREAK11DW",oneSided=False,type="W")]
CommonWeightSystematics += [getSystematics(name="LTAGBREAK12",nameUp="LTAGBREAK12UP",nameDown="LTAGBREAK12DW",oneSided=False,type="W")]

##____________________________________________________________
## Defining the samples
def getSampleUncertainties(type="",name="",objSyst=[],weightSyst=[],isSignal=False):
    Sample = {
                'sampleType':type,
                'name':name,
                'objSyst':objSyst,
                'weightSyst':weightSyst,
                'isSignal':isSignal
            }
    return Sample
