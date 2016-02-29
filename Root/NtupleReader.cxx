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

NtupleReader::NtupleReader(OptionsBase* opt) : 
  m_opt(opt),
  m_chain(NULL),
  m_ntupData(NULL)
{  }


//_________________________________________________________________________________
//
NtupleReader::NtupleReader( const NtupleReader &q ){
    m_opt   = q.m_opt;
    m_chain = q.m_chain;
    m_ntupData = q.m_ntupData;
}

//_________________________________________________________________________________
//
NtupleReader::~NtupleReader()
{

  delete m_opt;
  delete m_chain;
  delete m_ntupData;

}

//_________________________________________________________________________________
//
int NtupleReader::GetChainEntry( long entry ) const {
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::getChainEntry(" << entry << ")" << std::endl;
    return m_chain->GetEntry(entry);
}

//_________________________________________________________________________________
//
int NtupleReader::ChainNEntries() const {
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::chainNEntries()" << std::endl;
    return m_chain->GetEntries();
}

//_________________________________________________________________________________
//
void NtupleReader::Init(){
    
    //decide whether it is a comma separated list of files, or a text file containing
    //the names of the files to be added
    
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::Init()" << std::endl;
    
    m_chain = new TChain(m_opt->InputTree().c_str());
    
    if( m_opt->TextFileList() ){
        ChainFromTextFile(m_chain, m_opt->InputFile());
    }
    else{
        ChainFromStrList(m_chain, m_opt->InputFile());
    }
    m_chain -> SetBranchStatus("*",0);
    
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::Init()" << std::endl;
    return;
}

//_________________________________________________________________________________
//
void NtupleReader::Finalise(){
    
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::Finalize()" << std::endl;
    
    m_ntupData->EmptyNtupleData();

    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::Finalize()" << std::endl;
    return;
}

//_________________________________________________________________________________
//
void NtupleReader::ChainFromTextFile(TChain* ch, string inputfilename){
    //read the contents of the file and make the filelist
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::ChainFromTextFile()" << std::endl;
    
    ifstream inlist(inputfilename.c_str());
    string fstr="";
    while(getline(inlist,fstr)){
        ch->Add(fstr.c_str());
        fstr.clear();
    }
    inlist.close();
    
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::ChainFromTextFile()" << std::endl;
    return;
}

//_________________________________________________________________________________
//
void NtupleReader::ChainFromStrList(TChain* ch, string inputfilelist){
    
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::ChainFromStrList()" << std::endl;
    
    for (size_t i=0,n; i <= inputfilelist.length(); i=n+1)
    {
        n = inputfilelist.find_first_of(',',i);
        if (n == string::npos)
            n = inputfilelist.length();
        string tmp = inputfilelist.substr(i,n-i);
        ch->Add(tmp.c_str());
        tmp.clear();
    }
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::ChainFromStrList()" << std::endl;
    return;
}

int NtupleReader::SetVariableToChain(const std::string& name, void* variable){
  m_chain->SetBranchStatus(name.c_str(), 1);

  TBranch* branch = 0;
  int stat = m_chain->SetBranchAddress(name.c_str(), variable, &branch);
  return stat;
}

//_________________________________________________________________________________
//
int NtupleReader::SetEventBranchAddresses(){
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "In NtupleReader::setEventBranchAddresses(): this is empty" << std::endl;
    return -1;
}

//_________________________________________________________________________________
//
int NtupleReader::SetJetBranchAddresses(const string &){
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "In NtupleReader::setJetBranchAddresses(): this is empty" << std::endl;
    return -1;
}

//_________________________________________________________________________________
//
int NtupleReader::SetFatJetBranchAddresses(int , const string &){
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::setFatJetBranchAddresses(): this is empty" << std::endl;
    return -1;
}

//_________________________________________________________________________________
//
int NtupleReader::SetElectronBranchAddresses(const string &){
    
    if(m_opt->MsgLevel()==Debug::DEBUG)std::cout << "Entering in NtupleReader::setElectronBranchAddresses(): this is empty" << std::endl;
    return -1;
}

//_________________________________________________________________________________
//
int NtupleReader::SetMuonBranchAddresses(const string &){
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::setMuonBranchAddresses(): this is empty" << std::endl;
    return -1;
}

//_________________________________________________________________________________
//
int NtupleReader::SetLeptonBranchAddresses(const string &){
    if(m_opt->MsgLevel()==Debug::DEBUG)std::cout << "Entering in NtupleReader::setLeptonBranchAddresses(): this is empty" << std::endl;
    return -1;
}

//_________________________________________________________________________________
//
int NtupleReader::SetMETBranchAddresses(const string &){
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::setMETBranchAddresses(): this is empty"<< std::endl;
    return -1;
}

//_________________________________________________________________________________
//
int NtupleReader::SetWeightBranchAddresses(const string &){
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::setWeightBranchAddresses(): this is empty" << std::endl;
    return -1;
}

//_________________________________________________________________________________
//
int NtupleReader::SetTruthParticleBranchAddresses(const string &){
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::SetTruthParticleBranchAddresses(): this is empty" << std::endl;
    return -1;
}

