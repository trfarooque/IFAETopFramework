#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H

//ROOT libraries
#include "TString.h"

//Standard libraries
#include <map>
#include <vector>

//IFAEFramework classes
#include "IFAETopFramework/SystManager.h"
#include "IFAETopFramework/VariableDef.h"
#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/HistManager.h"

class TreeManager;
class OutputData;

class OutputManager {
    
public:
    
    //________________________
    //Some enums
    enum OutputType{
        HISTOS=1,
        TREES,
        ALL
    };
    
    //________________________
    // Structs to make the code readable
    struct h1Def{
        VariableDef var;
        double width;
        double min;
        double max;
        bool hasSyst;
    };
    
    struct h2Def{
        VariableDef varX;
        double widthX;
        double minX;
        double maxX;
        VariableDef varY;
        double widthY;
        double minY;
        double maxY;
        bool hasSyst;
    };
    
    //_________________________
    // Typedefs
    typedef std::map < TString, h1Def* > StdTH1;
    typedef std::map < TString, h2Def* > StdTH2;
    typedef std::map < TString, VariableDef* > StdBranches;
    
    //________________________
    // Member functions
    OutputManager(OptionsBase *opt, OutputType type);
    OutputManager( const OutputManager &q );
    ~OutputManager();
    
    //________________________
    // Inline functions
    bool SetSystVector( SystManager::SystVector *sysVector ){ m_sysVector = sysVector; return true;}
    bool SetData( OutputData *data ){ m_data = data; return true;}
    HistManager* HistMngr(){ return m_histMngr; }
    
    //
    //___________________________________________________________
    // TH1-SPECIFIC FUNCTIONS
    //
    //
    
    //___________________________________________________
    //
    template< typename T > bool AddStandardTH1( const TString &name, const double width, const double min,
                                               const double max, const TString &title, const TString &variableType,
                                               const bool hasSyst, T *t, const int vec_ind = -1) {
        
        if(m_type==TREES){
            std::cout << "<!> ERROR in OutputManager::addStandardTH1 : You requested the booking of a histogram, but OutputManager is in TREE mode." << std::endl;
            return false;
        }
        
        if(!t) std::cerr << "<!> ERROR in OutputManager::addStandardTH1(template): I cannot access the pointer (" << t << "). Please check !" << std::endl;
        
        bool added = AddStandardTH1(name, width, min, max, hasSyst);
        if(!added) {
            std::cerr << "<!> ERROR in OutputManager::addStandardTH1(template): could not add the variable !! Please check." << std::endl;
            return false;
        }
        
        VariableDef _var(name, title, variableType, t, vec_ind);
        
        std::map< TString , h1Def*>::iterator it = m_stdTH1Def -> find(name);
        if(it != m_stdTH1Def->end()){
            m_stdTH1Def->at(name)->var = _var;
        }
        else {
            std::cerr << "<!> ERROR in OutputManager::addStandardTH1(template): The searched variable has not been booked previously. Please check !" << std::endl;
        }
        
        return true;
    }
    bool BookStandardTH1( const TString &pattern, const bool hasSyst = false);
    bool FillStandardTH1( const TString &name );
    bool SaveStandardTH1( const TString& );
    
    
    //
    //___________________________________________________________
    // TH2-SPECIFIC FUNCTIONS
    //
    //
    template< typename TX, typename TY > bool AddStandardTH2( const TString &nameX, const TString &nameY,
                                                             const double widthX, const double minX, const double maxX,
                                                             const double widthY, const double minY, const double maxY,
                                                             const TString &titleX, const TString &titleY, const TString &variableTypeX, const TString &variableTypeY,
                                                             const bool hasSyst, TX *tX, TY *tY, const int vec_indX = -1, const int vec_indY = -1) {
        
        if(m_type==TREES){
            std::cout << "<!> ERROR in OutputManager::addStandardTH2 : You requested the booking of a histogram, but OutputManager is in TREE mode." << std::endl;
            return false;
        }
        
        if(!tX ) std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): I cannot access the pointer (" << tX << "). Please check !" << std::endl;
        if(!tY ) std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): I cannot access the pointer (" << tY << "). Please check !" << std::endl;
        
        TString name = nameY + "_vs_" + nameX;
        bool added = AddStandardTH2(name, widthX, minX, maxX, widthY, minY, maxY, hasSyst);
        if(!added) {
            std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): could not add the variable !! Please check." << std::endl;
            return false;
        }
        
        VariableDef _varX(nameX,"", variableTypeX, tX, vec_indX);
        VariableDef _varY(nameY,"", variableTypeY, tY, vec_indY);
        
        std::map< TString , h2Def*>::iterator it = m_stdTH2Def -> find(name);
        if(it != m_stdTH2Def->end()){
            m_stdTH2Def->at(name)->varX = _varX;
            m_stdTH2Def->at(name)->varY = _varY;
        }
        
        else std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): The searched variable has not been booked previously. Please check !" << std::endl;
        return true;
    }
    bool BookStandardTH2( const TString &pattern, const bool hasSyst = false);
    bool FillStandardTH2( const TString &name );
    bool SaveStandardTH2( const TString& );
    
    
    //
    //___________________________________________________________
    // TREE-SPECIFIC FUNCTIONS
    //
    //
    template< typename T > bool AddStandardBranch( const TString &name,  const TString &title, const TString &variableType,
                                                  T *t, const int vec_ind = -1) {
        
        if(m_type==HISTOS){
            std::cout << "<!> ERROR in OutputManager::addStandardTH2 : You requested the booking of a branch, but OutputManager is in HISTOS mode." << std::endl;
            return false;
        }
        
        if(m_opt -> MsgLevel() == Debug::DEBUG){
            std::cout << "In OutputManager::addStandardBranch" << std::endl;
            std::cout << "Adding variable: "<< name << std::endl;
            std::cout << "  title  = " << title << std::endl;
        }
        if(!t) std::cerr << "<!> ERROR in OutputManager::addStandardBranch(template): I cannot access the pointer (" << t << "). Please check !" << std::endl;
        VariableDef *_var = new VariableDef(name, title, variableType, t, vec_ind);
        m_stdBranchDef -> insert( std::pair < TString, VariableDef* >( name, _var ) );
        
        if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::addStandardTH1" << std::endl;
        
        return true;
        
        
    }
    bool BookStandardTree( const TString &pattern, const TString &title);
    bool FillStandardTree( const TString &name ); //probably do not need this one
    bool SaveStandardTree( const TString& );
    
    
private:
    bool AddStandardTH1(const TString name, const double width, const double min, const double max, const bool hasSyst);
    bool AddStandardTH2(const TString name, const double widthX, const double minX, const double maxX,
                        const double widthY, const double minY, const double maxY, const bool hasSyst);

    bool FillTH1FromVector( void* t, const VariableDef::VariableType type, const TString &histName, const double weight, const int index = -1 );
    
private:
    OptionsBase *m_opt;
    OutputType m_type;
    StdTH1 *m_stdTH1Def;
    StdTH2 *m_stdTH2Def;
    StdBranches *m_stdBranchDef;
    
    HistManager *m_histMngr;
    TreeManager *m_treeMngr;
    
    SystManager::SystVector *m_sysVector;
    OutputData *m_data;
    std::map < TString, bool > *m_mapHasSyst;
};
#endif





