#ifndef VARIABLEDEF_H
#define VARIABLEDEF_H

#include <iostream>
#include <string>
#include <vector>

class VariableDef {
    
public:
    
    enum VariableType{
        INT=1,
        PTRINT,
        FLOAT,
        PTRFLOAT,
        DOUBLE,
        PTRDOUBLE,
	BOOL,
	PTRBOOL,
        VECINT,
        PTRVECINT,
        VECFLOAT,
        PTRVECFLOAT,
        VECDOUBLE,
        PTRVECDOUBLE,
        VECBOOL,
        PTRVECBOOL,
        VECVECINT,
        PTRVECVECINT,
        VECVECFLOAT,
        PTRVECVECFLOAT,
        VECVECDOUBLE,
        PTRVECVECDOUBLE,
        VECVECBOOL,
        PTRVECVECBOOL,
	AOBJ,
	PTRAOBJ,
	VECAO,
	PTRVECAO
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
    static std::string GetVarTypeString(int varType);
    static VariableType GetVarType(const std::string& varTypeString);
    static bool IsPrimitive(int varType);
    static bool IsAnaObject(int varType);
    static bool IsVector(int varType);
    static bool IsPointer(int varType);

    void CalcDoubleValue();
    void FillVectorStore();
        
    //
    // Getter functions
    //
    inline const std::string& Name() const{ return m_name; }
    inline const std::string& Title() const{ return m_title; }
    inline const std::string& VarTypeString() const{ return m_varTypeString; }
    inline VariableType VarType() const{ return m_varType; }
    inline void* Address() const{ return m_address; }
    inline int VecInd() const{ return m_vec_ind; }
    inline bool IsPrimitive() const{ return m_isPrimitive; }
    inline bool IsPointer() const{ return m_isPointer; }
    inline bool IsVector() const{ return m_isVector; }
    inline bool IsAnaObject() const{ return m_isAnaObject; }
    inline const std::string& Moment() const{ return m_moment; }    
    inline double* ValStore() const{ return m_val_store; }
    inline std::vector<double>* VecStore() const{ return m_vec_store; }
    inline double GetDoubleValue() const{ return *m_val_store; }
    inline int GetVecSize() const{ return m_vec_size; }

    //
    // Setter functions
    //
    inline void SetName( const std::string& name ){m_name = name;}
    inline void SetTitle( const std::string& title ){m_title = title;}
    void SetVarType( int varType ){
      m_varType       = (VariableType)varType;
      m_varTypeString = GetVarTypeString( (VariableType)varType );
    }
    void SetVarTypeString( const std::string& varTypeString ){
      m_varTypeString = varTypeString;
      m_varType = GetVarType(varTypeString); 
    }
    inline void SetVecInd(int vec_ind){m_vec_ind = vec_ind;}
    inline void SetMoment(const std::string& moment){ m_moment = moment; }
    //
    // Template constructors
    //
    template<typename T> VariableDef(const std::string& name, const std::string& title, VariableType varType, T *t, int vec_ind = -1, const std::string& moment="", bool fill_vec=false):
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
      m_val_store(NULL),
      m_vec_store(NULL),
      m_fill_vec(fill_vec) 
    {
        m_isPrimitive = IsPrimitive(varType);
        m_isPointer = IsPointer(varType);
	m_isVector = IsVector(varType);
	m_isAnaObject = IsAnaObject(varType);
	if(m_fill_vec){
	  m_vec_store = new std::vector<double>();
	  m_vec_store->clear(); 
	}
	else{
	  m_val_store = new double();
	}
        SetAddress(t);
    }
    
    template<typename T> VariableDef(const std::string& name, const std::string& title, const std::string& varTypeString, T *t, int vec_ind = -1, const std::string& moment="", bool fill_vec=false):
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
      m_val_store(NULL),
      m_vec_store(NULL),
      m_fill_vec(fill_vec) 

    {
	VariableType varType = GetVarType(varTypeString);
        m_isPrimitive = IsPrimitive(varType);
        m_isPointer = IsPointer(varType);
	m_isVector = IsVector(varType);
	m_isAnaObject = IsAnaObject(varType);
        SetAddress(t);
	if(m_fill_vec){
	  m_vec_store = new std::vector<double>();
	  m_vec_store->clear(); 
	}
	else{ m_val_store = new double(); }
    }
    
    template< typename T > void SetAddress( T *t ){ m_address = (void*)t; }

protected:
    //
    // Data members
    //
    std::string m_name;
    std::string m_title;
    std::string m_varTypeString;
    VariableType m_varType;
    int m_vec_ind;
    void *m_address;
    bool m_isPrimitive;
    bool m_isPointer;
    bool m_isVector;
    bool m_isAnaObject;

    std::string m_moment; //for analysis objects;
    double* m_val_store; 
    std::vector<double>* m_vec_store; 
    int m_vec_size;
    bool m_fill_vec;        
};


#endif
