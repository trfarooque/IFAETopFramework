#ifndef SYSTMANAGER_H
#define SYSTMANAGER_H

#include "IFAETopFramework/CommonConstants.h"
#include <string>
#include <vector>

class OptionsBase;

class SystManager {
    
public:
    
    //
    // Some structs to make the code readable
    //
    typedef VariableDef Systematic;
    typedef std::map< std::string, Systematic* > SystVector;
    
    //
    // Standard C++ functions
    //
    SystManager( OptionsBase *opt );
    ~SystManager();
    
    //
    // Inline functions
    //
    inline SystVector* GetSystVector() { return m_systVector; }
    
    //
    // Specific functions
    //
    template< typename T > bool AddSystematic( const std::string &name, T *t, int type ) {
        
        if(!t) std::cerr << "<!> ERROR in SystManager::AddSystematic(template): I cannot access the pointer (" << t << "). Please check !" << std::endl;
        Systematic *sys = new Systematic(name, name, type, t);
        
        std::map< std::string , Systematic*>::iterator it = m_systVector -> find(name);
        if(it != m_systVector->end()){
            std::cerr << "<!> WARNING in SystManager::AddSystematic(template): I will replace an existing systematic (" << name << "). Please check !!" << std::endl;
            m_systVector[name] = sys;
        }
        else {
            m_systVector.insert ( std::pair < std::string, Systematic* > ( name, sys ) );
        }
        return true;
    }

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
