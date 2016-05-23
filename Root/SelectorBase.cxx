#include <iostream>
#include <set>
#include "TString.h"
#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/SelectorBase.h"
#include "IFAETopFramework/NtupleData.h"
#include "IFAETopFramework/OutputData.h"

//___________________________________________________________
//
SelectorBase::SelectorBase( OptionsBase* opt, OutputData* outData, const bool add_ancestors ):
m_opt(opt),
m_add_ancestors(add_ancestors),
m_outData(outData),
m_selections(0),
m_top_selections(0),
m_map_sel_int_string(0),
m_nPass(0)
{
    m_selections         = new std::map < int, Selection >;
    m_top_selections     = new std::map < int, Selection* >;
    m_map_sel_int_string = new std::map<int, std::string>;

    m_selections        -> clear();
    m_top_selections    -> clear();
    m_map_sel_int_string-> clear();
}

//___________________________________________________________
//
SelectorBase::SelectorBase( const SelectorBase &q ){
    m_opt                  = q.m_opt;
    m_add_ancestors        = q.m_add_ancestors;
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
bool SelectorBase::AddAncestorsRecursive(Selection& sel){

  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<" Entering AddAncestorsRecursive "<< sel.name << std::endl; }
  //____________________________________________________________________
  if(m_add_ancestors){
    sel.primary_ancestor = AddAncestors(sel.ancestors, sel.selec_ind);
    for(int anc : sel.ancestors){
      if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<" CHECKING Selection " << sel.selec_ind << " with name "<< sel.name << " ; ancestor = " << anc << std::endl; }
      if( (anc > 0) && (m_selections->find(anc) == m_selections->end()) ){
	if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<" Adding Ancestral Selection :: " << sel.selec_ind << " with name "<< sel.name << " ; ancestor = " << anc << std::endl; }
	AddSelection(anc, Form("anc_%i",anc), false);
	if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<" Added Ancestral Selection :: " << sel.selec_ind << " with name "<< sel.name << " ; ancestor = " << anc << std::endl; }
      }
      if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<" CHECKED Selection " << sel.selec_ind << " with name "<< sel.name << " ; ancestor = " << anc << std::endl; }
    }
  }//add immediate ancestors to chain if needed
  else{
    sel.ancestors.push_back(sel.selec_ind);
    sel.primary_ancestor = sel.selec_ind;
    int primary = -1;

    bool orphan = true;
    std::set<int> orphan_ancestors; orphan_ancestors.clear();
    std::vector<int> new_ancestors; new_ancestors.clear();

    while(orphan){ 
      for(int anc : sel.ancestors){
	if( (anc == sel.selec_ind) //starting position - swayambhu 
	    || ( (anc >= 0) //this point should not be reached
		 && (m_selections->find(anc) == m_selections->end()) //ancestor not in chain
		 && (orphan_ancestors.find(anc) == orphan_ancestors.end()) ) ){ //a top-level ancestor (swayambhu)
	  primary = AddAncestors(new_ancestors, anc);
	  if(primary < 0){ orphan_ancestors.insert(anc); }
	  if(sel.primary_ancestor == anc){ sel.primary_ancestor = primary; }
	}
      }

      orphan = false;
      for(int new_anc : new_ancestors){ 
	if( (new_anc >= 0) && (orphan_ancestors.find(new_anc) == orphan_ancestors.end())
	    && (m_selections->find(new_anc) == m_selections->end()) ){ orphan = true; break; }
      }

      //----------------------------------------
      sel.ancestors.clear();
      sel.ancestors = new_ancestors;
    }//while(orphan)
    orphan_ancestors.clear();
  }//recursive addition of ancestors


  //--------------- Add selection as a primary descendant of its primary ancestor ----------------------
  int _primary_ancestor_index = sel.primary_ancestor;
  if( _primary_ancestor_index >= 0 ){
    Selection& _clos_ancestor = m_selections->at(_primary_ancestor_index);
    if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<"Adding descendant selection " << sel.name << " to ancestor " << _clos_ancestor.name 
						    << "; current size of descendants =  " << _clos_ancestor.primary_descendants.size() << std::endl; }
    (_clos_ancestor).primary_descendants.push_back( sel.selec_ind );
    if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<"Added descendant selection " << sel.name << " to parent " << _clos_ancestor.name 
						    << "; current size of descendants =  " << _clos_ancestor.primary_descendants.size() << std::endl; }
  }//if selection has ancestor
  
  if(_primary_ancestor_index < 0){
    if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<"Adding " << sel.name << " as top selection " << std::endl; }
    m_top_selections->insert( std::pair< int, Selection* >(sel.selec_ind, &sel ) );
  }

  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<" Exiting AddAncestorsRecursive "<< sel.name << std::endl; }

  return true;

}

bool SelectorBase::AddSelection( const int index, const std::string &name, const bool do_runop, const bool do_histos, const bool do_trees ) {

  if(m_opt->MsgLevel() == Debug::DEBUG){
    std::cout<<" Adding Selection "<<index<<" with name "<<name<<std::endl;
  }

  std::pair< std::map<int, Selection>::iterator, bool > selit_pair = m_selections -> insert( std::pair< int, Selection >(index, Selection()) );
  if(m_outData->o_sel_decisions == NULL){ m_outData->o_sel_decisions = new std::map<int, bool>;}
  std::pair< std::map<int, bool>::iterator, bool > decit_pair = m_outData->o_sel_decisions->insert( std::pair<int, bool>(index, false) );
  if(!decit_pair.second){
    std::cout << " Warning in SelectorBase::AddSelection --> OutputData already contains a decision element for selection index " << index <<std::endl;
  }

  if( selit_pair.second ) {
    std::cout<<" 00 HERE "<<std::endl;
    m_map_sel_int_string -> insert( std::pair< int, std::string >(index, name) );
    std::cout<<" 01 HERE "<<std::endl;

    (selit_pair.first->second).selec_ind = index;
    (selit_pair.first->second).decision = &(decit_pair.first->second);

    (selit_pair.first->second).ancestors = std::vector<int>();
    (selit_pair.first->second).primary_ancestor = -1;
    (selit_pair.first->second).primary_descendants = std::vector<int>();

    (selit_pair.first->second).ancestors.clear();
    (selit_pair.first->second).primary_descendants.clear();

    std::cout<<" 02 HERE "<<std::endl;
    //_______________________________________________

    AddAncestorsRecursive(selit_pair.first->second);
    //_______________________________________________
    std::cout<<" 03 HERE "<<std::endl;

  }
  else{
    std::cout<<" 03 01 HERE "<<std::endl;
    std::cout << " Warning in SelectorBase::AddSelection --> selection index "
	      << index << " already exists with name "<<(selit_pair.first)->second.name<<"; new name will be "<<name<<std::endl;
    m_map_sel_int_string -> at( index ) = name;
    std::cout<<" 03 02 HERE "<<std::endl;
  }
  std::cout<<" 04 HERE "<<std::endl;

  (selit_pair.first->second).name = name;
  (selit_pair.first->second).flags = 0;
  AddFlag( (selit_pair.first->second), DORUNOP, do_runop );
  AddFlag( (selit_pair.first->second), DOHIST, do_histos ); 
  AddFlag( (selit_pair.first->second), DOTREE, do_trees ); 
  std::cout<<" 05 HERE "<<std::endl;

  if(m_opt->MsgLevel() == Debug::DEBUG){
    std::cout<<" 06 HERE "<<std::endl;
    std::cout << " SELECTION " << index; 
    std::cout<<" 06 00 HERE "<<std::endl;
    std::cout << " name = " << name; 
    std::cout<<" 06 0 HERE "<<std::endl;
    std::cout  << " do_runop = " << do_runop; 
    std::cout<<" 06 1 HERE "<<std::endl;
    std::cout    << " do_histos = " << do_histos ;
    std::cout<<" 06 2 HERE "<<std::endl;
    std::cout << " do_trees = " << do_trees; 
    std::cout<<" 06 3 HERE "<<std::endl;
    std::cout  << " (flag & DORUNOP) = " << PassFlag((selit_pair.first->second), DORUNOP);
    std::cout<<" 06 4 HERE "<<std::endl;
    std::cout  << " (flag & DOHIST) = " << PassFlag((selit_pair.first->second), DOHIST);
    std::cout<<" 06 5 0 HERE "<<std::endl;
    std::cout << " (flag & DOTREE) = " << PassFlag((selit_pair.first->second), DOTREE);
    std::cout<<" 06 6 0 HERE "<<std::endl;
    std::cout <<std::endl;
    std::cout<<" 07 HERE "<<std::endl;
  }
  std::cout<<" 08 HERE "<<std::endl;

  return true;

}

/*
bool SelectorBase::AddSelection( const int index, const std::string &name, const bool do_runop, const bool do_histos, const bool do_trees ) {

  if(m_opt->MsgLevel() == Debug::DEBUG){
    std::cout<<" Adding Selection "<<index<<" with name "<<name<<std::endl;
  }
  std::pair< std::map<int, Selection>::iterator, bool > selit_pair = m_selections -> insert( std::pair< int, Selection >(index, Selection()) );
  if(m_outData->o_sel_decisions == NULL){ m_outData->o_sel_decisions = new std::map<int, bool>;}
  std::pair< std::map<int, bool>::iterator, bool > decit_pair = m_outData->o_sel_decisions->insert( std::pair<int, bool>(index, false) );
  if(!decit_pair.second){
    std::cerr << " Error in SelectorBase::AddSelection --> OutputData already contains a decision element for selection index " << index <<std::endl;
    return false;
  }

  if( selit_pair.second ) {

    m_map_sel_int_string -> insert( std::pair< int, std::string >(index, name) );

    (selit_pair.first->second).selec_ind = index;
    (selit_pair.first->second).name = name;
    (selit_pair.first->second).decision = &(decit_pair.first->second);

    (selit_pair.first->second).flags = 0;
    AddFlag( (selit_pair.first->second), DORUNOP, do_runop );
    AddFlag( (selit_pair.first->second), DOHIST, do_histos ); 
    AddFlag( (selit_pair.first->second), DOTREE, do_trees ); 

    (selit_pair.first->second).ancestors = std::vector<int>();
    (selit_pair.first->second).primary_ancestor = -1;
    (selit_pair.first->second).primary_descendants = std::vector<int>();

    (selit_pair.first->second).ancestors.clear();
    (selit_pair.first->second).primary_descendants.clear();

    //____________________________________________________________________

    AddAncestorsRecursive(selit_pair.first->second);

    //____________________________________________________________________

    return true;
  }
  else{
      std::cerr << " Error in SelectorBase::AddSelection --> Attempt to overwrite selection index "
	      << index << " with name "<<(selit_pair.first)->second.name<<" by new name "<<name<<std::endl;
    return false;
  }

  return false;

}

 */

/*
  virtual bool AddFlag(const int index, const std::string& flag, const bool value=true );
  virtual bool AddFlag(Selection& sel , const std::string& flag, const bool value=true );
  bool AddFlag(const int index, const int flag, const bool value=true );
  void AddFlag(Selection& sel , const int flag, const bool value=true );

 */
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

//___________________________________________________________
//
bool SelectorBase::AddFlag(const int index, const int flag, const bool value){

  std::map<int, Selection>::iterator selit = m_selections->find(index);
  if( selit == m_selections->end() ){
    std::cerr<<" Error in SelectorBase::AddFlag() --> Selection at index "<< index << "cannot be found"<<std::endl;
    return false;
  }
  AddFlag(selit->second, flag, value); 
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

  if(flag=="do_runop"){ AddFlag(sel, DORUNOP, value); }
  else if(flag=="do_histos"){ AddFlag(sel, DOHIST, value); }
  else if(flag=="do_trees"){ AddFlag(sel, DOTREE, value); }
  else{ std::cerr << " ERROR in SelectorBase --> Unknown flag "<<flag<<std::endl; return false; }
  return true;

}

bool SelectorBase::PassFlag(const Selection& sel, const int flag) const{
  return (sel.flags & 0x1<<flag); 
}
//___________________________________________________________
//
/*
bool SelectorBase::PassSelection( const int sel ) const {

  bool pass = false;
  std::map<int, Selection>::iterator selit = m_selections->find(sel);
  if( selit == m_selections->end() ){
    std::cerr<<" Error in SelectorBase::PassSelection() --> Selection at index "<< sel << "cannot be found"<<std::endl;
    return false;
  }
  else{
    if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<"SelectorBase::PassSelection on index "<<sel<<std::endl; }
    pass = PassSelection(selit->second); 
  } 

  return pass;

}
*/
//___________________________________________________________
//
bool SelectorBase::PassSelection( const Selection& sel, bool check_primary ) const {

  if( m_opt -> MsgLevel() == Debug::DEBUG ){ 
    std::cout << "Entering SelectorBase::PassSelection for selection " << sel.name << std::endl; 
    }
  //std::cout << " Running full selection for " << sel.name << std::endl;
  bool pass = true;
  if(sel.ancestors.size() > 0){ 
    for(int anc : sel.ancestors){ 
      //std::cout << " Now checking ancestor " << anc << " of selection " << sel.name << std::endl;
      if(anc < 0 ) pass = true;
      else if( !check_primary && (anc == sel.primary_ancestor) ) continue;
      else{
	std::map<int, Selection>::iterator selit = m_selections->find(anc);
	if(selit != m_selections->end()){ pass = PassSelection( selit->second, check_primary ); }
	else{  pass = PassSelection(anc); }
	//std::cout << "Decision for ancestor "<<anc<<" : "<<pass<<std::endl;
      }
      if(!pass) break;
    }
  }
  pass = pass && PassSelection(sel.selec_ind);
  *(sel.decision) = pass;
  //std::cout << "Decision for " << sel.name << " : " << pass << std::endl;

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
bool SelectorBase::RunSelectionNode( const Selection& sel ){

  if(m_opt -> MsgLevel() == Debug::DEBUG){
    std::cout << " FIRST SelectorBase::RunSelectionNode() --> selection = " << sel.name 
	      << " Number of descendants = " << sel.primary_descendants.size()  
	      << std::endl;

  }

  bool pass_node = PassSelection(sel, false);
  if(m_opt -> MsgLevel() == Debug::DEBUG){
    std::cout << " SECOND SelectorBase::RunSelectionNode() --> selection = " << sel.name << " pass_node = " << pass_node 
	      << " Number of descendants = " << sel.primary_descendants.size()  
	      << std::endl;

  }
  if( !pass_node ) return pass_node;
  m_nPass++;

  if( PassFlag(sel, DORUNOP) ){
    //std::cout << " Calling run operations for selection "<<sel.name<<std::endl;
    if(!RunOperations(sel)){ std::cerr << "ERROR in SelectorBase::RunSelectionNode() -> Failure to execute RunOperations on selection node " << sel.selec_ind << std::endl;}
  }

  for( const int descendant : sel.primary_descendants ){ RunSelectionNode(descendant); }

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

//__________________________________________________________
//
void SelectorBase::PrintSelectionTree() const{

  std::cout<< "************ TOP_SELECTIONS **************" << std::endl;
  for(std::pair<int, Selection*> sel : *m_top_selections){ std::cout << sel.second->name << std::endl; }
  std::cout<< "*******************************************" << std::endl;
  std::cout<<std::endl<<std::endl<<std::endl;

  std::cout<< "************ ALL_SELECTIONS **************" << std::endl;
  for(std::pair<int, Selection> sel : *m_selections){ 
    std::cout <<"SELECTION : " << sel.second.name << ";  INDEX : " << sel.second.selec_ind << std::endl;
    if(sel.second.primary_ancestor  >= 0){ std::cout <<" PRIMARY ANCESTOR : " << m_selections->at(sel.second.primary_ancestor).name << std::endl; }
    else{ std::cout <<" PRIMARY ANCESTOR : " << sel.second.primary_ancestor << std::endl; } 
    std::cout <<" ANCESTOR LIST : "<<std::endl;
    for( int anc : sel.second.ancestors ){
      if( (anc >= 0) && (m_selections->find(anc) != m_selections->end()) ){ std::cout << "   " << m_selections->at(anc).name << std::endl; }
      else{ std::cout << "   " << anc << std::endl; }
    }
    std::cout <<" PRIMARY DESCENDANT LIST : "<<std::endl;
    for( int dec : sel.second.primary_descendants ){
      std::cout << "   " << m_selections->at(dec).name << std::endl;
    }

    std::cout << std::endl << std::endl << std::endl;

  }


  return;

}

const Selection* SelectorBase::GetSelection(const int node) const{

 std::map<int, Selection>::iterator selit = m_selections->find(node);
 if( selit == m_selections->end() ){
   std::cerr<<" Error in SelectorBase::RunOperations() --> Selection at index "<< node << "cannot be found"<<std::endl;
   return NULL;
 }
 Selection* sel = &(selit->second);
 return sel;

}
