#ifndef VARIABLEDEF_H
#define VARIABLEDEF_H

#include <iostream>
#include "TString.h"

class VariableDef {
    
public:
    
    enum VariableType{
        INT=1,
        FLOAT,
        DOUBLE,
        VECINT,
        VECFLOAT,
        VECDOUBLE,
        VECVECINT,
        VECVECFLOAT,
        VECVECDOUBLE
    };
    
    //
    // Public standard functions
    //
    VariableDef();
    ~VariableDef();
    VariableDef( VariableDef &q );
    
    //
    // Class functions
    //
    TString GetVarTypeString(int varType);
    VariableType GetVarType(TString varTypeString);
    bool IsPrimitive(int varType);
    double GetDoubleValue();
    
    //
    // Getter functions
    //
    inline TString Name(){return m_name;}
    inline TString Title(){return m_title;}
    inline TString VarTypeString(){return m_varTypeString;}
    inline VariableType VarType(){return m_varType;}
    inline void* Address(){return m_address;}
    inline int VecInd(){return m_vec_ind;}
    inline bool IsPrimitive(){return m_isPrimitive;}
    
    //
    // Setter functions
    //
    inline void SetName( TString name ){m_name = name;}
    inline void SetTitle( TString title ){m_title = title;}
    inline void SetVarType( int varType ){m_varType = (VariableType)varType;}
    inline void SetVarTypeString( int varTypeString ){m_varTypeString = varTypeString;}
    inline void setVecInd(int vec_ind){m_vec_ind = vec_ind;}
    
    //
    // Template constructors
    //
    template<typename T> VariableDef(TString name, TString title, int varType, T *t, int vec_ind = -1):
    m_name(name),
    m_title(title),
    m_varTypeString(GetVarTypeString(varType)),
    m_varType(varType),
    m_vec_ind(vec_ind),
    m_address(0),
    m_isPrimitive(true)
    {
        SetAddress(t);
        m_isPrimitive = IsPrimitive(varType);
    }
    
    template<typename T> VariableDef(TString name, TString title, TString varTypeString, T *t, int vec_ind = -1):
    m_name(name),
    m_title(title),
    m_varTypeString(varTypeString),
    m_varType(GetVarType(varTypeString)),
    m_vec_ind(vec_ind),
    m_address(0),
    m_isPrimitive(true)
    {
        SetAddress(t);
        m_isPrimitive = IsPrimitive(GetVarType(varTypeString));
    }
    
    
    template< typename T > void SetAddress( T *t ){
        m_address = (void*)t;
    }
    
private:
    //
    // Data members
    //
    TString m_name;
    TString m_title;
    TString m_varTypeString;
    VariableType m_varType;
    int m_vec_ind;
    void *m_address;
    bool m_isPrimitive;
    
};


#endif
