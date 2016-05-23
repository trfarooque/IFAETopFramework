#include "IFAETopFramework/OutputData.h"
#include <iostream>
//__________________________________________________________
//
OutputData::OutputData():
  o_D_weight_components(NULL),
  o_F_weight_components(NULL),
  o_vecD_weight_components(NULL),
  o_vecF_weight_components(NULL),
  o_eventWeight_Nom(0.),
  o_eventWeight_Systs(NULL),
  o_sel_decisions(NULL),
  o_TRF_bjets_n(0),
  o_TRF_isIncl(true)
{
  o_eventWeight_Systs = new std::map<std::string, double>;
}

//__________________________________________________________
//
OutputData::OutputData( const OutputData & q){
  o_D_weight_components      = q.o_D_weight_components;
  o_F_weight_components      = q.o_F_weight_components;
  o_vecD_weight_components   = q.o_vecD_weight_components;
  o_vecF_weight_components   = q.o_vecF_weight_components;
  o_eventWeight_Nom          = q.o_eventWeight_Nom;
  o_eventWeight_Systs        = q.o_eventWeight_Systs;
  o_sel_decisions            = q.o_sel_decisions;
  o_TRF_bjets_n              = q.o_TRF_bjets_n;     
  o_TRF_isIncl               = q.o_TRF_isIncl;     
}

//__________________________________________________________
//
OutputData::~OutputData()
{
  if(o_eventWeight_Systs){
    delete o_eventWeight_Systs;
  }
  if(o_D_weight_components){
    delete o_D_weight_components;
  }
  if(o_F_weight_components){
    delete o_F_weight_components;
  }
  if(o_vecD_weight_components){
    for(std::pair<std::string, std::vector<double>* > wgt : *o_vecD_weight_components){
      wgt.second->clear(); delete wgt.second;
    }
    o_vecD_weight_components->clear();
    delete o_vecD_weight_components;
  }

  if(o_vecF_weight_components){
    for(std::pair<std::string, std::vector<float>* > wgt : *o_vecF_weight_components){
      wgt.second->clear(); delete wgt.second;
    }
    o_vecF_weight_components->clear();
    delete o_vecF_weight_components;
  }
  if(o_sel_decisions){ delete o_sel_decisions; }

}

//__________________________________________________________
//
void OutputData::ClearOutputData()
{
  o_eventWeight_Nom = 0;
  if(o_eventWeight_Systs){
    for ( std::pair < std::string, double > wgt : *o_eventWeight_Systs ) wgt.second = 0.;
  }
  if(o_D_weight_components){
    for ( std::pair < std::string, double > wgt : *o_D_weight_components ) wgt.second = 0.;
  }
  if(o_F_weight_components){
    for ( std::pair < std::string, float > wgt : *o_F_weight_components ) wgt.second = 0.;
  }
  if(o_vecD_weight_components){
    for(std::pair<std::string, std::vector<double>* > comp : *o_vecD_weight_components){
      for(double& el : *comp.second){ el = 0.; }
    }
  }  
  if(o_vecF_weight_components){
    for(std::pair<std::string, std::vector<float>* > comp : *o_vecF_weight_components){
      for(float& el : *comp.second){ el = 0.; }
    }
  }  
  if(o_sel_decisions){
    for(std::map<int, bool>::iterator decit = o_sel_decisions->begin(); decit != o_sel_decisions->end(); ++decit){decit->second = false;}
    //for ( std::pair < int, bool > dec : *o_sel_decisions ) dec.second = false;
  }

  o_TRF_bjets_n = 0;
  o_TRF_isIncl = true;

  return;

}

//__________________________________________________________
//
void OutputData::EmptyOutputData()
{}
