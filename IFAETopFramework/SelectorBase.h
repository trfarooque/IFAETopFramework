#ifndef SELECTORBASE_H
#define SELECTORBASE_H

#include <string>
#include <vector>
#include <map>

class OptionsBase;
//class NtupleData;
class OutputData;

struct Selection{
  int selec_ind;
  std::string name;
  int parent;
  std::vector<int> descendants;
  bool* decision;
  bool has_histos;
  bool has_trees;
  std::map<std::string, bool> flags;
};

  

class SelectorBase {

public:
  SelectorBase( OptionsBase *opt, OutputData *outData);
  SelectorBase( const SelectorBase &q );
  virtual ~SelectorBase();
  
  bool AddSelection( const int index, const std::string &name, const bool has_histos = true, const bool has_trees = true );
  bool AddFlag(const int index, const std::string& flag, const bool value=true );
  bool AddFlag(Selection& sel , const std::string& flag, const bool value=true );

  int FindClosestAncestor(const int index) const;
  virtual bool PassSelection( const int sel) const;

  bool GetDecision(const int sel) const;
  bool GetDecision(const Selection& sel) const;
  bool PassSelectionChain() const;
  bool RunSelectionChain() const;
  bool PassSelectionNode(const int node) const;
  bool PassSelectionNode(const Selection& sel_node) const;

  bool RunSelectionNode(const int node) const;
  bool RunSelectionNode(const Selection& sel_node) const;
  bool RunOperations(const int node) const;
  virtual bool RunOperations(const Selection& /*sel*/) const{ return false;}

  inline std::map < int, std::string >* GetSelectionMap() const { return m_map_sel_int_string; }
  inline std::map < int, Selection >* GetSelections() const { return m_selections; }
  inline std::map < int, Selection* >* GetTopSelections() const { return m_top_selections; }
  
 protected:

  virtual int GetParentIndex(const int index) const = 0;
  int FindClosestAncestor(Selection& sel);
  void SetDecision(const int sel, const bool decision);

  OptionsBase *m_opt;
  OutputData  *m_outData;
  std::map < int, Selection > *m_selections;
  std::map < int, Selection* > *m_top_selections;
  //std::vector < int > *m_top_sel_inds;
  std::map < int, std::string > *m_map_sel_int_string;
  int m_nPass; //a boolean to be used to determine if one selection in the chain has already passed

};

#endif // SELECTORBASE_H
