//Standard includes
#include <iostream>
#include <string>

//IFAETopFramework includes
#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/OutputManager.h"
#include "IFAETopFramework/OutputData.h"
//ROOT libraries
#include "TSystem.h"

//______________________________________________________________________________________
//
OutputManager::OutputManager( OptionsBase* opt, OutputData* data ):
m_opt(opt),
m_data(data)
{}

//______________________________________________________________________________________
//
OutputManager::OutputManager( const OutputManager &q )
{
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputManager copy-constructor" << std::endl;
    
    m_opt           = q.m_opt;
    m_data          = q.m_data;
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager copy-constructor" << std::endl;
}

//______________________________________________________________________________________
//
OutputManager::~OutputManager()
{}
