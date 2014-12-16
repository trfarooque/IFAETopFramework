#include <iostream>
#include "IFAETopFramework/NtupleData.h"

using namespace std;
NtupleData::NtupleData():
mu(0),
vxp_n(0),
runNumber(0),
eventNumber(0),
jet_n(0),
jet_pt(0),
jet_eta(0),
jet_phi(0),
jet_E(0),
jet_m(0),
jet_jvf(0),
jet_trueflav(0),
jet_tagged(0),
jet_MV1(0),
jet_nTrk_pv0_1GeV(0),
jet_trackWIDTH_pv0_1GeV(0),
jet_constit_n(0),
jet_constit_index(0),
lep_n(0),
lep_ind(0),
lep_flav(0),
lep_E(0),
lep_pt(0),
lep_eta(0),
lep_phi(0),
lep_charge(0),
lep_d0(0),
lep_z0(0),
lep_d0sig(0),
lep_z0sig(0),
el_n(0),
el_E(0),
el_Et(0),
el_pt(0),
el_eta(0),
el_phi(0),
el_charge(0),
el_author(0),
el_tightPP(0),
el_trackz0pvunbiased(0),
el_tracketa(0),
el_trackphi(0),
el_cl_E(0),
el_cl_pt(0),
el_cl_eta(0),
el_cl_phi(0),
el_OQ(0),
el_etap(0),
el_etas2(0),
el_MI10_max40_ptsum(0),
mu_n(0),
mu_E(0),
mu_pt(0),
mu_eta(0),
mu_phi(0),
mu_charge(0),
mu_author(0),
mu_tight(0),
BTag_breakdown_up(0)
{
    //ClearNtupleData();
    return;
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

    WeightNom = 0;
    
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
