#include "IFAETopFramework/OutputData.h"

//__________________________________________________________
//
OutputData::OutputData():
o_eventWeight_Nom(0.)
{}

//__________________________________________________________
//
OutputData::OutputData( const OutputData & q){
    o_eventWeight_Nom = q.o_eventWeight_Nom;
}

//__________________________________________________________
//
OutputData::~OutputData()
{}

//__________________________________________________________
//
void OutputData::ClearOutputData()
{
    o_eventWeight_Nom = 0;
}

//__________________________________________________________
//
void OutputData::EmptyOutputData()
{}
