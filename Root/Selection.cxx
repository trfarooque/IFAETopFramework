#include "IFAETopFramework/Selection.h"
#include "IFAETopFramework/AnalysisUtils.h"
#include "IFAETopFramework/OutputData.h"

//___________________________________________________________
//
Selection::Selection( const int index, const std::string& name, 
		      bool* decision, bool* isSet,
		       std::vector<VarCut*>* cuts, const int flags, 
		       std::vector<Selection*>* ancestors, std::vector<int>* primary_descendants) :
  m_selec_ind(index),
  m_name(name),
  m_decision(decision),
  m_isSet(isSet),
  m_numPass_raw(0.),
  m_numPass_wgt(0.),
  m_ancestors(ancestors),
  m_primary_ancestor(-1),
  m_primary_descendants(primary_descendants),
  m_flags(flags),
  m_cuts(cuts)
{

  if(m_ancestors == NULL) m_ancestors = new std::vector<Selection*>();
  if(m_primary_descendants == NULL) m_primary_descendants = new std::vector<int>();
  if(m_cuts == NULL) m_cuts = new std::vector<VarCut*>();

  /*
  if(ancestors) m_ancestors = *ancestors;
  else{ m_ancestors = std::vector<Selection*>(); m_ancestors.clear(); }
  if(primary_descendants) m_primary_descendants = *primary_descendants;
  else{ m_primary_descendants = std::vector<int>(); m_primary_descendants.clear(); }
  if(cuts) m_cuts = *cuts;
  else{ m_cuts = std::vector<VarCut*>(); m_cuts.clear(); }
  */
}

Selection::Selection( const int index, const std::string& name, 
		      OutputData* outData,
		      std::vector<VarCut*>* cuts, const int flags, 
		      std::vector<Selection*>* ancestors, std::vector<int>* primary_descendants) :
  m_selec_ind(index),
  m_name(name),
  m_decision(NULL),
  m_isSet(NULL),
  m_numPass_raw(0.),
  m_numPass_wgt(0.),
  m_ancestors(ancestors),
  m_primary_ancestor(-1),
  m_primary_descendants(primary_descendants),
  m_flags(flags),
  m_cuts(cuts)
{ 

  if(m_ancestors == NULL) m_ancestors = new std::vector<Selection*>();
  if(m_primary_descendants == NULL) m_primary_descendants = new std::vector<int>();
  if(m_cuts == NULL) m_cuts = new std::vector<VarCut*>();


  /*
  if(ancestors) m_ancestors = *ancestors;
  else{ m_ancestors = std::vector<Selection*>(); m_ancestors.clear(); }
  if(primary_descendants) m_primary_descendants = *primary_descendants;
  else{ m_primary_descendants = std::vector<int>(); m_primary_descendants.clear(); }
  if(cuts) m_cuts = *cuts;
  else{ m_cuts = std::vector<VarCut*>(); m_cuts.clear(); }
  */
  if(outData==NULL){ std::cerr << "Error in Selection constructor --> Please provide a valid OutputData object" << std::endl; exit(1); }
  else{

    if(outData->o_sel_decisions == NULL){ outData->o_sel_decisions = new std::map<int, bool>;}
    std::pair< std::map<int, bool>::iterator, bool > decit_pair = outData->o_sel_decisions->insert( std::pair<int, bool>(index, false) );
    if(!decit_pair.second){
      std::cout << " Warning in Selection constructor --> OutputData already contains a decision element for selection index " << index <<std::endl;
    }

    if(outData->o_sel_isSet == NULL){ outData->o_sel_isSet = new std::map<int, bool>;}
    std::pair< std::map<int, bool>::iterator, bool > setit_pair = outData->o_sel_isSet->insert( std::pair<int, bool>(index, false) );
    if(!decit_pair.second){
      std::cout << " Warning in Selection constructor --> OutputData already contains a decision element for selection index " << index <<std::endl;
    }
    if(!setit_pair.second){
      std::cout << " Warning in Selection constructor --> OutputData already contains an isSet element for selection index " << index <<std::endl;
    }
    m_decision     = &(decit_pair.first->second);
    m_isSet        = &(setit_pair.first->second);

  }

}


Selection::~Selection(){

  std::cout<<" Deleting a selection with name "<<m_name<<"  with CutSet "<< m_cuts << " and " <<m_cuts->size()<<" number of cuts"<<std::endl;
  for( VarCut* varcut : *m_cuts ){
    std::cout<<" Deleting  cut "<<varcut<<std::endl;
    delete varcut;
  }
  m_cuts->clear();
  delete m_cuts;

  m_ancestors->clear();
  delete m_ancestors;
  m_primary_descendants->clear();
  delete m_primary_descendants;

}


//___________________________________________________________
//
void Selection::AddAncestor(Selection* ancestor){
  m_ancestors->push_back(ancestor);
  return;
}

//___________________________________________________________
//
void Selection::AddPrimaryDescendant(int primary_descendant){
  m_primary_descendants->push_back(primary_descendant);
  return;
}

//___________________________________________________________
//
void Selection::AddFlagAtBit(const int bit_posn, const bool value ){

  AddFlag(0x1<<bit_posn, value);
  return; 

}

//___________________________________________________________
//
void Selection::AddFlag(const int flag, const bool value ){

  if(value) m_flags |= flag;
  else      m_flags &= ~flag;
  return; 

}

//___________________________________________________________
//
void Selection::AddCut(VarCut* cut){

  m_cuts->push_back(cut);
  return;
}


//___________________________________________________________
//
bool Selection::PassFlag(const int flag) const{

  return (m_flags & flag); 

}

//___________________________________________________________
//
bool Selection::PassFlagAtBit(const int bit_posn) const{ 

  return PassFlag(0x1<<bit_posn);

}

//___________________________________________________________
//
bool Selection::PassCut(VarCut& varcut){

  varcut.var.CalcDoubleValue();
  double val = varcut.var.GetDoubleValue();

  bool pass = false;
  if(varcut.comparator == GT){ pass = (val > varcut.cut); }
  else if(varcut.comparator == GEQ){ pass = (val >= varcut.cut); }
  else if(varcut.comparator == EQ){ pass = AnalysisUtils::FloatEq( val, varcut.cut ); }
  else if(varcut.comparator == LEQ){ pass = (val <= varcut.cut); }
  else if(varcut.comparator == LT){ pass = (val < varcut.cut); }

  return pass;

}

//___________________________________________________________
//
bool Selection::PassSelectionCuts() const{

  bool pass = false;
  std::cout<<" Selection::PassSelectionCuts() -> m_name = "<<m_name<<" n_cuts = "<< m_cuts->size()<<std::endl;
  for(VarCut* varcut : *m_cuts){
    std::cout<<" varcut = "<<varcut<<std::endl;
    pass = pass && PassCut(*varcut);
    if(!pass) break;
  }
  return pass;

}

//___________________________________________________________
//
bool Selection::PassSelection( const bool useDecision, const bool check_primary){

  /*
    PassSelection checks the following in sequence:
    1. The decision bit of the selector IF it is set and IF useDecision=true
    2. Otherwise, it calls PassSelection first on the ancestors of this selection
    3. Then it checks additional cuts defined for this selection

    The decision bit of the selection is then set, and if the selection passes, then the number of 
    passed events for the selection is incremented

   */

  if(useDecision && *(m_isSet) ){ return *(m_decision); }

  bool pass = true;
  if(m_ancestors->size() > 0){ 
    for(Selection* anc : *m_ancestors){ 
      if( !check_primary && (anc->SelecInd() == m_primary_ancestor) ) continue;
      else{ pass = anc->PassSelection( useDecision, check_primary ); }
      if(!pass) break;
    }
  }
  pass = pass && PassSelectionCuts();
  if(!(*(m_isSet)) && pass){
    m_numPass_raw += 1.;
    //m_numPass_wgt += m_outData -> o_eventWeight_Nom; //redesign
  }

  *(m_decision) = pass;
  *(m_isSet) = true;

  return pass;
}

