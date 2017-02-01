#ifndef OUTPUTTREEMANAGER_H
#define OUTPUTTREEMANAGER_H

//Standard libraries
#include <map>
#include <set>
#include <string>
#include <vector>

//IFAEFramework classes
#include "IFAETopFramework/WeightManager.h"
#include "IFAETopFramework/VariableDef.h"
#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/OutputManager.h"

class TreeManager;
class OutputData;

class OutputTreeManager : public OutputManager {
    
public:

    //_________________________
    // Typedefs
    typedef std::map < std::string, VariableDef* > StdBranches;
    
    //________________________
    // Member functions
    OutputTreeManager(OptionsBase *opt, OutputData* data=NULL);
    OutputTreeManager( const OutputTreeManager &q );
    ~OutputTreeManager();
    //void SetWeightBranchName( const std::string &name, const bool add_nominal);
    bool AddAllWeightBranches(  const std::string &name, WeightManager *wgtMngr, const bool add_components=true );
    bool AddWeightBranches(const std::string& name, WeightManager::WeightMap *wgtMap, bool add_components=true );

    //________________________
    // Inline functions
    inline TreeManager* TreeMngr(){ return m_treeMngr; }


    //
    //___________________________________________________________
    // TREE-SPECIFIC FUNCTIONS
    //
    //

    bool AddStandardBranch( const VariableDef* var,  const std::string &name,  const std::string &title) {
        
        if(m_opt -> MsgLevel() == Debug::DEBUG){
            std::cout << "In OutputManager::addStandardBranch from VarDef" << std::endl;
            std::cout << "Adding variable: "<< name << std::endl;
            std::cout << "title: "<< title << std::endl;
            std::cout << "var: "<< var << std::endl;
        }
        if(!var) std::cerr << "<!> ERROR in OutputManager::addStandardBranch(template): I cannot access the pointer (" << var << "). Please check !" << std::endl;

        VariableDef *_var = new VariableDef(*var);
	if(!name.empty()) _var->SetName(name);
	if(!title.empty()) _var->SetTitle(name);

	bool fill_vec = false;
	if( ((_var->VarType() == VariableDef::VECAO) || (_var->VarType() == VariableDef::PTRVECAO))  && (_var->VecInd() < 0) ){fill_vec = true;}
	_var->SetFillVec(fill_vec);
        m_stdBranchDef -> insert( std::pair < std::string, VariableDef* >( name, _var ) );
        
        if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::addStandardBranch" << std::endl;
        
        return true;
    }

    template< typename T > bool AddStandardBranch( const std::string &name,  const std::string &title, T *t, const int vec_ind = -1, const std::string moment="") {
        
        if(m_opt -> MsgLevel() == Debug::DEBUG){
            std::cout << "In OutputManager::addStandardBranch" << std::endl;
            std::cout << "Adding variable: "<< name << std::endl;
            std::cout << "  title  = " << title << std::endl;
            std::cout << "  address  = " << t << std::endl;
        }
        if(!t) std::cerr << "<!> ERROR in OutputManager::addStandardBranch(template): I cannot access the pointer (" << t << "). Please check !" << std::endl;

        VariableDef *_var = new VariableDef(name, title, t, vec_ind, moment);
	bool fill_vec = false;
	if( ((_var->VarType() == VariableDef::VECAO) || (_var->VarType() == VariableDef::PTRVECAO))  && (vec_ind < 0) ){fill_vec = true;}
	_var->SetFillVec(fill_vec);
        m_stdBranchDef -> insert( std::pair < std::string, VariableDef* >( name, _var ) );
        
        if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::addStandardBranch" << std::endl;
        
        return true;
    }


    bool BookStandardTree( const std::string &pattern, const std::string &title);
    bool FillStandardTree( const std::string &name, const bool updateStores=true );
    bool SaveStandardTree( const std::string& );

    bool UpdateStores();    
    
private:
    

private:

    StdBranches *m_stdBranchDef;
    TreeManager *m_treeMngr;
    
};

#endif // OUTPUTMANAGER_H





