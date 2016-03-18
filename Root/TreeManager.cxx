#include "IFAETopFramework/TreeManager.h"

//______________________________________________________________________
//
TreeManager::TreeManager(){
    
    m_tree.clear();
    m_branches.clear();
    return;
    
}

//______________________________________________________________________
//
void TreeManager::BookTree(const std::string& name, const std::string& title, const std::string& key){
    
    TTree* t1 = new TTree(name.c_str(), title.c_str());
    const std::string& _key = (key == "") ? name : key;
    if(m_tree.find(_key) != m_tree.end()){
        std::cout<<"Warning: Replacing existing Tree "<<name<<std::endl;
        delete m_tree[_key];
    }
    m_tree[_key]=t1;
    
    return;
    
}

//______________________________________________________________________
//
void TreeManager::AddBranchToTree(const std::string& tname, VariableDef& bVar){
    if(!m_tree[tname]){std::cout<<"TREE "<<tname<<" was not found. Could not add branch"<<std::endl; return;}
    
    if(bVar.IsPrimitive()){
      std::string leaf = bVar.Name() + "/" + bVar.VarTypeString();
      m_tree[tname]->Branch(bVar.Name().c_str(), bVar.Address(), leaf.c_str());
    }
    else if( ( bVar.IsVector() && (bVar.VecInd()>=0) ) || ( bVar.IsAnaObject() && !bVar.IsVector() ) ){
      std::string leaf = bVar.Name() + "/D";
      m_tree[tname]->Branch(bVar.Name().c_str(),bVar.ValStore(), leaf.c_str() );
    }
    else if( bVar.IsAnaObject() && bVar.IsVector() && (bVar.VecInd()<0) ){
      m_tree[tname]->Branch(bVar.Name().c_str(),"std::vector<double>",bVar.VecStore() );
    }
    else if( !bVar.IsAnaObject() && bVar.IsVector() && (bVar.VecInd()<0) ){
        TString typeVar = "";

        if(bVar.VarType()==VariableDef::VECINT) typeVar = "std::vector<int>";
        else if(bVar.VarType()==VariableDef::VECVECINT) typeVar = "std::vector<std::vector<int> >";
        else if(bVar.VarType()==VariableDef::VECFLOAT) typeVar = "std::vector<float>";
        else if(bVar.VarType()==VariableDef::VECVECFLOAT) typeVar = "std::vector<std::vector<float> >";
        else if(bVar.VarType()==VariableDef::VECDOUBLE) typeVar = "std::vector<double>";
        else if(bVar.VarType()==VariableDef::VECVECDOUBLE) typeVar = "std::vector<std::vector<double> >";
	else{ std::cerr << "<!> Error in TreeManager::AddBranchToTree(): the variable type is not recognized !!" << std::endl; }
	m_tree[tname]->Branch(bVar.Name().c_str(),typeVar,bVar.Address() );
    }
    
    return;
}

//______________________________________________________________________
//
int TreeManager::ReadTree(const std::string& name, TFile* f, const std::string& key){
    
  const std::string& s = (key == "") ? name : key;
  if(m_tree.find(s)!= m_tree.end()){
    std::cout<<"Tree "<<s.c_str()<<" already exists"<<std::endl;
    return -1;
  } //if tree already exists, do nothing
  m_tree[s]=(TTree*)(f->Get(name.c_str()));
  if(m_tree[s] == NULL){std::cout<<name.c_str()<<" not found in file"<<std::endl; return -1;}
  m_tree[s]->SetDirectory(0);
  m_tree[s]->SetName(s.c_str());
    
  std::map<std::string, TBranch*> _branchList; _branchList.clear();
  m_branches[s] = _branchList; //WTF
    
  return 0;
    
}

//______________________________________________________________________
//
void TreeManager::SetBranchToTree(const std::string& tname, VariableDef& bVar, const std::string& inputVarName){
    
    if(!m_tree[tname]){std::cout<<"TREE "<<tname<<" was not found. Could not set branch"<<std::endl; return;}
    
    std::string _branchName = (inputVarName == "") ? inputVarName : bVar.Name();
    _branchName = "b_" + _branchName;
    m_branches[tname][_branchName] = NULL;
    m_tree[tname]->SetBranchAddress( _branchName.c_str(), bVar.Address(), &(m_branches[tname][_branchName])  );
    
    return;
}

//______________________________________________________________________
//
void TreeManager::SetTree(const std::string& tkey, TTree* tree){
    //This one is tricky. How do branch addresses propagate
    if(m_tree.find(tkey) != m_tree.end()){
        std::cout<<"Error: TTREE "<<tkey<<" already exists"<<std::endl;
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
    if(m_tree.find(tkey) != m_tree.end()){
        delete m_tree[tkey];
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
    if(m_tree.find(s_tree) == m_tree.end() ){
        std::cout<<"Tree "<<s_tree<<" not found "<<std::endl;
        return;
    }
    
    //Remember to also clear the branch list if needed
    delete m_tree[s_tree];
    m_tree.erase(m_tree.find(s_tree));
    return;
}

//______________________________________________________________________
//
void TreeManager::FillTree(const std::string& name, const std::string& key){
  const std::string& _key = (key == "") ? name : key;
  if(m_tree.find(_key) != m_tree.end()){
    m_tree[_key]->Fill();
  }
    return;
}
