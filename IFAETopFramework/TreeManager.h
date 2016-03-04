#ifndef TREEMANAGER_H
#define TREEMANAGER_H

#include <TTree.h>
#include <TFile.h>
#include <map>
#include <math.h>
#include <string>
#include <iostream>
#include "IFAETopFramework/VariableDef.h"

class TreeManager{
    
public:
    TreeManager();
    
    void BookTree(const std::string& name, const std::string& title, const std::string& key="");
    void AddBranchToTree(const std::string& tname, VariableDef& bVar);
    
    int ReadTree(const std::string& name, TFile* f, const std::string& key);
    void SetBranchToTree(const std::string& tname, VariableDef& bVar, const std::string& inputVarName = "");
    
    TTree* Tree(const std::string& tname){ return m_tree[tname]; }
    
    void SetTree(const std::string& tkey, TTree* tree);
    void ReplaceTree(const std::string& tkey, TTree* tree);
    
    std::vector<std::string> TreeKeyList();
    
    void ClearTree(const std::string& s_tree);
    void FillTree(const std::string& name, const std::string& key="");
    
protected:
    std::map<std::string, TTree*> m_tree;
    std::map<std::string, std::map<std::string, TBranch*> > m_branches;
    
};

#endif
