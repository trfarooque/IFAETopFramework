#ifndef VARIABLEDEF_H
#define VARIABLEDEF_H

#include "TString.h"


class VariableDef {

 public:

  enum VariableType{
    INT=1,
    FLOAT,
    DOUBLE,
    VECINT,
    VECFLOAT,
    VECDOUBLE
  };


  VariableDef();

  TString getVarTypeString(int varType);
  VariableType getVarType(TString varTypeString); 
  bool isPrimitive(VariableType varType);


  template<typename T> VariableDef(TString name, TString title, int varType, T *t, int vec_ind = -1)
    {
      VariableDef(name, title, (VariableType)varType, t, getVarTypeString((VariableType)m_varType), vec_ind);
    }
  
  template<typename T> VariableDef(TString name, TString title, TString varTypeString, T *t, int vec_ind = -1)
    {
      VariableDef(name, title, getVarType(varTypeString), t, varTypeString, vec_ind);
    }
  
  
  TString name(){return m_name;}
  TString title(){return m_title;}
  TString varTypeString(){return m_varTypeString;}
  VariableType varType(){return m_varType;}
  void* address(){return m_address;}
  int vec_ind(){return m_vec_ind;}
  bool isPrimitive(){return m_isPrimitive;}
  double GetDoubleValue();


  void setName( TString name ){m_name = name;}
  void setTitle( TString title ){m_title = title;}
  void setType( int varType ){m_varType = (VariableType)varType;}
  void setTypeString( int varTypeString ){m_varTypeString = varTypeString;}
  void setVecInd(int vec_ind){m_vec_ind = vec_ind;}
  template< typename T > void setAddress( T *t )
    { m_address = (void*)t; }

 protected:

  template<typename T> VariableDef(TString name, TString title, VariableType varType, T* t, TString varTypeString, int vec_ind):
  m_name(name),
    m_title(title),
    m_varType(varType),
    m_varTypeString(varTypeString),
    m_vec_ind(vec_ind)
      {
	setAddress(t);
      }
  
  
  TString m_name;
  TString m_title;
  TString m_varTypeString;
  VariableType m_varType;
  void *m_address;
  int m_vec_ind;
  bool m_isPrimitive;  

};


#endif
