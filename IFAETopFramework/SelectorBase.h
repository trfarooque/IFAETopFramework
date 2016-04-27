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
  std::vector<int> children;
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
  
  bool AddSelection( const int index, const std::string &name, const int parent_index = -1, const bool has_histos = true, const bool has_trees = true );
  virtual bool PassSelection( const int /*sel*/) const;
  bool GetDecision(const int sel) const;
  bool PassSelectionChain() const;
  bool PassSelectionNode(const int node) const;


  inline std::map < int, std::string >* GetSelectionMap() const { return m_map_sel_int_string; }
  inline std::map < int, Selection >* GetSelections() const { return m_selections; }
  inline std::vector < int >* GetTopSelections() const { return m_top_selections; }
  
 protected:
  void SetDecision(const int sel, const bool decision);

  OptionsBase *m_opt;
  OutputData  *m_outData;
  std::map < int, Selection > *m_selections;
  std::vector<int> *m_top_selections;
  std::map<int, std::string > *m_map_sel_int_string;

};

#endif // SELECTORBASE_H
