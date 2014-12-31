#!/usr/bin/env python
from BatchTools import *

##____________________________________________________________
##
## OBJECT SYSTEMATICS
##
ObjectSystematics =  []
ObjectSystematics += ["nominal"] # the nominal is considered as a systematic variation
ObjectSystematics += ["BJesUnc_down"]
ObjectSystematics += ["BJesUnc_up"]
ObjectSystematics += ["eer_down"]
ObjectSystematics += ["eer_up"]
ObjectSystematics += ["ees_down"]
ObjectSystematics += ["ees_up"]
ObjectSystematics += ["muid_up"]
ObjectSystematics += ["muid_down"]
ObjectSystematics += ["mums_up"]
ObjectSystematics += ["mums_down"]
ObjectSystematics += ["musc_up"]
ObjectSystematics += ["musc_down"]
ObjectSystematics += ["EtaIntercalibrationModel_down"]
ObjectSystematics += ["EtaIntercalibrationModel_up"]
ObjectSystematics += ["EtaIntercalibrationStatMethod_down"]
ObjectSystematics += ["EtaIntercalibrationStatMethod_up"]
ObjectSystematics += ["flavor_comp_down"]
ObjectSystematics += ["flavor_comp_up"]
ObjectSystematics += ["flavor_response_down"]
ObjectSystematics += ["flavor_response_up"]
ObjectSystematics += ["jeff"]
ObjectSystematics += ["jer"]
ObjectSystematics += ["JesEffectiveDet1_down"]
ObjectSystematics += ["JesEffectiveDet1_up"]
ObjectSystematics += ["JesEffectiveDet2_down"]
ObjectSystematics += ["JesEffectiveDet2_up"]
ObjectSystematics += ["JesEffectiveDet3_down"]
ObjectSystematics += ["JesEffectiveDet3_up"]
ObjectSystematics += ["JesEffectiveMix1_down"]
ObjectSystematics += ["JesEffectiveMix1_up"]
ObjectSystematics += ["JesEffectiveMix2_down"]
ObjectSystematics += ["JesEffectiveMix2_up"]
ObjectSystematics += ["JesEffectiveModel1_down"]
ObjectSystematics += ["JesEffectiveModel1_up"]
ObjectSystematics += ["JesEffectiveModel2_down"]
ObjectSystematics += ["JesEffectiveModel2_up"]
ObjectSystematics += ["JesEffectiveModel3_down"]
ObjectSystematics += ["JesEffectiveModel3_up"]
ObjectSystematics += ["JesEffectiveModel4_down"]
ObjectSystematics += ["JesEffectiveModel4_up"]
ObjectSystematics += ["JesEffectiveStat1_down"]
ObjectSystematics += ["JesEffectiveStat1_up"]
ObjectSystematics += ["JesEffectiveStat2_down"]
ObjectSystematics += ["JesEffectiveStat2_up"]
ObjectSystematics += ["JesEffectiveStat3_down"]
ObjectSystematics += ["JesEffectiveStat3_up"]
ObjectSystematics += ["jvf_down"]
ObjectSystematics += ["jvf_up"]
ObjectSystematics += ["Pileup_OffsetMu_down"]
ObjectSystematics += ["Pileup_OffsetMu_up"]
ObjectSystematics += ["Pileup_OffsetNPV_down"]
ObjectSystematics += ["Pileup_OffsetNPV_up"]
ObjectSystematics += ["Pileup_Pt_down"]
ObjectSystematics += ["Pileup_Pt_up"]
ObjectSystematics += ["Pileup_Rho_down"]
ObjectSystematics += ["Pileup_Rho_up"]
ObjectSystematics += ["res_soft_down"]
ObjectSystematics += ["res_soft_up"]
ObjectSystematics += ["sc_soft_down"]
ObjectSystematics += ["sc_soft_up"]
ObjectSystematics += ["SinglePart_down"]
ObjectSystematics += ["SinglePart_up"]

##____________________________________________________________
##
## WEIGHT SYSTEMATICS
##
CommonWeightSystematics =  []

CommonWeightSystematics += ["BTAGBREAK1UP"]
CommonWeightSystematics += ["BTAGBREAK2UP"]
CommonWeightSystematics += ["BTAGBREAK3UP"]
CommonWeightSystematics += ["BTAGBREAK4UP"]
CommonWeightSystematics += ["BTAGBREAK5UP"]
CommonWeightSystematics += ["BTAGBREAK6UP"]
CommonWeightSystematics += ["BTAGBREAK1DW"]
CommonWeightSystematics += ["BTAGBREAK2DW"]
CommonWeightSystematics += ["BTAGBREAK3DW"]
CommonWeightSystematics += ["BTAGBREAK4DW"]
CommonWeightSystematics += ["BTAGBREAK5DW"]
CommonWeightSystematics += ["BTAGBREAK6DW"]

CommonWeightSystematics += ["CTAGBREAK1UP"]
CommonWeightSystematics += ["CTAGBREAK2UP"]
CommonWeightSystematics += ["CTAGBREAK3UP"]
CommonWeightSystematics += ["CTAGBREAK4UP"]
CommonWeightSystematics += ["CTAGBREAK5UP"]
CommonWeightSystematics += ["CTAGBREAK6UP"]
CommonWeightSystematics += ["CTAGBREAK1DW"]
CommonWeightSystematics += ["CTAGBREAK2DW"]
CommonWeightSystematics += ["CTAGBREAK3DW"]
CommonWeightSystematics += ["CTAGBREAK4DW"]
CommonWeightSystematics += ["CTAGBREAK5DW"]
CommonWeightSystematics += ["CTAGBREAK6DW"]

CommonWeightSystematics += ["LTAGBREAK1UP"]
CommonWeightSystematics += ["LTAGBREAK2UP"]
CommonWeightSystematics += ["LTAGBREAK3UP"]
CommonWeightSystematics += ["LTAGBREAK4UP"]
CommonWeightSystematics += ["LTAGBREAK5UP"]
CommonWeightSystematics += ["LTAGBREAK6UP"]
CommonWeightSystematics += ["LTAGBREAK7UP"]
CommonWeightSystematics += ["LTAGBREAK8UP"]
CommonWeightSystematics += ["LTAGBREAK9UP"]
CommonWeightSystematics += ["LTAGBREAK10UP"]
CommonWeightSystematics += ["LTAGBREAK11UP"]
CommonWeightSystematics += ["LTAGBREAK12UP"]
CommonWeightSystematics += ["LTAGBREAK1DW"]
CommonWeightSystematics += ["LTAGBREAK2DW"]
CommonWeightSystematics += ["LTAGBREAK3DW"]
CommonWeightSystematics += ["LTAGBREAK4DW"]
CommonWeightSystematics += ["LTAGBREAK5DW"]
CommonWeightSystematics += ["LTAGBREAK6DW"]
CommonWeightSystematics += ["LTAGBREAK7DW"]
CommonWeightSystematics += ["LTAGBREAK8DW"]
CommonWeightSystematics += ["LTAGBREAK9DW"]
CommonWeightSystematics += ["LTAGBREAK10DW"]
CommonWeightSystematics += ["LTAGBREAK11DW"]
CommonWeightSystematics += ["LTAGBREAK12DW"]

##____________________________________________________________
## Defining the samples
def getSampleUncertainties(type="",name="",objSyst=[],weightSyst=[]):
    Sample = {
                'sampleType':type,
                'name':name,
                'objSyst':objSyst,
                'weightSyst':weightSyst
            }
    return Sample