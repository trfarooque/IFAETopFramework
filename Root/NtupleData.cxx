#include <iostream>
#include "IFAETopFramework/NtupleData.h"

using namespace std;


//________________________________________________________________________
//
NtupleData::NtupleData() : 
  d_D_weight_components(NULL), 
  d_F_weight_components(NULL), 
  d_vecD_weight_components(NULL), 
  d_vecF_weight_components(NULL) 
{ }

//________________________________________________________________________
//
NtupleData::~NtupleData()
{ 
  delete d_D_weight_components; 
  delete d_F_weight_components; 
  delete d_vecD_weight_components; 
  delete d_vecF_weight_components; 

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
