#include <iostream>

#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/SelectorBase.h"
#include "IFAETopFramework/NtupleData.h"
#include "IFAETopFramework/OutputData.h"

//___________________________________________________________
//
SelectorBase::SelectorBase( OptionsBase* opt, OutputData* outData ):
m_opt(opt),
m_outData(outData),
m_selections(0),
m_top_selections(0),
//m_top_sel_inds(0),
m_map_sel_int_string(0),
m_nPass(0)
{
    m_selections         = new std::map < int, Selection >;
    m_top_selections     = new std::map < int, Selection* >;
    //m_top_sel_inds       = new std::vector < int >;
    m_map_sel_int_string = new std::map<int, std::string>;

    m_selections        -> clear();
    m_top_selections    -> clear();
    //m_top_sel_inds      -> clear();
    m_map_sel_int_string-> clear();
}

//___________________________________________________________
//
SelectorBase::SelectorBase( const SelectorBase &q ){
    m_opt                  = q.m_opt;
    m_outData              = q.m_outData;
    m_selections           = q.m_selections;
    m_top_selections       = q.m_top_selections; 
    m_map_sel_int_string   = q.m_map_sel_int_string;
    m_nPass                = q.m_nPass;
}

//___________________________________________________________
//
SelectorBase::~SelectorBase(){
    m_selections         -> clear();     delete m_selections;
    m_top_selections     -> clear();     delete m_top_selections;
    m_map_sel_int_string -> clear();     delete m_map_sel_int_string;
}

//___________________________________________________________
//
//___________________________________________________________
//
int SelectorBase::FindClosestAncestor(const int index) const{

  int ancestor = GetParentIndex(index);
  while( (ancestor >= 0) && (m_selections->find(ancestor) == m_selections->end()) ){ ancestor = GetParentIndex(ancestor); }

  return ancestor;

}

bool SelectorBase::AddSelection( const int index, const std::string &name, const bool has_histos, const bool has_trees ) {

  if(m_opt->MsgLevel() == Debug::DEBUG){
    std::cout<<" Adding Selection "<<index<<" with name "<<name<<std::endl;
  }
  std::pair< std::map<int, Selection>::iterator, bool > selit_pair = m_selections -> insert( std::pair< int, Selection >(index, Selection()) );
  if( selit_pair.second ) {

    if( (m_outData -> o_sel_decisions) == NULL ) { m_outData -> o_sel_decisions = new std::map<int, bool>; }
    m_map_sel_int_string -> insert( std::pair< int, std::string >(index, name) );
    std::pair< std::map<int, bool>::iterator, bool > decit_pair = (m_outData -> o_sel_decisions)->insert( std::pair< int, bool >(index, false) );

    (selit_pair.first->second).selec_ind = index;
    (selit_pair.first->second).name = name;

    (selit_pair.first->second).decision = &((decit_pair.first)->second);
    (selit_pair.first->second).has_histos = has_histos;
    (selit_pair.first->second).has_trees = has_trees;
    (selit_pair.first->second).flags = std::map<std::string, bool>();
    (selit_pair.first->second).parent = GetParentIndex(index);
    (selit_pair.first->second).descendants = std::vector<int>();

    (selit_pair.first->second).flags.clear();
    (selit_pair.first->second).descendants.clear();

    int _ancestor_index = FindClosestAncestor( index );
    if( _ancestor_index >= 0 ){
      Selection& _ancestor = m_selections->at(_ancestor_index);
      if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<"Adding descendant selection " << name << " to ancestor " << _ancestor.name 
						      << "; current size of descendants =  " << _ancestor.descendants.size() << std::endl; }
	(_ancestor).descendants.push_back( index );
	if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<"Added descendant selection " << name << " to parent " << _ancestor.name 
							<< "; current size of descendants =  " << _ancestor.descendants.size() << std::endl; }
    }//if selection has ancestor
  
    if(_ancestor_index < 0){
      if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<"Adding " << name << " as top selection " << std::endl; }
      m_top_selections->insert( std::pair< int, Selection* >(index, &(selit_pair.first->second)) );
    }
    return true;
  }
  else{
      std::cerr << " Error in SelectorBase::AddSelection --> Attempt to overwrite selection index "
	      << index << " with name "<<(selit_pair.first)->second.name<<" by new name "<<name<<std::endl;
    return false;
  }


}

//___________________________________________________________
//
bool SelectorBase::AddFlag(const int index, const std::string& flag, const bool value){

  bool stat = true;
  std::map<int, Selection>::iterator selit = m_selections->find(index);
  if( selit == m_selections->end() ){
    std::cerr<<" Error in SelectorBase::AddFlag() --> Selection at index "<< index << "cannot be found"<<std::endl;
    return false;
  }
  else{
    stat = AddFlag(selit->second, flag, value); 
  } 
  return stat;

}

bool SelectorBase::AddFlag(Selection& sel, const std::string& flag, const bool value){

  if( sel.flags.find(flag) != sel.flags.end() ){
    std::cerr<<" Error in SelectorBase::AddFlag() --> Flag "<< flag << " already exists for selection "<< sel.name <<std::endl;
    return false;
  }
  else{ sel.flags.insert( std::pair<std::string, bool>(flag, value) ); }

  return true;

}

//___________________________________________________________
//
void SelectorBase::SetDecision( const int sel, const bool decision) {

  std::map<int, Selection>::iterator selit = m_selections->find(sel);
  if( selit == m_selections->end() ){
    std::cerr<<" Error in SelectorBase::SetDecision() --> Selection at index "<< sel << "cannot be found"<<std::endl;
    return;
  }
  else{
    *(selit->second.decision) = decision;
  } 
  return;
}

//___________________________________________________________
//
bool SelectorBase::GetDecision( const int sel) const {

  std::map<int, Selection>::iterator selit = m_selections->find(sel);
  if( selit == m_selections->end() ){
    std::cerr<<" Error in SelectorBase::GetDecision() --> Selection at index "<< sel << "cannot be found"<<std::endl;
    return false;
  }

  return *(selit->second.decision);

}

//___________________________________________________________
//
bool SelectorBase::GetDecision( const Selection& sel) const {

  return *(sel.decision);

}

//___________________________________________________________
//
bool SelectorBase::PassSelection( const int /*sel*/ ) const {
  std::cout << "SelectorBase::PassSelection() is empty ... You should not be there !!" << std::endl;
  return false;
}

//___________________________________________________________
//
bool SelectorBase::PassSelectionChain( ) const {
  if( m_opt -> MsgLevel() == Debug::DEBUG ){ 
    std::cout << "Entering SelectorBase::PassSelectionChain " << std::endl; 

    for(std::pair<int, Selection> sel : *m_selections){
      std::cout<<" selection = " << sel.second.name << " initial decision = " << *(sel.second.decision) << std::endl;
    }

  }
  bool stat = true;
  m_nPass = 0;
  for(std::pair< int, Selection* > top_sel : *m_top_selections){
    stat = PassSelectionNode( *(top_sel.second) ) || stat;
  }

  if( m_opt -> MsgLevel() == Debug::DEBUG ){
    std::cout << "Exiting SelectorBase::PassSelectionChain " << std::endl; 
  }
  return stat;

}

//___________________________________________________________
//
bool SelectorBase::RunSelectionChain( ) const {
  if( m_opt -> MsgLevel() == Debug::DEBUG ){ 
    std::cout << "Entering SelectorBase::RunSelectionChain m_top_selections->size() = " << m_top_selections->size() << std::endl; 
    /*
    for(std::pair<int, Selection> sel : *m_selections){
      std::cout<<" selection = " << sel.second.name << " initial decision = " << *(sel.second.decision) 
	       << " Ndescendants = " << sel.second.descendants.size() << std::endl;
    }
    */
  }
  bool stat = true; 
  m_nPass = 0; 
  for(std::pair< int, Selection* > top_sel : *m_top_selections){

    if( m_opt -> MsgLevel() == Debug::DEBUG ){ 
      std::cout<<" TOP selection = " << top_sel.second->name << " initial decision = " << *(top_sel.second->decision) 
	       << " Ndescendants = " << top_sel.second->descendants.size() << std::endl;
    }
    stat = RunSelectionNode(*(top_sel.second)) || stat;
  }

  if( m_opt -> MsgLevel() == Debug::DEBUG ){
    std::cout << "Exiting SelectorBase::RunSelectionChain " << std::endl; 
  }
  return stat;

}

//___________________________________________________________
//
bool SelectorBase::PassSelectionNode( const int node ) const {

  std::map<int, Selection>::iterator selit = m_selections->find(node);
  if( selit == m_selections->end() ){
    std::cerr<<" Error in SelectorBase::PassSelectionNode() --> Selection at index "<< node << "cannot be found"<<std::endl;
    return false;
  }
  bool pass_node = PassSelectionNode(selit->second);
 
  return pass_node;

}

//___________________________________________________________
//
bool SelectorBase::PassSelectionNode( const Selection& sel ) const {

  bool pass_node = PassSelection(sel.selec_ind);
 
  *(sel.decision) = pass_node;
  if(m_opt -> MsgLevel() == Debug::DEBUG){
    std::cout<<"INFO SelectorBase::PassSelectionNode --> selection = " << sel.name << " pass_node = " << pass_node 
	     << " decision = " << *(sel.decision) 
	     << " GetDecision(sel) = " << GetDecision(sel) << " descendants = " << sel.descendants.size() << std::endl;
  }
  if( !pass_node ) return pass_node;
  m_nPass++;
  for( const int descendant : sel.descendants ){ PassSelectionNode(descendant); }

  return pass_node;

}

//___________________________________________________________
//
bool SelectorBase::RunSelectionNode( const int node ) const {

  std::map<int, Selection>::iterator selit = m_selections->find(node);
  if( selit == m_selections->end() ){
    std::cerr<<" Error in SelectorBase::RunSelectionNode() --> Selection at index "<< node << "cannot be found"<<std::endl;
    return false;
  }
  bool pass_node = RunSelectionNode(selit->second);

  return pass_node;

}

//___________________________________________________________
//
bool SelectorBase::RunSelectionNode( const Selection& sel ) const {

  if(m_opt -> MsgLevel() == Debug::DEBUG){
    std::cout << " FIRST SelectorBase::RunSelectionNode() --> selection = " << sel.name 
	      << " decision = " << *(sel.decision) 
	      << " GetDecision(node) = " << GetDecision(sel)
	      << " Number of descendants = " << sel.descendants.size()  
	      << std::endl;

  }

  bool pass_node = PassSelection(sel.selec_ind);
  *(sel.decision) = pass_node;
  if(m_opt -> MsgLevel() == Debug::DEBUG){
    std::cout << " SECOND SelectorBase::RunSelectionNode() --> selection = " << sel.name << " pass_node = " << pass_node 
	      << " decision = " << *(sel.decision) 
	      << " GetDecision(node) = " << GetDecision(sel)
	      << " Number of descendants = " << sel.descendants.size()  
	      << std::endl;

  }
  if( !pass_node ) return pass_node;
  m_nPass++;

  bool stat = RunOperations(sel);
  if(!stat){ std::cerr << "ERROR in SelectorBase::RunSelectionNode() -> Failure to execute RunOperations on selection node " << sel.selec_ind << std::endl;}


  for( const int descendant : sel.descendants ){ RunSelectionNode(descendant); }

  return pass_node;

}

//___________________________________________________________
//
bool SelectorBase::RunOperations( const int node ) const {

  std::map<int, Selection>::iterator selit = m_selections->find(node);
  if( selit == m_selections->end() ){
    std::cerr<<" Error in SelectorBase::RunOperations() --> Selection at index "<< node << "cannot be found"<<std::endl;
    return false;
  }
  bool stat = RunOperations(selit->second);

  return stat;

}
