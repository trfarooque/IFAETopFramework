#include <iostream>
#include "IFAETopFramework/NtupleData.h"

using namespace std;


//________________________________________________________________________
//
NtupleData::NtupleData()
{
  d_nominal_weight_components = new std::map<std::string, double>;
  d_syst_weight_components    = new std::map<std::string, double>;
}

//________________________________________________________________________
//
NtupleData::~NtupleData()
{ 
  delete d_nominal_weight_components; 
  delete d_syst_weight_components;
}

bool NtupleData::InsertNomComponent(const std::string& name) const{

  if( d_nominal_weight_components->find(name) != d_nominal_weight_components->end() ){
    std::cerr << "<!> ERROR in NtupleData::InsertNomComponent:  nominal weight (" << name << ") exists already. Please chack !!" << std::endl;
    return false;
  }
  std::cout<<" Inserting nominal weight component ("<<name<<") into NtupleData "<<std::endl;
  d_nominal_weight_components->insert(std::pair<std::string, double>( name, 0.) );

  return true;
}

bool NtupleData::InsertSystComponent(const std::string& name) const{

  if( d_syst_weight_components->find(name) != d_syst_weight_components->end() ){
    std::cerr << "<!> ERROR in NtupleData::InsertSystComponent:  systematics weight (" << name << ") exists already. Please chack !!" << std::endl;
    return false;
  }
 
 std::cout<<" Inserting systematic weight component ("<<name<<") into NtupleData "<<std::endl;
  d_syst_weight_components->insert(std::pair<std::string, double>( name, 0.) );

  return true;
}

/*
//________________________________________________________________________
//
void NtupleData::ClearNtupleData()
{}

//________________________________________________________________________
//
void NtupleData::EmptyNtupleData()
{}
*/
