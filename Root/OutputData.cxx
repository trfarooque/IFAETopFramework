#include "IFAETopFramework/OutputData.h"


//__________________________________________________________
//
OutputData::OutputData():
o_eventWeight_Nom(0.)
{}

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
