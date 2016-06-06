#include "IFAETopFramework/TreeManager.h"
#include <TFile.h>
#include <math.h>

//______________________________________________________________________
//
TreeManager::TreeManager(){
    m_tree.clear();
    return;
}

//______________________________________________________________________
//
TTree* TreeManager::BookTree(const std::string& name, const std::string& title, const std::string& key){
  TTree* t1 = new TTree(name.c_str(), title.c_str());
  const std::string& _key = (key == "") ? name : key;
  if(m_tree.size() > 0 ){
    std::map<std::string, TTree*>::iterator it = m_tree.find(_key);
    if( it != m_tree.end()){
      std::cout<<"TreeManager::BookTree --> Warning: Replacing existing Tree "<<name<<std::endl;
      delete it->second;
    }
  }
  m_tree[_key] = t1;
  return t1;
    
}

//______________________________________________________________________
//

//______________________________________________________________________
//
TTree* TreeManager::ReadTree(const std::string& name, TFile* f, const std::string& key){
  TTree* _tree = NULL;
  const std::string& s = (key == "") ? name : key;
  std::map<std::string, TTree*>::iterator it = m_tree.find(s);
  if(it != m_tree.end()){
    std::cerr<<"TreeManager::ReadTree()-> ERROR Tree "<<s.c_str()<<" already exists"<<std::endl;
    return _tree;
  } //if tree already exists, do nothing
  _tree=(TTree*)(f->Get(name.c_str()));
  if(_tree == NULL){std::cerr<<" TreeManager::ReadTree -> ERROR: "<<name.c_str()<<" not found in file"<<std::endl; return _tree;}
  _tree->SetDirectory(0);
  _tree->SetName(s.c_str());

  m_tree[s] = _tree;
  return _tree;
}

//______________________________________________________________________
//
void TreeManager::SetTree(const std::string& tkey, TTree* tree){
    //This one is tricky. How do branch addresses propagate
  std::map<std::string, TTree*>::iterator it = m_tree.find(tkey);
  if(it != m_tree.end()){
      std::cerr<<"TreeManager::SetTree() -> ERROR: TTREE "<<tkey<<" already exists"<<std::endl;
    }
  else{
    m_tree[tkey] = tree;
  }
  return;
}

//______________________________________________________________________
//
void TreeManager::ReplaceTree(const std::string& tkey, TTree* tree){
    //Need decision on branch address replacements
  std::map<std::string, TTree*>::iterator it = m_tree.find(tkey);
  if(it != m_tree.end()){
    delete it->second;
  }
  m_tree[tkey] = tree;
  return;
}

//______________________________________________________________________
//
std::vector<std::string> TreeManager::TreeKeyList(){
  std::vector<std::string> v_key; v_key.clear();
  for(std::map<std::string, TTree*>::iterator t_it = m_tree.begin(); t_it != m_tree.end(); ++t_it){
    v_key.push_back(t_it->first);
  }
  return v_key;
}

//______________________________________________________________________
//
void TreeManager::ClearTree(const std::string& s_tree){

  std::map<std::string, TTree*>::iterator it = m_tree.find(s_tree);
  if(it != m_tree.end()){
    delete it->second;
    m_tree.erase(it);
  }
  else{
    std::cout<<"TreeManager::ClearTree() -> WARNING : Tree "<<s_tree<<" not found "<<std::endl;
  }
  return;
}

//______________________________________________________________________
//
void TreeManager::FillTree(const std::string& _key){
  std::map<std::string, TTree*>::iterator it = m_tree.find(_key);
  if(it != m_tree.end()){
    it->second->Fill();
  }
  else{
    std::cerr<<" TreeManager::FillTree() --> ERROR: No tree at key "<<_key<<std::endl;
  } 
   return;
}
