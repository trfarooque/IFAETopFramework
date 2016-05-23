#ifndef SELECTORBASE_H
#define SELECTORBASE_H

#include <string>
#include <vector>
#include <map>

class OptionsBase;
class OutputData;

struct Selection{
  int selec_ind;
  std::string name;
  bool* decision;
  std::vector<int> ancestors;
  int primary_ancestor;
  std::vector<int> primary_descendants;
  int flags;
};



class SelectorBase {


public:

  enum BaseSelFlags{ DORUNOP=0,DOHIST,DOTREE };

  SelectorBase( OptionsBase *opt, OutputData *outData, const bool add_ancestors=false);
  SelectorBase( const SelectorBase &q );
  virtual ~SelectorBase();
  
  bool AddSelection( const int index, const std::string &name, const bool do_runop = true, const bool do_histos = true, const bool do_trees = true );

  bool AddFlag(const int index, const std::string& flag, const bool value=true );
  virtual bool AddFlag(Selection& sel , const std::string& flag, const bool value=true );
  bool AddFlag(const int index, const int flag, const bool value=true );
  void AddFlag(Selection& sel, const int flag, const bool value=true );

  bool PassFlag(const Selection& sel, const int flag) const;

  bool AddAncestorsRecursive(Selection& sel);

  bool PassSelection( const Selection& sel, const bool check_primary=true) const;
  virtual bool PassSelection( const int /*sel*/) const{ return true; }

  bool RunSelectionChain();
  bool RunSelectionNode(const int node);
  bool RunSelectionNode(const Selection& sel_node);

  bool RunOperations(const int node) const;
  virtual bool RunOperations(const Selection& /*sel*/) const{ return false;}
  void PrintSelectionTree() const;

  const Selection* GetSelection(const int node) const;
  inline std::map < int, std::string >* GetSelectionMap() const { return m_map_sel_int_string; }
  inline std::map < int, Selection >* GetSelections() const { return m_selections; }
  inline std::map < int, Selection* >* GetTopSelections() const { return m_top_selections; }
  
 protected:

  virtual int AddAncestors(std::vector<int>& /* anclist*/, const int /*node*/){ return -1; }

  OptionsBase *m_opt;
  bool m_add_ancestors;
  OutputData  *m_outData;
  std::map < int, Selection > *m_selections;
  std::map < int, Selection* > *m_top_selections;
  std::map < int, std::string > *m_map_sel_int_string;
  int m_nPass; //a boolean to be used to determine if one selection in the chain has already passed

};

#endif // SELECTORBASE_H
