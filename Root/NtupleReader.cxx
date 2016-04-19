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
{ m_branchList.clear(); }


//_________________________________________________________________________________
//
NtupleReader::NtupleReader( const NtupleReader &q ){
    m_opt        = q.m_opt;
    m_chain      = q.m_chain;
    m_ntupData   = q.m_ntupData;
    m_nomMap     = q.m_nomMap;
    m_sysMap     = q.m_sysMap;
    m_branchList = q.m_branchList;
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
    int stat = m_chain->GetEntry(entry);
    //Make sure all the input WeightObjects are updated
    if(m_nomMap){
      for(std::pair<std::string, WeightObject*> nom : *m_nomMap){
	if(nom.second->IsInput()){ nom.second->UpdateComponentStore(); }
      }
    }
    if(m_sysMap){
      for(std::pair<std::string, WeightObject*> sys : *m_sysMap){ 
	if(sys.second->IsInput()){ sys.second->UpdateComponentStore(); }
      }
    }
    return stat;
}

//_________________________________________________________________________________
//
int NtupleReader::ChainNEntries() const {
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::chainNEntries()" << std::endl;
    return m_chain->GetEntries();
}

//_________________________________________________________________________________
//
bool NtupleReader::Init(std::map<std::string, WeightObject*> *nomMap, std::map<std::string, WeightObject*> *sysMap){
    
    //decide whether it is a comma separated list of files, or a text file containing
    //the names of the files to be added
    
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::Init()" << std::endl;
    if(m_ntupData == NULL){
      std::cerr << "NtupleReader::Init() --> ERROR : NtupleData is NULL. Please check your code."<<std::endl;
      return false;
    }
    m_branchList.clear();
    if(nomMap){ m_nomMap = nomMap; }
    if(sysMap){ m_sysMap = sysMap; }

    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "NtupleReader::Init() ; m_nomMap = " << m_nomMap << " ; m_sysMap = " << m_sysMap << std::endl;

    m_chain = new TChain(m_opt->InputTree().c_str());
    int nfile = 0;   
    if( m_opt->TextFileList() ){
        nfile = ChainFromTextFile(m_chain, m_opt->InputFile());
    }
    else{
        nfile = ChainFromStrList(m_chain, m_opt->InputFile());
    }
    if( nfile <= 0 ){
      std::cerr << "NtupleReader::Init() --> No files were added to the chain "<<std::endl;
      return false;
    }
    //This is important to allow checks on the return codes of SetBranchAddress
    m_chain->LoadTree(0);
    m_chain -> SetBranchStatus("*",0);

    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::Init()" << std::endl;
    return true;
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
int NtupleReader::ChainFromTextFile(TChain* ch, const std::string& inputfilename){
    //read the contents of the file and make the filelist
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::ChainFromTextFile()" << std::endl;
    
    ifstream inlist(inputfilename.c_str());
    string fstr="";
    int nfile =0;
    while(getline(inlist,fstr)){
        nfile += ch->Add(fstr.c_str());
        fstr.clear();
    }
    inlist.close();
    
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::ChainFromTextFile()" << std::endl;

    return nfile;
}

//_________________________________________________________________________________
//
int NtupleReader::ChainFromStrList(TChain* ch, const std::string& inputfilelist){
    
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in NtupleReader::ChainFromStrList()" << std::endl;
    int nfile = 0; 
    for (size_t i=0,n; i <= inputfilelist.length(); i=n+1)
    {
        n = inputfilelist.find_first_of(',',i);
        if (n == string::npos)
            n = inputfilelist.length();
        string tmp = inputfilelist.substr(i,n-i);
        nfile += ch->Add(tmp.c_str());
        tmp.clear();
    }
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::ChainFromStrList()" << std::endl;
    return nfile;
}


int NtupleReader::SetVariableToChain(const std::string& name, void* variable){

  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<"NtupleReader::SetVariableToChain(void*) :: Setting "<<name<<" to "<<variable<<std::endl; }
  std::set<std::string>::iterator it = m_branchList.find(name);
  if(it != m_branchList.end()){
    std::cout << "NtupleReader::SetVariableToChain()--> WARNING: Branch with name "<<name<<" has already been set to an address. Ignoring. "<<std::endl;
    return 0;
  }

  m_chain->SetBranchStatus(name.c_str(), 1);

  TBranch* branch = 0;
  int stat = m_chain->SetBranchAddress(name.c_str(), variable, &branch);
  if(stat == 0){ m_branchList.insert(name); }
  return stat;
}

//_________________________________________________________________________________
//
int NtupleReader::SetAllBranchAddresses(){

    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Begin NtupleReader::SetAllBranchAddresses() " << std::endl;
    int stat = SetWeightBranchAddresses();
    if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "End NtupleReader::SetAllBranchAddresses() " << std::endl;

    return stat;
}

//_________________________________________________________________________________
//
int NtupleReader::SetWeightBranchAddress(WeightObject* wgt){

  const std::string& name = wgt->Name();
  const std::string& branchName = wgt->BranchName();
  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout << "Setting weight branch " << name << " to branch " << branchName << std::endl; }
  VariableDef::VariableType varType = wgt->GetComponentType();

  int stat = -1;
  if(varType == VariableDef::DOUBLE){
    if( m_ntupData->d_D_weight_components->find(branchName) == m_ntupData->d_D_weight_components->end() ){
      std::cout << " WARNING NtupleReader::SetWeightBranchAddress : " << name
		<< " does not exist in the list of input nominal weight branches. Please chack!!! " << std::endl;
    }
    stat = SetVariableToChain(branchName, &(m_ntupData->d_D_weight_components->at(branchName)) );
  }
  else if(varType == VariableDef::FLOAT){
    if( m_ntupData->d_F_weight_components->find(branchName) == m_ntupData->d_F_weight_components->end() ){
      std::cout << " WARNING NtupleReader::SetWeightBranchAddress : " << name
		<< " does not exist in the list of input nominal weight branches. Please chack!!! " << std::endl;
    }
    stat = SetVariableToChain(branchName, &(m_ntupData->d_F_weight_components->at(branchName)) );
  }
  else if( (varType == VariableDef::PTRVECDOUBLE) || (varType == VariableDef::VECDOUBLE) ){
    if( m_ntupData->d_vecD_weight_components->find(branchName) == m_ntupData->d_vecD_weight_components->end() ){
      std::cout << " WARNING NtupleReader::SetWeightBranchAddress : " << name
		<< " does not exist in the list of input nominal weight branches. Please chack!!! " << std::endl;
    }
    if(varType == VariableDef::PTRVECDOUBLE){
      stat = SetVariableToChain(branchName, &(m_ntupData->d_vecD_weight_components->at(branchName)) );
    }
    else{
      stat = SetVariableToChain(branchName, m_ntupData->d_vecD_weight_components->at(branchName) );
    }
  }
  else if( (varType == VariableDef::PTRVECFLOAT) || (varType == VariableDef::VECFLOAT) ){
    if( m_ntupData->d_vecF_weight_components->find(branchName) == m_ntupData->d_vecF_weight_components->end() ){
      std::cout << " WARNING NtupleReader::SetWeightBranchAddress : " << name
		<< " does not exist in the list of input nominal weight branches. Please chack!!! " << std::endl;
    }
    if(varType == VariableDef::PTRVECFLOAT){
      stat = SetVariableToChain(branchName, &(m_ntupData->d_vecF_weight_components->at(branchName)) );
    }
    else{
      stat = SetVariableToChain(branchName, m_ntupData->d_vecF_weight_components->at(branchName) );
    }
  }
  else{
    std::cerr<<"NtupleReader::SetWeightBranchAddress --> ERROR : Variable type "<<VariableDef::GetVarTypeString(varType)<<" is not allowed for weight components."<<std::endl;
    stat = -1;
  }
  if(stat != 0){
    std::cerr<< "NtupleReader::SetWeightBranchAddresses() --> ERROR code " << stat << " encountered on branch "<<branchName<<". Please chack!!"<<std::endl;
    return stat;
  }
  
  return stat;

}

int NtupleReader::SetWeightBranchAddresses(){

  if(m_opt->MsgLevel() == Debug::DEBUG) std::cout << "Entering NtupleReader::SetWeightBranchAddresses()" << std::endl;
  int stat = 0;
  if(m_nomMap){
    for( std::pair<std::string, WeightObject*> nom : *m_nomMap ){ 
      if(!nom.second->IsInput()) continue;
      stat = SetWeightBranchAddress(nom.second);
      if(stat != 0 ) return stat;
    }
  }//if the nominal weights map exists

  //_____________________________________________________________________________
  if(m_opt->ComputeWeightSys() && m_sysMap){
    for( std::pair<std::string, WeightObject*> sys : *m_sysMap ){
      if(!sys.second->IsInput()) continue;
      stat = SetWeightBranchAddress(sys.second);
      if(stat != 0 ) return stat;
    }
  }//if the systematic weights map exists

  if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Leaving NtupleReader::SetWeightBranchAddresses()" << std::endl;

  return 0;
}

