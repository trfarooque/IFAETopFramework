#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H

//Standard libraries
#include <map>
#include <set>
#include <string>
#include <vector>

//IFAEFramework classes

class OptionsBase;
class OutputData;

#include "IFAETopFramework/WeightManager.h"
class OutputManager {
    
public:
    
    // Member functions
    OutputManager( OptionsBase *opt );
    OutputManager( const OutputManager &q );
    virtual ~OutputManager();
    
    virtual bool SetSystMap( WeightManager::WeightMap *sysMap );
    //________________________
    // Inline functions
    inline bool SetData( OutputData *data ){ m_data = data; return true;}
    
protected:
    OptionsBase *m_opt;
    WeightManager::WeightMap *m_sysMap;
    OutputData *m_data;
    std::map < std::string, bool > *m_mapHasSyst;
    std::string m_weightVarName;
};

#endif // OUTPUTMANAGER_H





