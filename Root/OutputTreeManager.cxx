//Standard includes
#include <iostream>
#include <string>

//IFAETopFrameowrk includes
#include "IFAETopFramework/OutputTreeManager.h"
#include "IFAETopFramework/OutputData.h"
#include "IFAETopFramework/TreeManager.h"
#include "IFAETopFramework/AnalysisObject.h"
//ROOT libraries
#include <string>
#include "TSystem.h"

//______________________________________________________________________________________
//
OutputTreeManager::OutputTreeManager( OptionsBase* opt ):
  OutputManager(opt),
  m_stdBranchDef(0),
  m_treeMngr(0)
{
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Entering in OutputTreeManager constructor" << std::endl;
    
    m_stdBranchDef  = new StdBranches();
    m_treeMngr      = new TreeManager();
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputTreeManager constructor" << std::endl;
}

//______________________________________________________________________________________
//
OutputTreeManager::OutputTreeManager( const OutputTreeManager &q ) :
  OutputManager(q)
{
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputTreeManager copy-constructor" << std::endl;
    
    m_stdBranchDef  = q.m_stdBranchDef;
    m_treeMngr      = q.m_treeMngr;
    m_weightVarName = q.m_weightVarName;
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputTreeManager copy-constructor" << std::endl;
}

//______________________________________________________________________________________
//
OutputTreeManager::~OutputTreeManager()
{
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputTreeManager destructor" << std::endl;
    delete m_stdBranchDef;
    delete m_treeMngr;
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputTreeManager destructor" << std::endl;
}


//-----------------------------TREE-SPECIFIC METHODS-------------------------------
bool OutputTreeManager::SetSystMap( WeightManager::WeightMap *sysMap ){
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputTreeManager::SetSystVector()" << std::endl;
    OutputManager::SetSystMap( sysMap );   

    AddStandardBranch( m_weightVarName, "Nominal weight", "D", &(m_data -> o_eventWeight_Nom) );
    if(m_sysMap){
      for( const auto &sys : *m_sysMap ){
	if(sys.second -> IsPrimitive()){
	  std::string branchName = m_weightVarName + "_" + sys.second -> Name();
	  AddStandardBranch( branchName, sys.second -> Title(), sys.second -> VarTypeString(), sys.second -> Address() );
	} 
	else {
	  std::cout << "<!> In OutputTreeManager::SetSystVector(): you are not allowed to use non-primitive types for systematic weights yet ... You can kindly ask :-)" << std::endl;
	}
      }
    }

    return true;
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputTreeManager::SetSystVector()" << std::endl;
}

//______________________________________________________________________________________
//
bool OutputTreeManager::BookStandardTree( const std::string &pattern, const std::string &title){
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputTreeManager::bookStandardTree" << std::endl;
    
    //Book a tree with the given name
    m_treeMngr->BookTree( pattern, title );
    
    //Loop over the list of standard branches and add those branches to the tree
    for ( const auto branch : *m_stdBranchDef ){
      m_treeMngr->AddBranchToTree( pattern, *(branch.second) );
        if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Added branch : " << branch.first << std::endl;
    }
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputTreeManager::bookStandardTree" << std::endl;
    return true;
}

//______________________________________________________________________________________
//
bool OutputTreeManager::FillStandardTree( const std::string &pattern ){
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputTreeManager::fillStandardTree" << std::endl;
    for ( const auto branch : *m_stdBranchDef ){
      if( branch.second->IsAnaObject() && branch.second->IsVector() && (branch.second->VecInd() < 0) ){
	branch.second->FillVectorStore();
      }
      else if( ( branch.second->IsAnaObject() && !branch.second->IsVector() ) 
	       || ( branch.second->IsVector() && (branch.second->VecInd() >= 0) ) ){
	branch.second->CalcDoubleValue();
      }
    }

    m_treeMngr->FillTree(pattern);
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputTreeManager::fillStandardTree" << std::endl;
    return true;
}

//______________________________________________________________________________________
//
bool OutputTreeManager::SaveStandardTree( const std::string &outputName ){
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputTreeManager::SaveStandardTree" << std::endl;
    TFile *f = new TFile(outputName.c_str(),"recreate");
    std::vector<std::string> treeList = m_treeMngr->TreeKeyList();
    for( const auto treeName : treeList ){
        f -> cd();
        m_treeMngr -> Tree(treeName) -> Write();
    }
    f -> Close();
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputTreeManager::SaveStandardTree" << std::endl;
    return true;
}


