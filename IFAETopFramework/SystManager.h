#ifndef SYSTMANAGER_H
#define SYSTMANAGER_H

#include "IFAETopFramework/CommonConstants.h"
#include <string>
#include <vector>

class NtupleData;
class OptionsBase;

class SystManager {
    
public:
    
    struct Systematic {
        WeightSys::wgtSys sysType;
        std::string name;
        double value;
    };
    typedef std::vector<Systematic*> SystVector;
    
    SystManager( OptionsBase *opt );
    ~SystManager();
    
    inline SystVector* GetSystVector() { return m_systVector; }
    
    bool AddToSystVector( const WeightSys::wgtSys &sys_type, const std::string &name) const;
    bool LoadWeightSysts() const;
    bool ComputeWeights( const NtupleData *data) const;
    bool ClearWeights() const;
    bool Clear() const;
    
private:
    SystVector* m_systVector;
    OptionsBase* m_opt;
    
};

#endif
