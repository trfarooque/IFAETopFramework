#ifndef OUTPUTTREEMANAGER_H
#define OUTPUTTREEMANAGER_H

//ROOT libraries
#include "TString.h"

//Standard libraries
#include <map>
#include <set>
#include <vector>

//IFAEFramework classes
#include "IFAETopFramework/SystManager.h"
#include "IFAETopFramework/VariableDef.h"
#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/OutputManager.h"

class TreeManager;
class OutputData;

class OutputTreeManager : public OutputManager {
    
public:

    //_________________________
    // Typedefs
    typedef std::map < TString, VariableDef* > StdBranches;
    
    //________________________
    // Member functions
    OutputTreeManager(OptionsBase *opt);
    OutputTreeManager( const OutputTreeManager &q );
    ~OutputTreeManager();
    
    //________________________
    // Inline functions
    inline bool SetData( OutputData *data ){ m_data = data; return true;}
    inline TreeManager* TreeMngr(){ return m_treeMngr; }
    inline void SetWeightBranchName( const std::string &name ) { m_weightVarName = name; }
    
    //
    //___________________________________________________________
    // TREE-SPECIFIC FUNCTIONS
    //
    //
    template< typename T > bool AddStandardBranch( const TString &name,  const TString &title, const TString &variableType,
						   T *t, const int vec_ind = -1, const std::string moment="") {
        
        if(m_opt -> MsgLevel() == Debug::DEBUG){
            std::cout << "In OutputManager::addStandardBranch" << std::endl;
            std::cout << "Adding variable: "<< name << std::endl;
            std::cout << "  title  = " << title << std::endl;
            std::cout << "  address  = " << t << std::endl;
        }
        if(!t) std::cerr << "<!> ERROR in OutputManager::addStandardBranch(template): I cannot access the pointer (" << t << "). Please check !" << std::endl;
	bool fill_vec = false;
	if( variableType == "VAO" && (vec_ind < 0) ){fill_vec = true;}
        VariableDef *_var = new VariableDef(name, title, variableType, t, vec_ind, moment, fill_vec);
        m_stdBranchDef -> insert( std::pair < TString, VariableDef* >( name, _var ) );
        
        if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::addStandardTH1" << std::endl;
        
        return true;
    }
    bool BookStandardTree( const TString &pattern, const TString &title);
    bool FillStandardTree( const TString &name );
    bool SaveStandardTree( const TString& );
    
    
private:
    

private:

    StdBranches *m_stdBranchDef;
    TreeManager *m_treeMngr;
    
};

#endif // OUTPUTMANAGER_H





