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
    
    TTree* BookTree(const std::string& name, const std::string& title, const std::string& key="");
    //void AddBranchToTree(const std::string& tname, VariableDef& bVar);

    /*
    template<typename T> bool AddBranchToTree(const std::string& tname, const std::string& bname, T* obj){
      if(!m_tree[tname]){std::cout<<"TreeManager::AddBranchToTree()-->ERROR : TREE "<<tname<<" was not found. Could not add branch"<<std::endl; return false;}
      TBranch* check_br = m_tree[tname]->Branch(bname, obj);
      if(check_br == NULL) return false;
      else return true;
    }
    template<typename T> bool AddBranchToTree(const std::string& tname, const std::string& bname, T** addobj){
      if(!m_tree[tname]){std::cout<<"TreeManager::AddBranchToTree()-->ERROR : TREE "<<tname<<" was not found. Could not add branch"<<std::endl; return false;}
      TBranch* check_br = m_tree[tname]->Branch(bname, addobj);
      if(check_br == NULL) return false;
      else return true;
    }
    */
    TTree* ReadTree(const std::string& name, TFile* f, const std::string& key);
//______________________________________________________________________
//
/*
    template<typename T > bool SetBranchToTree(const std::string& tname, T* obj, const std::string& inputVarName){
      
      if(!m_tree[tname]){std::cout<<"TreeManager::SetBranchToTree --> ERROR: TREE "<<tname<<" was not found. Could not set branch"<<std::endl; return false;}
      TBranch* _br = NULL;
      m_tree[tname]->SetBranchAddress( inputVarName.c_str(), obj, &_br );
      
      return;
    }

    template<typename T > bool SetBranchToTree(const std::string& tname, T** addobj, const std::string& inputVarName){
      
      if(!m_tree[tname]){std::cout<<"TreeManager::SetBranchToTree --> ERROR: TREE "<<tname<<" was not found. Could not set branch"<<std::endl; return false;}
      TBranch* _br = NULL;
      m_tree[tname]->SetBranchAddress( inputVarName.c_str(), addobj, &_br );
      
      return;
    }
*/
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
