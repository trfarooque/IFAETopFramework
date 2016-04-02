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
OutputManager::OutputManager( OptionsBase* opt ):
m_opt(opt),
m_sysMap(0),
m_data(0),
m_mapHasSyst(0),
m_weightVarName("weight")
{
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Entering in OutputManager constructor" << std::endl;
    
    m_mapHasSyst    = new std::map <std::string,bool>();
    m_mapHasSyst    -> clear();
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager constructor" << std::endl;
}

//______________________________________________________________________________________
//
OutputManager::OutputManager( const OutputManager &q )
{
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputManager copy-constructor" << std::endl;
    
    m_opt           = q.m_opt;
    m_sysMap        = q.m_sysMap;
    m_data          = q.m_data;
    m_mapHasSyst    = q.m_mapHasSyst;
    m_weightVarName = q.m_weightVarName;
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager copy-constructor" << std::endl;
}

//______________________________________________________________________________________
//
OutputManager::~OutputManager()
{
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputManager destructor" << std::endl;
    delete m_mapHasSyst;
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager destructor" << std::endl;
}

//______________________________________________________________________________________
//
bool OutputManager::SetSystMap( WeightManager::WeightMap *sysMap ){
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputManager::SetSystMap()" << std::endl;
    
    m_sysMap = sysMap;
    return true;
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::SetSystMap()" << std::endl;
}
