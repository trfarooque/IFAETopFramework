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

  if(m_nomMap){
    for(std::pair<std::string, WeightObject*> nom : *m_nomMap){ delete nom.second;  }
    m_nomMap -> clear();
    delete m_nomMap;
  }
  if(m_systMap){
    for(std::pair<std::string, WeightObject*> syst : *m_systMap){ delete syst.second;  }
    m_systMap -> clear();
    delete m_systMap;
  }


}

bool WeightManager::AddAllWeights(){

  bool stat = true;
  if(m_opt->StrNominalName() != ""){
    stat = stat && AddWeightsFromString(m_opt -> StrNominalName(), true);
  }
  if(m_opt->ComputeWeightSys() && (m_opt->StrSysName() != "") ){
    stat = stat && AddWeightsFromString(m_opt -> StrSysName(), false);
  }
  if(m_opt->WeightsConfigList() != ""){
    stat = stat && AddWeightsFromConfig(m_opt -> WeightsConfigList());
  }

  //================ Quick loop to check that the affected nominal component for each systematic truly exists ========
  if(m_systMap){
    for(std::pair<std::string, WeightObject*> sys : *m_systMap){ 
      const std::string& replace = sys.second->AffectedComponent();
      if( (replace == "ALL") || (replace == "NONE") ) continue;
      if( m_nomMap->find( replace ) == m_nomMap->end()){ 
	std::cout<< "WeightManager::ComputeSystematicWeight() --> ERROR : affected nominal component "<< replace << " does not exist "<<std::endl;
	stat = false; break;
      }
    }
  }

  return stat;
}

bool WeightManager::ComputeAllWeights(){

  bool stat = ComputeNominalWeight();
  if( m_opt->ComputeWeightSys() ){ 
    bool stat_sys = ComputeSystematicWeights();
    stat = stat && stat_sys;
  }
  return stat;

}

//________________________________________________________________________________________________________
//
bool WeightManager::ComputeNominalWeight(){
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
  if(m_systMap == NULL){ 
    std::cout<<" WARNING : WeightManager::ComputeSystematicsWeights was called with empty systematics map "<<std::endl;
    return false;
  }
  for(std::pair<std::string, WeightObject*> sys : *m_systMap){ 
    stat = ComputeSystematicWeight(sys.second);
  }

  return stat;

}


//--------------------------------------------------------------------------------
bool WeightManager::ComputeSystematicWeight(const std::string& name){
  
  WeightMap::iterator it = m_systMap -> find(name);
  if(it == m_systMap->end()){
    std::cout << "WeightManager::ComputeSystematicWeight() --> ERROR : Weight "<<name<<" not found in map"<<std::endl;
    return false;
  }
  bool stat = ComputeSystematicWeight(it->second); 
  
  return stat;
  
}

bool WeightManager::SetNominalComponent(const std::string& name, double value ){
  bool stat = SetWeightComponent(name, value, true); 
  return stat;
}

bool WeightManager::SetSystematicComponent(const std::string& name, double value ){
  bool stat = SetWeightComponent(name, value, false); 
  return stat;
}



//==========================================================================================
bool WeightManager::UpdateNominalComponent(const std::string& name, double value){

  //------------------------------------------- Update the nominal weight --------------------------
  std::map<std::string, double>* weightValMap = m_outData -> o_nominal_weight_components;
  std::map<std::string, double>::iterator it = weightValMap -> find(name);
  if(it == weightValMap -> end()){ 
    std::cout << "WeightManager::UpdateNominalComponent() --> ERROR: Weight component " << name << " not found in map in OutputData " << std::endl;
    return false;
  }

  double old_value = it->second;
  it->second = value;

  double old_nomWeight = m_outData->o_eventWeight_Nom;
  //(The weights have pointers to the map value, so this should actually be fairly easy). 
  if((old_value > 0.) || (old_value < 0.)){
    double new_nomWeight = (old_nomWeight/old_value)*value;
    m_outData->o_eventWeight_Nom = new_nomWeight;
  }
  else{ ComputeNominalWeight(); }
  //------------------------------------------------------------------------------------------------

  if(m_systMap){
    //------------------------------------------- Update all affected systematic weights ------------------
    for(std::pair<std::string, WeightObject*> sys : *m_systMap){
      if( sys.second -> AffectedComponent() == name ){ continue; }

      double old_sysWeight = m_outData->o_eventWeight_Systs->at(name);
      if((old_value > 0.) || (old_value < 0.)){
	double new_sysWeight = (old_sysWeight/old_value)*value;
	m_outData->o_eventWeight_Systs->at(name) = new_sysWeight;
      }
      else{ ComputeSystematicWeight(name); }

    }
  }
 
  return true;
}
 
bool WeightManager::UpdateSystematicComponent(const std::string& name, double value){
  
  std::map<std::string, double>* weightValMap = m_outData -> o_syst_weight_components;
  
  std::map<std::string, double>::iterator it = weightValMap -> find(name);
  if(it == weightValMap -> end()){ 
    std::cout << "WeightManager::UpdateSystematicComponent() --> ERROR: Weight component " << name << " not found in map in OutputData " << std::endl;
    return false;
  }
  
  double old_value = it->second;
  it->second = value;
  
  double old_sysWeight = m_outData->o_eventWeight_Systs->at(name);
  //(The weights have pointers to the map value, so this should actually be fairly easy). 
  if((old_value > 0.) || (old_value < 0.)){
    double new_sysWeight = (old_sysWeight/old_value)*value;
    m_outData->o_eventWeight_Systs->at(name) = new_sysWeight;
  }
  else{ ComputeSystematicWeight(name); }
  
  return true;
}

//==================================== PROTECTED METHODS =================================================
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
    WeightObject *sys = new WeightObject(name, t, isInput, _branch, inputType, false, affected_component);
    m_systMap -> insert ( std::pair < std::string, WeightObject* > ( name, sys ) );
    
  }
  return true;
}


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

bool WeightManager::AddWeightsFromConfig(const std::string& inputStr ){
  
  if( m_opt -> MsgLevel() == Debug::DEBUG ){ std::cout << "Adding weights from configuration files " << inputStr << std::endl;}
  std::string name           = "";
  bool isInput               = true;
  bool isNominal             = true;
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
      isNominal       = true;
      input_varType   = "D";
      branchName      = "";
      replace         = "";

      if( wgt_block.find("ISNOMINAL") != wgt_block.end() ){ isNominal = AnalysisUtils::BoolValue(wgt_block["ISNOMINAL"], "ISNOMINAL"); }
      if( !( m_opt->ComputeWeightSys() || isNominal) ){ continue; } //do not add systematic weights if ComputeWeightSys()==FALSE

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

bool WeightManager::ComputeSystematicWeight(WeightObject* sys){
  
  const std::string& name = sys->Name();
  if( sys->IsInput() ){ m_outData->o_eventWeight_Systs->at(name) = m_ntupData->d_syst_weight_components->at(name); }
  else{ m_outData->o_eventWeight_Systs->at(name) = m_outData->o_syst_weight_components->at(name); }
  
  const std::string& replace = sys->AffectedComponent();
  if( replace == "ALL" ) return true; //no nominal component should be included
  else if(replace == "NONE"){
    m_outData->o_eventWeight_Systs->at(name) *= m_outData -> o_eventWeight_Nom;
  }
  else{
    WeightMap::iterator nom_it = m_nomMap->find( replace );
    if( (nom_it == m_nomMap->end()) ){ 
      std::cout<< "WeightManager::ComputeSystematicWeight() --> ERROR : affected nominal component "<< replace << " does not exist "<<std::endl;
    }
    else{
      double affected_nom = ( (nom_it->second)->IsInput() ) ? m_ntupData->d_nominal_weight_components->at( replace )
	: m_outData->o_nominal_weight_components->at( replace );
      m_outData->o_eventWeight_Systs->at(name) *= m_outData -> o_eventWeight_Nom / affected_nom;
    }
  }

  return true;
  
}

bool WeightManager::SetWeightComponent(const std::string& name, double value, bool nominal ){

  std::map<std::string, double>* weightValMap = NULL;
  if(nominal){ weightValMap = m_outData -> o_nominal_weight_components; }
  else{ weightValMap = m_outData -> o_syst_weight_components; }

  std::map<std::string, double>::iterator it = weightValMap -> find(name);
  if(it == weightValMap -> end()){ 
    std::cout << "WeightManager::SetWeightComponent() --> ERROR: Weight component " << name << " not found in map in OutputData " << std::endl;
    return false;
  }

  it->second = value;

  return true;
}

