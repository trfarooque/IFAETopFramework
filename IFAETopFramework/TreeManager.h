#ifndef TREEMANAGER_H
#define TREEMANAGER_H

#include <TTree.h>
#include <TFile.h>
#include <map> 
#include <math.h>
#include <string>
#include <iostream>
#include "IFAETopFramework/VariableDef.h"
using namespace std;

class TreeManager{

 public:
  TreeManager();

  void BookTree(string name, string title, string key="");
  void AddBranchToTree(string tname, VariableDef bVar);

  int ReadTree(string name, TFile* f, string key);
  void SetBranchToTree(string tname, VariableDef bVar, string inputVarName = "");
  
  TTree* GetTree(string tname){ return m_tree[tname]; }

  void SetTree(string tkey, TTree* tree);
  void ReplaceTree(string tkey, TTree* tree);

  vector<string> GetTreeKeyList();

  void ClearTree(string s_tree);
  void FillTree(string name, string key="");

 protected:
  map<string, TTree*> m_tree;
  map<string, map<string, TBranch*> > m_branches;

};

#endif
