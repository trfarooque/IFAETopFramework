#include "IFAETopFramework/SystManager.h"
#include "IFAETopFramework/OptionsBase.h"

//________________________________________________________________________
//
SystManager::SystManager( OptionsBase *opt ):
m_systVector(0),
m_opt(opt)
{
    if(!m_systVector){
        m_systVector = new SystVector();
    }
}

//________________________________________________________________________
//
SystManager::SystManager( const SystManager &q ){
    m_systVector = q.m_systVector;
    m_opt = q.m_opt;
}

//________________________________________________________________________
//
SystManager::~SystManager(){
    m_systVector -> clear();
}


//________________________________________________________________________
//
void SystManager::Print() const {
    
    if(!m_systVector) {
        std::cerr << "<!> ERROR in SystManager::PrintSystematics(): the systematics pointer is NULL: please check." << std::endl;
        return;
    }
    
    std::cout << "=====================================================" << std::endl;
    std::cout << "Systematics list" << std::endl;
    std::cout << "=====================================================" << std::endl;
    std::cout << std::endl;
    for ( const auto sys : *m_systVector ){
        std::cout << sys.first << std::endl;
    }
}