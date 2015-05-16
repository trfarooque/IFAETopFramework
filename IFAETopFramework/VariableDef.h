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
    TString getVarTypeString(int varType);
    VariableType getVarType(TString varTypeString);
    bool isPrimitive(int varType);
    double GetDoubleValue();
    
    //
    // Getter functions
    //
    inline TString name(){return m_name;}
    inline TString title(){return m_title;}
    inline TString varTypeString(){return m_varTypeString;}
    inline VariableType varType(){return m_varType;}
    inline void* address(){return m_address;}
    inline int vec_ind(){return m_vec_ind;}
    inline bool isPrimitive(){return m_isPrimitive;}
    
    //
    // Setter functions
    //
    inline void setName( TString name ){m_name = name;}
    inline void setTitle( TString title ){m_title = title;}
    inline void setType( int varType ){m_varType = (VariableType)varType;}
    inline void setTypeString( int varTypeString ){m_varTypeString = varTypeString;}
    inline void setVecInd(int vec_ind){m_vec_ind = vec_ind;}
    
    //
    // Template constructors
    //
    template<typename T> VariableDef(TString name, TString title, int varType, T *t, int vec_ind = -1):
    m_name(name),
    m_title(title),
    m_varTypeString(getVarTypeString(varType)),
    m_varType(varType),
    m_vec_ind(vec_ind),
    m_address(0),
    m_isPrimitive(true)
    {
        setAddress(t);
        m_isPrimitive = isPrimitive(varType);
    }
    
    template<typename T> VariableDef(TString name, TString title, TString varTypeString, T *t, int vec_ind = -1):
    m_name(name),
    m_title(title),
    m_varTypeString(varTypeString),
    m_varType(getVarType(varTypeString)),
    m_vec_ind(vec_ind),
    m_address(0),
    m_isPrimitive(true)
    {
        setAddress(t);
        m_isPrimitive = isPrimitive(getVarType(varTypeString));
    }
    
    
    template< typename T > void setAddress( T *t ){
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
