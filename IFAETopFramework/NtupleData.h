#ifndef NTUPLEDATA_H
#define NTUPLEDATA_H

#include "IFAETopFramework/CommonConstants.h"
#include <string>
#include <vector>
#include <map>
#include <TROOT.h>

using namespace std;
class NtupleData {
    
public:
    NtupleData();
    virtual ~NtupleData(){};
    void ClearNtupleData();
    void EmptyNtupleData();
    
public:
    
    ///////////////////////////////////////////
    //
    // EVENT CARACTERISTIC VARIABLES
    //
    ///////////////////////////////////////////
    
    Float_t         mu;
    Int_t           vxp_n;
    
    Int_t           runNumber;
    Int_t           eventNumber;
    Int_t           HFtype;
    
    Int_t           finalEvent_VLQType;
    
    Int_t	    tree_mc_pdf_id1;
    Int_t	    tree_mc_pdf_id2;
    Float_t	    tree_mc_pdf_x1;
    Float_t	    tree_mc_pdf_x2;
    Float_t	    tree_mc_pdf_scale;
    Float_t     tree_truth_ttbar_pT;

    
    ///////////////////////////////////////////
    //
    // JET VARIABLES
    //
    ///////////////////////////////////////////
    
    Int_t                  jet_n;
    vector<float>*         jet_pt;
    vector<float>*         jet_eta;
    vector<float>*         jet_phi;
    vector<float>*         jet_E;
    vector<float>*         jet_m;
    vector<float>*         jet_jvf;
    vector<float>*         jet_btag_weight;
    vector<int>*           jet_trueflav;
    vector<bool>*          jet_tagged;
    vector<float>*         jet_MV1;
    vector<float>*         jet_nTrk_pv0_1GeV;
    vector<float>*         jet_trackWIDTH_pv0_1GeV;
    vector<int>*           jet_constit_n;
    vector<vector<int> >*  jet_constit_index;
    
    
    
    ///////////////////////////////////////////
    //
    // BJET VARIABLES
    //
    ///////////////////////////////////////////
    
    Int_t           bjet_n;
    
    ///////////////////////////////////////////
    //
    // LEPTON VARIABLES
    //
    ///////////////////////////////////////////
    
    Int_t           lep_n;
    Int_t           lep_ind;
    Int_t           lep_flav;
    vector<float>*  lep_E;
    vector<float>*  lep_pt;
    vector<float>*  lep_eta;
    vector<float>*  lep_phi;
    vector<float>*  lep_charge;
    vector<float>*  lep_d0;
    vector<float>*  lep_z0;
    vector<float>*  lep_d0sig;
    vector<float>*  lep_z0sig;
    Int_t n_lep_vetoed;
    
    
    ///////////////////////////////////////////
    //
    // ELECTRON VARIABLES
    //
    ///////////////////////////////////////////
    
    Int_t           el_n;
    vector<float>   *el_E;
    vector<float>   *el_Et;
    vector<float>   *el_pt;
    vector<float>   *el_eta;
    vector<float>   *el_phi;
    vector<float>   *el_charge;
    vector<int>     *el_author;
    vector<int>     *el_tightPP;
    vector<float>   *el_trackz0pvunbiased;
    vector<float>   *el_tracketa;
    vector<float>   *el_trackphi;
    vector<float>   *el_cl_E;
    vector<float>   *el_cl_pt;
    vector<float>   *el_cl_eta;
    vector<float>   *el_cl_phi;
    vector<unsigned int> *el_OQ;
    vector<float>   *el_etap;
    vector<float>   *el_etas2;
    vector<float>   *el_MI10_max40_ptsum;
    
    
    ///////////////////////////////////////////
    //
    // MUON VARIABLES
    //
    ///////////////////////////////////////////
    
    Int_t           mu_n;
    vector<float>   *mu_E;
    vector<float>   *mu_pt;
    vector<float>   *mu_eta;
    vector<float>   *mu_phi;
    vector<float>   *mu_charge;
    vector<int>     *mu_author;
    vector<int>     *mu_tight;
    
    ///////////////////////////////////////////
    //
    // MET VARIABLES
    //
    ///////////////////////////////////////////
    
    Float_t MET_et;
    Float_t MET_phi;
    
    
    ///////////////////////////////////////////
    //
    // WEIGHT VARIABLES
    //
    ///////////////////////////////////////////
    
    /*
     Two kinds of variables for the weights:
     * Variables extracted from the tree
     - Their name starts with tree_*
     * The computed variables (used in the common codes such as the systematics)
     - Their name starts with finalEvent_*
     The finalEvent_* variables are based on the assumption that they represent
     the *TOTAL* weight of a given event:
     --> Nominal case: product of all weights (leptons SF, btag SF, ...)
     --> Systematic case: product of all nominal weights but the systematic one
     !!!! Please stick to this convention !!!!
     */
    
    //Nominal weights
    Float_t tree_Weight, tree_BTagWeight,tree_LeptonSF;
    std::vector < double > *tree_TRFExBTagWeight, *tree_TRFInBTagWeight;
    Float_t tree_ToptTpTDataweight,tree_qcdWeight,tree_crossSection,tree_ttcc_rw,tree_ttbb_rw,tree_VpT_rw,tree_VpTHF_rw;
    
    
    std::vector< std::vector< float > > *tree_TRFDiscreteBTagWeight_ex, *tree_TRFDiscreteBTagWeight_in;
    std::vector< std::vector< bool > > *tree_TRFChosenTag_ex, *tree_TRFChosenTag_in;
    
    //Systematics for direct tagging
    std::vector < double > *tree_BTag_breakdown_up, *tree_BTag_breakdown_down;//1 component: 1 breakdown
    std::vector < double > *tree_CTag_breakdown_up, *tree_CTag_breakdown_down;
    std::vector < double > *tree_LTag_breakdown_up, *tree_LTag_breakdown_down;
    
    //Systematics for TRF "tagging" (exclusive selection)
    std::vector < std::vector < double > > *tree_TRFBTag_ex_breakdown_up, *tree_TRFBTag_ex_breakdown_down;//1 component: 1 tag mult., 1 breakdown
    std::vector < std::vector < double > > *tree_TRFCTag_ex_breakdown_up, *tree_TRFCTag_ex_breakdown_down;
    std::vector < std::vector < double > > *tree_TRFLTag_ex_breakdown_up, *tree_TRFLTag_ex_breakdown_down;
    
    //Systematics for TRF "tagging" (inclusive selection)
    std::vector < std::vector < double > > *tree_TRFBTag_in_breakdown_up, *tree_TRFBTag_in_breakdown_down;//1 component: 1 tag mult., 1 breakdown
    std::vector < std::vector < double > > *tree_TRFCTag_in_breakdown_up, *tree_TRFCTag_in_breakdown_down;
    std::vector < std::vector < double > > *tree_TRFLTag_in_breakdown_up, *tree_TRFLTag_in_breakdown_down;
    
    //Systematics for lepton scale factors
    Float_t tree_LeptonSF_TrigSF_up,tree_LeptonSF_TrigSF_down,tree_LeptonSF_IDSF_up,tree_LeptonSF_IDSF_down,tree_LeptonSF_RecoSF_up,tree_LeptonSF_RecoSF_down;
    Float_t tree_EleSF_TrigSF_up,tree_EleSF_TrigSF_down,tree_EleSF_IDSF_up,tree_EleSF_IDSF_down,tree_EleSF_RecoSF_up,tree_EleSF_RecoSF_down;
    Float_t tree_MuonSF_TrigSF_up,tree_MuonSF_TrigSF_down,tree_MuonSF_IDSF_up,tree_MuonSF_IDSF_down,tree_MuonSF_RecoSF_up,tree_MuonSF_RecoSF_down;
    
    //ttH weight systematic uncertainties
    Float_t tree_ttH_scale_static, tree_ttH_scale_up, tree_ttH_scale_down;
    
    //ttbar weight systematics
    Float_t tree_ttbartop_pt_rw_IFSR_up,tree_ttbartop_pt_rw_IFSR_down,tree_ttbartop_pt_rw_Fragmentation_up,tree_ttbartop_pt_rw_Fragmentation_down;
    Float_t tree_ttbartop_pt_rw_MCgen_up,tree_ttbartop_pt_rw_MCgen_down,tree_ttbartop_pt_rw_JER_up,tree_ttbartop_pt_rw_JER_down;
    Float_t tree_ttbartop_pt_rw_bJES_up,tree_ttbartop_pt_rw_bJES_down,tree_ttbartop_pt_rw_closebyJES_up,tree_ttbartop_pt_rw_closebyJES_down;
    Float_t tree_ttbartop_pt_rw_etacalibJES_up,tree_ttbartop_pt_rw_etacalibJES_down,tree_ttbartop_pt_rw_effdetset1JES_up,tree_ttbartop_pt_rw_effdetset1JES_down;
    Float_t tree_ttbartop_pt_rw_btageff_up,tree_ttbartop_pt_rw_btageff_down;
    Float_t tree_MadgraphMC_up,tree_MadgraphMC_down,tree_MadgraphMB_up,tree_MadgraphMB_down,tree_MadgraphQ2_up,tree_MadgraphQ2_down,tree_MadgraphMatch_up,tree_MadgraphPP;
    Float_t tree_toppt_only,tree_ttbarpt_only;
    Float_t tree_ttbarbb_CSS_kin,tree_ttbarbb_R_Mbb,tree_ttbarbb_NNPDF,tree_ttbarbb_MSTW,tree_ttbarbb_QCMMPS,tree_ttbarbb_scale05,tree_ttbarbb_scale2;
    
    //final systematic event weights
    Float_t     finalEvent_weightNom;
    vector<double> *finalEvent_BTag_breakdown_up;
    vector<double> *finalEvent_BTag_breakdown_down;
    vector<double> *finalEvent_CTag_breakdown_up;
    vector<double> *finalEvent_CTag_breakdown_down;
    vector<double> *finalEvent_LTag_breakdown_up;
    vector<double> *finalEvent_LTag_breakdown_down;
    double finalEvent_BTagExtrap_up,finalEvent_BTagExtrap_down;
    double finalEvent_EleID_up, finalEvent_EleID_down,finalEvent_EleReco_up, finalEvent_EleReco_down,finalEvent_EleTrig_up, finalEvent_EleTrig_down;
    double finalEvent_MuonID_up, finalEvent_MuonID_down,finalEvent_MuonReco_up, finalEvent_MuonReco_down,finalEvent_MuonTrig_up, finalEvent_MuonTrig_down;
    double finalEvent_ttH_scale_static, finalEvent_ttH_scale_up, finalEvent_ttH_scale_down;
    
    double finalEvent_ttbartop_pt_rw_IFSR_up,finalEvent_ttbartop_pt_rw_IFSR_down,finalEvent_ttbartop_pt_rw_Fragmentation_up,finalEvent_ttbartop_pt_rw_Fragmentation_down;
    double finalEvent_ttbartop_pt_rw_MCgen_up,finalEvent_ttbartop_pt_rw_MCgen_down,finalEvent_ttbartop_pt_rw_JER_up,finalEvent_ttbartop_pt_rw_JER_down;
    double finalEvent_ttbartop_pt_rw_bJES_up,finalEvent_ttbartop_pt_rw_bJES_down,finalEvent_ttbartop_pt_rw_closebyJES_up,finalEvent_ttbartop_pt_rw_closebyJES_down;
    double finalEvent_ttbartop_pt_rw_etacalibJES_up,finalEvent_ttbartop_pt_rw_etacalibJES_down,finalEvent_ttbartop_pt_rw_effdetset1JES_up,finalEvent_ttbartop_pt_rw_effdetset1JES_down;
    double finalEvent_ttbartop_pt_rw_btageff_up,finalEvent_ttbartop_pt_rw_btageff_down;
    double finalEvent_MadgraphMC_up,finalEvent_MadgraphMC_down,finalEvent_MadgraphMB_up,finalEvent_MadgraphMB_down;
    double finalEvent_MadgraphQ2_up,finalEvent_MadgraphQ2_down,finalEvent_MadgraphMatch_up,finalEvent_MadgraphPP;
    double finalEvent_toppt_only,finalEvent_ttbarpt_only;
    double finalEvent_ttbarbb_MPI,finalEvent_ttbarbb_FSR;
    double finalEvent_ttbarbb_CSS_kin,finalEvent_ttbarbb_R_Mbb,finalEvent_ttbarbb_NNPDF,finalEvent_ttbarbb_MSTW,finalEvent_ttbarbb_QCMMPS,finalEvent_ttbarbb_scale05,finalEvent_ttbarbb_scale2;
    double finalEvent_noVpt_rw;
    
    ///////////////////////////////////////////
    //
    // KINEMATIC VARIABLES
    //
    ///////////////////////////////////////////
    
    double HTalljets;//scalar sum of pt of all jets
    double LT;//scalar sum of pt of all leptons
    double HTleptonsJets;//scalar sum of pt of all leptons and jets
    double meff;//effective mass = HTleptonsJets + Met
    double mT;//transverse mass of the leptonic W boson
    
    
    UInt_t          larError;
    UInt_t          tileError;
    UInt_t          coreFlags;
    
    Int_t           channelNumber;
    
    Bool_t          EF_mu24i_tight;
    Bool_t          EF_mu36_tight;
    Bool_t          EF_e24vhi_medium1;
    Bool_t          EF_e60_medium1;
    
    Int_t           OrigNumOfEventsInFile;
    Float_t         PUweight;
    Float_t         ZVxpWeight;
    Float_t         XSection;
    Float_t         KFactor;
    Float_t         JVFSFweight;
    Float_t         BTagSFweight;
    Float_t         totalScale;
    Float_t         elecScale;
    Float_t         muonScale;
    Float_t         trigScale;
    Float_t         WJetsShapeScale;
    Float_t         WJetsScale;
    Int_t           bTagCat;
    Int_t           BoostedEvent;
    
    
    
    //Topoclusters
    Int_t           cl_n;
    vector<float>   *cl_pt;
    vector<float>   *cl_eta;
    vector<float>   *cl_phi;
    vector<float>   *cl_centermag;
    
    //Tracks
    Int_t           trk_n;
    vector<float>   *trk_pt;
    vector<float>   *trk_eta;
    vector<float>   *trk_d0_wrtPV;
    vector<float>   *trk_z0_wrtPV;
    vector<float>   *trk_phi_wrtPV;
    vector<float>   *trk_theta_wrtPV;
    vector<float>   *trk_qoverp_wrtPV;
    vector<float>   *trk_chi2;
    vector<int>     *trk_ndof;
    vector<int>     *trk_nPixHits;
    vector<int>     *trk_nSCTHits;
    
    
    
    //MC particles
    vector<vector<double> >*         mcevt_weight;
    vector<double>*        mcevt_pdf1;
    vector<double>*        mcevt_pdf2;
    vector<int>*           mcevt_pdf_id1;
    vector<int>*           mcevt_pdf_id2;
    vector<double>*        mcevt_pdf_scale;
    vector<double>*        mcevt_pdf_x1;
    vector<double>*        mcevt_pdf_x2;
    
    
    float v_mcevt_weight;
    double v_mcevt_pdf1;
    double v_mcevt_pdf2;
    int v_mcevt_pdf_id1;
    int v_mcevt_pdf_id2;
    double v_mcevt_pdf_scale;
    double v_mcevt_pdf_x1;
    double v_mcevt_pdf_x2;
    
    Int_t           mc_n;
    vector<float>   *mc_E;
    vector<float>   *mc_pt;
    vector<float>   *mc_eta;
    vector<float>   *mc_phi;
    vector<float>   *mc_m;
    vector<int>     *mc_status;
    vector<int>     *mc_barcode;
    vector<int>     *mc_pdgId;
    vector<float>   *mc_charge;
    vector<vector<int> > *mc_parent_index;
    vector<vector<int> > *mc_child_index;
    
    
    Int_t           trig_EF_el_n;
    vector<float>   *trig_EF_el_eta;
    vector<float>   *trig_EF_el_phi;
    vector<int>     *trig_EF_el_EF_e24vhi_medium1;
    vector<int>     *trig_EF_el_EF_e60_medium1;
    vector<int>     *trig_EF_trigmuonef_EF_mu24i_tight;
    vector<int>     *trig_EF_trigmuonef_EF_mu36_tight;
    
    Int_t           trig_EF_trigmuonef_n;
    vector<int>     *trig_EF_trigmuonef_track_n;
    vector<vector<float> > *trig_EF_trigmuonef_track_CB_pt;
    vector<vector<float> > *trig_EF_trigmuonef_track_CB_eta;
    vector<vector<float> > *trig_EF_trigmuonef_track_CB_phi;
    
    vector<int> m_fjcol_list;
    map<int, int> fatjet_n;
    map<int, vector<float>* > fatjet_pt;
    map<int, vector<float>* > fatjet_eta;
    map<int, vector<float>* > fatjet_phi;
    map<int, vector<float>* > fatjet_m;
    map<int, vector<float>* > fatjet_constscale_pt;
    map<int, vector<float>* > fatjet_constscale_eta;
    map<int, vector<float>* > fatjet_constscale_phi;
    map<int, vector<float>* > fatjet_constscale_m;
    map<int, vector<float>* > fatjet_ActiveArea;
    map<int, vector<float>* > fatjet_ActiveArea_px;
    map<int, vector<float>* > fatjet_ActiveArea_py;
    map<int, vector<float>* > fatjet_ActiveArea_pz;
    map<int, vector<float>* > fatjet_ActiveArea_E;
    
    map<int, vector<float>* > fatjet_SPLIT12;
    map<int, vector<float>* > fatjet_SPLIT23;
    map<int, vector<float>* > fatjet_ZCUT12;
    map<int, vector<float>* > fatjet_ZCUT23;
    map<int, vector<float>* > fatjet_WIDTH;
    map<int, vector<float>* > fatjet_PlanarFlow;
    map<int, vector<float>* > fatjet_Angularity;
    map<int, vector<float>* > fatjet_PullMag;
    map<int, vector<float>* > fatjet_PullPhi;
    map<int, vector<float>* > fatjet_Tau1;
    map<int, vector<float>* > fatjet_Tau2;
    map<int, vector<float>* > fatjet_Tau3;
    map<int, vector<int>* > fatjet_nbtag;
    map<int, vector<int>* > fatjet_nbtag_geom;
    map<int, vector<int>* > fatjet_MCclass;
    map<int, vector<int>* > fatjet_orig_ind;
    map<int, vector<int>* > fatjet_assoc_ind;
    
    map<int, vector<int>* > fatjet_constit_n;
    map<int, vector<vector<int> >* > fatjet_constit_index;
    
    
    
    
};

#endif
