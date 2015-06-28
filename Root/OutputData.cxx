#include "IFAETopFramework/OutputData.h"


//__________________________________________________________
//
OutputData::OutputData():
finalEvent_weightNom(0.)
{}

//__________________________________________________________
//
OutputData::~OutputData()
{}

//__________________________________________________________
//
void OutputData::ClearOutputData()
{
    finalEvent_weightNom = 0;
}

//__________________________________________________________
//
void OutputData::EmptyOutputData()
{}
