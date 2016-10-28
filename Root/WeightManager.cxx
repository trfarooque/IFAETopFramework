#include "IFAETopFramework/WeightManager.h"
#include "IFAETopFramework/NtupleData.h"
#include "IFAETopFramework/OutputData.h"

#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/AnalysisUtils.h"

WeightManager::WeightManager(const OptionsBase* opt, const NtupleData* ntupData, OutputData* outData) : 
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

void WeightManager::PrintWeight(const std::string& name, bool isNominal) const{
  PrintWeight( GetWeightObject(name, isNominal) );
  return;
}

void WeightManager::PrintWeight(WeightObject* wgtObj) const{

  std::cout<<" Weight "<<wgtObj->Name()<<" isNominal: "<<wgtObj->IsNominal()
	   <<" component value: "<<wgtObj->GetComponentValue()
	   <<" total value: "<<wgtObj->GetWeightValue() << std::endl;
  std::cout<<" nominal weight: "<<m_outData->o_eventWeight_Nom<<std::endl;

  std::cout<<" Weight address: "<<wgtObj->GetWeightAddress()<<" nomAddress = "<<&(m_outData->o_eventWeight_Nom)<<std::endl;
}


void WeightManager::Print(const bool fullInfo) const{


  std::cout<<" Printing nominal weights :: m_nomMap = "<<m_nomMap<<std::endl;
  if(m_nomMap){
    std::cout << "=====================================================" << std::endl;
    std::cout << "Nominal weights list" << std::endl;
    std::cout << "=====================================================" << std::endl;
    std::cout << std::endl;
    for ( const std::pair < std::string, WeightObject* > nom : *m_nomMap ){
      if(fullInfo) PrintWeight(nom.second);
      else std::cout<<nom.first<<std::endl;
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
      if(fullInfo) PrintWeight(sys.second);
      else std::cout<<sys.first<<std::endl;
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
    if(nom.second->IsReserve()) continue;
    m_outData -> o_eventWeight_Nom *= nom.second -> GetComponentValue();

  }

  return stat;

}

bool WeightManager::ComputeSystematicWeights(){
  bool stat = true;
  if(m_systMap == NULL){ 
    std::cout<<" WARNING : WeightManager::ComputeSystematicsWeights was called with empty systematics map "<<std::endl;
    return false;
  }
  for(std::pair<std::string, WeightObject*> sys : *m_systMap){ 
    if(sys.second->IsReserve()) continue;
    stat = ComputeSystematicWeight(sys.second);
  }

  return stat;

}


//--------------------------------------------------------------------------------
bool WeightManager::ComputeSystematicWeight(const std::string& name){
  std::cout<<" In WeightManager::ComputeSystematicWeight"<<std::endl; 
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
    wgt_nom->SetWeightValue(new_nomWeight);
    //m_outData->o_eventWeight_Nom = new_nomWeight;
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
WeightObject* WeightManager::AddAndInitWeight( const std::string &name, const std::string &title, bool isNominal, 
			       bool isInput, const std::string& branchName, 
			       const std::string& affected_component, 
			       const std::string& componentType, int vec_ind ) {

  WeightObject* wgt = AddWeight(name, title, isNominal, isInput, branchName, affected_component);
  if(wgt){ InitWeight(wgt, componentType, vec_ind); }
  return wgt;

}

WeightObject* WeightManager::AddWeight( const std::string &name, const std::string &title, bool isNominal, 
			       bool isInput, const std::string& branchName, 
			       const std::string& affected_component) {

  if(m_opt -> MsgLevel() == Debug::DEBUG){ std::cout << "WeightManager::AddWeight adding weight ("
 						     << name << ")" << " isInput = " << isInput << " isNominal = " << isNominal <<std::endl; }
  WeightMap* wgtMap = (isNominal) ? m_nomMap : m_systMap;
  WeightMap::iterator it = wgtMap -> find(name);
  if(it != wgtMap->end()){
    std::cerr << "<!> ERROR in WeightManager::AddWeight: weight (" << name << ") exists already. isNominal = "<<isNominal<<". Please chack !!" << std::endl;
    return NULL;
  }

  const std::string& _branch = (branchName == "") ? name : branchName;
  WeightObject *wgt = new WeightObject(name,title, isInput, _branch, isNominal, affected_component);

  wgtMap->insert( std::pair<std::string, WeightObject*>(name, wgt) );

  return wgt;

}

bool WeightManager::InitWeight( WeightObject* wgt, const std::string& componentType, int vec_ind ) {

  if(m_opt -> MsgLevel() == Debug::DEBUG){ std::cout << "WeightManager::InitWeight initialising weight ("
 						     << wgt->Name() << ")" << " componentType = " << componentType << " vec_ind = " << vec_ind <<std::endl; }

  bool isInput = wgt->IsInput();
  const std::string& _branch = wgt->BranchName();
  const std::string& name = wgt->Name();

  if(componentType == "D"){
    std::map<std::string, double>* &valMap = (isInput) ? m_ntupData -> d_D_weight_components : m_outData -> o_D_weight_components;
    if(valMap == NULL){ valMap = new std::map<std::string, double>; }
    if( valMap->find(_branch) != valMap->end() ){
      std::cerr << "<!> ERROR in WeightManager::InitWeight: weight (" << _branch << ") exists already. Please chack !!" << std::endl;
      return false;
    }
    else{ valMap->insert(std::pair<std::string, double>( _branch, 0.) ); }
    wgt->SetVarComponent(&(valMap -> at(_branch)) );
  }

  else if(componentType == "F"){
    std::map<std::string, float>* &valMap = (isInput) ? m_ntupData -> d_F_weight_components : m_outData -> o_F_weight_components;
    if(valMap == NULL){ valMap = new std::map<std::string, float>; }
    if( valMap->find(_branch) != valMap->end() ){
      std::cerr << "<!> ERROR in WeightManager::InitWeight: weight (" << _branch << ") exists already. Please chack !!" << std::endl;
      return false;
    }
    else{ valMap->insert(std::pair<std::string, float>( _branch, 0.) ); }
    wgt->SetVarComponent(&(valMap -> at(_branch)) );
  }

  else if( (componentType == "PVD") || (componentType == "VD") ){
    if(vec_ind < 0){
      std::cerr<<" <!> ERROR in WeightManager::InitWeight: weight (" << name << ") : Negative vector index is not permitted "<<std::endl;
      return false;
    }
    std::map<std::string, std::vector<double>* >* &valMap = (isInput) ? m_ntupData -> d_vecD_weight_components : m_outData -> o_vecD_weight_components;
    if(valMap == NULL){ valMap = new std::map<std::string, std::vector<double>* >; }
    if( valMap->find(_branch) == valMap->end() ){
      valMap->insert(std::pair<std::string, std::vector<double>* >( _branch, NULL) );
    }
    if( componentType == "PVD" ){ wgt->SetVarComponent( &(valMap -> at(_branch)), vec_ind ); }
    else{ wgt->SetVarComponent(valMap -> at(_branch), vec_ind ); }

  }

  else if( (componentType == "PVF") || (componentType == "VF") ){
    if(vec_ind < 0){
      std::cerr<<" <!> ERROR in WeightManager::InitWeight: weight (" << name << ") : Negative vector index is not permitted "<<std::endl;
      return false;
    }
    std::map<std::string, std::vector<float>* >* &valMap = (isInput) ? m_ntupData -> d_vecF_weight_components : m_outData -> o_vecF_weight_components;
    if(valMap == NULL){ valMap = new std::map<std::string, std::vector<float>* >; }
    if( valMap->find(_branch) == valMap->end() ){
      valMap->insert(std::pair<std::string, std::vector<float>* >( _branch, NULL) );
    }
    if(componentType == "PVF"){ wgt->SetVarComponent(&(valMap -> at(_branch)), vec_ind ); }
    else{ wgt->SetVarComponent(valMap -> at(_branch), vec_ind ); }
  }
  else{
    std::cerr <<" <!> ERROR in WeightManager::InitWeight() -> VariableType " << componentType << " is not recognised. "<<std::endl;
    return false;
  }

  if(wgt->IsNominal()){ 
    wgt->SetVarWeight( &(m_outData -> o_eventWeight_Nom) );
  }
  else{ 
    if( m_outData -> o_eventWeight_Systs->find(name) != m_outData -> o_eventWeight_Systs->end() ){
      std::cerr << "<!> ERROR in WeightManager::InitWeight: weight (" << name << ") exists already. Please chack !!" << std::endl;
      return false;
    }
    m_outData -> o_eventWeight_Systs -> insert( std::pair<std::string, double>(name, 0.) );
    wgt->SetVarWeight( &(m_outData -> o_eventWeight_Systs -> at(name)) );
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
    if(tmp!=""){ AddAndInitWeight(tmp, "", isNominal); }
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
  std::string str_in_vecInd  = "";
  std::string branchName     = "";
  std::string replace        = "";
  bool multi_ind             = false; 

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
      str_in_vecInd   = "";
      branchName      = "";
      replace         = "";
      multi_ind       = false;
 
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

      if( wgt_block.find("BRANCHNAME") != wgt_block.end() ){ branchName = wgt_block["BRANCHNAME"]; }
      else{ branchName = name; }

      if( wgt_block.find("VECIND") != wgt_block.end() ){ str_in_vecInd = wgt_block["VECIND"]; }
      if(str_in_vecInd.empty() || str_in_vecInd == "-1"){
	input_vecInd = -1; 
      }
      else if( (str_in_vecInd.find("-") != std::string::npos) || (str_in_vecInd.find(",") != std::string::npos) ){
	multi_ind = true;
      }//multiple vector indices
      else{ input_vecInd = atoi(str_in_vecInd.c_str()); }

      if(multi_ind){
	std::vector<std::string> rangeList; rangeList.clear();

	std::string range = "";
	std::string rangeString = str_in_vecInd;
	std::string::size_type pos = 0;
	bool clean = true;

	std::string str_firstInd = "";
	std::string str_lastInd = "";

	do{ 
	  pos = AnalysisUtils::ParseString(rangeString, range, ",");
	  AnalysisUtils::TrimString(range);
	  //Check formatting of range
	  if( AnalysisUtils::CountSubstring(range, "-") == 0 ){
	    AddAndInitWeight( name, title, isNominal, isInput, branchName, replace, input_varType, atoi(range.c_str()) );
	  }
	  else if( AnalysisUtils::CountSubstring(range, "-") > 1 ){
	    std::cerr << " Vector index range "<<range<<" does not follow expected format A-B. Please check";
	    clean = false;
	    break;
	  }
	  else{
	    str_firstInd = "";
	    str_lastInd = range;
	    AnalysisUtils::ParseString(str_lastInd, str_firstInd, "-");
	    int firstInd = atoi(str_firstInd.c_str());
	    int lastInd = atoi(str_lastInd.c_str());
	    if(firstInd>lastInd){
	      std::cerr << " Vector index range in reverse order. First index =  "<<firstInd<<", last index = "<<lastInd<<". Please check";
	      clean = false;
	      break;
	    }

	    for(int _ind = firstInd; _ind<=lastInd; ++_ind){
	      AddAndInitWeight( Form("%s_%i",name.c_str(),_ind), title, isNominal, isInput, branchName, replace, input_varType, _ind);
	    }//loop over the range

	  }//proper vector range

	}while(pos != std::string::npos);

      }//parse the multiple vector indices and add a weight for each one
      else{
	AddAndInitWeight( name, title, isNominal, isInput, branchName, replace, input_varType, input_vecInd);
      }//single vector index
      
    }//Loop over blocks in config file

  }//Loop over all config files
  
  return true;
}

bool WeightManager::ComputeSystematicWeight(WeightObject* sys){
  
  //First include the systematic weight component
  double sys_wgt = sys->GetComponentValue();

  //Now include the nominal part
  const std::string& replace = sys->AffectedComponent();
  if( replace == "ALL" ){   //no nominal component should be included
    ;
  }
  else if( (replace == "") || (replace == "NONE") ){ //nothing should be excluded
    sys_wgt *= m_outData -> o_eventWeight_Nom;
  }
  else{
    WeightMap::iterator nom_it = m_nomMap->find( replace );
    if( (nom_it == m_nomMap->end()) ){ 
      std::cerr<< "WeightManager::ComputeSystematicWeight() --> ERROR :  nominal component to be replaced ( "<< replace << " ) does not exist "<<std::endl;
      return false;
    }
    else{
      double nomWeight = m_outData -> o_eventWeight_Nom;
      if(nomWeight>0. || nomWeight<0.){
	sys_wgt *= nomWeight / (nom_it->second)->GetComponentValue();
      }
      else{
	//Read all input components and update the value held by weight
	for(std::pair<std::string, WeightObject*> nom : *m_nomMap){ 
	  if(nom.first == replace) continue;
	  sys_wgt *= nom.second -> GetComponentValue();
	}
      }//nominal weight is zero

    }//found the nominal component to replace

  }//Replace one nominal component

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
