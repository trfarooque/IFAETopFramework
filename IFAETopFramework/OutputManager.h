#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H

//ROOT libraries
#include "TString.h"

//Standard libraries
#include <map>
#include <set>
#include <vector>

//IFAEFramework classes

class OptionsBase;
class OutputData;

#include "IFAETopFramework/SystManager.h"
class OutputManager {
    
public:
    
    // Member functions
    OutputManager( OptionsBase *opt );
    OutputManager( const OutputManager &q );
    ~OutputManager();
    
    bool SetSystVector( SystManager::SystVector *sysVector );
    //________________________
    // Inline functions
    inline bool SetData( OutputData *data ){ m_data = data; return true;}
    
protected:
    OptionsBase *m_opt;
    SystManager::SystVector *m_sysVector;
    OutputData *m_data;
    std::map < TString, bool > *m_mapHasSyst;
    std::string m_weightVarName;
};

#endif // OUTPUTMANAGER_H





