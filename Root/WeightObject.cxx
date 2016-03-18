#include "IFAETopFramework/WeightObject.h"
//_____________________________________________________________________________________
//
WeightObject::WeightObject():
  VariableDef(),
  m_affected_component(""),
  m_branch_name(""),
  m_is_input(true),
  m_input_varType(DOUBLE),
  m_input_varTypeString("D") 
{ }


//_____________________________________________________________________________________
WeightObject::WeightObject(const std::string& name,  double *t, bool is_input, const std::string& branch_name
			   , VariableType input_varType, const std::string& affected_component) :
  VariableDef(name, name, "D", t, -1),
  m_affected_component(affected_component),
  m_branch_name(branch_name),
  m_is_input(is_input),
  m_input_varType(input_varType),
  m_input_varTypeString(GetVarTypeString(input_varType))
{ }

WeightObject::WeightObject(const std::string& name, double *t, bool is_input, const std::string& branch_name
		 , const std::string& input_varTypeString, const std::string& affected_component) :
  VariableDef(name, name, "D", t, -1),
  m_affected_component(affected_component),
  m_branch_name(branch_name),
  m_is_input(is_input),
  m_input_varType(GetVarType(input_varTypeString)),
  m_input_varTypeString(input_varTypeString)
{ }

//_____________________________________________________________________________________
//
WeightObject::~WeightObject()
{}

//_____________________________________________________________________________________
//
WeightObject::WeightObject( WeightObject &q ) : 
  VariableDef(q)
{
  m_affected_component   = q.m_affected_component;
  m_branch_name          = q.m_branch_name;
  m_is_input             = q.m_is_input;
  m_input_varType        = q.m_input_varType;
  m_input_varTypeString  = q.m_input_varTypeString;
}
