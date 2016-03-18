#include "IFAETopFramework/OutputData.h"
#include <iostream>
//__________________________________________________________
//
OutputData::OutputData():
  o_eventWeight_Nom(0.),
  o_eventWeight_Systs(NULL)
{
  o_eventWeight_Systs = new std::map<std::string, double>;
  o_nominal_weight_components = new std::map<std::string, double>;
  o_syst_weight_components = new std::map<std::string, double>;

}

//__________________________________________________________
//
OutputData::OutputData( const OutputData & q){
    o_eventWeight_Nom = q.o_eventWeight_Nom;
    o_eventWeight_Systs = q.o_eventWeight_Systs;
}

//__________________________________________________________
//
OutputData::~OutputData()
{
  delete o_eventWeight_Systs;
  delete o_nominal_weight_components;
  delete o_syst_weight_components;

}


//__________________________________________________________
//
bool OutputData::InsertNomComponent(const std::string& name) const{
 
  if( o_nominal_weight_components->find(name) != o_nominal_weight_components->end() ){
    std::cerr << "<!> ERROR in OuputData::InsertNomComponent:  nominal weight (" << name << ") exists already. Please chack !!" << std::endl;
    return false;
  }
  o_nominal_weight_components->insert(std::pair<std::string, double>( name, 0.) );

  return true;
}

bool OutputData::InsertSystComponent(const std::string& name) const{
 
  if( o_syst_weight_components->find(name) != o_syst_weight_components->end() ){
    std::cerr << "<!> ERROR in OutputData::InsertSystComponent:  systematics weight (" << name << ") exists already. Please chack !!" << std::endl;
    return false;
  }
  o_syst_weight_components->insert(std::pair<std::string, double>( name, 0.) );

  return true;
}

bool OutputData::InsertSystWeight(const std::string& name) const{
 
  if( o_eventWeight_Systs->find(name) != o_eventWeight_Systs->end() ){
    std::cerr << "<!> ERROR in OutputData::InsertSystWeight:  systematics weight (" << name << ") exists already. Please chack !!" << std::endl;
    return false;
  }
  o_eventWeight_Systs->insert(std::pair<std::string, double>( name, 0.) );

  return true;
}

//__________________________________________________________
//
void OutputData::ClearOutputData()
{
    o_eventWeight_Nom = 0;
    for ( std::pair < std::string, double > component : *o_eventWeight_Systs ) o_eventWeight_Systs -> at(component.first) = 0.;
}

//__________________________________________________________
//
void OutputData::EmptyOutputData()
{}
