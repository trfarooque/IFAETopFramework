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
#include "IFAETopFramework/NtupleData.h"
#include "IFAETopFramework/WeightObject.h"

using namespace std;

//_________________________________________________________________________________
//

NtupleReader::NtupleReader(OptionsBase* opt, std::map<std::string, WeightObject*>* nomMap, std::map<std::string, WeightObject*>* sysMap) : 
  m_opt(opt),
  m_chain(NULL),
  m_ntupData(NULL),
  m_nomMap(nomMap),
  m_sysMap(sysMap)
{  }


//_________________________________________________________________________________
//
NtupleReader::NtupleReader( const NtupleReader &q ){
    m_opt        = q.m_opt;
    m_chain      = q.m_chain;
    m_ntupData   = q.m_ntupData;
    m_nomMap     = q.m_nomMap;
    m_sysMap     = q.m_sysMap;
}

//_________________________________________________________________________________
//
NtupleReader::~NtupleReader()
{
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
void NtupleReader::Init(std::map<std::string, WeightObject*> *nomMap, std::map<std::string, WeightObject*> *sysMap){
    
    //decide whether it is a comma separated list of files, or a text file containing
    //the names of the files to be added
    
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::Init()" << std::endl;

    if(nomMap){ m_nomMap = nomMap; }
    if(sysMap){ m_nomMap = sysMap; }

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

  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<"NtupleReader::SetVariableToChain(void*) :: Setting "<<name<<" to "<<variable<<std::endl; }
  m_chain->SetBranchStatus(name.c_str(), 1);

  TBranch* branch = 0;
  int stat = m_chain->SetBranchAddress(name.c_str(), variable, &branch);
  return stat;
}

//_________________________________________________________________________________
//
int NtupleReader::SetAllBranchAddresses(){

    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Begin NtupleReader::SetAllBranchAddresses() " << std::endl;
    SetWeightBranchAddresses("");
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "End NtupleReader::SetAllBranchAddresses() " << std::endl;

    return -1;
}

//_________________________________________________________________________________
//
int NtupleReader::SetWeightBranchAddresses(const string &){

    if(m_opt->MsgLevel() == Debug::DEBUG) std::cout << "Entering NtupleReader::SetWeightBranchAddresses()" << std::endl;

    for( std::pair<std::string, WeightObject*> nom : *m_nomMap ){
      if(!nom.second->IsInput()) continue;
      const std::string& name = nom.second->Name();
      const std::string& branchName = nom.second->BranchName();
      if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout << "Setting weight branch " << name << " to branch " << branchName << std::endl; }

      std::cout<<" m_ntupData == " << m_ntupData << std::endl; 
      std::cout<<" m_ntupData->d_nominal_weight_components == " << m_ntupData->d_nominal_weight_components << std::endl; 
      if( m_ntupData->d_nominal_weight_components->find(name) == m_ntupData->d_nominal_weight_components->end() ){
	std::cout << " WARNING NtupleReader::SetWeightBranchAddress : " << name
		  << " does not exist in the list of input nominal weight branches. Please chack!!! " << std::endl;
	continue;
      }      
      SetVariableToChain(branchName, &(m_ntupData->d_nominal_weight_components->at(name)) );
    }

    //_____________________________________________________________________________
    for( std::pair<std::string, WeightObject*> sys : *m_sysMap ){
      if(!sys.second->IsInput()) continue;
      const std::string& name = sys.second->Name();
      const std::string& branchName = sys.second->BranchName();
      
      if( m_ntupData->d_syst_weight_components->find(name) == m_ntupData->d_syst_weight_components->end() ){
	std::cout << " WARNING NtupleReader::SetWeightBranchAddress : " << name 
		  << " does not exist in the list of input systematics weight branches. Please chack!!! " << std::endl;
	continue;
      }      
      SetVariableToChain(branchName, &(m_ntupData->d_syst_weight_components->at(name)) );
    }

    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::SetWeightBranchAddresses()" << std::endl;

    return -1;
}

