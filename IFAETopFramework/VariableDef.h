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
        VECVECDOUBLE,
	AOBJ,
	VECAO
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
    bool IsAnaObject(int varType);
    bool IsVector(int varType);
    void CalcDoubleValue();
    void FillVectorStore();
        
    //
    // Getter functions
    //
    inline TString Name(){ return m_name; }
    inline TString Title(){ return m_title; }
    inline TString VarTypeString(){ return m_varTypeString; }
    inline VariableType VarType(){ return m_varType; }
    inline void* Address(){ return m_address; }
    inline int VecInd(){ return m_vec_ind; }
    inline bool IsPrimitive(){ return m_isPrimitive; }
    inline bool IsVector(){ return m_isVector; }
    inline bool IsAnaObject(){ return m_isAnaObject; }
    inline const std::string& Moment(){ return m_moment; }    
    inline double* ValStore(){ return &m_val_store; }
    inline std::vector<double>* VecStore(){ return m_vec_store; }
    inline double GetDoubleValue(){ return m_val_store; }

    //
    // Setter functions
    //
    inline void SetName( TString name ){m_name = name;}
    inline void SetTitle( TString title ){m_title = title;}
    inline void SetVarType( int varType ){m_varType = (VariableType)varType;}
    inline void SetVarTypeString( int varTypeString ){m_varTypeString = varTypeString;}
    inline void SetVecInd(int vec_ind){m_vec_ind = vec_ind;}
    inline void SetMoment(const std::string& moment){ m_moment = moment; }
    //
    // Template constructors
    //
    template<typename T> VariableDef(TString name, TString title, VariableType varType, T *t, int vec_ind = -1, const std::string& moment="", bool fill_vec=false):
    m_name(name),
      m_title(title),
      m_varTypeString(GetVarTypeString(varType)),
      m_varType(varType),
      m_vec_ind(vec_ind),
      m_address(0),
      m_isPrimitive(true),
      m_isVector(false),
      m_isAnaObject(false),
      m_moment(moment),
      m_val_store(0.),
      m_vec_store(NULL),
      m_fill_vec(fill_vec) 
    {
        m_isPrimitive = IsPrimitive(varType);
	m_isVector = IsVector(varType);
	m_isAnaObject = IsAnaObject(varType);
	m_val_store = 0.;
	if(m_fill_vec){
	  m_vec_store = new std::vector<double>;
	  m_vec_store->clear(); 
	}
        SetAddress(t);
    }
    
    template<typename T> VariableDef(TString name, TString title, TString varTypeString, T *t, int vec_ind = -1, const std::string& moment="", bool fill_vec=false):
    m_name(name),
      m_title(title),
      m_varTypeString(varTypeString),
      m_varType(GetVarType(varTypeString)),
      m_vec_ind(vec_ind),
      m_address(0),
      m_isPrimitive(true),
      m_isVector(false),
      m_isAnaObject(false),
      m_moment(moment),
      m_val_store(0.),
      m_vec_store(NULL),
      m_fill_vec(fill_vec) 

    {
	VariableType varType = GetVarType(varTypeString);
        m_isPrimitive = IsPrimitive(varType);
	m_isVector = IsVector(varType);
	m_isAnaObject = IsAnaObject(varType);
        SetAddress(t);
	if(m_fill_vec){
	  m_vec_store = new std::vector<double>;
	  m_vec_store->clear(); 
	}

    }
    
    
    template< typename T > void SetAddress( T *t ){ m_address = (void*)t; }

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
    bool m_isVector;
    bool m_isAnaObject;

    std::string m_moment; //for analysis objects;
    double m_val_store; 
    std::vector<double>* m_vec_store; 
    bool m_fill_vec;        
};


#endif
