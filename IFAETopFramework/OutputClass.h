#ifndef OUTPUTCLASS_H
#define OUTPUTCLASS_H

//ROOT libraries
#include "TString.h"

//Standard libraries
#include <map>
#include <vector>

//IFAEFramework classes
#include "IFAETopFramework/SystManager.h"
class OptionsBase;
class HistManager;
class NtupleData;

class OutputClass {
    
public:
    
    //________________________
    //Some enums
    enum OutputType{
        HISTOS=1,
        TREES,
        ALL
    };
    enum VariableType{
        INT=1,
        FLOAT,
        DOUBLE,
        VECINT,
        VECFLOAT,
        VECDOUBLE
    };
    
    //________________________
    // Struct to make the code readable
    struct h1Def{
        TString name;
        double width;
        double min;
        double max;
        TString title;
        VariableType varType;
        bool hasSyst;
        void *value;
        int component;
    };
    typedef std::map < TString, h1Def* > StdHistos;

    //________________________
    // Member functions
    OutputClass(OptionsBase *opt, OutputType type);
    OutputClass( const OutputClass &q );
    ~OutputClass();
    
    //________________________
    // Inline functions
    bool setSystVector( SystManager::SystVector *sysVector ){ m_sysVector = sysVector; return true;}
    bool setData( NtupleData *data ){ m_data = data; return true;}
    
    //
    //___________________________________________________________
    // TH1-SPECIFIC FUNCTIONS
    //
    //
    template< typename T > bool addStandardTH1( const TString &name, const double width, const double min,
                                                const double max, const TString &title, const TString &variableType,
                                                const bool hasSyst, T *t, const int component = -1) {
        
        if(!t) std::cerr << "<!> ERROR in OutputClass::addStandardTH1(template): I cannot access the pointer (" << t << "). Please check !" << std::endl;
        
        bool added = addStandardTH1(name, width, min, max, title, variableType, hasSyst, component);
        if(!added) {
            std::cerr << "<!> ERROR in OutputClass::addStandardTH1(template): could not add the variable !! Please check." << std::endl;
            return false;
        }
        std::map< TString , h1Def*>::iterator it = m_stdh1Def -> find(name);
        if(it != m_stdh1Def->end()) m_stdh1Def->at(name)->value = (void*)t;
        else std::cerr << "<!> ERROR in OutputClass::addStandardTH1(template): The searched variable has not been booked previously. Please check !" << std::endl;
        return true;
    }
    bool bookStandardTH1( const TString &pattern, const bool hasSyst = false);
    bool fillStandardTH1( const TString &name );
    bool saveStandardTH1( const TString& );
    
private:
    bool addStandardTH1(const TString &name, const double width, const double min, const double max,
                        const TString &title, const TString &variableType, const bool hasSyst,
                        const int component = -1);
    
private:
    OptionsBase *m_opt;
    OutputType m_type;
    StdHistos *m_stdh1Def;
    HistManager *m_histMngr;
    SystManager::SystVector *m_sysVector;
    NtupleData *m_data;
    std::map < TString, bool > *m_mapHasSyst;
};
#endif





