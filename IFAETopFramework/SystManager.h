#ifndef SYSTMANAGER_H
#define SYSTMANAGER_H

#include <iostream>
#include <string>
#include <map>
#include "TString.h"

class OptionsBase;
class VariableDef;

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
    SystManager( const SystManager & );
    ~SystManager();
    
    //
    // Inline functions
    //
    inline SystVector* SystVec() { return m_systVec; }
    
    //
    // Specific functions
    //
    void Print() const;
    
    template< typename T > bool AddSystematic( const std::string &name, T *t, TString type ) {
        if(!t) std::cerr << "<!> ERROR in SystManager::AddSystematic(template): I cannot access the pointer (" << t << "). Please check !" << std::endl;
        Systematic *sys = new Systematic(name, name, type, t);
        
        std::map< std::string , Systematic*>::iterator it = m_systVec -> find(name);
        if(it != m_systVec->end()){
            std::cerr << "<!> WARNING in SystManager::AddSystematic(template): I will replace an existing systematic (" << name << "). Please check !!" << std::endl;
            std::cerr << "    Please use the function SystManager::UpdateSystematic(template) to do so !" << std::endl;
            m_systVec -> at(name) = sys;
        }
        else {
            m_systVec -> insert ( std::pair < std::string, Systematic* > ( name, sys ) );
        }
        return true;
    }
    
    template< typename T > bool UpdateSystematic( const std::string &name, T *t, TString type ) {
        if(!t) std::cerr << "<!> ERROR in SystManager::AddSystematic(template): I cannot access the pointer (" << t << "). Please check !" << std::endl;
        Systematic *sys = new Systematic(name, name, type, t);
        
        std::map< std::string , Systematic*>::iterator it = m_systVec -> find(name);
        if(it != m_systVec->end()){
            std::cerr << "<!> WARNING in SystManager::AddSystematic(template): I will replace an existing systematic (" << name << "). Please check !!" << std::endl;
            std::cerr << "    Please use the function SystManager::UpdateSystematic(template) to do so !" << std::endl;
            m_systVec -> at(name) = sys;
        }
        else {
            m_systVec -> insert ( std::pair < std::string, Systematic* > ( name, sys ) );
        }
        return true;
    }
    
private:
    SystVector* m_systVec;
    OptionsBase* m_opt;
    
};

#endif
