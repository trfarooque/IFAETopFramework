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
#include "TFile.h"

//______________________________________________________________________________________
//
OutputTreeManager::OutputTreeManager( OptionsBase* opt, OutputData* data ):
  OutputManager(opt, data),
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
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputTreeManager copy-constructor" << std::endl;
}

//______________________________________________________________________________________
//
OutputTreeManager::~OutputTreeManager()
{
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputTreeManager destructor" << std::endl;

    for(std::pair<std::string, VariableDef*> branch : *m_stdBranchDef){
      delete branch.second;
    }
    delete m_stdBranchDef;
    delete m_treeMngr;
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputTreeManager destructor" << std::endl;
}


//-----------------------------TREE-SPECIFIC METHODS-------------------------------
bool OutputTreeManager::AddAllWeightBranches( const std::string &name, WeightManager* wgtMngr, const bool add_components){
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputTreeManager::AddAllWeightBranches()" << std::endl;

  bool stat = true;
  if(add_components){
    WeightManager::WeightMap* nomMap = wgtMngr->NomMap();
    if(nomMap == NULL){
      std::cerr << "Error in OutputTreeManager::AddAllWeightBranches()  : Weight components cannot be added without a nominal weight map";
      return false;
    }
    stat = AddWeightBranches(name, nomMap, true);
    if(!stat){ return stat; }
  }
  else{
    AddStandardBranch( name, "Nominal weight", "D", &(m_data -> o_eventWeight_Nom) );
  }
  if( m_opt->ComputeWeightSys() ){
    WeightManager::WeightMap* sysMap = wgtMngr->SystMap();
    if(sysMap == NULL){
      std::cerr << "Error in OutputTreeManager::AddAllWeightBranches()  : NULL systematics map found while ComputeWeightSys is TRUE. Please check" << std::endl;
      return false;
    }
    stat = AddWeightBranches(name, sysMap, add_components);
    if(!stat){ return stat; }
  }

  return true;
}    

bool OutputTreeManager::AddWeightBranches(const std::string& name, WeightManager::WeightMap *wgtMap, const bool add_components){
  for( const auto &wgt : *wgtMap ){
    std::string branchName = name + "_" + wgt.second -> Name();
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "OutputTreeManager::AddWeightBranches(); adding branch for weight "<<wgt.second->Name() << std::endl;
    if(add_components){ AddStandardBranch( branchName, wgt.second -> Title(), wgt.second -> GetComponentTypeStr(), wgt.second -> GetComponentAddress() ); }
    else{ AddStandardBranch( branchName, wgt.second -> Title(), "D", wgt.second -> GetWeightAddress() ); }
  }

  return true;
}

//______________________________________________________________________________________
//
bool OutputTreeManager::BookStandardTree( const std::string &pattern, const std::string &title){
    
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputTreeManager::bookStandardTree ( "<< pattern <<" )" << std::endl;
    //Book a tree with the given name
    TTree* _tree = m_treeMngr->BookTree( pattern, title );
    TBranch* _branch = NULL;
    //Loop over the list of standard branches and add those branches to the tree
    for ( const auto branch : *m_stdBranchDef ){

      VariableDef* bVar = branch.second;
      const char* bname = bVar->Name().c_str();

      if( bVar->IsPrimitive() || ((bVar->VecInd()<0) && !bVar->IsAnaObject() ) ){
	if(bVar->VarType() == VariableDef::DOUBLE) _branch = _tree->Branch(bname,(double*)bVar->Address() );
	else if(bVar->VarType() == VariableDef::FLOAT) _branch = _tree->Branch(bname,(float*)bVar->Address() ) ;
	else if(bVar->VarType() == VariableDef::INT) _branch = _tree->Branch(bname,(int*)bVar->Address() );
																			    
	else if(bVar->VarType() == VariableDef::PTRVECDOUBLE) _branch = _tree->Branch(bname,(std::vector<double>**)bVar->Address() );
	else if(bVar->VarType() == VariableDef::PTRVECFLOAT) _branch = _tree->Branch(bname,(std::vector<float>**)bVar->Address() );
	else if(bVar->VarType() == VariableDef::PTRVECINT) _branch = _tree->Branch(bname,(std::vector<int>**)bVar->Address() );

	else if(bVar->VarType() == VariableDef::PTRVECVECDOUBLE) _branch = _tree->Branch(bname,(std::vector<std::vector<double> >**)bVar->Address() );
	else if(bVar->VarType() == VariableDef::PTRVECVECFLOAT) _branch = _tree->Branch(bname,(std::vector<std::vector<float> >**)bVar->Address() );
	else if(bVar->VarType() == VariableDef::PTRVECVECINT) _branch = _tree->Branch(bname,(std::vector<std::vector<int> >**)bVar->Address() );

	else if(bVar->VarType() == VariableDef::VECDOUBLE) _branch = _tree->Branch(bname,(std::vector<double>*)bVar->Address() );
	else if(bVar->VarType() == VariableDef::VECFLOAT) _branch = _tree->Branch(bname,(std::vector<float>*)bVar->Address() );
	else if(bVar->VarType() == VariableDef::VECINT) _branch = _tree->Branch(bname,(std::vector<int>*)bVar->Address() );

	else if(bVar->VarType() == VariableDef::VECVECDOUBLE) _branch = _tree->Branch(bname,(std::vector<std::vector<double> >*)bVar->Address() );
	else if(bVar->VarType() == VariableDef::VECVECFLOAT) _branch = _tree->Branch(bname,(std::vector<std::vector<float> >*)bVar->Address() );
	else if(bVar->VarType() == VariableDef::VECVECINT) _branch = _tree->Branch(bname,(std::vector<std::vector<int> >*)bVar->Address() );

	else if(bVar->VarType() == VariableDef::PTRDOUBLE) _branch = _tree->Branch(bname,(double**)bVar->Address() );
	else if(bVar->VarType() == VariableDef::PTRFLOAT) _branch = _tree->Branch(bname,(float**)bVar->Address() );
	else if(bVar->VarType() == VariableDef::PTRINT) _branch = _tree->Branch(bname,(int**)bVar->Address() );
	else{ std::cerr << "<!> Error in TreeManager::AddBranchToTree(): the variable type is not recognized !!" << std::endl; }
      }
      else if( ( bVar->IsVector() && (bVar->VecInd()>=0) ) || ( bVar->IsAnaObject() && !bVar->IsVector() ) ){
	_branch = _tree->Branch(bVar->Name().c_str(), bVar->ValStore() );
      }
      else if( bVar->IsAnaObject() && bVar->IsVector() && (bVar->VecInd()<0) ){
	std::vector<double>* vecStore = bVar->VecStore();
	_branch = _tree->Branch(bVar->Name().c_str(), vecStore);
      }
      else{ std::cerr << "<!> Error in OutputTreeManager::BookStandardTree(): the variable type "<<bVar->VarTypeString()
		      <<" for branch "<<bname<< " is not recognised !!" << std::endl; }

      if(_branch == NULL){
	std::cerr << "<!> Error in OutputTreeManager::BookStandardTree(); Failed to add branch "<<bname << std::endl;
	return false;
      }

      if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Added branch : " << branch.first << std::endl;

    }
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputTreeManager::bookStandardTree" << std::endl;
    return true;
}

//______________________________________________________________________________________
//
bool OutputTreeManager::UpdateStores(  ){

    for ( const auto branch : *m_stdBranchDef ){
      if( branch.second->IsAnaObject() && branch.second->IsVector() && (branch.second->VecInd() < 0) ){
	branch.second->FillVectorStore();
      }
      else if( ( branch.second->IsAnaObject() && !branch.second->IsVector() ) 
	       || ( branch.second->IsVector() && (branch.second->VecInd() >= 0) ) ){
	branch.second->CalcDoubleValue();
      }
    }

  return true;
}
//______________________________________________________________________________________
//
bool OutputTreeManager::FillStandardTree( const std::string &pattern, const bool updateStores ){
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputTreeManager::fillStandardTree" << std::endl;
    for ( const auto branch : *m_stdBranchDef ){
      if(updateStores){
	if( branch.second->IsAnaObject() && branch.second->IsVector() && (branch.second->VecInd() < 0) ){
	  branch.second->FillVectorStore();
	}
	else if( ( branch.second->IsAnaObject() && !branch.second->IsVector() ) 
		 || ( branch.second->IsVector() && (branch.second->VecInd() >= 0) ) ){
	  branch.second->CalcDoubleValue();
	}
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


