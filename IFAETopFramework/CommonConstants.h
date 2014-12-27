#ifndef COMMONCONSTANTS_H
#define COMMONCONSTANTS_H

namespace Debug{ enum debug{SILENT=0,STANDARD,VERBOSE,DEBUG}; };
namespace AnaType{ enum ana{ HSG8=1, SUSY, VLQ }; };
namespace InputType{ enum input { IND3PD=0, INMINI }; }; //will be moved to ttH
namespace SampleName{
    enum sample{
        DATA=1,
        TTBAR,
        TTBARBB,
        TTBARCC,
        TTBARLIGHT,
        DIBOSONS,
        SINGLETOP,
        WJETS,
        ZJETS,
        TTH,
        TTZ,
        TTW,
        TTV,
        VLQ,
        GLUINO
    };
};
namespace SysName{ enum sys{ NOMINAL=0, JES, JER, ELEID, MUID }; };

namespace WeightSys{
    enum wgtSys{
        /*Btag breakdown*/
        BtagBreak1Up=0,
        BtagBreak1Dw,
        BtagBreak2Up,
        BtagBreak2Dw,
        BtagBreak3Up,
        BtagBreak3Dw,
        BtagBreak4Up,
        BtagBreak4Dw,
        BtagBreak5Up,
        BtagBreak5Dw,
        BtagBreak6Up,
        BtagBreak6Dw,
        
        /*Btag C breakdown*/
        CtagBreak1Up,
        CtagBreak1Dw,
        CtagBreak2Up,
        CtagBreak2Dw,
        CtagBreak3Up,
        CtagBreak3Dw,
        CtagBreak4Up,
        CtagBreak4Dw,
        CtagBreak5Up,
        CtagBreak5Dw,
        CtagBreak6Up,
        CtagBreak6Dw,
        
        /*Btag Light breakdown*/
        LtagBreak1Up,
        LtagBreak1Dw,
        LtagBreak2Up,
        LtagBreak2Dw,
        LtagBreak3Up,
        LtagBreak3Dw,
        LtagBreak4Up,
        LtagBreak4Dw,
        LtagBreak5Up,
        LtagBreak5Dw,
        LtagBreak6Up,
        LtagBreak6Dw,
        LtagBreak7Up,
        LtagBreak7Dw,
        LtagBreak8Up,
        LtagBreak8Dw,
        LtagBreak9Up,
        LtagBreak9Dw,
        LtagBreak10Up,
        LtagBreak10Dw,
        LtagBreak11Up,
        LtagBreak11Dw,
        LtagBreak12Up,
        LtagBreak12Dw,
        
        /*Btag extrapolation*/
        BtagExtrapUp,
        BtagExtrapDw,
        
        /*Muon SF*/
        MuonTrigSFUp,
        MuonTrigSFDw,
        MuonRecoSFUp,
        MuonRecoSFDw,
        MuonIdSFUp,
        MuonIdSFDw,
        
        /*Electron SF*/
        ElectronTrigSFUp,
        ElectronTrigSFDw,
        ElectronRecoSFUp,
        ElectronRecoSFDw,
        ElectronIdSFUp,
        ElectronIdSFDw,
        
        /*ttbar specific uncertainties*/
        ttbarTopPtDataWeight_ISRUp,
        ttbarTopPtDataWeight_ISRDw,
        ttbarTopPtDataWeight_FragmentationUp,
        ttbarTopPtDataWeight_FragmentationDw,
        ttbarTopPtDataWeight_MCgenUp,
        ttbarTopPtDataWeight_MCgenDw,
        ttbarTopPtDataWeight_JERUp,
        ttbarTopPtDataWeight_JERDw,
        ttbarTopPtDataWeight_bJESUp,
        ttbarTopPtDataWeight_bJESDw,
        ttbarTopPtDataWeight_CloseByJESUp,
        ttbarTopPtDataWeight_CloseByJESDw,
        ttbarTopPtDataWeight_EtaCalibJESUp,
        ttbarTopPtDataWeight_EtaCalibJESDw,
        ttbarTopPtDataWeight_effdetset1JESUp,
        ttbarTopPtDataWeight_effdetset1JESDw,
        ttbarTopPtDataWeight_BtagEffUp,
        ttbarTopPtDataWeight_BtagEffDw,
        
        ttbar_MadgraphMatchUp,
        ttbar_MadgraphMatchDw,
        ttbar_MadgraphMbUp,
        ttbar_MadgraphMbDw,
        ttbar_MadgraphMcUp,
        ttbar_MadgraphMcDw,
        ttbar_MadgraphQ2Up,
        ttbar_MadgraphQ2Dw,
        ttbar_MadgraphPPUp,
        ttbar_MadgraphPPDw,
        
        ttbb_defaultX2_rw,
        ttbb_defaultX05_rw,
        ttbb_CSS_KIN_rw,
        ttbb_Q_CMMPS_rw,
        ttbb_NNPDF_rw,
        ttbb_MSTW_rw,
        ttbb_R_Mbb_rw,
        
        MPI_up,
        FSR_up,
        
        /*V+jets*/
        noVpTWeight,
        noVpTOnly,
        
        /*ttH*/
        ttH_scaleUp,
        ttH_scaleDw,
        
        /*ttV*/
        ttV_scaleUp,
        ttV_scaleDw
    };
}

#endif
