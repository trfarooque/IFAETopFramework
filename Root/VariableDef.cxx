#include "IFAETopFramework/VariableDef.h"
#include <iostream>

//_____________________________________________________________________________________
//
VariableDef::VariableDef():
m_name(""),
m_title(""),
m_varTypeString(""),
m_varType(VariableType::DOUBLE),
m_vec_ind(-1),
m_address(NULL),
m_isPrimitive(true)
{}

//_____________________________________________________________________________________
//
VariableDef::~VariableDef()
{}

//_____________________________________________________________________________________
//
VariableDef::VariableDef( VariableDef &q ){
    m_name  = q.m_name;
    m_title = q.m_title;
    m_varTypeString = q.m_varTypeString;
    m_varType = q.m_varType;
    m_vec_ind = q.m_vec_ind;
    m_address = q.m_address;
    m_isPrimitive = q.m_isPrimitive;
}

//_____________________________________________________________________________________
//
TString VariableDef::getVarTypeString(int varType){
    
    TString _varTypeString = "";
    
    if(varType == VariableType::INT){_varTypeString = "I";}
    else if(varType == VariableType::FLOAT){_varTypeString = "F";}
    else if(varType == VariableType::DOUBLE){_varTypeString = "D";}
    else if(varType == VariableType::VECINT){_varTypeString = "VI";}
    else if(varType == VariableType::VECFLOAT){_varTypeString = "VF";}
    else if(varType == VariableType::VECDOUBLE){_varTypeString = "VD";}
    else if(varType == VariableType::VECVECINT){_varTypeString = "VVI";}
    else if(varType == VariableType::VECVECFLOAT){_varTypeString = "VVF";}
    else if(varType == VariableType::VECVECDOUBLE){_varTypeString = "VVD";}
    else { std::cout << "Unknown variable type" << std::endl; }
    
    return _varTypeString;
}

//_____________________________________________________________________________________
//
VariableDef::VariableType VariableDef::getVarType(TString varTypeString){
    VariableType _varType;
    if(varTypeString == "I"){_varType = VariableType::INT;}
    else if(varTypeString == "F"){_varType = VariableType::FLOAT;}
    else if(varTypeString == "D"){_varType = VariableType::DOUBLE;}
    else if(varTypeString == "VI"){_varType = VariableType::VECINT;}
    else if(varTypeString == "VF"){_varType = VariableType::VECFLOAT;}
    else if(varTypeString == "VD"){_varType = VariableType::VECDOUBLE;}
    else if(varTypeString == "VVI"){_varType = VariableType::VECVECINT;}
    else if(varTypeString == "VVF"){_varType = VariableType::VECVECFLOAT;}
    else if(varTypeString == "VVD"){_varType = VariableType::VECVECDOUBLE;}
    
    return _varType;
}


//_____________________________________________________________________________________
//
bool VariableDef::isPrimitive(int varType){
    
    bool _isPrimitive = false;
    _isPrimitive = (varType == VariableType::INT || varType == VariableType::FLOAT || varType == VariableType::DOUBLE);
    return _isPrimitive;
}

//_____________________________________________________________________________________
//
double VariableDef::GetDoubleValue(){
    
    double value = 0.;
    
    if(m_address == NULL){std::cout << "Error: VariableDef points to NULL" << std::endl; return value;}
    
    if(m_varType == VariableType::DOUBLE){
        value = *(double*)(m_address);
    }
    else if(m_varType == VariableType::FLOAT){
        value = (double)(*(float*)(m_address));
    }
    else if(m_varType == VariableType::INT){
        value = (double)(*(int*)(m_address));
    }
    else{
        if(m_vec_ind < 0){std::cout<<"Error : Please provide vector index to obtain value from VariableDef" << std::endl; return value;}
        
        if(m_varType == VariableType::VECDOUBLE){
            if( ((std::vector<double>*)m_address)->size() > m_vec_ind ){
                value = ((std::vector<double>*)m_address)->at(m_vec_ind);
            }
        }
        else if(m_varType == VariableType::VECFLOAT){
            if( ((std::vector<float>*)m_address)->size() > m_vec_ind ){
                value = (double)( ((std::vector<float>*)m_address)->at(m_vec_ind) );
            }
        }
        else if(m_varType == VariableType::VECINT){
            if( ((std::vector<int>*)m_address)->size() > m_vec_ind ){
                value = (double)( ((std::vector<int>*)m_address)->at(m_vec_ind) );
            }
        }
        
    }//Vector variables
    return value;
}
