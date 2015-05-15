#include "IFAETopFramework/VariableDef.h"
#include <iostream>

VariableDef::VariableDef(): 
  m_name(""),
  m_title(""),
  m_varType(VariableType::DOUBLE),
  m_vec_ind(-1),
  m_address(NULL)
{
}



TString VariableDef::getVarTypeString(int varType){

  TString _varTypeString = "";

  if(varType == VariableType::INT){_varTypeString = "I";}
  else if(varType == VariableType::FLOAT){_varTypeString = "F";}
  else if(varType == VariableType::DOUBLE){_varTypeString = "D";}
  else if(varType == VariableType::VECINT){_varTypeString = "VI";}
  else if(varType == VariableType::VECFLOAT){_varTypeString = "VF";}
  else if(varType == VariableType::VECDOUBLE){_varTypeString = "VD";}
  else { std::cout << "Unknown variable type" << std::endl; }

  return _varTypeString;
}


VariableDef::VariableType VariableDef::getVarType(TString varTypeString){

  VariableType _varType;
  if(varTypeString == "I"){_varType = VariableType::INT;}
  else if(varTypeString == "F"){_varType = VariableType::FLOAT;}
  else if(varTypeString == "D"){_varType = VariableType::DOUBLE;}
  else if(varTypeString == "VI"){_varType = VariableType::VECINT;}
  else if(varTypeString == "VF"){_varType = VariableType::VECFLOAT;}
  else if(varTypeString == "VD"){_varType = VariableType::VECDOUBLE;}

  return _varType;
} 


bool VariableDef::isPrimitive(VariableType varType){
  
  bool _isPrimitive = false;
  _isPrimitive = (varType == VariableType::INT || varType == VariableType::FLOAT || varType == VariableType::DOUBLE);

  return _isPrimitive;
}

double VariableDef::GetDoubleValue(){

  double value = 0.;
  if(m_address == NULL){std::cout << "Error: VariableDef points to NULL" << std::endl; return value;}

  if(m_varType == VariableType::DOUBLE){
    value = *(double*)(m_address);
  }
  else if(m_varType == VariableType::FLOAT){
    value = (double)(*(float*)(m_address));
  }
  else if(m_varType == VariableType::INT){
    value = (double)(*(int*)(m_address));
  }
  else{
    if(m_vec_ind < 0){std::cout<<"Error : Please provide vector index to obtain value from VariableDef" << std::endl; return value;}

    if(m_varType == VariableType::VECDOUBLE){
      if( ((std::vector<double>*)m_address)->size() > m_vec_ind ){
	value = ((std::vector<double>*)m_address)->at(m_vec_ind);
      }
    }
    else if(m_varType == VariableType::VECFLOAT){
      if( ((std::vector<float>*)m_address)->size() > m_vec_ind ){
	value = (double)( ((std::vector<float>*)m_address)->at(m_vec_ind) );
      }
    }
    else if(m_varType == VariableType::VECINT){
      if( ((std::vector<int>*)m_address)->size() > m_vec_ind ){
	value = (double)( ((std::vector<int>*)m_address)->at(m_vec_ind) );
      }
    }

  }//Vector variables

  return value;


}
