#ifndef WEIGHTOBJECT_H
#define WEIGHTOBJECT_H

#include <iostream>
#include "TString.h"
#include "IFAETopFramework/VariableDef.h"

class WeightObject : public VariableDef{
    
public:
    //
    // Public standard functions
    //
    WeightObject();

    WeightObject(const std::string& name,  double *t, bool is_input=true, const std::string& branch_name=""
		 , VariableType input_varType=DOUBLE, bool is_nominal=true, const std::string& affected_component="");

    WeightObject(const std::string& name, double *t, bool is_input=true, const std::string& branch_name=""
		 , const std::string& input_varTypeString="D", bool is_nominal=true, const std::string& affected_component="");
    ~WeightObject();
    WeightObject( WeightObject &q );
    
    inline const std::string& AffectedComponent() const{ return m_affected_component; }
    inline const std::string& BranchName() const{ return m_branch_name; }
    inline bool IsInput() const{ return m_is_input; }
    inline bool IsNominal() const{ return m_is_nominal; }
    inline VariableType InputVarType() const{ return m_input_varType; }
    inline const std::string& InputVarTypeString() const{ return m_input_varTypeString; }
  
    inline void SetAffectedComponent(const std::string& affected_component){ m_affected_component = affected_component; }
    inline void SetBranchName( std::string& branch_name){ m_branch_name = branch_name; }
    inline void SetIsInput( bool is_input){ m_is_input = is_input; }
    inline void SetIsNominal( bool is_nominal){ m_is_nominal = is_nominal; }
    inline void SetInputVarType( int input_varType ){
      m_input_varType = (VariableType)input_varType;
      m_input_varTypeString = GetVarTypeString( (VariableType)input_varType );
    }
    inline void SetInputVarTypeString( const std::string& input_varTypeString ){
      m_input_varTypeString = input_varTypeString; 
      m_input_varType = GetVarType(input_varTypeString);
    }


private:
    //
    // Data members
    //
    std::string m_affected_component;
    std::string m_branch_name;
    bool m_is_input;
    bool m_is_nominal;
    VariableType m_input_varType;
    std::string m_input_varTypeString;

};


#endif
