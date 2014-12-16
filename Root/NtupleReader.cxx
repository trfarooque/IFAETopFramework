#include <iostream>
#include <fstream>

//ROOT includes
#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"
#include "TBranch.h"

//Package specific includes
#include "IFAETopFramework/NtupleReader.h"
#include "IFAETopFramework/CommonConstants.h"
#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/NtupleData.h"

using namespace std;

//_________________________________________________________________________________
//
NtupleReader::NtupleReader(NtupleData* ntupData, OptionsBase* opt )
: m_ntupData(ntupData), m_opt(opt){
    
    m_chain = NULL;
    
    EmptyBranches();
    //m_treetype = m_opt; -->OptionsBase will also have inheritances
    return;
}

//_________________________________________________________________________________
//
NtupleReader::~NtupleReader()
{}

//_________________________________________________________________________________
//
void NtupleReader::Init(){
    
    //decide whether it is a comma separated list of files, or a text file containing
    //the names of the files to be added
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::Init()" << std::endl;
    
    m_chain = new TChain(m_opt->inputTree().c_str());
    
    if( m_opt->textFileList() ){
        ChainFromTextFile(m_chain, m_opt->inputFile());
    }
    else{
        ChainFromStrList(m_chain, m_opt->inputFile());
    }
    m_chain -> SetBranchStatus("*",0);
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::Init()" << std::endl;

    return;
}

//_________________________________________________________________________________
//
void NtupleReader::Finalise(){
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::Finalize()" << std::endl;
    
    EmptyBranches();
    delete m_chain;
    delete m_opt;
    
    m_ntupData->EmptyNtupleData();
    delete m_ntupData;

    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::Finalize()" << std::endl;
    
    return;
}

//_________________________________________________________________________________
//
void NtupleReader::EmptyBranches(){
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::EmptyBranches()" << std::endl;
    
    //Fat jets
    for(vector<int>::iterator brit = (m_ntupData->m_fjcol_list).begin(); brit != (m_ntupData->m_fjcol_list).end(); brit++){
        if(b_fatjet_n.find(*brit) != b_fatjet_n.end()){delete b_fatjet_n[*brit];}
        if(b_fatjet_n.find(*brit) != b_fatjet_n.end()){delete b_fatjet_pt[*brit];}
    }
    
    b_fatjet_n.clear();
    b_fatjet_pt.clear();
    
    //Jet
    delete b_jet_n;
    delete b_jet_pt;
    
    delete b_mu;
    delete b_vxp_n;
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::EmptyBranches()" << std::endl;
    
    return;
}


//_________________________________________________________________________________
//
void NtupleReader::ChainFromTextFile(TChain* ch, string inputfilename){
    //read the contents of the file and make the filelist
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::ChainFromTextFile()" << std::endl;
    
    ifstream inlist(inputfilename.c_str());
    string fstr="";
    while(getline(inlist,fstr)){
        ch->Add(fstr.c_str());
        fstr.clear();
    }
    inlist.close();
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::ChainFromTextFile()" << std::endl;
    
    return;
}

//_________________________________________________________________________________
//
void NtupleReader::ChainFromStrList(TChain* ch, string inputfilelist){
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::ChainFromStrList()" << std::endl;
    
    for (size_t i=0,n; i <= inputfilelist.length(); i=n+1)
    {
        n = inputfilelist.find_first_of(',',i);
        if (n == string::npos)
            n = inputfilelist.length();
        string tmp = inputfilelist.substr(i,n-i);
        ch->Add(tmp.c_str());
        tmp.clear();
    }
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::ChainFromStrList()" << std::endl;

    return;
}

//_________________________________________________________________________________
//
int NtupleReader::setEventBranchAddresses(){
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::setEventBranchAddresses()" << std::endl;
    
    int b_stat = 0;
    
    m_chain -> SetBranchStatus("vxp_n",1);
    m_chain -> SetBranchStatus("averageIntPerXing",1);
    b_stat += m_chain->SetBranchAddress("vxp_n", &(m_ntupData->vxp_n), &b_vxp_n);
    b_stat += m_chain->SetBranchAddress("averageIntPerXing", &(m_ntupData->mu), &b_mu);
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::setEventBranchAddresses()" << std::endl;
    
    return b_stat;
    
}

//_________________________________________________________________________________
//
int NtupleReader::setJetBranchAddresses(const string &sj){
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::setJetBranchAddresses()" << std::endl;
    
    int b_stat = 0;
    m_chain -> SetBranchStatus(Form("%s_n",sj.c_str()),1);
    m_chain -> SetBranchStatus(Form("%s_pt",sj.c_str()),1);
    b_stat += m_chain->SetBranchAddress(Form("%s_n",sj.c_str()), &(m_ntupData->jet_n), &b_jet_n);
    b_stat += m_chain->SetBranchAddress(Form("%s_pt",sj.c_str()), &(m_ntupData->jet_pt), &b_jet_pt);
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::setJetBranchAddresses()" << std::endl;
    
    return b_stat;
    
}

//_________________________________________________________________________________
//
int NtupleReader::setFatJetBranchAddresses(int sfj_key, const string &sfj){
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::setFatJetBranchAddresses()" << std::endl;
    
    int b_stat = 0;
    
    (m_ntupData->fatjet_n)[sfj_key] = 0;
    (m_ntupData->fatjet_pt)[sfj_key] = 0;
    
    //
    b_fatjet_n[sfj_key] = 0;
    b_fatjet_pt[sfj_key] = 0;
    //
    
    m_chain -> SetBranchStatus(Form("%s_n",sfj.c_str()),1);
    m_chain -> SetBranchStatus(Form("%s_pt",sfj.c_str()),1);
    b_stat += m_chain->SetBranchAddress(Form("%s_n",sfj.c_str()), &((m_ntupData->fatjet_n)[sfj_key]), &b_fatjet_n[sfj_key]);
    b_stat += m_chain->SetBranchAddress(Form("%s_pt",sfj.c_str()), &((m_ntupData->fatjet_pt)[sfj_key]), &b_fatjet_pt[sfj_key]);
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::setFatJetBranchAddresses()" << std::endl;
    
    return b_stat;
}


//_________________________________________________________________________________
//
int NtupleReader::setElectronBranchAddresses(const string &sfj){
    
    if(m_opt->msgLevel()==Debug::DEBUG){
        std::cout << "Entering in NtupleReader::setElectronBranchAddresses()";
        std::cout << "==> Noting implemented ... You should not be there" << std::endl;
    }
    
    return -1;
}

//_________________________________________________________________________________
//
int NtupleReader::setMuonBranchAddresses(const string &sfj){
    
    if(m_opt->msgLevel()==Debug::DEBUG){
        std::cout << "Entering in NtupleReader::setMuonBranchAddresses()";
        std::cout << "==> Noting implemented ... You should not be there" << std::endl;
    }
    return -1;
}


//_________________________________________________________________________________
//
int NtupleReader::setLeptonBranchAddresses(const string &sfj){
    
    if(m_opt->msgLevel()==Debug::DEBUG){
        std::cout << "Entering in NtupleReader::setLeptonBranchAddresses()";
        std::cout << "==> Noting implemented ... You should not be there" << std::endl;
    }
    return -1;
}


//_________________________________________________________________________________
//
int NtupleReader::setMETBranchAddresses(const string &sfj){
    
    if(m_opt->msgLevel()==Debug::DEBUG){
        std::cout << "Entering in NtupleReader::setMETBranchAddresses()";
        std::cout << "==> Noting implemented ... You should not be there" << std::endl;
    }
    return -1;
}

//_________________________________________________________________________________
//
int NtupleReader::setWeightBranchAddresses(const string &sfj){
    
    if(m_opt->msgLevel()==Debug::DEBUG){
        std::cout << "Entering in NtupleReader::setWeightBranchAddresses()";
        std::cout << "==> Noting implemented ... You should not be there" << std::endl;
    }
    return -1;
}
