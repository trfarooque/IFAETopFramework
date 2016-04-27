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
m_map_sel_int_string(0)
{
    m_selections         = new std::map < int, Selection >;
    m_top_selections     = new std::vector< int >;
    m_map_sel_int_string = new std::map<int, std::string>;

    m_selections        ->clear();
    m_top_selections    ->clear();
    m_map_sel_int_string->clear();
}

//___________________________________________________________
//
SelectorBase::SelectorBase( const SelectorBase &q ){
    m_opt                  = q.m_opt;
    m_outData              = q.m_outData;
    m_selections           = q.m_selections;
    m_top_selections       = q.m_top_selections; 
    m_map_sel_int_string   = q.m_map_sel_int_string;
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
bool SelectorBase::AddSelection( const int index, const std::string &name, const int parent_index, const bool has_histos, const bool has_trees ) {

  if(m_opt->MsgLevel() == Debug::DEBUG){
    std::cout<<" Adding Selection "<<index<<" with name "<<name<<" and parent index "<<parent_index<<std::endl;
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
    (selit_pair.first->second).children = std::vector<int>();

    (selit_pair.first->second).flags.clear();
    (selit_pair.first->second).children.clear();

    if( parent_index >= 0 ){

      std::map<int, Selection>::iterator par_it = m_selections -> find( parent_index );
      if( par_it == m_selections->end() ){
	std::cerr << " Error in SelectorBase::AddSelection -> The parent selection with index "
		  << parent_index << " is not in the selection chain. Please add it first." << std::endl;
	return false;
      }

      (par_it->second).children.push_back( index );

    }//if selection has parent
    else{
      m_top_selections->push_back(index);
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
//bool SelectorBase::AddFlag(const int index, const std::string& flag, const bool value){
//;
//}
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
bool SelectorBase::PassSelection( const int /*sel*/) const {
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
  for(const int top_sel : *m_top_selections){
    stat = PassSelectionNode(top_sel) || stat;
  }

  if( m_opt -> MsgLevel() == Debug::DEBUG ){
    std::cout << "Exiting SelectorBase::PassSelectionChain " << std::endl; 
  }
  return stat;

}

//___________________________________________________________
//
bool SelectorBase::PassSelectionNode( const int node ) const {

  bool pass_node = PassSelection(node);
  *(m_selections->at(node).decision) = pass_node;
  if(m_opt -> MsgLevel() == Debug::DEBUG){
    std::cout<<" selection = " << m_selections->at(node).name << " pass_node = " << pass_node 
	     << " decision = " << *(m_selections->at(node).decision) 
	     << " GetDecision(node) = " << GetDecision(node) << std::endl;
  }
  if( !pass_node ) return pass_node;

  for( const int child : m_selections->at(node).children ){ PassSelectionNode(child); }

  return pass_node;

}
