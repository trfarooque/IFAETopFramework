#include "IFAETopFramework/TreeManager.h"

TreeManager::TreeManager(){
    
    m_tree.clear();
    m_branches.clear();
    return;
    
}

void TreeManager::BookTree(string name, string title, string key){
    
    TTree* t1 = new TTree(name.c_str(), title.c_str());
    
    if(key == ""){key = name;}
    if(m_tree.find(key) != m_tree.end()){
        cout<<"Warning: Replacing existing Tree "<<name<<endl;
        delete m_tree[key];
    }
    m_tree[key]=t1;
    
    return;
    
}

/*
 
 
 if(variableType=="I") hist -> varType = VariableType::INT;
 else if(variableType=="D") hist -> varType = VariableType::DOUBLE;
 else if(variableType=="F") hist -> varType = VariableType::FLOAT;
 else if(variableType=="VI") hist -> varType = VariableType::VECINT;
 else if(variableType=="VF") hist -> varType = VariableType::VECFLOAT;
 else if(variableType=="VD") hist -> varType = VariableType::VECDOUBLE;
 
 */

void TreeManager::AddBranchToTree(string tname, VariableDef bVar){
    if(!m_tree[tname]){cout<<"TREE "<<tname<<" was not found. Could not add branch"<<endl; return;}
    
    if(bVar.isPrimitive()){
        m_tree[tname]->Branch(bVar.name(), bVar.address(), bVar.name() + "/" + bVar.varTypeString());
    }
    else{
        m_tree[tname]->Branch(bVar.name(), bVar.address());
    }
    
    return;
    
}

int TreeManager::ReadTree(string name, TFile* f, string key){
    
    string s;
    if(key!=""){s=key;}
    else{ s=name; }
    
    if(m_tree.find(s)!= m_tree.end()){
        cout<<"Tree "<<s.c_str()<<" already exists"<<endl;
        return -1;
    } //if tree already exists, do nothing
    m_tree[s]=(TTree*)(f->Get(name.c_str()));
    if(m_tree[s] == NULL){cout<<name.c_str()<<" not found in file"<<endl; return -1;}
    m_tree[s]->SetDirectory(0);
    m_tree[s]->SetName(s.c_str());
    
    map<string, TBranch*> _branchList; _branchList.clear();
    m_branches[s] = _branchList; //WTF
    
    return 0;
    
}

void TreeManager::SetBranchToTree(string tname, VariableDef bVar, string inputVarName){
    
    if(!m_tree[tname]){cout<<"TREE "<<tname<<" was not found. Could not set branch"<<endl; return;}
    
    string _branchName = (inputVarName == "") ? inputVarName : (string)bVar.name();
    _branchName = "b_" + _branchName;
    m_branches[tname][_branchName] = NULL;
    m_tree[tname]->SetBranchAddress( _branchName.c_str(), bVar.address(), &(m_branches[tname][_branchName])  );
    
    return;
}

void TreeManager::SetTree(string tkey, TTree* tree){
    //This one is tricky. How do branch addresses propagate
    if(m_tree.find(tkey) != m_tree.end()){cout<<"Error: TTREE "<<tkey<<" already exists"<<endl;}
    else{m_tree[tkey] = tree;}
    
    return;
}

void TreeManager::ReplaceTree(string tkey, TTree* tree){
    //Need decision on branch address replacements
    if(m_tree.find(tkey) != m_tree.end()){ delete m_tree[tkey]; }
    m_tree[tkey] = tree;
    
    return;
}

vector<string> TreeManager::GetTreeKeyList(){
    vector<string> v_key; v_key.clear();
    for(map<string, TTree*>::iterator t_it = m_tree.begin(); t_it != m_tree.end(); ++t_it){
        v_key.push_back(t_it->first);
    }
    return v_key;
}


void TreeManager::ClearTree(string s_tree){
    
    if(m_tree.find(s_tree) == m_tree.end() ){cout<<"Tree "<<s_tree<<" not found "<<endl; return;}
    //Remember to also clear the branch list if needed
    delete m_tree[s_tree];
    m_tree.erase(m_tree.find(s_tree));
    return;
}

void TreeManager::FillTree(string name, string key){
    if(key=="") key = name;
    if(m_tree.find(key) != m_tree.end()){
        m_tree[key]->Fill();
    }
    return;
}
