#ifndef SELECTION_H
#define SELECTION_H

#include <string>
#include <vector>
#include <map>

#include "IFAETopFramework/VariableDef.h"

class OutputData;
enum Comparator{GT=1, GEQ, EQ, LEQ, LT};

struct VarCut{
  std::string name;
  VariableDef* var;
  //bool isIntegerType;
  double cut;
  int int_cut;
  int comparator; 

  ~VarCut(){delete var;}
};


class Selection{

 public:

 Selection( const int index, const std::string& name, OutputData* outData,
	     std::vector<VarCut*>* cuts=NULL, const int flags=0, 
	     std::vector<Selection*>* ancestors=NULL,  std::vector<int>* primary_descendants=NULL );

 Selection( const int index, const std::string& name, bool* decision=NULL, bool* isSet=NULL, 
	     std::vector<VarCut*>* cuts=NULL, const int flags=0, 
	     std::vector<Selection*>* ancestors=NULL,  std::vector<int>* primary_descendants=NULL );

 Selection(Selection& q);

 ~Selection();


  //Getters
  inline int SelecInd() const { return m_selec_ind; }
  inline std::string Name() const { return m_name; }
  inline bool Decision() const { return *m_decision; }
  inline bool IsSet() const { return *m_isSet; }
  inline bool* GetDecisionAddress() const { return m_decision; }
  inline bool* GetIsSetAddress() const { return m_isSet; }

  inline double NumPassRaw() const { return m_numPass_raw; }
  inline double NumPassWgt() const { return m_numPass_wgt; }
  inline const std::vector<Selection*>* Ancestors() const { return m_ancestors; }
  inline int PrimaryAncestor() const { return m_primary_ancestor; }
  inline const std::vector<int>* PrimaryDescendants() const { return m_primary_descendants; }
  inline int Flags() const { return m_flags; }
  inline const std::vector<VarCut*>* VarCuts() const { return  m_cuts; }
  
  //Setters
  inline void SetSelecInd(int selec_ind) { m_selec_ind = selec_ind; }
  inline void SetName(const std::string& name) { m_name = name; }
  inline void SetDecisionAddress(bool* decision) { m_decision = decision; }
  inline void SetDecisionValue(bool decision) { *m_decision = decision; }
  inline void SetIsSetAddress(bool* isSet) { m_isSet = isSet; }
  inline void SetIsSetValue(bool isSet) { *m_isSet = isSet; }
  inline void SetNumPassRaw(int numPass_raw) { m_numPass_raw = numPass_raw; }
  inline void SetNumPassWgt(int numPass_wgt) { m_numPass_wgt = numPass_wgt; }
  //inline void SetAncestors(std::vector<Selection*>* ancestors) { m_ancestors = ancestors; }
  inline void SetPrimaryAncestor(int primary_ancestor) { m_primary_ancestor = primary_ancestor; }
  //inline void SetPrimaryDescendants(std::vector<int>* primary_descendants) { m_primary_descendants = primary_descendants; }
  inline void SetFlags(int flags) { m_flags = flags; }
  //inline void SetCuts(std::vector<VarCut*>* cuts) { m_cuts = cuts; }

  //C++ functions
  void AddAncestor(Selection* ancestor);
  void AddPrimaryDescendant(int decendant);
  void AddCut(VarCut* cut);

  template<typename T> VarCut* MakeCut(T* t, double cut, const std::string& comparator=">=", int vec_ind=-1, const std::string& moment=""){ //protected

    int _comparator = 0;
    if(comparator == ">"){ _comparator = GT; }
    else if(comparator == ">="){ _comparator = GEQ; }
    else if( (comparator == "=") || (comparator == "==") ){ _comparator = EQ; }
    else if(comparator == "<"){ _comparator = LT; }
    else if(comparator == "<="){ _comparator = LEQ; }
    else{ 
      std::cerr << " Selection::MakeCut--> ERROR : Comparator "<<comparator<<" can not be recognised "<<std::endl;
      return NULL;
    }

    VarCut* varcut = new VarCut();
    varcut->comparator = _comparator;
    varcut->var = new VariableDef("","", t, vec_ind, moment);
    varcut->cut = cut;


    return varcut;
  }

  template<typename T> bool AddCut(const std::string& name, T* t, double cut, const std::string& comparator=">=", int vec_ind=-1, const std::string& moment=""){

    VarCut* varcut = MakeCut(t, cut, comparator, vec_ind, moment);
    if(varcut == NULL) return false;
    varcut->name = name;
    AddCut(varcut);
    return true;

  }
  template<typename T> bool AddCut(T* t, double cut, const std::string& comparator=">=", int vec_ind=-1, const std::string& moment=""){

    bool stat = AddCut("", t, cut, comparator, vec_ind, moment);

    return stat;
  }

  void AddFlag(const int flag, const bool value); //Add this bit to the flags
  void AddFlagAtBit(const int bit_posn, const bool value); //Add the bit at position counted by bit_posn to the flags
  bool PassFlag(const int flag) const;
  bool PassFlagAtBit(const int bit_posn) const;
  bool PassSelection( const bool useDecision=true, const bool check_primary=true);
  bool PassSelectionCuts() const;
  static bool PassCut(VarCut& cut);

 private:
  int m_selec_ind;
  std::string m_name;
  bool* m_decision;
  bool* m_isSet;
  double m_numPass_raw;
  double m_numPass_wgt; 
  std::vector<Selection*>* m_ancestors; //NOT owned
  int m_primary_ancestor;
  std::vector<int>* m_primary_descendants;
  int m_flags;
  std::vector<VarCut*>* m_cuts; //Owned

  
};


#endif
