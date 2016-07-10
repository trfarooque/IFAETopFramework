#include "IFAETopFramework/WeightObject.h"
//_____________________________________________________________________________________
//
WeightObject::WeightObject():
  m_name(""),
  m_affected_component(""),
  m_branch_name(""),
  m_is_input(true),
  m_is_nominal(true),
  m_var_component(NULL),
  m_var_weight(NULL)
  //m_input_varType(DOUBLE),
  //m_input_varTypeString("D") 
{ }


//_____________________________________________________________________________________
WeightObject::WeightObject(const std::string& name, const std::string& title, bool is_input, const std::string& branch_name
			   , bool is_nominal, const std::string& affected_component) :
  m_name(name),
  m_title(title),
  m_affected_component(affected_component),
  m_branch_name(branch_name),
  m_is_input(is_input),
  m_is_nominal(is_nominal),
  m_var_component(NULL),
  m_var_weight(NULL)
{}

//_____________________________________________________________________________________
//
WeightObject::~WeightObject()
{}

//_____________________________________________________________________________________
//
WeightObject::WeightObject( WeightObject &q )
{
  m_name                 = q.m_name;
  m_title                = q.m_title;
  m_affected_component   = q.m_affected_component;
  m_branch_name          = q.m_branch_name;
  m_is_input             = q.m_is_input;
  m_is_nominal           = q.m_is_nominal;
  m_var_component        = q.m_var_component;
  m_var_weight           = q.m_var_weight;
}

//_____________________________________________________________________________________
//
double WeightObject::GetWeightValue() const{
  if(m_is_nominal) m_var_weight->CalcDoubleValue();
  return m_var_weight->GetDoubleValue();
}

void WeightObject::SetVarWeight(double* t){
  m_var_weight = new VariableDef(m_name + "_weight", "", "D", t);
}

bool WeightObject::SetComponentValue(double value){
  if(m_is_input){
    std::cerr << "WeightObject::SetComponentValue() --> ERROR: Cannot reset value of weight component "<<m_name<<". Expected from input tree "<<std::endl;
  }

  void* compAdd = m_var_component->Address();
  VariableDef::VariableType compType = m_var_component->VarType();

  if(compType == VariableDef::DOUBLE){
    *((double*)compAdd) = value;
  }
  else if(compType == VariableDef::FLOAT){
    *((float*)compAdd) = value;
  }
  else{
    int vecInd = m_var_component->VecInd();

    if( (compType == VariableDef::PTRVECDOUBLE) || (compType == VariableDef::VECDOUBLE) ){
      std::vector<double>* vecAdd = (compType == VariableDef::PTRVECDOUBLE) ? *(std::vector<double>**)compAdd : (std::vector<double>*)compAdd;
      int vecSize = (int)(vecAdd)->size();
      
      if( vecInd < vecSize )        { vecAdd->at(vecInd) = value; }
      else if( vecInd == vecSize )  { vecAdd->push_back(value); }
      else{ std::cerr << "WeightManager::SetWeightComponent() --> ERROR: Attempt to access vector weight component " 
		      << m_name << " at index "<< vecInd << " exceeding vector size " 
		      << vecSize << std::endl;
      }
    }
    if( (compType == VariableDef::PTRVECFLOAT) || (compType == VariableDef::VECFLOAT) ){
      std::vector<float>* vecAdd = (compType == VariableDef::PTRVECFLOAT) ? *(std::vector<float>**)compAdd : (std::vector<float>*)compAdd;
      int vecSize = (int)(vecAdd)->size();
      
      if( vecInd < vecSize )        { vecAdd->at(vecInd) = (float)value; }
      else if( vecInd == vecSize )  { vecAdd->push_back((float)value); }
      else{ std::cerr << "WeightManager::SetWeightComponent() --> ERROR: Attempt to access vector weight component " 
		      << m_name << " at index "<< vecInd << " exceeding vector size " 
		      << vecSize << std::endl;
      }
    }
    else{
      std::cerr << " WeightManager::SetWeightComponent() --> ERROR: VariableType of weight component " 
		<< m_name << " does not conform to permitted ones. Code should never have reached this point. " << std::endl;
	return false;
    }
    
  }
  m_var_component->CalcDoubleValue();
  return true;

}

//The computed weight is ALWAYS a double and, for now, always in OutputData 
bool WeightObject::SetWeightValue(double value){
  *((double*) m_var_weight->Address() ) = value;
  m_var_weight->CalcDoubleValue();
  return true;
}
