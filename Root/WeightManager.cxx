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
  m_systMap(NULL),
  m_config_flags(""),
  m_config_vetoFlags("")
{
  m_nomMap = new WeightMap();
  m_systMap = new WeightMap();
}

WeightManager::WeightManager( const WeightManager& q){

  m_opt                = q.m_opt;
  m_ntupData           = q.m_ntupData;
  m_outData            = q.m_outData;
  m_nomMap             = q.m_nomMap;
  m_systMap            = q.m_systMap;
  m_config_flags       = m_config_flags;
  m_config_vetoFlags   = m_config_vetoFlags;
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

WeightObject* WeightManager::GetWeightObject(const std::string& name, const bool isNominal) const{

  WeightMap* wgtMap = isNominal ? m_nomMap : m_systMap;
  WeightObject* wgtObj = NULL;
  if(wgtMap){
    WeightMap::iterator it = wgtMap -> find(name);
    if(it != wgtMap->end()){ wgtObj = it->second; }
    else{ std::cerr << "WeightManager::GetWeightObject() --> ERROR : Weight " << name 
		    << " not found in map; isNominal = " << isNominal <<std::endl;
    }
  }
  else{
    std::cerr << "WeightManager::GetWeightObject() --> ERROR : Weight " << name 
		    << " not found in map; isNominal = " << isNominal <<std::endl;
  }

  return wgtObj;

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
      if( (replace == "") || (replace == "ALL") || (replace == "NONE") ) continue;
      if( m_nomMap->find( replace ) == m_nomMap->end()){ 
	std::cerr<< "WeightManager::ComputeSystematicWeight() --> ERROR : affected nominal component "<< replace << " does not exist "<<std::endl;
	stat = false; break;
      }
    }
  }

  if(m_opt->MsgLevel() == Debug::DEBUG){
    Print();
  }

  return stat;
}

void WeightManager::Print() const{


  std::cout<<" Printing nominal weights :: m_nomMap = "<<m_nomMap<<std::endl;
  if(m_nomMap){
    std::cout << "=====================================================" << std::endl;
    std::cout << "Nominal weights list" << std::endl;
    std::cout << "=====================================================" << std::endl;
    std::cout << std::endl;
    for ( const std::pair < std::string, WeightObject* > nom : *m_nomMap ){
      std::cout << nom.first << std::endl;
    }
    std::cout << std::endl;
    std::cout << "=====================================================" << std::endl;
  }


  std::cout<<" Printing systematic weights :: m_systMap = "<<m_systMap<<std::endl;
  if(m_systMap){
    std::cout << "=====================================================" << std::endl;
    std::cout << "Systematics list" << std::endl;
    std::cout << "=====================================================" << std::endl;
    std::cout << std::endl;
    for ( const std::pair < std::string, WeightObject* > sys : *m_systMap ){
      std::cout << sys.first << std::endl;
    }
    std::cout << std::endl;
    std::cout << "=====================================================" << std::endl;
  }
  return;
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
  if( m_opt -> IsData() ){ return stat; }
  if(m_nomMap == NULL){ 
    std::cout<<" WARNING : WeightManager::ComputeNominalWeights was called with empty nominal map "<<std::endl;
    return false;
  }

  //Read all input components and update the value held by weight
  for(std::pair<std::string, WeightObject*> nom : *m_nomMap){ 
    if(m_opt -> MsgLevel() == Debug::DEBUG){ std::cout << " nominal weight component ("<<nom.first<<") = "<<nom.second->GetComponentValue()<<std::endl; }
    m_outData -> o_eventWeight_Nom *= nom.second -> GetComponentValue();
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
    std::cerr << "WeightManager::ComputeSystematicWeight() --> ERROR : Weight "<<name<<" not found in map"<<std::endl;
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
  WeightMap::iterator it = m_nomMap -> find(name);
  if(it == m_nomMap -> end()){ 
    std::cerr << "WeightManager::UpdateNominalComponent() --> ERROR: Weight component " << name << " not found in nominal weight map" << std::endl;
    return false;
  }
  WeightObject* wgt_nom = (it->second);
  double old_value = wgt_nom->GetComponentValue();

  wgt_nom->SetComponentValue(value);

  double old_nomWeight = m_outData->o_eventWeight_Nom;
  //(The weights have pointers to the map value, so this should actually be fairly easy). 
  if((old_value > 0.) || (old_value < 0.)){
    double new_nomWeight = (old_nomWeight/old_value)*value;
    m_outData->o_eventWeight_Nom = new_nomWeight;
  }
  else{ ComputeNominalWeight(); }

  if(m_systMap){
    //------------------------------------------- Update all affected systematic weights ------------------
    for(std::pair<std::string, WeightObject*> sys : *m_systMap){
      if( sys.second -> AffectedComponent() == name ){ continue; }

      double old_sysWeight = sys.second->GetWeightValue();
      if((old_value > 0.) || (old_value < 0.)){
	double new_sysWeight = (old_sysWeight/old_value)*value;
	sys.second->SetWeightValue(new_sysWeight);
      }
      else{ ComputeSystematicWeight(name); }

    }
  }
 
  return true;
}
 
bool WeightManager::UpdateSystematicComponent(const std::string& name, double value){
  
  WeightMap::iterator it = m_systMap -> find(name);
  if(it == m_systMap -> end()){ 
    std::cerr << "WeightManager::UpdateSystematicComponent() --> ERROR: Weight component " << name << " not found in systematic map " << std::endl;
    return false;
  }
  WeightObject* wgt = it->second;
  double old_value = wgt->GetComponentValue();
  wgt->SetComponentValue( value );

  double old_sysWeight = wgt->GetWeightValue();
  if((old_value > 0.) || (old_value < 0.)){
    double new_sysWeight = (old_sysWeight/old_value)*value;
    wgt->SetWeightValue( new_sysWeight );
  }
  else{ ComputeSystematicWeight(name); }

  return true;
}

//==================================== PROTECTED METHODS =================================================
bool WeightManager::AddWeight( const std::string &name, const std::string &title, bool isNominal, 
			       bool isInput, const std::string& branchName, 
			       const std::string& affected_component, 
			       const std::string& inputType, int vec_ind ) {

  if(m_opt -> MsgLevel() == Debug::DEBUG){ std::cout << "WeightManager::AddWeight adding weight ("
 						     << name << ")" << " isInput = " << isInput << " isNominal = " << isNominal <<std::endl; }
  WeightMap* wgtMap = (isNominal) ? m_nomMap : m_systMap;
  WeightMap::iterator it = wgtMap -> find(name);
  if(it != wgtMap->end()){
    std::cerr << "<!> ERROR in WeightManager::AddWeight: weight (" << name << ") exists already. isNominal = "<<isNominal<<". Please chack !!" << std::endl;
    return false;
  }
  else{ 
    const std::string& _branch = (branchName == "") ? name : branchName;
    WeightObject *wgt = new WeightObject(name,title, isInput, _branch, isNominal, affected_component);

    if(inputType == "D"){
      std::map<std::string, double>* &valMap = (isInput) ? m_ntupData -> d_D_weight_components : m_outData -> o_D_weight_components;
      if(valMap == NULL){ valMap = new std::map<std::string, double>; }
      if( valMap->find(_branch) != valMap->end() ){
	std::cerr << "<!> ERROR in WeightManager::AddWeight: weight (" << name << ") exists already. Please chack !!" << std::endl;
	return false;
      }
      else{ valMap->insert(std::pair<std::string, double>( _branch, 0.) ); }
      wgt->SetVarComponent(inputType, &(valMap -> at(_branch)) );
    }

    else if(inputType == "F"){
      std::map<std::string, float>* &valMap = (isInput) ? m_ntupData -> d_F_weight_components : m_outData -> o_F_weight_components;
      if(valMap == NULL){ valMap = new std::map<std::string, float>; }
      if( valMap->find(_branch) != valMap->end() ){
	std::cerr << "<!> ERROR in WeightManager::AddWeight: weight (" << name << ") exists already. Please chack !!" << std::endl;
	return false;
      }
      else{ valMap->insert(std::pair<std::string, float>( _branch, 0.) ); }
      wgt->SetVarComponent(inputType, &(valMap -> at(_branch)) );
    }

    else if( (inputType == "PVD") || (inputType == "VD") ){
      if(vec_ind < 0){
	std::cerr<<" <!> ERROR in WeightManager::AddWeight: weight (" << name << ") : Negative vector index is not permitted "<<std::endl;
	return false;
      }
      std::map<std::string, std::vector<double>* >* &valMap = (isInput) ? m_ntupData -> d_vecD_weight_components : m_outData -> o_vecD_weight_components;
      if(valMap == NULL){ valMap = new std::map<std::string, std::vector<double>* >; }
      if( valMap->find(_branch) == valMap->end() ){
	valMap->insert(std::pair<std::string, std::vector<double>* >( _branch, NULL) );
      }
      if( inputType == "PVD" ){ wgt->SetVarComponent(inputType, &(valMap -> at(_branch)), vec_ind ); }
      else{ wgt->SetVarComponent(inputType, valMap -> at(_branch), vec_ind ); }

    }

    else if( (inputType == "PVF") || (inputType == "VF") ){
      if(vec_ind < 0){
	std::cerr<<" <!> ERROR in WeightManager::AddWeight: weight (" << name << ") : Negative vector index is not permitted "<<std::endl;
	return false;
      }
      std::map<std::string, std::vector<float>* >* &valMap = (isInput) ? m_ntupData -> d_vecF_weight_components : m_outData -> o_vecF_weight_components;
      if(valMap == NULL){ valMap = new std::map<std::string, std::vector<float>* >; }
      if( valMap->find(_branch) == valMap->end() ){
	valMap->insert(std::pair<std::string, std::vector<float>* >( _branch, NULL) );
      }
      if(inputType == "PVF"){ wgt->SetVarComponent(inputType, &(valMap -> at(_branch)), vec_ind ); }
      else{ wgt->SetVarComponent(inputType, valMap -> at(_branch), vec_ind ); }
    }
    else{
      std::cerr <<" <!> ERROR in WeightManager::AddWeight() -> VariableType " << inputType << " is not recognised. "<<std::endl;
      return false;
    }

    if(!isNominal){ 
      if( m_outData -> o_eventWeight_Systs->find(name) != m_outData -> o_eventWeight_Systs->end() ){
	std::cerr << "<!> ERROR in WeightManager::AddWeight: weight (" << name << ") exists already. Please chack !!" << std::endl;
	return false;
      }
      m_outData -> o_eventWeight_Systs -> insert( std::pair<std::string, double>(name, 0.) );
      wgt->SetVarWeight( &(m_outData -> o_eventWeight_Systs -> at(name)) );
    }

    wgtMap->insert( std::pair<std::string, WeightObject*>(name, wgt) );
 
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
    if(tmp!=""){ AddWeight(tmp, "", isNominal); }
    tmp.clear();
  }
  
  return true;  
}

bool WeightManager::AddWeightsFromConfig(const std::string& inputStr ){

  if( m_opt -> MsgLevel() == Debug::DEBUG ){ std::cout << "Adding weights from configuration files " << inputStr << std::endl;}
  std::string name           = "";
  std::string title          = "";
  bool isInput               = true;
  bool isNominal             = true;
  std::string input_varType  = "D";
  int input_vecInd           = -1;
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
    AnalysisUtils::ParseConfigFile_Blocks( tmp, wgt_map, " : ", m_config_flags, m_config_vetoFlags );
    if( m_opt -> MsgLevel() == Debug::DEBUG ){ std::cout << "Number of systematic blocks in configuration file " 
							 << inputStr << " = " << wgt_map.size() << std::endl; }
    
    for( std::map<std::string, std::string> wgt_block : wgt_map ){
      name            = "";
      title           = "";
      isInput         = true;
      isNominal       = true;
      input_varType   = "D";
      input_vecInd    = -1;
      branchName      = "";
      replace         = "";

      if( wgt_block.find("ISNOMINAL") != wgt_block.end() ){ isNominal = AnalysisUtils::BoolValue(wgt_block["ISNOMINAL"], "ISNOMINAL"); }
      if( !( m_opt->ComputeWeightSys() || isNominal) ){ continue; } //do not add systematic weights if ComputeWeightSys()==FALSE

      if( wgt_block.find("NAME") != wgt_block.end() ){ name = wgt_block["NAME"]; }
      else{ 
	std::cerr<<"ERROR : No name given for systematic."<<std::endl;
	continue;
      }

      if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<"WeightManager::AddWeightFromConfig; isNominal = "<<isNominal<<"; Adding weight ("<<name<<std::endl; }
      if( !isNominal && ( wgt_block.find("TITLE") != wgt_block.end() ) ){ title = wgt_block["TITLE"]; }
      if( !isNominal && ( wgt_block.find("REPLACE") != wgt_block.end() ) ){ replace = wgt_block["REPLACE"]; }

      if( wgt_block.find("ISINPUT") != wgt_block.end() ){ isInput = AnalysisUtils::BoolValue(wgt_block["ISINPUT"], "ISINPUT"); }

      if( wgt_block.find("VARTYPE") != wgt_block.end() ){ input_varType = wgt_block["VARTYPE"]; }

      if( wgt_block.find("VECIND") != wgt_block.end() ){ input_vecInd = atoi(wgt_block["VECIND"].c_str()); }

      if( wgt_block.find("BRANCHNAME") != wgt_block.end() ){ branchName = wgt_block["BRANCHNAME"]; }
      else{ branchName = name; }

      AddWeight( name, title, isNominal, isInput, branchName, replace, input_varType, input_vecInd);
      
    }//Loop over blocks in config file

  }//Loop over all config files
  
  return true;
}

bool WeightManager::ComputeSystematicWeight(WeightObject* sys){
  
  double sys_wgt = sys->GetComponentValue();

  const std::string& replace = sys->AffectedComponent();
  if( replace == "ALL" ){   //no nominal component should be included
    ;
  }
  else if( (replace == "") || (replace == "NONE") ){
    sys_wgt *= m_outData -> o_eventWeight_Nom;
  }
  else{
    WeightMap::iterator nom_it = m_nomMap->find( replace );
    if( (nom_it == m_nomMap->end()) ){ 
      std::cerr<< "WeightManager::ComputeSystematicWeight() --> ERROR :  nominal component to be replaced ( "<< replace << " ) does not exist "<<std::endl;
      return false;
    }
    else{
      double replaced_nom = (nom_it->second)->GetComponentValue();
      sys_wgt *= m_outData -> o_eventWeight_Nom / replaced_nom;
    }
  }
  sys->SetWeightValue(sys_wgt);
  return true; 

  
}

bool WeightManager::SetWeightComponent(const std::string& name, double value, bool nominal ){

  WeightMap* wgtMap = (nominal) ? m_nomMap : m_systMap;
  WeightMap::iterator it = wgtMap->find(name);
  if(it == wgtMap -> end()){ 
    std::cerr << "WeightManager::SetWeightComponent() --> ERROR: Weight component " << name << " not found in map " << std::endl;
    return false;
  }
  bool stat = (it->second)->SetComponentValue(value);

  return stat;

}


void WeightManager::SetConfigBlock(const std::string& flag, const bool value){

  if(value){ m_config_flags += "__"+flag+"__"; }
  else     { m_config_vetoFlags += "__"+flag+"__"; }

  return;
}
