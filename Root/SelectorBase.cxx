#include <iostream>
#include <set>
#include "TString.h"
#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/Selection.h"
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

  for(std::pair<int, Selection* > sel : *m_selections){
    delete sel.second;
  }
  m_selections         -> clear();     delete m_selections;

}

int SelectorBase::AddPrimary(Selection& sel, const int primary){

  int _primary = primary;
  std::map<int, Selection*>::iterator primit = m_selection_tree->find(_primary); //primary in tree
  std::map<int, Selection*>::iterator storeit = m_selections->find(_primary); //primary in selection store
  while( (_primary>=0) && (primit == m_selection_tree->end()) ){
    if(m_add_primaries){ 
      AddSelection(_primary, "", false);
      sel.SetPrimaryAncestor(_primary);
    }
    else{
      //This is a temporary object being created only to find its primary ancestor
      Selection* sel_prim = (storeit != m_selections->end()) ? storeit->second : MakeSelection(primary);
      int _temp_primary = sel_prim->PrimaryAncestor();
      if(storeit == m_selections->end()){ //If the temporary object was created
	delete sel_prim;
	m_selections->erase( m_selections->find(_primary) ); 
      }
      _primary = _temp_primary;
    }
    if( _primary >= 0 ){ primit = m_selection_tree->find(_primary); }
  }

  sel.SetPrimaryAncestor(_primary);

  return _primary;

}

bool SelectorBase::AddAncestor(Selection& sel, const int anc, bool is_primary){

  std::map<int, Selection*>::iterator ancit = m_selections->find(anc);
  Selection* sel_anc = NULL;;
  if(ancit != m_selections->end()){ sel_anc = ancit->second; }
  else{ sel_anc = MakeSelection(anc);  }
  sel.AddAncestor(sel_anc);

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
    selit_pair.first->second = new Selection(index, name, m_outData);
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

Selection* SelectorBase::AddSelection( const int index, const std::string &name, const bool do_runop, const bool do_histos, const bool do_trees ) {

  if(m_opt->MsgLevel() == Debug::DEBUG){
    std::cout<<" Adding Selection " << index << " with name " << name << std::endl;
    std::cout<<" ______BASE_______INDEX______"<<index<<" __NAME__ "<<" __DORUNOP__ "<<do_runop
	     <<" __DOHISTOS__ "<<do_histos<<" __DOTREES__ "<<do_trees<<std::endl;
  }

  std::pair< std::map<int, Selection*>::iterator, bool > selit_pair = m_selection_tree -> insert( std::pair< int, Selection* >(index, NULL) );
  if( selit_pair.second ) {
    selit_pair.first->second = MakeSelection( index, name );
    if( (selit_pair.first->second) -> PrimaryAncestor() < 0 ){
      m_top_selections->insert( std::pair<int, Selection*>( (selit_pair.first->second)->SelecInd(), (selit_pair.first->second) ) );
    }
    else{
      m_selection_tree->at( (selit_pair.first->second)->PrimaryAncestor() )->
	AddPrimaryDescendant( (selit_pair.first->second) -> SelecInd() );
    }
  }
  else{
    std::cout << " Warning in SelectorBase::AddSelection --> selection index "
	      << index << " already exists with _name " << (selit_pair.first)->second->Name() 
	      << "; new _name will be " << name << std::endl;
  }

  if( m_opt->MsgLevel() == Debug::DEBUG ){
      std::cout << "SelectorBase::AddSelection:: About to set parameters for selection " << index
		<< " with name " << selit_pair.first->second->Name() << std::endl;
  }
  Selection* sel = selit_pair.first->second;

  bool _do_histos = do_runop ? do_histos : false;
  bool _do_trees = do_runop ? do_trees : false;

  //sel -> flags        = 0;
  sel->AddFlagAtBit( DORUNOP, do_runop );
  sel->AddFlagAtBit( DOHIST, _do_histos ); 
  sel->AddFlagAtBit( DOTREE, _do_trees ); 

  if(m_opt->MsgLevel() == Debug::DEBUG){
    std::cout << " SelectorBase::AddSelection -->> ADDED SELECTION " << index; 
    std::cout << " name = " << sel -> Name(); 
    std::cout << " primary_ancestor = "   << sel -> PrimaryAncestor(); 
    std::cout << " (flag & DORUNOP) =   " << sel->PassFlagAtBit(DORUNOP);
    std::cout << " (flag & DOHIST)  =   " << sel->PassFlagAtBit(DOHIST);
    std::cout << " (flag & DOTREE)  =   " << sel->PassFlagAtBit(DOTREE);
    std::cout <<std::endl;
  }

  return sel;

}

//___________________________________________________________
//
bool SelectorBase::AddFlag(const int index, const int flag, const bool value){

  std::map<int, Selection*>::iterator selit = m_selections->find(index);
  if( selit == m_selections->end() ){
    std::cerr<<" Error in SelectorBase::AddFlag() --> Selection at index "<< index << "cannot be found"<<std::endl;
    return false;
  }
  (selit->second)->AddFlag(flag, value); 
  return true;
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
    std::cout << " FIRST SelectorBase::RunSelectionNode() --> selection = " << sel.Name() 
	      << " Number of descendants = " << sel.PrimaryDescendants()->size()  
	      << std::endl;

  }

  bool pass_node = sel.PassSelection(m_useDecisions, false);
  if(m_opt -> MsgLevel() == Debug::DEBUG){
    std::cout << " SECOND SelectorBase::RunSelectionNode() --> selection = " << sel.Name() << " pass_node = " << pass_node 
	      << " Number of descendants = " << sel.PrimaryDescendants()->size()  
	      << std::endl;

  }
  if( !pass_node ) return pass_node;
  m_nPass++;

  if( sel.PassFlagAtBit(DORUNOP) ){
    if(!RunOperations(sel)){ std::cerr << "ERROR in SelectorBase::RunSelectionNode() -> Failure to execute RunOperations on selection node " << sel.SelecInd() << std::endl;}
  }

  for( const int descendant : *(sel.PrimaryDescendants()) ){ RunSelectionNode(descendant); }

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
  for(std::pair<int, Selection*> sel : *m_top_selections){ std::cout << sel.second->Name() << std::endl; }
  std::cout<< "*******************************************" << std::endl;
  std::cout<<std::endl<<std::endl<<std::endl;

  std::cout<< "************ ALL_SELECTIONS **************" << std::endl;
  for(std::pair<int, Selection*> sel : *m_selection_tree){ 
    std::cout <<"SELECTION : " << sel.second->Name() << ";  INDEX : " << sel.second->SelecInd() << std::endl;
    std::cout << "   DORUNOP =   " << sel.second->PassFlagAtBit(DORUNOP)
	      << "   DOHIST  =   " << sel.second->PassFlagAtBit(DOHIST)
	      << "   DOTREE  =   " << sel.second->PassFlagAtBit(DOTREE) << std::endl;
    if(printYields){
      std::cout << "EVENTS PASSED (UNWEIGHTED): " << sel.second->NumPassRaw() << std::endl;
      std::cout << "EVENTS PASSED (WEIGHTED):   " << sel.second->NumPassWgt() << std::endl;
    }

    if(sel.second->PrimaryAncestor() >= 0){ std::cout <<" PRIMARY ANCESTOR : " << m_selections->at(sel.second->PrimaryAncestor())->Name() << std::endl; }
    std::cout <<" ANCESTOR LIST : "<<std::endl;
    for( Selection* anc : *(sel.second->Ancestors()) ){ std::cout << "   " << anc->Name() << std::endl; }

    std::cout <<" PRIMARY DESCENDANT LIST : "<<std::endl;
    for( int dec : *(sel.second->PrimaryDescendants()) ){
      std::cout << "   " << m_selections->at(dec)->Name() << std::endl;
    }
    std::cout << std::endl << std::endl << std::endl;

  }


  return;

}

Selection* SelectorBase::GetSelection(const int node){

 std::map<int, Selection*>::iterator selit = m_selections->find(node);
 if( selit == m_selections->end() ){
   std::cerr<<" Error in SelectorBase::RunOperations() --> Selection at index "<< node << "cannot be found"<<std::endl;
   return NULL;
 }
 Selection* sel = (selit->second);
 return sel;

}
