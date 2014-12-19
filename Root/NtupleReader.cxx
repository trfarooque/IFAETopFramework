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
NtupleReader::NtupleReader(NtupleData* ntupData, OptionsBase* opt ):
m_opt(opt),
m_ntupData(ntupData)
{
    m_chain = NULL;
    EmptyBranches();
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
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "In NtupleReader::EmptyBranches(): this is empty" << std::endl;
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
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "In NtupleReader::setEventBranchAddresses(): this is empty" << std::endl;
    return -1;
    
}

//_________________________________________________________________________________
//
int NtupleReader::setJetBranchAddresses(const string &sj){
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "In NtupleReader::setJetBranchAddresses(): this is empty" << std::endl;
    return -1;
    
}

//_________________________________________________________________________________
//
int NtupleReader::setFatJetBranchAddresses(int sfj_key, const string &sfj){
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::setFatJetBranchAddresses(): this is empty" << std::endl;
    return -1;
}


//_________________________________________________________________________________
//
int NtupleReader::setElectronBranchAddresses(const string &sfj){
    
    if(m_opt->msgLevel()==Debug::DEBUG)std::cout << "Entering in NtupleReader::setElectronBranchAddresses(): this is empty" << std::endl;
    return -1;
}

//_________________________________________________________________________________
//
int NtupleReader::setMuonBranchAddresses(const string &sfj){
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::setMuonBranchAddresses(): this is empty" << std::endl;
    return -1;
}


//_________________________________________________________________________________
//
int NtupleReader::setLeptonBranchAddresses(const string &sfj){
    
    if(m_opt->msgLevel()==Debug::DEBUG)std::cout << "Entering in NtupleReader::setLeptonBranchAddresses(): this is empty" << std::endl;
    return -1;
}


//_________________________________________________________________________________
//
int NtupleReader::setMETBranchAddresses(const string &sfj){
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::setMETBranchAddresses(): this is empty"<< std::endl;
    return -1;
}

//_________________________________________________________________________________
//
int NtupleReader::setWeightBranchAddresses(const string &sfj){
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::setWeightBranchAddresses(): this is empty" << std::endl;
    return -1;
}

