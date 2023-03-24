#ifndef VARIABLEDEF_H
#define VARIABLEDEF_H

#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>

#include "IFAETopFramework/AnalysisObject.h"

class VariableDef {
    
 public:  

  enum VariableType{
    UNKNOWN,
    VOID,
    INT,
    LONGINT,
    LONGLONGINT,
    UINT,
    ULONGINT,
    ULONGLONGINT,
    FLOAT,
    DOUBLE,
    BOOL,
    VECINT,
    VECFLOAT,
    VECDOUBLE,
    VECBOOL,
    VECVECINT,
    VECVECFLOAT,
    VECVECDOUBLE,
    VECVECBOOL,
    AOBJ,
    VECAO,

    PTRINT,
    PTRFLOAT,
    PTRDOUBLE,
    PTRBOOL,
    PTRVECINT,
    PTRVECFLOAT,
    PTRVECDOUBLE,
    PTRVECBOOL,
    PTRVECVECINT,
    PTRVECVECFLOAT,
    PTRVECVECDOUBLE,
    PTRVECVECBOOL,
    PTRAOBJ,
    PTRVECAO
  };

  //
  // Public standard functions
  //
  VariableDef();
  ~VariableDef();
  VariableDef( const VariableDef &q );
  
    //
    // Class functions
    //
    static std::string GetVarTypeString(int varType);
    static VariableType GetVarType(const std::string& varTypeString);
    static bool IsPrimitive(int varType);
    static bool IsAnaObject(int varType);
    static bool IsVector(int varType);
    static bool IsPointer(int varType);

    bool PointsToNull();
    AnalysisObject* RetrieveAnalysisObject();
    void CalcFloatValue();
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
    inline float* FloatValStore() const{ return m_float_val_store; }
    inline double* ValStore() const{ return m_val_store; }
    inline std::vector<double>* VecStore() const{ return m_vec_store; }
    inline float GetFloatValue() const{ return *m_float_val_store; }
    inline double GetDoubleValue() const{ return *m_val_store; }
    inline bool IsValidValue() const{ return m_valid_value; }
    inline int GetVecSize() const{ return m_vec_size; }
    inline bool FillVec() const{ return m_fill_vec; }
    inline double DefaultValue() const{ return m_default; }
 
    //
    // Setter functions
    //
    inline void SetName( const std::string& name ){m_name = name;}
    inline void SetTitle( const std::string& title ){m_title = title;}
    /*
      void SetVarType( int varType ){
      m_varType       = (VariableType)varType;
      m_varTypeString = GetVarTypeString( (VariableType)varType );
      }
      void SetVarTypeString( const std::string& varTypeString ){
      m_varTypeString = varTypeString;
      m_varType = GetVarType(varTypeString); 
      }
    */
    inline void SetVecInd(int vec_ind){m_vec_ind = vec_ind;}
    inline void SetMoment(const std::string& moment){ m_moment = moment; }

    void SetFillVec(bool fill_vec){
      m_fill_vec = fill_vec;
      if(fill_vec){
	if(m_vec_store == NULL) m_vec_store = new std::vector<double>();
	m_vec_store->clear(); 
      }
      else{
	if(m_float_val_store == NULL) m_float_val_store = new float();
	if(m_val_store == NULL) m_val_store = new double();
      }
    }

    inline void SetDefault( double _default){ m_default = _default; } 
    
    //
    // Template constructors
    //
    template<typename T> VariableDef(const std::string& name, const std::string& title, T *t, int vec_ind = -1, const std::string& moment="", bool fill_vec=false):
    m_name(name),
      m_title(title),
      m_varType(FindVarType(t)), //t is the address to the variable, so dereference it first
      m_varTypeString(GetVarTypeString(m_varType)),
      m_vec_ind(vec_ind),
      m_address(0),
      m_isPrimitive(true),
      m_isVector(false),
      m_isAnaObject(false),
      m_moment(moment),
      m_float_val_store(NULL),
      m_val_store(NULL),
      m_vec_store(NULL),
      m_fill_vec(false),
      m_valid_value(true),
      m_default(0.)
      {
	
        m_isPrimitive = IsPrimitive(m_varType);
        m_isPointer = IsPointer(m_varType);
	m_isVector = IsVector(m_varType);
	m_isAnaObject = IsAnaObject(m_varType);
	SetFillVec(fill_vec);
        SetAddress(t);

      }
    
    
    template< typename T > void SetAddress( T *t ){ m_address = (void*)t; }
    //VariableType FindVarType(const std::string& vartypestr){ return INT; }

    template<typename T> VariableType FindVarType(T* t){

      const std::type_info& testtype = typeid(t);
      if(testtype == typeid(void*)) return VOID;
      else if(testtype == typeid(int*)) return INT;
      else if(testtype == typeid(unsigned int*)) return UINT;
      else if(testtype == typeid(long int*)) return LONGINT;
      else if(testtype == typeid(unsigned long int*)) return ULONGINT;
      else if(testtype == typeid(long long int*)) return LONGINT;
      else if(testtype == typeid(unsigned long long int*)) return ULONGINT;
      else if(testtype == typeid(float*)) return FLOAT;
      else if(testtype == typeid(double*)) return DOUBLE;
      else if(testtype == typeid(bool*)) return BOOL;

      else if(testtype == typeid(std::vector<int>*)) return VECINT;
      else if(testtype == typeid(std::vector<float>*)) return VECFLOAT;
      else if(testtype == typeid(std::vector<double>*)) return VECDOUBLE;
      else if(testtype == typeid(std::vector<bool>*)) return VECBOOL;

      else if(testtype == typeid(std::vector<std::vector<int> >*)) return VECVECINT;
      else if(testtype == typeid(std::vector<std::vector<float> >*)) return VECVECFLOAT;
      else if(testtype == typeid(std::vector<std::vector<double> >*)) return VECVECDOUBLE;
      else if(testtype == typeid(std::vector<std::vector<bool> >*)) return VECVECBOOL;

      else if(testtype == typeid(AnalysisObject*)) return AOBJ;
      else if(testtype == typeid(AOVector*)) return VECAO;
      else{
	std::cerr << " VariableDef::FindVarType --> ERROR: type "<<testtype.name()<<" of variable "<<m_name<<" is unknown."<<std::endl;
	exit(1);
      }

      return UNKNOWN; //Check if needed

    } 


    template< typename T > VariableType FindVarType(T** t){
      
      VariableType ret = FindVarType(*t);
      
      if(ret == INT) return PTRINT;
      else if(ret == FLOAT) return PTRFLOAT;
      else if(ret == DOUBLE) return PTRDOUBLE;
      else if(ret == BOOL) return PTRBOOL;

      else if(ret == VECINT) return PTRVECINT;
      else if(ret == VECFLOAT) return PTRVECFLOAT;
      else if(ret == VECDOUBLE) return PTRVECDOUBLE;
      else if(ret == VECBOOL) return PTRVECBOOL;

      else if(ret == VECVECINT) return PTRVECVECINT;
      else if(ret == VECVECFLOAT) return PTRVECVECFLOAT;
      else if(ret == VECVECDOUBLE) return PTRVECVECDOUBLE;
      else if(ret == VECVECBOOL) return PTRVECVECBOOL;

      else if(ret == AOBJ) return PTRAOBJ;
      else if(ret == VECAO) return PTRVECAO;

      else{
	std::cerr << " VariableDef::FindVarType(pointer) --> ERROR: type "<<ret<<" return type from VariableDef::FindVarType is not recognised"<<std::endl;
	exit(1);
      }
      return UNKNOWN; // CHeck if required

    }



 protected:
    //
    // Data members
    //
    std::string m_name;
    std::string m_title;
    VariableType m_varType;
    std::string m_varTypeString;
    int m_vec_ind;
    void *m_address;
    bool m_isPrimitive;
    bool m_isPointer;
    bool m_isVector;
    bool m_isAnaObject;

    std::string m_moment; //for analysis objects;
    float* m_float_val_store; 
    double* m_val_store; 
    std::vector<double>* m_vec_store; 
    int m_vec_size;
    bool m_fill_vec;        
    bool m_valid_value; //whether value in store is sensible 
    double m_default; //a dummy value to use as default, especially for trees
};


#endif
