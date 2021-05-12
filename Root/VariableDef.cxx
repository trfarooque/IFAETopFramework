#include "IFAETopFramework/VariableDef.h"
#include "IFAETopFramework/AnalysisObject.h"
#include <iostream>

//_____________________________________________________________________________________
//
VariableDef::VariableDef():
  m_name(""),
  m_title(""),
  m_varType(VariableType::DOUBLE),
  m_varTypeString(""),
  m_vec_ind(-1),
  m_address(NULL),
  m_isPrimitive(true),
  m_isPointer(false),
  m_isVector(true),
  m_isAnaObject(true),
  m_moment(""),
  m_val_store(NULL),
  m_vec_store(NULL),
  m_vec_size(0),
  m_fill_vec(false),
  m_valid_value(true),
  m_default(0.)
{}

//_____________________________________________________________________________________
//
VariableDef::~VariableDef()
{ 
  delete m_val_store;
  delete m_vec_store;
}

//_____________________________________________________________________________________
//
VariableDef::VariableDef( const VariableDef &q ){
    m_name          = q.m_name;
    m_title         = q.m_title;
    m_varType       = q.m_varType;
    m_varTypeString = q.m_varTypeString;
    m_vec_ind       = q.m_vec_ind;
    m_address       = q.m_address;
    m_isPrimitive   = q.m_isPrimitive;
    m_isPointer     = q.m_isPointer;
    m_isVector      = q.m_isVector;
    m_isAnaObject   = q.m_isAnaObject;
    m_moment        = q.m_moment;
    m_val_store     = q.m_val_store;
    m_vec_store     = q.m_vec_store;
    m_vec_size      = q.m_vec_size;
    m_fill_vec      = q.m_fill_vec;
    m_valid_value   = q.m_valid_value;
    m_default       = q.m_default;
}

//_____________________________________________________________________________________
//
std::string VariableDef::GetVarTypeString(int varType){
    
  std::string _varTypeString = "";
    
    if     (varType == VariableType::INT)             {_varTypeString = "I";}
    else if(varType == VariableType::PTRINT)          {_varTypeString = "PI";}
    else if(varType == VariableType::UINT)            {_varTypeString = "UI";}
    else if(varType == VariableType::LONGINT)         {_varTypeString = "L";}
    else if(varType == VariableType::ULONGINT)        {_varTypeString = "UL";}
    else if(varType == VariableType::LONGLONGINT)     {_varTypeString = "LL";}
    else if(varType == VariableType::ULONGLONGINT)    {_varTypeString = "ULL";}
    else if(varType == VariableType::FLOAT)           {_varTypeString = "F";}
    else if(varType == VariableType::PTRFLOAT)        {_varTypeString = "PF";}
    else if(varType == VariableType::DOUBLE)          {_varTypeString = "D";}
    else if(varType == VariableType::PTRDOUBLE)       {_varTypeString = "PD";}
    else if(varType == VariableType::BOOL)            {_varTypeString = "B";}
    else if(varType == VariableType::PTRBOOL)         {_varTypeString = "PB";}
    else if(varType == VariableType::VECINT)          {_varTypeString = "VI";}
    else if(varType == VariableType::PTRVECINT)       {_varTypeString = "PVI";}
    else if(varType == VariableType::VECFLOAT)        {_varTypeString = "VF";}
    else if(varType == VariableType::PTRVECFLOAT)     {_varTypeString = "PVF";}
    else if(varType == VariableType::VECDOUBLE)       {_varTypeString = "VD";}
    else if(varType == VariableType::PTRVECDOUBLE)    {_varTypeString = "PVD";}
    else if(varType == VariableType::VECBOOL)         {_varTypeString = "VB";}
    else if(varType == VariableType::PTRVECBOOL)      {_varTypeString = "PVB";}
    else if(varType == VariableType::VECVECINT)       {_varTypeString = "VVI";}
    else if(varType == VariableType::PTRVECVECINT)    {_varTypeString = "PVVI";}
    else if(varType == VariableType::VECVECFLOAT)     {_varTypeString = "VVF";}
    else if(varType == VariableType::PTRVECVECFLOAT)  {_varTypeString = "PVVF";}
    else if(varType == VariableType::VECVECDOUBLE)    {_varTypeString = "VVD";}
    else if(varType == VariableType::PTRVECVECDOUBLE) {_varTypeString = "PVVD";}
    else if(varType == VariableType::VECVECBOOL)      {_varTypeString = "VVB";}
    else if(varType == VariableType::PTRVECVECBOOL)   {_varTypeString = "PVVB";}
    else if(varType == VariableType::AOBJ)            {_varTypeString = "AO";}
    else if(varType == VariableType::PTRAOBJ)         {_varTypeString = "PAO";}
    else if(varType == VariableType::VECAO)           {_varTypeString = "VAO";}
    else if(varType == VariableType::PTRVECAO)        {_varTypeString = "PVAO";}
    else { std::cout << "Unknown variable type " << varType << std::endl; }
    
    return _varTypeString;
}

//_____________________________________________________________________________________
//
VariableDef::VariableType VariableDef::GetVarType(const std::string& varTypeString){
    VariableType _varType;
    if     (varTypeString == "I")         {_varType = VariableType::INT;}
    else if(varTypeString == "PI")        {_varType = VariableType::PTRINT;}
    else if(varTypeString == "UI")        {_varType = VariableType::UINT;}
    else if(varTypeString == "L")         {_varType = VariableType::LONGINT;}
    else if(varTypeString == "UL")        {_varType = VariableType::ULONGINT;}
    else if(varTypeString == "LL")        {_varType = VariableType::LONGLONGINT;}
    else if(varTypeString == "ULL")       {_varType = VariableType::ULONGLONGINT;}
    else if(varTypeString == "F")         {_varType = VariableType::FLOAT;}
    else if(varTypeString == "PF")        {_varType = VariableType::PTRFLOAT;}
    else if(varTypeString == "D")         {_varType = VariableType::DOUBLE;}
    else if(varTypeString == "PD")        {_varType = VariableType::PTRDOUBLE;}
    else if(varTypeString == "B")         {_varType = VariableType::BOOL;}
    else if(varTypeString == "PB")        {_varType = VariableType::PTRBOOL;}
    else if(varTypeString == "VI")        {_varType = VariableType::VECINT;}
    else if(varTypeString == "PVI")       {_varType = VariableType::PTRVECINT;}
    else if(varTypeString == "VF")        {_varType = VariableType::VECFLOAT;}
    else if(varTypeString == "PVF")       {_varType = VariableType::PTRVECFLOAT;}
    else if(varTypeString == "VD")        {_varType = VariableType::VECDOUBLE;}
    else if(varTypeString == "PVD")       {_varType = VariableType::PTRVECDOUBLE;}
    else if(varTypeString == "VB")        {_varType = VariableType::VECBOOL;}
    else if(varTypeString == "PVB")       {_varType = VariableType::PTRVECBOOL;}
    else if(varTypeString == "VVI")       {_varType = VariableType::VECVECINT;}
    else if(varTypeString == "PVVI")      {_varType = VariableType::PTRVECVECINT;}
    else if(varTypeString == "VVF")       {_varType = VariableType::VECVECFLOAT;}
    else if(varTypeString == "PVVF")      {_varType = VariableType::PTRVECVECFLOAT;}
    else if(varTypeString == "VVD")       {_varType = VariableType::VECVECDOUBLE;}
    else if(varTypeString == "PVVD")      {_varType = VariableType::PTRVECVECDOUBLE;}
    else if(varTypeString == "AO")        {_varType = VariableType::AOBJ;}
    else if(varTypeString == "PAO")       {_varType = VariableType::PTRAOBJ;}
    else if(varTypeString == "VAO")       {_varType = VariableType::VECAO;}
    else if(varTypeString == "PVAO")      {_varType = VariableType::PTRVECAO;}
    else { std::cout << "Unknown variable type string " << varTypeString << std::endl; }
    
    return _varType;
}


//_____________________________________________________________________________________
//
bool VariableDef::IsPrimitive(int varType){
    
    bool _isPrimitive = false;
    _isPrimitive = (varType == VariableType::DOUBLE || varType == VariableType::FLOAT 
		    || varType == VariableType::INT || varType == VariableType::UINT 
		    || varType == VariableType::LONGINT || varType == VariableType::ULONGINT 
		    || varType == VariableType::LONGLONGINT || varType == VariableType::ULONGLONGINT 
		    || varType == VariableType::BOOL
		    || varType == VariableType::PTRDOUBLE || varType == VariableType::PTRFLOAT 
		    || varType == VariableType::PTRINT || varType == VariableType::PTRBOOL) ;
    return _isPrimitive;
}

bool VariableDef::IsPointer(int varType){
    
    bool _isPointer = false;
    _isPointer = ( varType == VariableType::PTRVECDOUBLE || varType == VariableType::PTRVECFLOAT 
		   || varType == VariableType::PTRVECINT || varType == VariableType::PTRVECBOOL
		   || varType == VariableType::PTRVECVECDOUBLE || varType == VariableType::PTRVECVECFLOAT 
		   || varType == VariableType::PTRVECVECINT || varType == VariableType::PTRVECVECBOOL
		   || varType == VariableType::PTRAOBJ || varType == VariableType::PTRVECAO
		   || varType == VariableType::PTRDOUBLE || varType == VariableType::PTRFLOAT 
		   || varType == VariableType::PTRINT || varType == VariableType::PTRBOOL );
		   
      ;
    return _isPointer;
}

bool VariableDef::IsVector(int varType){
    
    bool _isVector = false;
    _isVector = ( (varType == VariableType::VECINT) || (varType == VariableType::VECBOOL) 
		  || (varType == VariableType::VECFLOAT) || (varType == VariableType::VECDOUBLE)
		  || (varType == VariableType::VECVECINT) || (varType == VariableType::VECVECBOOL) 
		  || (varType == VariableType::VECVECFLOAT) || (varType == VariableType::VECVECDOUBLE)
		  || (varType == VariableType::VECAO)
		  || (varType == VariableType::PTRVECINT) || (varType == VariableType::PTRVECBOOL) 
		  || (varType == VariableType::PTRVECFLOAT) || (varType == VariableType::PTRVECDOUBLE)
		  || (varType == VariableType::PTRVECVECINT) || (varType == VariableType::PTRVECVECBOOL) 
		  || (varType == VariableType::PTRVECVECFLOAT) || (varType == VariableType::PTRVECVECDOUBLE)
		  || (varType == VariableType::PTRVECAO) );

    return _isVector;
}

bool VariableDef::IsAnaObject(int varType){
    
    bool _isAnaObj = false;
    _isAnaObj = ( (varType == VariableType::AOBJ) || (varType == VariableType::VECAO)
		  || (varType == VariableType::PTRAOBJ) || (varType == VariableType::PTRVECAO) );
    return _isAnaObj;
}





//
void VariableDef::FillVectorStore(){

  //Currently only call this for AOVector
  if( (m_varType == VariableDef::VECAO) || (m_varType == VariableDef::PTRVECAO) ){
    m_vec_store->clear();
    AOVector& vec = (m_varType == VariableDef::VECAO) ? *((AOVector*)m_address) : **((AOVector**)m_address);
    for ( AnalysisObject* obj : vec ){ 
      //Only use objects for which the moment exists
      if(obj -> IsKnownMoment(m_moment)){
	m_vec_store->push_back( obj->GetMoment(m_moment) );
      }
    }
  }

  return;
}

//_____________________________________________________________________________________
//
bool VariableDef::PointsToNull(){
  // Check if any PTR type variable is currently pointing to NULL
    if(m_address == NULL){std::cout << "Error: VariableDef points to NULL" << std::endl; return true;}
    
    if( (m_varType == VariableType::PTRDOUBLE)){
      return (*(double**)(m_address) == NULL);
    }
    else if(m_varType == VariableType::PTRFLOAT){
      return (*(float**)(m_address) == NULL);
    }
    else if(m_varType == VariableType::PTRINT){
      return (*(int**)(m_address) == NULL);
    }
    else if(m_varType == VariableType::PTRBOOL){
      return (*(bool**)(m_address) == NULL);
    }
    else if(m_varType == VariableType::PTRAOBJ){
      return (*(AnalysisObject**)(m_address) == NULL);
    }
    else if(m_varType == VariableType::PTRVECDOUBLE){
      return (*((std::vector<double>**)m_address) == NULL);
    }
    else if(m_varType == VariableType::PTRVECFLOAT){
      return (*((std::vector<float>**)m_address) == NULL);
    }
    else if(m_varType == VariableType::PTRVECINT){
      return (*((std::vector<int>**)m_address) == NULL);
    }
    else if(m_varType == VariableType::PTRVECBOOL){
      return (*((std::vector<bool>**)m_address) == NULL);
    }
    else if(m_varType == VariableType::PTRVECAO){
      return (*((AOVector**)m_address) == NULL);
    }

    return false;
}

//_____________________________________________________________________________________
//
AnalysisObject* VariableDef::RetrieveAnalysisObject(){

  AnalysisObject* aobj = NULL;
  if(m_varType == VariableType::AOBJ){
    aobj = (AnalysisObject*)(m_address);
  }
  else if(m_varType == VariableType::PTRAOBJ){
    aobj = *(AnalysisObject**)(m_address);
  }
  else if( (m_varType == VariableType::PTRVECAO) || (m_varType == VariableType::VECAO) ){
    const AOVector* aovec = (m_varType == VariableType::PTRVECAO) 
      ? *(AOVector**)m_address : (AOVector*)m_address;

    if( (m_vec_ind >= 0) && m_vec_ind < (int)(aovec->size()) ){
      aobj = aovec->at(m_vec_ind); 
    }
  }

  return aobj;

}

//_____________________________________________________________________________________
//
void VariableDef::CalcDoubleValue(){
    *m_val_store = m_default;
    if(m_address == NULL){std::cout << "Error: VariableDef points to NULL" << std::endl; return;}

    m_valid_value = true;

    //Check if points to null
    if(m_isPointer && PointsToNull()){
      m_valid_value = false;
      return; 
    }

    //Check if vector index is valid
    if(m_isVector && (m_vec_ind < 0)){
      std::cerr<<"ERROR : Please provide vector index to obtain value from VariableDef" << std::endl;
      m_valid_value = false;
      return;
    }

    if( m_isAnaObject ){
      AnalysisObject* aobj = RetrieveAnalysisObject();

      //Check if moment exists
      if( ! (aobj && aobj->IsKnownMoment(m_moment)) ){ 
	m_valid_value = false;
	return;
      }
      *m_val_store = aobj -> GetMoment(m_moment);

    }
    else if( (m_varType == VariableType::DOUBLE)){
        *m_val_store = *(double*)(m_address);
    }
    else if( (m_varType == VariableType::PTRDOUBLE)){
        *m_val_store = **(double**)(m_address);
    }
    else if(m_varType == VariableType::FLOAT){
        *m_val_store = (double)(*(float*)(m_address));
    }
    else if(m_varType == VariableType::PTRFLOAT){
        *m_val_store = (double)(**(float**)(m_address));
    }
    else if(m_varType == VariableType::INT){
        *m_val_store = (double)(*(int*)(m_address));
    }
    else if(m_varType == VariableType::UINT){
        *m_val_store = (double)(*(unsigned int*)(m_address));
    }
    else if(m_varType == VariableType::LONGINT){
        *m_val_store = (double)(*(long int*)(m_address));
    }
    else if(m_varType == VariableType::ULONGINT){
        *m_val_store = (double)(*(unsigned long int*)(m_address));
    }
    else if(m_varType == VariableType::LONGLONGINT){
        *m_val_store = (double)(*(long long int*)(m_address));
    }
    else if(m_varType == VariableType::ULONGLONGINT){
        *m_val_store = (double)(*(unsigned long long int*)(m_address));
    }
    else if(m_varType == VariableType::PTRINT){
        *m_val_store = (double)(**(int**)(m_address));
    }
    else if(m_varType == VariableType::BOOL){
        *m_val_store = (double)(*(bool*)(m_address));
    }
    else if(m_varType == VariableType::PTRBOOL){
        *m_val_store = (double)(**(bool**)(m_address));
    }
    else{
      if( (m_varType == VariableType::VECDOUBLE) || (m_varType == VariableType::PTRVECDOUBLE) ){
	const std::vector<double>& vecD = (m_varType == VariableType::PTRVECDOUBLE) 
	  ? **((std::vector<double>**)m_address) : *((std::vector<double>*)m_address);
	m_vec_size = (int)vecD.size();
	if( m_vec_size > m_vec_ind ){ *m_val_store = vecD.at(m_vec_ind); }
      }
      else if( (m_varType == VariableType::VECFLOAT) || (m_varType == VariableType::PTRVECFLOAT) ){
	const std::vector<float>& vecF = (m_varType == VariableType::PTRVECFLOAT) 
	  ? **((std::vector<float>**)m_address) : *((std::vector<float>*)m_address);
	m_vec_size = (int)vecF.size();
	if( m_vec_size > m_vec_ind ){ *m_val_store = (double)(vecF.at(m_vec_ind)); }
      }
      else if( (m_varType == VariableType::VECINT) || (m_varType == VariableType::PTRVECINT) ){
	const std::vector<int>& vecI = (m_varType == VariableType::PTRVECINT) 
	  ? **((std::vector<int>**)m_address) : *((std::vector<int>*)m_address);

	m_vec_size = (int)vecI.size();
	if( m_vec_size > m_vec_ind ){ *m_val_store = (double)(vecI.at(m_vec_ind)); }
      }
      else if( (m_varType == VariableType::VECBOOL) || (m_varType == VariableType::PTRVECBOOL) ){
	const std::vector<bool>& vecB = (m_varType == VariableType::PTRVECBOOL) 
	  ? **((std::vector<bool>**)m_address) : *((std::vector<bool>*)m_address);

	m_vec_size = (int)vecB.size();
	if( m_vec_size > m_vec_ind ){ *m_val_store = (double)(vecB.at(m_vec_ind)); }
      }
      if(m_vec_ind >= m_vec_size){ m_valid_value = false; }


    }//Vector variables

    return;
}
