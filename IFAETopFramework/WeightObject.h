#ifndef WEIGHTOBJECT_H
#define WEIGHTOBJECT_H

#include <iostream>
#include "TString.h"
#include "IFAETopFramework/VariableDef.h"

class WeightObject{
    
public:
    //
    // Public standard functions
    //
    WeightObject();

    WeightObject(const std::string& name, const std::string& title="", bool is_input=true, const std::string& branch_name=""
		 ,bool is_nominal=true, const std::string& affected_component="");

    ~WeightObject();
    WeightObject( WeightObject &q );

    //
    //Getter functions
    //
    inline const std::string& Name(){ return m_name; }
    inline const std::string& Title(){ return m_title; }
    inline const std::string& AffectedComponent() const{ return m_affected_component; }
    inline const std::string& BranchName() const{ return m_branch_name; }
    inline bool IsInput() const{ return m_is_input; }
    inline bool IsNominal() const{ return m_is_nominal; }

    inline double GetComponentValue() const{ return m_var_component->GetDoubleValue(); }
    inline double GetWeightValue() const{ return m_var_weight->GetDoubleValue(); }
    inline VariableDef::VariableType GetComponentType() const{ return m_var_component->VarType(); } 
    inline const std::string& GetComponentTypeStr() const{ return m_var_component->VarTypeString(); } 
    inline void* GetComponentAddress() const{ return m_var_component->Address(); }
    inline void* GetWeightAddress() const{ return m_var_weight->Address(); }

 
    //
    //Setter functions
    //

    inline void SetAffectedComponent(const std::string& affected_component){ m_affected_component = affected_component; }
    inline void SetBranchName( std::string& branch_name){ m_branch_name = branch_name; }
    inline void SetIsInput( bool is_input){ m_is_input = is_input; }
    inline void SetIsNominal( bool is_nominal){ m_is_nominal = is_nominal; }

    template<typename T> void SetVarComponent(const std::string& varTypeString, T* t, int vec_ind = -1){
      m_var_component = new VariableDef(m_name + "_component", "", varTypeString, t, vec_ind);
    }
    void SetVarWeight(double* t); 

    bool SetWeightValue(double value);
    //Only allowed for weights that do not come from the input tree
    bool SetComponentValue(double value);
    void UpdateComponentStore(){ m_var_component->CalcDoubleValue(); return;}
    void UpdateWeightStore(){ m_var_weight->CalcDoubleValue(); return;}

private:
    //
    // Data members
    //
    std::string m_name;
    std::string m_title;
    std::string m_affected_component;
    std::string m_branch_name;
    bool m_is_input;
    bool m_is_nominal;

    VariableDef* m_var_component;
    VariableDef* m_var_weight;

};


#endif
