#ifndef WEIGHTOBJECT_H
#define WEIGHTOBJECT_H

#include <iostream>
#include "TString.h"
#include "IFAETopFramework/VariableDef.h"

class WeightObject{
    
public:

  enum FlagTypes{NOMINAL=0,INPUT,RESERVE};

    //
    // Public standard functions
    //
    WeightObject();

    WeightObject(const std::string& name, const std::string& title="", bool is_input=true, const std::string& branch_name=""
		 ,bool is_nominal=true, const std::string& affected_component="", bool is_reserve = false);

    ~WeightObject();
    WeightObject( WeightObject &q );

    //
    //Getter functions
    //
    inline const std::string& Name(){ return m_name; }
    inline const std::string& Title(){ return m_title; }
    inline const std::string& AffectedComponent() const{ return m_affected_component; }
    inline const std::string& BranchName() const{ return m_branch_name; }

    inline bool IsInput() const{ return PassFlagAtBit(INPUT); }
    inline bool IsNominal() const{ return PassFlagAtBit(NOMINAL); }
    inline bool IsReserve() const{ return PassFlagAtBit(RESERVE); }
    inline int Flags() const{ return m_flags; }

    //inline bool IsInput() const{ return m_is_input; }
    //inline bool IsNominal() const{ return m_is_nominal; }
    //inline bool IsReserve() const{ return m_is_reserve; }

 
    inline double GetComponentValue() const{ return m_var_component->GetDoubleValue(); }
    double GetWeightValue() const;

    inline VariableDef::VariableType GetComponentType() const{ return m_var_component->VarType(); } 
    inline const std::string& GetComponentTypeStr() const{ return m_var_component->VarTypeString(); } 
    inline void* GetComponentAddress() const{ return m_var_component->Address(); }
    inline void* GetWeightAddress() const{ return m_var_weight->Address(); }
    inline void* GetComponentValStore() const{ return m_var_component->ValStore(); }
    inline void* GetWeightValStore() const{ return m_var_weight->ValStore(); }
    inline const VariableDef* GetComponentVariable() const{ return m_var_component; }
    inline const VariableDef* GetWeightVariable() const{ return m_var_weight; }

     //
    //Setter functions
    //

    inline void SetAffectedComponent(const std::string& affected_component){ m_affected_component = affected_component; }
    inline void SetBranchName( const std::string& branch_name){ m_branch_name = branch_name; }
    //inline void SetIsInput( bool is_input){ m_is_input = is_input; }
    //inline void SetIsNominal( bool is_nominal){ m_is_nominal = is_nominal; }
    //inline void SetIsReserve( bool is_reserve){ m_is_reserve = is_reserve; }

    inline void SetIsInput( bool is_input){ AddFlagAtBit(INPUT, is_input); }
    inline void SetIsNominal( bool is_nominal){ AddFlagAtBit(NOMINAL, is_nominal); }
    inline void SetIsReserve( bool is_reserve){ AddFlagAtBit(RESERVE, is_reserve); }

    inline void SetFlags( int flags ){ m_flags = flags; }

    /*
    template<typename T> void SetVarComponent(const std::string& varTypeString, T* t, int vec_ind = -1){
      m_var_component = new VariableDef(m_name + "_component", "", varTypeString, t, vec_ind);
    }
    */
    template<typename T> void SetVarComponent(T* t, int vec_ind = -1){
      m_var_component = new VariableDef(m_name + "_component", "", t, vec_ind);
    }

    void SetVarWeight(double* t); 

    bool SetWeightValue(double value);
    //Only allowed for weights that do not come from the input tree
    bool SetComponentValue(double value);
    void UpdateComponentStore(){ m_var_component->CalcDoubleValue(); return;}
    void UpdateWeightStore(){ m_var_weight->CalcDoubleValue(); return;}

    void AddFlagAtBit(const int bit, const bool value=true);
    bool PassFlagAtBit(const int bit) const;

private:
    //
    // Data members
    //
    std::string m_name;
    std::string m_title;
    std::string m_affected_component;
    std::string m_branch_name;
    //bool m_is_input;
    //bool m_is_nominal;
    //bool m_is_reserve; 
    int m_flags;

    VariableDef* m_var_component;
    VariableDef* m_var_weight;

};


#endif
