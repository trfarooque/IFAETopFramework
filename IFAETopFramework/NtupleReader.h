#ifndef NTUPLEREADER_H
#define NTUPLEREEADER_H

#include <string>
#include <map>
#include "TChain.h"

class OptionsBase;
class NtupleData;
class TBranch;

class NtupleReader{
    
public:
    NtupleReader(NtupleData* ntupData, OptionsBase* opt);
    
    virtual ~NtupleReader();
    
    void Init();
    void Finalise();
    virtual void EmptyBranches();
    
    virtual int setEventBranchAddresses();
    
    virtual int setJetBranchAddresses(const std::string &sj);
    virtual int setFatJetBranchAddresses(int sfj_key, const std::string &sfj);
    
    virtual int setLeptonBranchAddresses(const std::string &sj);
    virtual int setElectronBranchAddresses(const std::string &sj);
    virtual int setMuonBranchAddresses(const std::string &sj);
    
    virtual int setMETBranchAddresses(const std::string &sj);
    
    virtual int setWeightBranchAddresses(const std::string &sj);
    
    int getChainEntry(long entry){return m_chain->GetEntry(entry); }
    int chainNEntries(){return m_chain->GetEntries();}
    
    void ChainFromTextFile(TChain* ch, std::string infilename);
    void ChainFromStrList(TChain* ch, std::string infilelist);
    
    //NtupleReader needs to know:
    // the location and name of the chain to read (read here from given file name and tree name)
    // the type of input tree
    // the ntupledata which it must fill
    
protected:
    
    TChain* m_chain;    //!
    OptionsBase* m_opt;
    //InputType::input m_treetype;
    NtupleData* m_ntupData;
    
    //Branches
    //Fat jets
    std::map<int, TBranch*> b_fatjet_n;
    std::map<int, TBranch*> b_fatjet_pt;
    std::map<int, TBranch*> b_fatjet_eta;
    std::map<int, TBranch*> b_fatjet_phi;
    std::map<int, TBranch*> b_fatjet_m;
    std::map<int, TBranch*> b_fatjet_constscale_pt;
    std::map<int, TBranch*> b_fatjet_constscale_eta;
    std::map<int, TBranch*> b_fatjet_constscale_phi;
    std::map<int, TBranch*> b_fatjet_constscale_m;
    std::map<int, TBranch*> b_fatjet_ActiveArea;
    std::map<int, TBranch*> b_fatjet_ActiveArea_px;
    std::map<int, TBranch*> b_fatjet_ActiveArea_py;
    std::map<int, TBranch*> b_fatjet_ActiveArea_pz;
    std::map<int, TBranch*> b_fatjet_ActiveArea_E;
    
    std::map<int, TBranch*> b_fatjet_SPLIT12;
    std::map<int, TBranch*> b_fatjet_SPLIT23;
    std::map<int, TBranch*> b_fatjet_ZCUT12;
    std::map<int, TBranch*> b_fatjet_ZCUT23;
    std::map<int, TBranch*> b_fatjet_WIDTH;
    std::map<int, TBranch*> b_fatjet_PlanarFlow;
    std::map<int, TBranch*> b_fatjet_Angularity;
    std::map<int, TBranch*> b_fatjet_PullMag;
    std::map<int, TBranch*> b_fatjet_PullPhi;
    
    std::map<int, TBranch*> b_fatjet_Tau1;
    std::map<int, TBranch*> b_fatjet_Tau2;
    std::map<int, TBranch*> b_fatjet_Tau3;
    std::map<int, TBranch*> b_fatjet_nbtag;
    std::map<int, TBranch*> b_fatjet_nbtag_geom;
    std::map<int, TBranch*> b_fatjet_MCclass;
    std::map<int, TBranch*> b_fatjet_orig_ind;
    std::map<int, TBranch*> b_fatjet_assoc_ind;
    
    std::map<int, TBranch*> b_fatjet_constit_n;
    std::map<int, TBranch*> b_fatjet_constit_index;
    
    //Jet
    TBranch        *b_jet_n;   //!
    TBranch        *b_jet_pt;   //!
    TBranch        *b_jet_eta;   //!
    TBranch        *b_jet_phi;   //!
    TBranch        *b_jet_E;   //!
    TBranch        *b_jet_jvf;   //!
    TBranch        *b_jet_trueflav;   //!
    TBranch        *b_jet_MV1;   //!
    TBranch        *b_jet_MV1c;   //!
    TBranch        *b_jet_nTrk_pv0_1GeV;   //!
    TBranch        *b_jet_trackWIDTH_pv0_1GeV;   //!
    
    TBranch        *b_jet_constit_n;   //!
    TBranch        *b_jet_constit_index;   //!
    
    
    TBranch   *b_el_n;   //!
    TBranch   *b_el_E;   //!
    TBranch   *b_el_Et;   //!
    TBranch   *b_el_pt;   //!
    TBranch   *b_el_m;   //!
    TBranch   *b_el_eta;   //!
    TBranch   *b_el_phi;   //!
    TBranch   *b_el_charge;   //!
    TBranch   *b_el_author;   //!
    TBranch   *b_el_tightPP;   //!
    TBranch   *b_el_trackz0pvunbiased;   //!
    TBranch   *b_el_tracketa;   //!
    TBranch   *b_el_trackphi;   //!
    TBranch   *b_el_cl_E;   //!
    TBranch   *b_el_cl_pt;   //!
    TBranch   *b_el_cl_eta;   //!
    TBranch   *b_el_cl_phi;   //!
    TBranch   *b_el_OQ;   //!
    TBranch   *b_el_etap;   //!
    TBranch   *b_el_etas2;   //!
    TBranch   *b_el_MI10_max40_ptsum;   //!
    
    
    TBranch   *b_mu_n;   //!
    TBranch   *b_mu_E;   //!
    TBranch   *b_mu_pt;   //!
    TBranch   *b_mu_m;   //!
    TBranch   *b_mu_eta;   //!
    TBranch   *b_mu_phi;   //!
    TBranch   *b_mu_charge;   //!
    TBranch   *b_mu_author;   //!
    TBranch   *b_mu_tight;   //!
    TBranch   *b_mu_nPixHits;   //!
    TBranch   *b_mu_nSCTHits;   //!
    TBranch   *b_mu_nTRTHits;   //!
    TBranch   *b_mu_nPixHoles;   //!
    TBranch   *b_mu_nSCTHoles;   //!
    TBranch   *b_mu_nTRTOutliers;   //!
    TBranch   *b_mu_nPixelDeadSensors;   //!
    TBranch    *b_mu_nSCTDeadSensors;   //!
    TBranch   *b_mu_id_z0_exPV;   //!
    TBranch   *b_mu_MI10_max40_ptsum;   //!
    
    
    TBranch        *b_larError;   //!
    TBranch        *b_tileError;   //!
    TBranch        *b_coreFlags;   //!
    
    TBranch        *b_runNumber;   //!
    TBranch        *b_eventNumber;   //!
    TBranch        *b_channelNumber;   //!
    TBranch        *b_mu;   //!
    TBranch        *b_vxp_n;   //!
    TBranch        *b_vxp_nTracks;   //!
    TBranch        *b_vxp_type;   //!
    
    TBranch          *b_EF_mu24i_tight;
    TBranch          *b_EF_mu36_tight;
    TBranch          *b_EF_e24vhi_medium1;
    TBranch          *b_EF_e60_medium1;
    
    
    TBranch        *b_lep_pt;   //!
    TBranch        *b_lep_eta;   //!
    TBranch        *b_lep_phi;   //!
    TBranch        *b_lep_E;   //!
    TBranch        *b_lep_charge;   //!
    TBranch        *b_lep_flav;   //!
    
    TBranch* b_MET_et;   //!
    TBranch* b_MET_phi;   //!
    
    TBranch        *b_OrigNumOfEventsInFile;   //!
    TBranch        *b_PUweight;   //!
    TBranch        *b_ZVxpWeight;   //!
    TBranch        *b_XSection;   //!
    TBranch        *b_KFactor;   //!
    TBranch        *b_JVFSFweight;   //!
    TBranch        *b_BTagSFweight;   //!
    
    TBranch        *b_totalScale;   //!
    TBranch        *b_elecScale;   //!
    TBranch        *b_muonScale;   //!
    TBranch        *b_trigScale;   //!
    TBranch        *b_WJetsShapeScale;   //!
    TBranch        *b_WJetsScale;   //!
    TBranch        *b_bTagCat;   //!
    TBranch        *b_BoostedEvent;   //!
    
    TBranch        *b_mc_n;   //!
    TBranch        *b_mc_E;   //!
    TBranch        *b_mc_pt;   //!
    TBranch        *b_mc_eta;   //!
    TBranch        *b_mc_phi;   //!
    TBranch        *b_mc_m;   //!
    TBranch        *b_mc_status;   //!
    TBranch        *b_mc_barcode;   //!
    TBranch        *b_mc_pdgId;   //!
    TBranch        *b_mc_charge;   //!
    TBranch        *b_mc_parent_index;   //!
    TBranch        *b_mc_child_index;   //!
    
    
    TBranch        *b_mcevt_weight;   //!
    TBranch        *b_mcevt_pdf1;   //!
    TBranch        *b_mcevt_pdf2;   //!
    TBranch        *b_mcevt_pdf_id1;   //!
    TBranch        *b_mcevt_pdf_id2;   //!
    TBranch        *b_mcevt_pdf_scale;   //!
    TBranch        *b_mcevt_pdf_x1;   //!
    TBranch        *b_mcevt_pdf_x2;   //!
    
    TBranch        *b_cl_n;   //!
    TBranch        *b_cl_pt;   //!
    TBranch        *b_cl_eta;   //!
    TBranch        *b_cl_phi;   //!
    TBranch        *b_cl_centermag;   //!
    
    
    TBranch   *b_trk_n;   //!
    TBranch   *b_trk_pt;   //!
    TBranch   *b_trk_eta;   //!
    TBranch   *b_trk_d0_wrtPV;   //!
    TBranch   *b_trk_z0_wrtPV;   //!
    TBranch   *b_trk_phi_wrtPV;   //!
    TBranch   *b_trk_theta_wrtPV;   //!
    TBranch   *b_trk_qoverp_wrtPV;   //!
    TBranch   *b_trk_chi2;   //!
    TBranch   *b_trk_ndof;  //!
    TBranch   *b_trk_nPixHits;  //!
    TBranch   *b_trk_nSCTHits;  //!
    
    
    
    
    TBranch        *b_trig_EF_el_n;   //!
    TBranch        *b_trig_EF_el_eta;   //!
    TBranch        *b_trig_EF_el_phi;   //!
    TBranch        *b_trig_EF_el_EF_e24vhi_medium1;   //!
    TBranch        *b_trig_EF_el_EF_e60_medium1;   //!
    TBranch        *b_trig_EF_trigmuonef_EF_mu24i_tight;   //!
    TBranch        *b_trig_EF_trigmuonef_EF_mu36_tight;   //!
    
    TBranch        *b_trig_EF_trigmuonef_n;   //!
    TBranch        *b_trig_EF_trigmuonef_track_n;   //!
    TBranch        *b_trig_EF_trigmuonef_track_CB_pt;   //!
    TBranch        *b_trig_EF_trigmuonef_track_CB_eta;   //!
    TBranch        *b_trig_EF_trigmuonef_track_CB_phi;   //!
    
};


#endif
