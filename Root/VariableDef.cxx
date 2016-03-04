#include "IFAETopFramework/VariableDef.h"
#include "IFAETopFramework/AnalysisObject.h"
#include <iostream>

//_____________________________________________________________________________________
//
VariableDef::VariableDef():
  m_name(""),
  m_title(""),
  m_varTypeString(""),
  m_varType(VariableType::DOUBLE),
  m_vec_ind(-1),
  m_address(NULL),
  m_isPrimitive(true),
  m_isVector(true),
  m_isAnaObject(true),
  m_moment(""),
  m_val_store(0.),
  m_vec_store(NULL),
  m_fill_vec(false) 
{}

//_____________________________________________________________________________________
//
VariableDef::~VariableDef()
{}

//_____________________________________________________________________________________
//
VariableDef::VariableDef( VariableDef &q ){
    m_name          = q.m_name;
    m_title         = q.m_title;
    m_varTypeString = q.m_varTypeString;
    m_varType       = q.m_varType;
    m_vec_ind       = q.m_vec_ind;
    m_address       = q.m_address;
    m_isPrimitive   = q.m_isPrimitive;
    m_isVector      = q.m_isVector;
    m_isAnaObject   = q.m_isAnaObject;
    m_moment        = q.m_moment;
    m_val_store     = q.m_val_store;
    m_vec_store     = q.m_vec_store;
    m_fill_vec      = q.m_fill_vec;
}

//_____________________________________________________________________________________
//
TString VariableDef::GetVarTypeString(int varType){
    
    TString _varTypeString = "";
    
    if(varType == VariableType::INT)               {_varTypeString = "I";}
    else if(varType == VariableType::FLOAT)        {_varTypeString = "F";}
    else if(varType == VariableType::DOUBLE)       {_varTypeString = "D";}
    else if(varType == VariableType::VECINT)       {_varTypeString = "VI";}
    else if(varType == VariableType::VECFLOAT)     {_varTypeString = "VF";}
    else if(varType == VariableType::VECDOUBLE)    {_varTypeString = "VD";}
    else if(varType == VariableType::VECVECINT)    {_varTypeString = "VVI";}
    else if(varType == VariableType::VECVECFLOAT)  {_varTypeString = "VVF";}
    else if(varType == VariableType::VECVECDOUBLE) {_varTypeString = "VVD";}
    else if(varType == VariableType::AOBJ)         {_varTypeString = "AO";}
    else if(varType == VariableType::VECAO)        {_varTypeString = "VAO";}
    else { std::cout << "Unknown variable type" << std::endl; }
    
    return _varTypeString;
}

//_____________________________________________________________________________________
//
VariableDef::VariableType VariableDef::GetVarType(TString varTypeString){
    VariableType _varType;
    if(varTypeString == "I")         {_varType = VariableType::INT;}
    else if(varTypeString == "F")    {_varType = VariableType::FLOAT;}
    else if(varTypeString == "D")    {_varType = VariableType::DOUBLE;}
    else if(varTypeString == "VI")   {_varType = VariableType::VECINT;}
    else if(varTypeString == "VF")   {_varType = VariableType::VECFLOAT;}
    else if(varTypeString == "VD")   {_varType = VariableType::VECDOUBLE;}
    else if(varTypeString == "VVI")  {_varType = VariableType::VECVECINT;}
    else if(varTypeString == "VVF")  {_varType = VariableType::VECVECFLOAT;}
    else if(varTypeString == "VVD")  {_varType = VariableType::VECVECDOUBLE;}
    else if(varTypeString == "AO")   {_varType = VariableType::AOBJ;}
    else if(varTypeString == "VAO")  {_varType = VariableType::VECAO;}
    
    return _varType;
}


//_____________________________________________________________________________________
//
bool VariableDef::IsPrimitive(int varType){
    
    bool _isPrimitive = false;
    _isPrimitive = (varType == VariableType::INT || varType == VariableType::FLOAT || varType == VariableType::DOUBLE);
    return _isPrimitive;
}

bool VariableDef::IsVector(int varType){
    
    bool _isVector = false;
    _isVector = ( (varType == VariableType::VECINT) || (varType == VariableType::VECFLOAT) || (varType == VariableType::VECDOUBLE)
		     || (varType == VariableType::VECVECINT) || (varType == VariableType::VECVECFLOAT) || (varType == VariableType::VECVECDOUBLE)
		     || (varType == VariableType::VECAO) );
    return _isVector;
}

bool VariableDef::IsAnaObject(int varType){
    
    bool _isAnaObj = false;
    _isAnaObj = ( (varType == VariableType::AOBJ) || (varType == VariableType::VECAO) );
    return _isAnaObj;
}

//
void VariableDef::FillVectorStore(){

  //Currently only call this AOVector
  m_vec_store->clear();
  if(m_varType == VariableDef::VECAO){
    AOVector* vec = (AOVector*)m_address;
    for ( AnalysisObject* obj : *vec ){ m_vec_store->push_back( obj->GetMoment(m_moment) ); }
  }

  return;
}
//_____________________________________________________________________________________
//
void VariableDef::CalcDoubleValue(){
    m_val_store = 0.;
    if(m_address == NULL){std::cout << "Error: VariableDef points to NULL" << std::endl; return;}
    
    if( (m_varType == VariableType::DOUBLE)){
        m_val_store = *(double*)(m_address);
    }
    else if(m_varType == VariableType::FLOAT){
        m_val_store = (double)(*(float*)(m_address));
    }
    else if(m_varType == VariableType::INT){
        m_val_store = (double)(*(int*)(m_address));
    }
    else if(m_varType == VariableType::AOBJ){
      m_val_store = ((AnalysisObject*)(m_address))->GetMoment(m_moment);
    } 
    else{
        if(m_vec_ind < 0){
            std::cout<<"Error : Please provide vector index to obtain value from VariableDef" << std::endl;
            return;
        }
        if(m_varType == VariableType::VECDOUBLE){
            if( (int)((std::vector<double>*)m_address)->size() > m_vec_ind ){
                m_val_store = ((std::vector<double>*)m_address)->at(m_vec_ind);
            }
        }
        else if(m_varType == VariableType::VECFLOAT){
            if( (int)((std::vector<float>*)m_address)->size() > m_vec_ind ){
                m_val_store = (double)( ((std::vector<float>*)m_address)->at(m_vec_ind) );
            }
        }
        else if(m_varType == VariableType::VECINT){
            if( (int)((std::vector<int>*)m_address)->size() > m_vec_ind ){
                m_val_store = (double)( ((std::vector<int>*)m_address)->at(m_vec_ind) );
            }
        }
	else if(m_varType == VariableType::VECAO){
	  AOVector* aovec = (AOVector*)m_address;
          if( (int)(aovec->size()) > m_vec_ind ){
	    m_val_store = (double)( ((AOVector*)m_address)->at(m_vec_ind)->GetMoment(m_moment) );
	  }
	}
    }//Vector variables

    return;
}
