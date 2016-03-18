#include "IFAETopFramework/WeightManager.h"
#include "IFAETopFramework/NtupleData.h"
#include "IFAETopFramework/OutputData.h"

#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/AnalysisUtils.h"

WeightManager::WeightManager(OptionsBase* opt, const NtupleData* ntupData, OutputData* outData) : 
  m_opt(opt),
  m_ntupData(ntupData),
  m_outData(outData),
  m_nomMap(NULL),
  m_systMap(NULL)
{
  m_nomMap = new WeightMap();
  m_systMap = new WeightMap();
}

WeightManager::WeightManager( const WeightManager& q){

  m_opt          = q.m_opt;
  m_ntupData     = q.m_ntupData;
  m_outData      = q.m_outData;
  m_nomMap       = q.m_nomMap;
  m_systMap      = q.m_systMap;

}

WeightManager::~WeightManager(){

  for(std::pair<std::string, WeightObject*> nom : *m_nomMap){ delete nom.second;  }
  m_nomMap -> clear();
  delete m_nomMap;

  for(std::pair<std::string, WeightObject*> syst : *m_systMap){ delete syst.second;  }
  m_systMap -> clear();
  delete m_systMap;

}

//_________________________________________________________________________________
bool WeightManager::AddNominal( const std::string &name, bool isInput, const std::string& branchName, const std::string& inputType ) {
  if(m_opt -> MsgLevel() == Debug::DEBUG){ std::cout << "WeightManager::AddNominal adding nominal weight (" 
						     << name << ")" << " isInput = " << isInput<<std::endl; }
  std::map< std::string , WeightObject*>::iterator it = m_nomMap -> find(name);
  if(it != m_nomMap->end()){
    std::cerr << "<!> ERROR in WeightManager::AddNominal:  nominal weight (" << name << ") exists already. Please chack !!" << std::endl;
    return false;
  }
  else {
    double* t = NULL;
    if(isInput) { 
      m_ntupData -> InsertNomComponent(name); 
      t = &(m_ntupData -> d_nominal_weight_components -> at(name));
    }
    else {
      m_outData -> InsertNomComponent(name); 
      t = &(m_outData -> o_nominal_weight_components -> at(name));
    }
    
    const std::string& _branch = (branchName == "") ? name : branchName;
    WeightObject *nom = new WeightObject(name, t, isInput, _branch, inputType);
    m_nomMap -> insert ( std::pair < std::string, WeightObject* > ( name, nom ) );
    
  }
  
  return true;
}

bool WeightManager::AddSystematic( const std::string &name, const std::string& affected_component
				   , bool isInput, const std::string& branchName, const std::string& inputType ) {
  
  if(m_opt -> MsgLevel() == Debug::DEBUG){ std::cout << "WeightManager::AddSystematic adding systematic weight (" 
						     << name << ")" << " isInput = " << isInput<<std::endl; }

  std::map< std::string , WeightObject*>::iterator it = m_systMap -> find(name);
  if(it != m_systMap->end()){
    std::cerr << "<!> ERROR in WeightManager::AddSystematic: systematic weight (" << name << ") exists already. Please chack !!" << std::endl;
    return false;
  }
  else {
    if(isInput) { m_ntupData -> InsertSystComponent(name); }
    else { m_outData -> InsertSystComponent(name); }
    
    m_outData -> InsertSystWeight(name);
    double *t = &(m_outData -> o_eventWeight_Systs -> at(name));

    const std::string& _branch = (branchName == "") ? name : branchName;
    WeightObject *sys = new WeightObject(name, t, isInput, _branch, inputType, affected_component);
    m_systMap -> insert ( std::pair < std::string, WeightObject* > ( name, sys ) );
    
  }
  return true;
}


//


//____________________________________________________________________________
bool WeightManager::AddWeightsFromString(const std::string& inputStr, bool isNominal){

  for ( size_t i = 0,n; i <= inputStr.length(); i=n+1){
    //Loop over the coma-separated list of weights
    n = inputStr.find_first_of(',',i);
    if (n == std::string::npos) {
      n = inputStr.length();
    }
    std::string tmp = inputStr.substr(i,n-i);
    //Introduce the weight systematics
    if(tmp!=""){
      if(isNominal){ AddNominal( tmp ); }
      else{ AddSystematic( tmp, tmp ); } // Assume that a systematic component should replace the same component from nominal map
    }
    tmp.clear();
  }
  
  return true;  
}

bool WeightManager::AddWeightsFromConfig(const std::string& inputStr, bool isNominal){

  if( m_opt -> MsgLevel() == Debug::DEBUG ){ std::cout << "Adding weights from configuration files " << inputStr << std::endl;}
  std::string name           = "";
  bool isInput               = true;
  std::string input_varType  = "D";
  std::string branchName     = "";
  std::string replace        = "";


  std::vector<std::map<std::string, std::string> > wgt_map;
  for ( size_t i = 0,n; i <= inputStr.length(); i=n+1){
    //Loop over the coma-separated list of weight configuration files
    n = inputStr.find_first_of(',',i);
    if (n == std::string::npos) {
      n = inputStr.length();
    }
    std::string tmp = inputStr.substr(i,n-i);
    if( m_opt -> MsgLevel() == Debug::DEBUG ){ std::cout << "Adding weights from configuration file " << tmp << std::endl;}
    //Add weights from configuration file
    wgt_map.clear();
    AnalysisUtils::ParseConfigFile_Blocks(tmp, wgt_map, " : ");
    if( m_opt -> MsgLevel() == Debug::DEBUG ){ std::cout << "Number of systematic blocks in configuration file " 
							 << inputStr << " = " << wgt_map.size() << std::endl;}
    
    for( std::map<std::string, std::string> wgt_block : wgt_map ){
      name            = "";
      isInput         = true;
      input_varType   = "D";
      branchName      = "";
      replace         = "";

      if( wgt_block.find("NAME") != wgt_block.end() ){ name = wgt_block["NAME"]; }
      else{ 
	std::cout<<"ERROR : No name given for systematic."<<std::endl;
	continue;
      }

      if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<"WeightManager::AddWeightFromConfig; isNominal = "<<isNominal<<"; Adding weight ("<<name<<std::endl; }
      if( !isNominal && ( wgt_block.find("REPLACE") != wgt_block.end() ) ){ replace = wgt_block["REPLACE"]; }
      else if(!isNominal){ replace = name; } // replace systematic component with same name from nominal map

      if( wgt_block.find("ISINPUT") != wgt_block.end() ){ isInput = AnalysisUtils::BoolValue(wgt_block["ISINPUT"], "ISINPUT"); }

      if( wgt_block.find("VARTYPE") != wgt_block.end() ){ input_varType = wgt_block["VARTYPE"]; }

      if( wgt_block.find("BRANCHNAME") != wgt_block.end() ){ branchName = wgt_block["BRANCHNAME"]; }
      else{ branchName = name; }

      if(isNominal){ AddNominal( name, isInput, branchName, input_varType ); }
      else{ AddSystematic( name, replace, isInput, branchName, input_varType ); }
      
    }

  }//Loop over all config files
  
  return true;
}

//____________________________________________________________________________
bool WeightManager::AddNominalsFromString(){

  bool stat = AddWeightsFromString(m_opt -> StrNominalName() + ",", true);
  return stat;  

}

bool WeightManager::AddNominalsFromConfig(){

  //bool stat = AddWeightsFromConfig(m_opt -> NominalConfigList() + ",", true);
  bool stat = AddWeightsFromConfig(m_opt -> NominalConfigList(), true);
  return stat;  

}

bool WeightManager::AddSystematicsFromString(){

  bool stat = AddWeightsFromString(m_opt -> StrSysName() + ",", false);
  return stat;  

}


bool WeightManager::AddSystematicsFromConfig(){

  //bool stat = AddWeightsFromConfig(m_opt -> SysConfigList() + ",", false);
  bool stat = AddWeightsFromConfig(m_opt -> SysConfigList(), false);
  return stat;  

}

//________________________________________________________________________________________________________
//
bool WeightManager::ComputeNominalWeights(){
  bool stat = true;
  m_outData -> o_eventWeight_Nom = 1.;

  for( std::pair<std::string, double> in_nom : *(m_ntupData->d_nominal_weight_components) ){
    if(m_opt -> MsgLevel() == Debug::DEBUG){ std::cout << " input nominal weight component ("<<in_nom.first<<") = "<<in_nom.second<<std::endl; }
    m_outData -> o_eventWeight_Nom *= in_nom.second;
  }
  for( std::pair<std::string, double> proc_nom : *(m_outData->o_nominal_weight_components) ){
    if(m_opt -> MsgLevel() == Debug::DEBUG){ std::cout << " proc nominal weight component ("<<proc_nom.first<<") = "<<proc_nom.second<<std::endl; }
    m_outData -> o_eventWeight_Nom *= proc_nom.second;
  }

  if(m_opt -> MsgLevel() == Debug::DEBUG){ std::cout << " Calculated nominal weight = "<< m_outData -> o_eventWeight_Nom <<std::endl; }

  return stat;

}

bool WeightManager::ComputeSystematicWeights(){
  bool stat = true;
  for(std::pair<std::string, WeightObject*> sys : *m_systMap){ 
    //Read in the systematic component
    if(sys.second->IsInput()){ m_outData->o_eventWeight_Systs->at(sys.first) = m_ntupData->d_syst_weight_components->at(sys.first); }
    else{ m_outData->o_eventWeight_Systs->at(sys.first) = m_outData->o_syst_weight_components->at(sys.first); }

    if( sys.second->AffectedComponent() == "ALL" ) continue; //no nominal component should be included
    WeightMap::iterator it = m_nomMap->find( sys.second->AffectedComponent() );
    if( it != m_nomMap->end()){ 
      double affected_nom = (it->second->IsInput()) ? m_ntupData->d_nominal_weight_components->at(it->first)
	: m_outData->o_nominal_weight_components->at(it->first);
      m_outData->o_eventWeight_Systs->at(sys.first) *= m_outData -> o_eventWeight_Nom / affected_nom;
    }

  }

  return stat;

}

bool WeightManager::ComputeAllWeights(){

  bool stat = ComputeNominalWeights();
  if( m_opt->ComputeWeightSys() ){ 
    bool stat_sys = ComputeSystematicWeights();
    stat = stat && stat_sys;
  }
  return (stat);

}
