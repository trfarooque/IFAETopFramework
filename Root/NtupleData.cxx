#include <iostream>
#include "IFAETopFramework/NtupleData.h"

using namespace std;
NtupleData::NtupleData():
//Event variables
mu(0),vxp_n(0),runNumber(0),eventNumber(0),HFtype(0),
finalEvent_VLQType(0),
tree_mc_pdf_id1(0),tree_mc_pdf_id2(0),
tree_mc_pdf_x1(0),tree_mc_pdf_x2(0),
tree_mc_pdf_scale(0),
tree_truth_ttbar_pT(0),

//Jet variables
jet_n(0),
jet_pt(0),jet_eta(0),jet_phi(0),jet_E(0),jet_m(0),jet_jvf(0),
jet_btag_weight(0),jet_trueflav(0),jet_tagged(0),jet_MV1(0),
jet_nTrk_pv0_1GeV(0),jet_trackWIDTH_pv0_1GeV(0),
jet_constit_n(0),jet_constit_index(0),

//Bjets
bjet_n(0),

//Lepton variables
lep_n(0),lep_ind(0),lep_flav(0),
lep_E(0),lep_pt(0),lep_eta(0),lep_phi(0),lep_charge(0),
lep_d0(0),lep_z0(0),lep_d0sig(0),lep_z0sig(0),
n_lep_vetoed(0),

//Electron variables
el_n(0),
el_E(0),el_Et(0),el_pt(0),el_eta(0),el_phi(0),el_charge(0),
el_author(0),el_tightPP(0),
el_trackz0pvunbiased(0),el_tracketa(0),el_trackphi(0),
el_cl_E(0),el_cl_pt(0),el_cl_eta(0),el_cl_phi(0),
el_OQ(0),el_etap(0),el_etas2(0),
el_MI10_max40_ptsum(0),

//Muon variables
mu_n(0),
mu_E(0),mu_pt(0),mu_eta(0),mu_phi(0),
mu_charge(0),mu_author(0),mu_tight(0),

//Weight variables
tree_Weight(0),tree_BTagWeight(0),tree_LeptonSF(0),
tree_TRFExBTagWeight(0),tree_TRFInBTagWeight(0),
tree_ToptTpTDataweight(0),
tree_qcdWeight(0),tree_crossSection(0),
tree_ttcc_rw(0),tree_ttbb_rw(0),tree_VpT_rw(0),tree_VpTHF_rw(0),

tree_TRFDiscreteBTagWeight_ex(0),tree_TRFDiscreteBTagWeight_in(0),
tree_TRFChosenTag_ex(0),tree_TRFChosenTag_in(0),

tree_BTag_breakdown_up(0),tree_BTag_breakdown_down(0),
tree_CTag_breakdown_up(0),tree_CTag_breakdown_down(0),
tree_LTag_breakdown_up(0),tree_LTag_breakdown_down(0),
tree_TRFBTag_ex_breakdown_up(0),tree_TRFBTag_ex_breakdown_down(0),
tree_TRFCTag_ex_breakdown_up(0),tree_TRFCTag_ex_breakdown_down(0),
tree_TRFLTag_ex_breakdown_up(0),tree_TRFLTag_ex_breakdown_down(0),
tree_TRFBTag_in_breakdown_up(0),tree_TRFBTag_in_breakdown_down(0),
tree_TRFCTag_in_breakdown_up(0),tree_TRFCTag_in_breakdown_down(0),
tree_TRFLTag_in_breakdown_up(0),tree_TRFLTag_in_breakdown_down(0),

tree_LeptonSF_TrigSF_up(0),tree_LeptonSF_TrigSF_down(0),
tree_LeptonSF_IDSF_up(0),tree_LeptonSF_IDSF_down(0),
tree_LeptonSF_RecoSF_up(0),tree_LeptonSF_RecoSF_down(0),
tree_EleSF_TrigSF_up(0),tree_EleSF_TrigSF_down(0),
tree_EleSF_IDSF_up(0),tree_EleSF_IDSF_down(0),
tree_EleSF_RecoSF_up(0),tree_EleSF_RecoSF_down(0),
tree_MuonSF_TrigSF_up(0),tree_MuonSF_TrigSF_down(0),
tree_MuonSF_IDSF_up(0),tree_MuonSF_IDSF_down(0),
tree_MuonSF_RecoSF_up(0),tree_MuonSF_RecoSF_down(0),

tree_ttH_scale_static(0),tree_ttH_scale_up(0), tree_ttH_scale_down(0),

tree_ttbartop_pt_rw_IFSR_up(0),tree_ttbartop_pt_rw_IFSR_down(0),
tree_ttbartop_pt_rw_Fragmentation_up(0),tree_ttbartop_pt_rw_Fragmentation_down(0),
tree_ttbartop_pt_rw_MCgen_up(0),tree_ttbartop_pt_rw_MCgen_down(0),
tree_ttbartop_pt_rw_JER_up(0),tree_ttbartop_pt_rw_JER_down(0),
tree_ttbartop_pt_rw_bJES_up(0),tree_ttbartop_pt_rw_bJES_down(0),
tree_ttbartop_pt_rw_closebyJES_up(0),tree_ttbartop_pt_rw_closebyJES_down(0),
tree_ttbartop_pt_rw_etacalibJES_up(0),tree_ttbartop_pt_rw_etacalibJES_down(0),
tree_ttbartop_pt_rw_effdetset1JES_up(0),tree_ttbartop_pt_rw_effdetset1JES_down(0),
tree_ttbartop_pt_rw_btageff_up(0),tree_ttbartop_pt_rw_btageff_down(0),

tree_MadgraphMC_up(0),tree_MadgraphMC_down(0),tree_MadgraphMB_up(0),tree_MadgraphMB_down(0),
tree_MadgraphQ2_up(0),tree_MadgraphQ2_down(0),tree_MadgraphMatch_up(0),tree_MadgraphPP(0),
tree_toppt_only(0),tree_ttbarpt_only(0),

tree_ttbarbb_CSS_kin(0),tree_ttbarbb_R_Mbb(0),tree_ttbarbb_NNPDF(0),tree_ttbarbb_MSTW(0),
tree_ttbarbb_QCMMPS(0),tree_ttbarbb_scale05(0),tree_ttbarbb_scale2(0),

finalEvent_weightNom(0),
finalEvent_BTag_breakdown_up(0),finalEvent_BTag_breakdown_down(0),
finalEvent_CTag_breakdown_up(0),finalEvent_CTag_breakdown_down(0),
finalEvent_LTag_breakdown_up(0),finalEvent_LTag_breakdown_down(0),
finalEvent_BTagExtrap_up(0),finalEvent_BTagExtrap_down(0),
finalEvent_EleID_up(0), finalEvent_EleID_down(0),
finalEvent_EleReco_up(0), finalEvent_EleReco_down(0),
finalEvent_EleTrig_up(0), finalEvent_EleTrig_down(0),
finalEvent_MuonID_up(0), finalEvent_MuonID_down(0),
finalEvent_MuonReco_up(0), finalEvent_MuonReco_down(0),
finalEvent_MuonTrig_up(0), finalEvent_MuonTrig_down(0),
finalEvent_ttH_scale_static(0),finalEvent_ttH_scale_up(0), finalEvent_ttH_scale_down(0),

finalEvent_ttbartop_pt_rw_IFSR_up(0),finalEvent_ttbartop_pt_rw_IFSR_down(0),
finalEvent_ttbartop_pt_rw_Fragmentation_up(0),finalEvent_ttbartop_pt_rw_Fragmentation_down(0),
finalEvent_ttbartop_pt_rw_MCgen_up(0),finalEvent_ttbartop_pt_rw_MCgen_down(0),
finalEvent_ttbartop_pt_rw_JER_up(0),finalEvent_ttbartop_pt_rw_JER_down(0),
finalEvent_ttbartop_pt_rw_bJES_up(0),finalEvent_ttbartop_pt_rw_bJES_down(0),
finalEvent_ttbartop_pt_rw_closebyJES_up(0),finalEvent_ttbartop_pt_rw_closebyJES_down(0),
finalEvent_ttbartop_pt_rw_etacalibJES_up(0),finalEvent_ttbartop_pt_rw_etacalibJES_down(0),
finalEvent_ttbartop_pt_rw_effdetset1JES_up(0),finalEvent_ttbartop_pt_rw_effdetset1JES_down(0),
finalEvent_ttbartop_pt_rw_btageff_up(0),finalEvent_ttbartop_pt_rw_btageff_down(0),

finalEvent_MadgraphMC_up(0),finalEvent_MadgraphMC_down(0),finalEvent_MadgraphMB_up(0),finalEvent_MadgraphMB_down(0),
finalEvent_MadgraphQ2_up(0),finalEvent_MadgraphQ2_down(0),finalEvent_MadgraphMatch_up(0),finalEvent_MadgraphPP(0),
finalEvent_toppt_only(0),finalEvent_ttbarpt_only(0),

finalEvent_ttbarbb_CSS_kin(0),finalEvent_ttbarbb_R_Mbb(0),finalEvent_ttbarbb_NNPDF(0),
finalEvent_ttbarbb_MSTW(0),finalEvent_ttbarbb_QCMMPS(0),finalEvent_ttbarbb_scale05(0),
finalEvent_ttbarbb_scale2(0),

finalEvent_noVpt_rw(0),

//Kinematic variables
HTalljets(0),LT(0),HTleptonsJets(0),meff(0),mT(0)
{
    jet_pt = new std::vector < float >;
    
    jet_pt  = new vector<float>;
    jet_eta = new vector<float>;
    jet_phi = new vector<float>;
    jet_E   = new vector<float>;
    jet_m   = new vector<float>;
    jet_jvf = new vector<float>;
    jet_btag_weight = new vector<float>;
    jet_trueflav    = new vector<int>;
    jet_tagged      = new vector<bool>;
    jet_MV1         = new vector<float>;
    jet_nTrk_pv0_1GeV = new vector<float>;
    jet_trackWIDTH_pv0_1GeV = new vector<float>;
    jet_constit_n   = new vector<int>;
    
    lep_E       = new vector<float>;
    lep_pt      = new vector<float>;
    lep_eta     = new vector<float>;
    lep_phi     = new vector<float>;
    lep_charge  = new vector<float>;
    lep_d0      = new vector<float>;
    lep_z0      = new vector<float>;
    lep_d0sig   = new vector<float>;
    lep_z0sig   = new vector<float>;
    
    el_E                 = new vector<float>;
    el_Et                = new vector<float>;
    el_pt                = new vector<float>;
    el_eta               = new vector<float>;
    el_phi               = new vector<float>;
    el_charge            = new vector<float>;
    el_author            = new vector<int>;
    el_tightPP           = new vector<int>;
    el_trackz0pvunbiased = new vector<float>;
    el_tracketa          = new vector<float>;
    el_trackphi          = new vector<float>;
    el_cl_E              = new vector<float>;
    el_cl_pt             = new vector<float>;
    el_cl_eta            = new vector<float>;
    el_cl_phi            = new vector<float>;
    el_OQ                = new vector<unsigned int>;
    el_etap              = new vector<float>;
    el_etas2             = new vector<float>;
    el_MI10_max40_ptsum  = new vector<float>;
    
    mu_E        = new vector<float>;
    mu_pt       = new vector<float>;
    mu_eta      = new vector<float>;
    mu_phi      = new vector<float>;
    mu_charge   = new vector<float>;
    mu_author   = new vector<int>;
    mu_tight    = new vector<int>;
    
    finalEvent_BTag_breakdown_up = new std::vector < double >;
    finalEvent_BTag_breakdown_down = new std::vector < double >;
    finalEvent_CTag_breakdown_up = new std::vector < double >;
    finalEvent_CTag_breakdown_down = new std::vector < double >;
    finalEvent_LTag_breakdown_up = new std::vector < double >;
    finalEvent_LTag_breakdown_down = new std::vector < double >;
}

void NtupleData::ClearNtupleData(){
    
    ///////////////////////////////////////////
    //
    // EVENT CARACTERISTIC VARIABLES
    //
    ///////////////////////////////////////////
    
    mu = 0;
    vxp_n = 0;
    runNumber = 0;
    eventNumber = 0;
    
    ///////////////////////////////////////////
    //
    // JET VARIABLES
    //
    ///////////////////////////////////////////
    
    jet_n = 0;
    
    //Clear vectors
    if(jet_pt)jet_pt -> clear();
    if(jet_eta)jet_eta -> clear();
    if(jet_phi)jet_phi -> clear();
    if(jet_E)jet_E -> clear();
    if(jet_m)jet_m -> clear();
    if(jet_jvf)jet_jvf -> clear();
    if(jet_trueflav)jet_trueflav -> clear();
    if(jet_tagged)jet_tagged -> clear();
    if(jet_MV1)jet_MV1 -> clear();
    if(jet_nTrk_pv0_1GeV)jet_nTrk_pv0_1GeV-> clear();
    if(jet_trackWIDTH_pv0_1GeV)jet_trackWIDTH_pv0_1GeV-> clear();
    if(jet_constit_n)jet_constit_n-> clear();
    if(jet_constit_index)jet_constit_index-> clear();
    
    ///////////////////////////////////////////
    //
    // LEPTON VARIABLES
    //
    ///////////////////////////////////////////
    
    lep_n = 0;
    if(lep_E)lep_E -> clear();
    if(lep_pt)lep_pt -> clear();
    if(lep_eta)lep_eta -> clear();
    if(lep_phi)lep_phi -> clear();
    if(lep_charge)lep_charge -> clear();
    if(lep_d0)lep_d0 -> clear();
    if(lep_z0)lep_z0 -> clear();
    if(lep_d0sig)lep_d0sig -> clear();
    if(lep_z0sig)lep_z0sig -> clear();
    
    
    ///////////////////////////////////////////
    //
    // ELECTRON VARIABLES
    //
    ///////////////////////////////////////////
    
    el_n = 0;
    if(el_E)el_E -> clear();
    if(el_Et)el_Et -> clear();
    if(el_pt)el_pt -> clear();
    if(el_eta)el_eta -> clear();
    if(el_phi)el_phi -> clear();
    if(el_charge)el_charge -> clear();
    if(el_author)el_author -> clear();
    if(el_tightPP)el_tightPP -> clear();
    if(el_trackz0pvunbiased)el_trackz0pvunbiased -> clear();
    if(el_tracketa)el_tracketa -> clear();
    if(el_trackphi)el_trackphi -> clear();
    if(el_cl_E)el_cl_E -> clear();
    if(el_cl_pt)el_cl_pt -> clear();
    if(el_cl_eta)el_cl_eta -> clear();
    if(el_cl_phi)el_cl_phi -> clear();
    

    ///////////////////////////////////////////
    //
    // MUON VARIABLES
    //
    ///////////////////////////////////////////
    
    mu_n = 0;
    if(mu_E) mu_E -> clear();
    if(mu_pt) mu_pt -> clear();
    if(mu_eta) mu_eta -> clear();
    if(mu_phi) mu_phi -> clear();
    if(mu_charge) mu_charge -> clear();
    if(mu_author) mu_author -> clear();
    if(mu_tight) mu_tight -> clear();
    
    
    ///////////////////////////////////////////
    //
    // MET VARIABLES
    //
    ///////////////////////////////////////////
    
    MET_et = 0;
    MET_phi = 0;
 
    ///////////////////////////////////////////
    //
    // WEIGHT VARIABLES (including systematics)
    //
    ///////////////////////////////////////////
    
    m_fjcol_list.clear();
    fatjet_n.clear();
    fatjet_pt.clear();
    
    return;
}

void NtupleData::EmptyNtupleData(){
    
    for(vector<int>::iterator jit = m_fjcol_list.begin(); jit != m_fjcol_list.end(); jit++){
        delete fatjet_pt[*jit];
    }
    fatjet_n.clear();
    fatjet_pt.clear();
    
    delete jet_pt;
    
    return;
}
