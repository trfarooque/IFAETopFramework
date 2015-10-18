#include "IFAETopFramework/SystManager.h"
#include "IFAETopFramework/OptionsBase.h"

//________________________________________________________________________
//
SystManager::SystManager( OptionsBase *opt ):
m_systVec(0),
m_opt(opt)
{
    if(!m_systVec){
        m_systVec = new SystVector();
    }
}

//________________________________________________________________________
//
SystManager::SystManager( const SystManager &q ){
    m_systVec = q.m_systVec;
    m_opt = q.m_opt;
}

//________________________________________________________________________
//
SystManager::~SystManager(){
    m_systVec -> clear();
}


//________________________________________________________________________
//
void SystManager::Print() const {
    
    if(!m_systVec) {
        std::cerr << "<!> ERROR in SystManager::PrintSystematics(): ";
        std::cerr << "the systematics pointer is NULL: please check." << std::endl;
        return;
    }
    
    std::cout << "=====================================================" << std::endl;
    std::cout << "Systematics list" << std::endl;
    std::cout << "=====================================================" << std::endl;
    std::cout << std::endl;
    for ( const std::pair < std::string, Systematic* > sys : *m_systVec ){
        std::cout << sys.first << std::endl;
    }
}
