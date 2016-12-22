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

  if(m_ancestors == NULL){ m_ancestors = new std::vector<Selection*>(); m_ancestors->clear(); }
  if(m_primary_descendants == NULL){ m_primary_descendants = new std::vector<int>(); m_primary_descendants->clear(); }
  if(m_cuts == NULL){ m_cuts = new std::vector<VarCut*>(); m_cuts->clear(); }

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


  if(m_ancestors == NULL){ m_ancestors = new std::vector<Selection*>(); m_ancestors->clear(); }
  if(m_primary_descendants == NULL){ m_primary_descendants = new std::vector<int>(); m_primary_descendants->clear(); }
  if(m_cuts == NULL){ m_cuts = new std::vector<VarCut*>(); m_cuts->clear(); }

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

Selection::Selection(const Selection& q){

  m_selec_ind = q.m_selec_ind;
  m_name = q.m_name;
  m_decision = q.m_decision;
  m_isSet = q.m_isSet;
  m_numPass_raw = q.m_numPass_raw;
  m_numPass_wgt = q.m_numPass_wgt; 
  m_ancestors = q.m_ancestors; //NOT owned
  m_primary_ancestor = q.m_primary_ancestor;
  m_primary_descendants = q.m_primary_descendants;
  m_flags = q.m_flags;
  m_cuts = q.m_cuts; //Owned
  
  

}

Selection::~Selection(){

  for(std::vector<VarCut*>::iterator varcut_it = m_cuts->begin(); varcut_it != m_cuts->end(); varcut_it = m_cuts->erase(varcut_it) ){
    delete *varcut_it; *varcut_it = NULL;
  }
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
bool Selection::PassCut(VarCut& varcut) {

  varcut.var->CalcDoubleValue();
  double val = varcut.var->GetDoubleValue();

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

  bool pass = true;
  for(VarCut* varcut : *m_cuts){
    pass = pass && PassCut(*varcut);
    if(!pass) break;
  }

  return pass;

}


