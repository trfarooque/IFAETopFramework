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
  bool* isSet;
  double numPass_raw;
  double numPass_wgt; 
  std::vector<Selection*> ancestors;
  int primary_ancestor;
  std::vector<int> primary_descendants;
  int flags;
};


class SelectorBase {


public:

  enum BaseSelFlags{ DORUNOP=0,DOHIST,DOTREE };

  //_________________________________

  //Constructors and destructors
  //__________________________________


  SelectorBase( OptionsBase *opt, OutputData *outData, const bool useDecisions=true, const bool add_primaries=false);
  SelectorBase( const SelectorBase &q );
  virtual ~SelectorBase();

  //_________________________________

  //Setup selections
  //__________________________________

  
  bool AddSelection( const int index, const std::string &name = "", const bool do_runop = true, const bool do_histos = true, const bool do_trees = true );
  bool AddFlag(const int index, const std::string& flag, const bool value=true );
  virtual bool AddFlag(Selection& sel , const std::string& flag, const bool value=true );
  bool AddFlag(const int index, const int flag, const bool value=true );
  void AddFlag(Selection& sel, const int flag, const bool value=true );

  bool PassFlag(const Selection& sel, const int flag) const;

  //_________________________________

  //Pass and run selections
  //__________________________________

  bool PassSelection( Selection& sel, const bool useDecision=true, const bool check_primary=true);
  virtual bool PassSelection( const int /*sel*/){ return true; }

  bool RunSelectionChain();
  bool RunSelectionNode(const int node);
  bool RunSelectionNode( Selection& sel_node);

  bool RunOperations(const int node) const;
  virtual bool RunOperations(const Selection& /*sel*/) const{ return false;}

  //_______________________

  //Getter functions
  //_______________________

  const Selection* GetSelection(const int node) const;
  inline std::map < int, Selection* >* GetSelections() const { return m_selections; }
  inline std::map < int, Selection* >* GetSelectionTree() const { return m_selection_tree; }
  inline std::map < int, Selection* >* GetTopSelections() const { return m_top_selections; }

  //_______________________

  //Diagnostic Tools
  //_______________________

  void PrintSelectionTree( const bool printYields = false) const;
  
 protected:

  virtual Selection* MakeSelection( const int index, const std::string& name="" );
  virtual std::string  FindName(const int index) const;

  bool AddAncestor( Selection& sel, const int index, bool is_primary=false);
  int  AddPrimary( Selection& sel, const int primary);
  bool AddAncestors( Selection& sel, const std::vector< int >& anclist, const int primary=-1 );

  OptionsBase *m_opt;
  bool m_add_primaries;
  bool m_useDecisions;
  OutputData  *m_outData;
  std::map < int, Selection* > *m_selections; //The store of ALL selections that have been defined

  std::map < int, Selection* > *m_selection_tree; //The actual tree of inter-linked selections that are to be run for each event
  std::map < int, Selection* > *m_top_selections; //The top nodes in the selection tree

  int m_nPass; //a boolean to be used to determine if one selection in the chain has already passed

};

#endif // SELECTORBASE_H
