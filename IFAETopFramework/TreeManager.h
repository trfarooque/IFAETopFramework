#ifndef TREEMANAGER_H
#define TREEMANAGER_H

#include <TTree.h>
#include <map>
#include <string>

#include <iostream>
class TFile;

//#include "IFAETopFramework/VariableDef.h"

class TreeManager{
    
public:

    TreeManager();
    TreeManager(const TreeManager& q);
    ~TreeManager();
    
    TTree* BookTree(const std::string& name, const std::string& title, const std::string& key="");
    TTree* ReadTree(const std::string& name, TFile* f, const std::string& key);
    TTree* Tree(const std::string& tname){ return m_tree[tname]; }
    
    void SetTree(const std::string& tkey, TTree* tree);
    void ReplaceTree(const std::string& tkey, TTree* tree);
    
    std::vector<std::string> TreeKeyList();
    inline int NTrees() const{ return m_tree.size();}
    void ClearTree(const std::string& s_tree);
    void FillTree(const std::string& key);
    
protected:
    std::map<std::string, TTree*> m_tree;
    //std::map<std::string, std::map<std::string, TBranch*> > m_branches;
    
};

#endif
