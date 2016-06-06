#include <iostream>
#include <set>
#include "TString.h"
#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/SelectorBase.h"
#include "IFAETopFramework/NtupleData.h"
#include "IFAETopFramework/OutputData.h"

//___________________________________________________________
//
SelectorBase::SelectorBase( OptionsBase* opt, OutputData* outData, const bool useDecisions, const bool add_primaries):
m_opt(opt),
m_add_primaries(add_primaries),
m_useDecisions(useDecisions),
m_outData(outData),
m_selections(0),
m_selection_tree(0),
m_top_selections(0),
m_nPass(0)
{
    m_selections         = new std::map < int, Selection* >;
    m_selection_tree     = new std::map < int, Selection* >;
    m_top_selections     = new std::map < int, Selection* >;

    m_selections        -> clear();
    m_top_selections    -> clear();
}

//___________________________________________________________
//
SelectorBase::SelectorBase( const SelectorBase &q ){
    m_opt                  = q.m_opt;
    m_add_primaries        = q.m_add_primaries;
    m_useDecisions         = q.m_useDecisions;
    m_outData              = q.m_outData;

    m_selections           = q.m_selections;
    m_selection_tree       = q.m_selection_tree;
    m_top_selections       = q.m_top_selections; 
    m_nPass                = q.m_nPass;

}

//___________________________________________________________
//
SelectorBase::~SelectorBase(){

  m_top_selections     -> clear();     delete m_top_selections;
  m_selection_tree     -> clear();     delete m_selection_tree;
 
  for(std::pair<int, Selection* > sel : *m_selections){ delete sel.second; }
  m_selections         -> clear();     delete m_selections;

}

int SelectorBase::AddPrimary(Selection& sel, const int primary){

  int _primary = primary;
  std::map<int, Selection*>::iterator primit = m_selection_tree->find(_primary);
  std::map<int, Selection*>::iterator storeit = m_selections->find(_primary);
  while( (_primary>=0) && (primit == m_selection_tree->end()) ){
    if(m_add_primaries){ sel.primary_ancestor = AddSelection(_primary, "", false); }
    else{
      Selection* sel_prim = (storeit != m_selections->end()) ? storeit->second : MakeSelection(primary);
      int _temp_primary = sel_prim->primary_ancestor;
      if(storeit == m_selections->end()){
	delete sel_prim;
	m_selections->erase( m_selections->find(_primary) ); 
      }
      _primary = _temp_primary;
    }
    if( _primary >= 0 ){ primit = m_selection_tree->find(_primary); }
  }

  sel.primary_ancestor = _primary;

  return _primary;

}

bool SelectorBase::AddAncestor(Selection& sel, const int anc, bool is_primary){

  std::map<int, Selection*>::iterator ancit = m_selections->find(anc);
  Selection* sel_anc = NULL;;
  if(ancit != m_selections->end()){ sel_anc = ancit->second; }
  else{ sel_anc = MakeSelection(anc);  }
  sel.ancestors.push_back(sel_anc);

  if(is_primary){ AddPrimary(sel, anc); }

  return true;

}

bool SelectorBase::AddAncestors(Selection& sel, const std::vector<int> &anclist, const int primary){

  bool found_primary = false;
  for(int anc : anclist){ 
    bool is_primary = (!found_primary) && (anc==primary);
    if(is_primary){ found_primary = true; }
    AddAncestor(sel, anc, is_primary);
  }
  if(!found_primary){ AddPrimary(sel, primary); }

  return true;

}
//_________________________________________________________________________________________
//
// Make a new selection and add it to the selection store, if it does not already exist.
// Otherwise the method exits silently and returns the existing selection
// The method is virtual, so the inherited classes are responsible for calling the appropriate 
// AddAncestors and AddPrimary functions on it inside this method
//_________________________________________________________________________________________
//
Selection* SelectorBase::MakeSelection( const int index, const std::string& name ) {

  std::pair< std::map<int, Selection*>::iterator, bool > selit_pair = m_selections -> insert( std::pair< int, Selection* >(index, NULL) );
  if( selit_pair.second ) {
    selit_pair.first->second = new Selection();
    if(m_outData->o_sel_decisions == NULL){ m_outData->o_sel_decisions = new std::map<int, bool>;}
    std::pair< std::map<int, bool>::iterator, bool > decit_pair = m_outData->o_sel_decisions->insert( std::pair<int, bool>(index, false) );
    if(!decit_pair.second){
      std::cout << " Warning in SelectorBase::MakeSelection --> OutputData already contains a decision element for selection index " << index <<std::endl;
    }

    if(m_outData->o_sel_isSet == NULL){ m_outData->o_sel_isSet = new std::map<int, bool>;}
    std::pair< std::map<int, bool>::iterator, bool > setit_pair = m_outData->o_sel_isSet->insert( std::pair<int, bool>(index, false) );
    if(!decit_pair.second){
      std::cout << " Warning in SelectorBase::MakeSelection --> OutputData already contains a decision element for selection index " << index <<std::endl;
    }
    if(!setit_pair.second){
      std::cout << " Warning in SelectorBase::MakeSelection --> OutputData already contains an isSet element for selection index " << index <<std::endl;
    }

    selit_pair.first->second->selec_ind    = index;
    selit_pair.first->second->name         = (name != "") ? name : FindName(index);
    selit_pair.first->second->decision     = &(decit_pair.first->second);
    selit_pair.first->second->isSet        = &(setit_pair.first->second);
    selit_pair.first->second->numPass_raw  = 0.;
    selit_pair.first->second->numPass_wgt  = 0.;
    selit_pair.first->second->flags        = 0;
    
    selit_pair.first->second->ancestors           = std::vector<Selection*>();
    selit_pair.first->second->primary_ancestor    = -1;
    selit_pair.first->second->primary_descendants = std::vector<int>();

    selit_pair.first->second->ancestors.clear();
    selit_pair.first->second->primary_descendants.clear();

  }
  return selit_pair.first->second;

}

//_________________________________________________________________________________________
//
// Add a selection with given attributes to the selection tree. A new selection is created and 
// added to the selection store if this index is not found. Else, the existing selection is updated 
// with the attributed given here and a warning is printed
//_________________________________________________________________________________________
//

bool SelectorBase::AddSelection( const int index, const std::string &name, const bool do_runop, const bool do_histos, const bool do_trees ) {

  if(m_opt->MsgLevel() == Debug::DEBUG){
    std::cout<<" Adding Selection " << index << " with name " << name << std::endl;
    std::cout<<" ______BASE_______INDEX______"<<index<<" __NAME__ "<<" __DORUNOP__ "<<do_runop
	     <<" __DOHISTOS__ "<<do_histos<<" __DOTREES__ "<<do_trees<<std::endl;
  }

  std::pair< std::map<int, Selection*>::iterator, bool > selit_pair = m_selection_tree -> insert( std::pair< int, Selection* >(index, NULL) );
  if( selit_pair.second ) {
    selit_pair.first->second = MakeSelection( index, name );
    if( (selit_pair.first->second) -> primary_ancestor < 0 ){
      m_top_selections->insert( std::pair<int, Selection*>( (selit_pair.first->second)->selec_ind, (selit_pair.first->second) ) );
    }
    else{
      m_selection_tree->at( (selit_pair.first->second)->primary_ancestor )->
	primary_descendants.push_back( (selit_pair.first->second) -> selec_ind );
    }
  }
  else{
    std::cout << " Warning in SelectorBase::AddSelection --> selection index "
	      << index << " already exists with _name " << (selit_pair.first)->second->name 
	      << "; new _name will be " << name << std::endl;
  }

  if( m_opt->MsgLevel() == Debug::DEBUG ){
      std::cout << "SelectorBase::AddSelection:: About to set parameters for selection " << index
		<< " with name " << selit_pair.first->second->name << std::endl;
  }
  Selection* sel = selit_pair.first->second;

  bool _do_histos = do_runop ? do_histos : false;
  bool _do_trees = do_runop ? do_trees : false;

  sel -> flags        = 0;
  AddFlag( *sel, DORUNOP, do_runop );
  AddFlag( *sel, DOHIST, _do_histos ); 
  AddFlag( *sel, DOTREE, _do_trees ); 

  if(m_opt->MsgLevel() == Debug::DEBUG){
    std::cout << " SelectorBase::AddSelection -->> ADDED SELECTION " << index; 
    std::cout << " name = " << sel -> name; 
    std::cout << " primary_ancestor = "   << sel -> primary_ancestor; 
    std::cout << " (flag & DORUNOP) =   " << PassFlag(*sel, DORUNOP);
    std::cout << " (flag & DOHIST)  =   " << PassFlag(*sel, DOHIST);
    std::cout << " (flag & DOTREE)  =   " << PassFlag(*sel, DOTREE);
    std::cout <<std::endl;
  }

  return true;

}

std::string SelectorBase::FindName(const int index) const{
  std::string name =  Form("selection_%i", index);
  return name;
}
//___________________________________________________________
//
bool SelectorBase::AddFlag(const int index, const std::string& flag, const bool value){

  bool stat = true;
  std::map<int, Selection*>::iterator selit = m_selections->find(index);
  if( selit == m_selections->end() ){
    std::cerr<<" Error in SelectorBase::AddFlag() --> Selection at index "<< index << "cannot be found"<<std::endl;
    return false;
  }
  else{
    stat = AddFlag(*(selit->second), flag, value); 
  } 
  return stat;

}

//___________________________________________________________
//
bool SelectorBase::AddFlag(const int index, const int flag, const bool value){

  std::map<int, Selection*>::iterator selit = m_selections->find(index);
  if( selit == m_selections->end() ){
    std::cerr<<" Error in SelectorBase::AddFlag() --> Selection at index "<< index << "cannot be found"<<std::endl;
    return false;
  }
  AddFlag(*(selit->second), flag, value); 
  return true;
}

//___________________________________________________________
//
void SelectorBase::AddFlag(Selection& sel , const int flag, const bool value ){

  if(value) sel.flags |= (0x1<<flag);
  else      sel.flags &= ~(0x1 << flag);
  return; 

}

//___________________________________________________________
//
bool SelectorBase::AddFlag(Selection& sel, const std::string& flag, const bool value){

  if(flag=="do_runop")       { AddFlag(sel, DORUNOP, value); }
  else if(flag=="do_histos") { AddFlag(sel, DOHIST, value); }
  else if(flag=="do_trees")  { AddFlag(sel, DOTREE, value); }
  else                       { std::cerr << " ERROR in SelectorBase --> Unknown flag "<<flag<<std::endl; return false; }
  return true;

}

bool SelectorBase::PassFlag(const Selection& sel, const int flag) const{
  return (sel.flags & 0x1<<flag); 
}

//___________________________________________________________
//
bool SelectorBase::PassSelection( Selection& sel, bool useDecision, bool check_primary ) {

  if( m_opt -> MsgLevel() == Debug::DEBUG ){ 
    std::cout << "Entering SelectorBase::PassSelection for selection " << sel.name << std::endl; 
  }
  if(useDecision && *(sel.isSet) ){ return *(sel.decision); }

  bool pass = true;
  if(sel.ancestors.size() > 0){ 
    for(Selection* anc : sel.ancestors){ 
      if( !check_primary && (anc->selec_ind == sel.primary_ancestor) ) continue;
      else{ pass = PassSelection( *anc, useDecision, check_primary ); }
      if(!pass) break;
    }
  }
  pass = pass && PassSelection(sel.selec_ind);
  if(!(*(sel.isSet)) && pass){
    sel.numPass_raw += 1.;
    sel.numPass_wgt += m_outData -> o_eventWeight_Nom;
  }

  *(sel.decision) = pass;
  *(sel.isSet) = true;

  return pass;
}

//___________________________________________________________
//
bool SelectorBase::RunSelectionChain( ) {
  if( m_opt -> MsgLevel() == Debug::DEBUG ){ 
    std::cout << "Entering SelectorBase::RunSelectionChain m_top_selections->size() = " << m_top_selections->size() << std::endl; 
  }
  bool stat = true; 
  m_nPass = 0; 
  for(std::pair< int, Selection* > top_sel : *m_top_selections){
    stat = RunSelectionNode(*(top_sel.second)) || stat;
  }

  if( m_opt -> MsgLevel() == Debug::DEBUG ){
    std::cout << "Exiting SelectorBase::RunSelectionChain " << std::endl; 
  }
  return stat;

}

//___________________________________________________________
//
bool SelectorBase::RunSelectionNode( const int node ){

  std::map<int, Selection*>::iterator selit = m_selection_tree->find(node);
  if( selit == m_selection_tree->end() ){
    std::cerr<<" Error in SelectorBase::RunSelectionNode() --> Selection at index "<< node << "cannot be found in tree"<<std::endl;
    return false;
  }
  bool pass_node = RunSelectionNode(*(selit->second));

  return pass_node;

}

//___________________________________________________________
//
bool SelectorBase::RunSelectionNode( Selection& sel ){

  if(m_opt -> MsgLevel() == Debug::DEBUG){
    std::cout << " FIRST SelectorBase::RunSelectionNode() --> selection = " << sel.name 
	      << " Number of descendants = " << sel.primary_descendants.size()  
	      << std::endl;

  }

  bool pass_node = PassSelection(sel, m_useDecisions, false);
  if(m_opt -> MsgLevel() == Debug::DEBUG){
    std::cout << " SECOND SelectorBase::RunSelectionNode() --> selection = " << sel.name << " pass_node = " << pass_node 
	      << " Number of descendants = " << sel.primary_descendants.size()  
	      << std::endl;

  }
  if( !pass_node ) return pass_node;
  m_nPass++;

  if( PassFlag(sel, DORUNOP) ){
    if(!RunOperations(sel)){ std::cerr << "ERROR in SelectorBase::RunSelectionNode() -> Failure to execute RunOperations on selection node " << sel.selec_ind << std::endl;}
  }

  for( const int descendant : sel.primary_descendants ){ RunSelectionNode(descendant); }

  return pass_node;

}

//___________________________________________________________
//
bool SelectorBase::RunOperations( const int node ) const {

  std::map<int, Selection*>::iterator selit = m_selections->find(node);
  if( selit == m_selections->end() ){
    std::cerr<<" Error in SelectorBase::RunOperations() --> Selection at index "<< node << "cannot be found"<<std::endl;
    return false;
  }
  bool stat = RunOperations( *(selit->second) );

  return stat;

}

//__________________________________________________________
//
void SelectorBase::PrintSelectionTree( const bool printYields ) const{

  std::cout<< "************ TOP_SELECTIONS **************" << std::endl;
  for(std::pair<int, Selection*> sel : *m_top_selections){ std::cout << sel.second->name << std::endl; }
  std::cout<< "*******************************************" << std::endl;
  std::cout<<std::endl<<std::endl<<std::endl;

  std::cout<< "************ ALL_SELECTIONS **************" << std::endl;
  for(std::pair<int, Selection*> sel : *m_selection_tree){ 
    std::cout <<"SELECTION : " << sel.second->name << ";  INDEX : " << sel.second->selec_ind << std::endl;
    std::cout << "   DORUNOP =   " << PassFlag(*(sel.second), DORUNOP)
	      << "   DOHIST  =   " << PassFlag(*(sel.second), DOHIST)
	      << "   DOTREE  =   " << PassFlag(*(sel.second), DOTREE) << std::endl;
    if(printYields){
      std::cout << "EVENTS PASSED (UNWEIGHTED): " << sel.second->numPass_raw << std::endl;
      std::cout << "EVENTS PASSED (WEIGHTED):   " << sel.second->numPass_wgt << std::endl;
    }

    if(sel.second->primary_ancestor >= 0){ std::cout <<" PRIMARY ANCESTOR : " << m_selections->at(sel.second->primary_ancestor)->name << std::endl; }
    std::cout <<" ANCESTOR LIST : "<<std::endl;
    for( Selection* anc : sel.second->ancestors ){ std::cout << "   " << anc->name << std::endl; }

    std::cout <<" PRIMARY DESCENDANT LIST : "<<std::endl;
    for( int dec : sel.second->primary_descendants ){
      std::cout << "   " << m_selections->at(dec)->name << std::endl;
    }

    std::cout << std::endl << std::endl << std::endl;

  }


  return;

}

const Selection* SelectorBase::GetSelection(const int node) const{

 std::map<int, Selection*>::iterator selit = m_selections->find(node);
 if( selit == m_selections->end() ){
   std::cerr<<" Error in SelectorBase::RunOperations() --> Selection at index "<< node << "cannot be found"<<std::endl;
   return NULL;
 }
 Selection* sel = (selit->second);
 return sel;

}
