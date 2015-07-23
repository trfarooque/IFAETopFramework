#include "IFAETopFramework/HistManager.h"

using namespace std;

//__________________________________________________________________
//
HistManager::HistManager(){
  m_h1d.clear();
  m_h2d.clear();
  m_h3d.clear();
}

//__________________________________________________________________
//
HistManager::HistManager( const HistManager &q, bool make_new){
  m_h1d.clear();
  m_h2d.clear();
  m_h3d.clear();

  if(make_new){
    //Loop over old HistManager, clone each histogram, and add it to the new HistManager
    map<string, TH1D*>::const_iterator h1_it = (q.m_h1d).begin();
    for(; h1_it != q.m_h1d.end(); ++h1_it){
      TH1D* h1copy = (TH1D*)( (h1_it->second)->Clone() );
      m_h1d[h1_it->first] = h1copy;
    }

    map<string, TH2D*>::const_iterator h2_it = q.m_h2d.begin();
    for(; h2_it != q.m_h2d.end(); ++h2_it){
      TH2D* h2copy = (TH2D*)( (h2_it->second)->Clone() );
      m_h2d[h2_it->first] = h2copy;
    }

    map<string, TH3D*>::const_iterator h3_it = q.m_h3d.begin();
    for(; h3_it != q.m_h3d.end(); ++h3_it){
      TH3D* h3copy = (TH3D*)( (h3_it->second)->Clone() );
      m_h3d[h3_it->first] = h3copy;
    }
  }
  else{
    m_h1d = q.m_h1d;
    m_h2d = q.m_h2d;
    m_h3d = q.m_h3d;
  } 

  return;

}

//__________________________________________________________________
//
vector<string> HistManager::GetTH1KeyList(){
  vector<string> v_key; v_key.clear();
  for(map<string, TH1D*>::iterator h_it = m_h1d.begin(); h_it != m_h1d.end(); ++h_it){
    v_key.push_back(h_it->first);
  }
  return v_key;
}

//__________________________________________________________________
//
vector<string> HistManager::GetTH2KeyList(){
  vector<string> v_key; v_key.clear();
  for(map<string, TH2D*>::iterator h_it = m_h2d.begin(); h_it != m_h2d.end(); ++h_it){
    v_key.push_back(h_it->first);
  }
  return v_key;
}

//__________________________________________________________________
//
vector<string> HistManager::GetTH3KeyList(){
  vector<string> v_key; v_key.clear();
  for(map<string, TH3D*>::iterator h_it = m_h3d.begin(); h_it != m_h3d.end(); ++h_it){
    v_key.push_back(h_it->first);
  }
  return v_key;
}

//__________________________________________________________________
//
void HistManager::FinaliseTH1Bins(const string &s_hist){
  map<string, TH1D*>::iterator h1_it = m_h1d.find(s_hist);
  if( h1_it == m_h1d.end() ){
      cout<<"H1D "<<s_hist<<" not found "<<endl;
      return;
  }
    
  int nbins = h1_it->second->GetNbinsX();
  double v_uf       = h1_it->second->GetBinContent(0);
  double v_of       = h1_it->second->GetBinContent(nbins+1);
  double v_first    = h1_it->second->GetBinContent(1);
  double v_last     = h1_it->second->GetBinContent(nbins);

  double e_uf       = h1_it->second->GetBinError(0);
  double e_of       = h1_it->second->GetBinError(nbins+1);
  double e_first    = h1_it->second->GetBinError(1);
  double e_last     = h1_it->second->GetBinError(nbins);

  h1_it->second->SetBinContent(1, v_first + v_uf);
  h1_it->second->SetBinContent(nbins, v_last + v_of);
    
  h1_it->second->SetBinError(1, sqrt(e_first*e_first + e_uf*e_uf) );
  h1_it->second->SetBinError(nbins, sqrt(e_last*e_last + e_of*e_of) );
    
  //Since the over/underflows are set in the last/first bins, remove the under/over flow component
  h1_it->second->SetBinContent(0, 0.);
  h1_it->second->SetBinContent(nbins+1, 0.);
  h1_it->second->SetBinError(0, 0.);
  h1_it->second->SetBinError(nbins+1, 0.);
  return;
}

//__________________________________________________________________
//
void HistManager::ClearAllTH1(){
  for(map<string, TH1D*>::iterator h1_it = m_h1d.begin(); h1_it != m_h1d.end(); ++h1_it){
    delete h1_it->second;
    m_h1d.erase(h1_it);
  }
}

//__________________________________________________________________
//
void HistManager::ClearAllTH2(){

  for(map<string, TH2D*>::iterator h2_it = m_h2d.begin(); h2_it != m_h2d.end(); ++h2_it){
    delete h2_it->second;
    m_h2d.erase(h2_it);
  }
}

//__________________________________________________________________
//
void HistManager::ClearAllTH3(){
  for(map<string, TH3D*>::iterator h3_it = m_h3d.begin(); h3_it != m_h3d.end(); ++h3_it){
    delete h3_it->second;
    m_h3d.erase(h3_it);
  }
}

//__________________________________________________________________
//
void HistManager::ClearTH1(const string &s_hist){
  map<string, TH1D*>::iterator h1_it = m_h1d.find(s_hist);
  if( h1_it == m_h1d.end() ){
      cout<<"H1D "<<s_hist<<" not found "<<endl;
      return;
  }
  delete h1_it->second;
  m_h1d.erase(h1_it);
  return;

}

//__________________________________________________________________
//
void HistManager::ClearTH2(const string &s_hist){
  map<string, TH2D*>::iterator h2_it = m_h2d.find(s_hist);
  if( h2_it == m_h2d.end() ){
      cout<<"H2D "<<s_hist<<" not found "<<endl;
      return;
  }
  delete h2_it->second;
  m_h2d.erase(h2_it);
  return;

}

//__________________________________________________________________
//
void HistManager::ClearTH3(const string &s_hist){
  map<string, TH3D*>::iterator h3_it = m_h3d.find(s_hist);
  if( h3_it == m_h3d.end() ){
      cout<<"H3D "<<s_hist<<" not found "<<endl;
      return;
  }
  delete h3_it->second;
  m_h3d.erase(h3_it);
  return;

}

//__________________________________________________________________
//
void HistManager::BookTH1D( const string &key, const string &name, const string &title){
  TH1D* h1 = new TH1D();
  h1->SetName(name.c_str());
  h1->SetTitle(title.c_str());
  h1->SetDirectory(0);
  const string &in_key = (key=="") ? name : key;
  if(m_h1d.find(in_key) != m_h1d.end()){
    cout<<"Warning: Replacing existing H1D "<<name<<endl;
  }
  m_h1d[in_key]=h1;
    
  return;
}

//__________________________________________________________________
//
void HistManager::BookTH1D( const string &name, const string &title, double binsize, double xlow, double xup,
                            const string &key, const string &xtitle, const string &ytitle, int lw, int lc){
    
    double dnbins=(xup-xlow)/binsize +0.5;
    int nbins=(int)(dnbins);
    TH1D* h1=new TH1D(name.c_str(), title.c_str(), nbins,xlow,xup);
    if(xtitle!=""){h1->GetXaxis()->SetTitle(xtitle.c_str());}
    if(ytitle!=""){h1->GetYaxis()->SetTitle(ytitle.c_str());}
    h1->SetLineWidth(lw);
    h1->SetLineColor(lc);
    h1->Sumw2();
    h1->SetDirectory(0);
    
    const string &in_key = (key=="") ? name : key;
    if(m_h1d.find(in_key) != m_h1d.end()){
        cout<<"Warning: Replacing existing H1D "<<name<<endl;
    }
    m_h1d[in_key]=h1;
    return;
}

//__________________________________________________________________
//
void HistManager::BookTH1D( const string &name, const string &title, int nbins, double* xedges,
                            const string &key, const string &xtitle, const string &ytitle, int lw, int lc){
    
    TH1D* h1=new TH1D(name.c_str(), title.c_str(), nbins,xedges);
    if(xtitle!="")h1->GetXaxis()->SetTitle(xtitle.c_str());
    if(ytitle!="")h1->GetYaxis()->SetTitle(ytitle.c_str());
    h1->SetLineWidth(lw);
    h1->SetLineColor(lc);
    h1->Sumw2();
    h1->SetDirectory(0);
    
    const string &in_key = (key=="") ? name : key;
    if(m_h1d.find(in_key) != m_h1d.end()){
        cout<<"Warning: Replacing existing H1D "<<name<<endl;
    }
    m_h1d[in_key]=h1;
    return;
}

//__________________________________________________________________
//
void HistManager::BookTH2D( const string &key, const string &name, const string &title ){
  TH2D* h2 = new TH2D();
  h2->SetName(name.c_str());
  h2->SetTitle(title.c_str());
  h2->SetDirectory(0);
    
  const string &in_key = (key=="") ? name : key;
  if(m_h2d.find(in_key) != m_h2d.end()){
      cout<<"Warning: Replacing existing H2D "<<name<<endl;
  }
  m_h2d[in_key]=h2;
  return;
}

//__________________________________________________________________
//
void HistManager::BookTH2D( const string &name, const string &title, double xbinsize, double xlow, double xup,
                            double ybinsize, double ylow, double yup, const string &key, const string &xtitle,
                            const string &ytitle, int lw, int lc){
    
    double dnxbins=(xup-xlow)/xbinsize +0.5;
    double dnybins=(yup-ylow)/ybinsize +0.5;
    
    int nxbins=(int)(dnxbins);
    int nybins=(int)(dnybins);
    
    TH2D* h2=new TH2D(name.c_str(), title.c_str(), nxbins,xlow,xup, nybins,ylow,yup);
    if(xtitle!="")h2->GetXaxis()->SetTitle(xtitle.c_str());
    if(ytitle!="")h2->GetYaxis()->SetTitle(ytitle.c_str());
    h2->SetLineWidth(lw);
    h2->SetLineColor(lc);
    h2->Sumw2();
    h2->SetDirectory(0);
    
    const string &in_key = (key=="") ? name : key;
    if(m_h2d.find(in_key) != m_h2d.end()){
        cout<<"Warning: Replacing existing H2D "<<name<<endl;
    }
    m_h2d[in_key]=h2;
    return;
}

//__________________________________________________________________
//
void HistManager::BookTH2D(const string &name, const string &title,
                           int nxbins, double* xedges, int nybins, double* yedges,
                           const string &key, const string &xtitle, const string &ytitle,
                           int lw, int lc){
    
    TH2D* h2=new TH2D(name.c_str(), title.c_str(), nxbins,xedges, nybins,yedges);
    if(xtitle!="")h2->GetXaxis()->SetTitle(xtitle.c_str());
    if(ytitle!="")h2->GetYaxis()->SetTitle(ytitle.c_str());
    h2->SetLineWidth(lw);
    h2->SetLineColor(lc);
    h2->Sumw2();
    h2->SetDirectory(0);
    
    const string &in_key = (key=="") ? name : key;
    if(m_h2d.find(in_key) != m_h2d.end()){
        cout<<"Warning: Replacing existing H2D "<<name<<endl;
    }
    m_h2d[in_key]=h2;
    return;
}

//__________________________________________________________________
//
void HistManager::BookTH2D( const string &name, const string &title, double xbinsize, double xlow, double xup,
                            int nybins, double* yedges,
                            const string &key, const string &xtitle, const string &ytitle, int lw, int lc){
    
    double dnxbins=(xup-xlow)/xbinsize +0.5;
    int nxbins=(int)(dnxbins);
    
    TH2D* h2=new TH2D(name.c_str(), title.c_str(), nxbins,xlow,xup, nybins,yedges);
    if(xtitle!="")h2->GetXaxis()->SetTitle(xtitle.c_str());
    if(ytitle!="")h2->GetYaxis()->SetTitle(ytitle.c_str());
    h2->SetLineWidth(lw);
    h2->SetLineColor(lc);
    h2->Sumw2();
    h2->SetDirectory(0);
    
    const string &in_key = (key=="") ? name : key;
    if(m_h2d.find(in_key) != m_h2d.end()){
        cout<<"Warning: Replacing existing H2D "<<name<<endl;
    }
    m_h2d[in_key]=h2;
    return;
    
}

//__________________________________________________________________
//
void HistManager::BookTH3D(const string &key, const string &name, const string &title){
  TH3D* h3 = new TH3D();
  h3->SetName(name.c_str());
  h3->SetTitle(title.c_str());
  h3->SetDirectory(0);

  const string &in_key = (key=="") ? name : key;
  if(m_h3d.find(in_key) != m_h3d.end()){
      cout<<"Warning: Replacing existing H3D "<<name<<endl;
  }
  m_h3d[in_key]=h3;
  return;
}

//__________________________________________________________________
//
void HistManager::BookTH3D(const string &name, const string &title, double xbinsize, double xlow, double xup,
                           double ybinsize, double ylow, double yup, int nzbins, double* zedges,
                           const string &key, const string &xtitle, const string &ytitle, const string &ztitle, int lw, int lc){
    
  double dnxbins=(xup-xlow)/xbinsize +0.5;
  double dnybins=(yup-ylow)/ybinsize +0.5;

  int nxbins=(int)(dnxbins);
  int nybins=(int)(dnybins);

  double xe[nxbins+1]; double ye[nybins+1];
  for(int i=0; i<=nxbins; i++){xe[i]=xlow+i*xbinsize;}// cout<<"i="<<"xe="<<xe[i]<<endl;}
  for(int i=0; i<=nybins; i++){ye[i]=ylow+i*ybinsize;}// cout<<"i="<<"ye="<<ye[i]<<endl;}
  //cout<<endl<<endl;
  double* xedges=&xe[0]; double* yedges=&ye[0];
  TH3D* h3=new TH3D(name.c_str(), title.c_str(), nxbins,xedges, nybins,yedges, nzbins,zedges);
  if(xtitle!="")h3->GetXaxis()->SetTitle(xtitle.c_str());
  if(ytitle!="")h3->GetYaxis()->SetTitle(ytitle.c_str());
  if(ztitle!="")h3->GetZaxis()->SetTitle(ztitle.c_str());
  h3->SetLineWidth(lw);
  h3->SetLineColor(lc);
  h3->Sumw2();
  h3->SetDirectory(0);
    
  const string &in_key = (key=="") ? name : key;
  if(m_h3d.find(in_key) != m_h3d.end()){
      cout<<"Warning: Replacing existing H3D "<<name<<endl;
  }
  m_h3d[in_key]=h3;
  return;
}

//__________________________________________________________________
//
void HistManager::BookTH3D( const string &name, const string &title, double xbinsize, double xlow, double xup,
                            double ybinsize, double ylow, double yup,
                            double zbinsize, double zlow, double zup,
                            const string &key, const string &xtitle, const string &ytitle, const string &ztitle, int lw, int lc){

  double dnxbins=(xup-xlow)/xbinsize +0.5;
  double dnybins=(yup-ylow)/ybinsize +0.5;
  double dnzbins=(zup-zlow)/zbinsize +0.5;

  int nxbins=(int)(dnxbins);
  int nybins=(int)(dnybins);
  int nzbins=(int)(dnzbins);

  TH3D* h3=new TH3D(name.c_str(), title.c_str(), nxbins,xlow,xup, nybins,ylow,yup, nzbins,zlow,zup);
  if(xtitle!="")h3->GetXaxis()->SetTitle(xtitle.c_str());
  if(ytitle!="")h3->GetYaxis()->SetTitle(ytitle.c_str());
  if(ztitle!="")h3->GetZaxis()->SetTitle(ztitle.c_str());
  h3->SetLineWidth(lw);
  h3->SetLineColor(lc);
  h3->Sumw2();
  h3->SetDirectory(0);
    
  const string &in_key = (key=="") ? name : key;
  if(m_h3d.find(in_key) != m_h3d.end()){
      cout<<"Warning: Replacing existing H3D "<<name<<endl;
  }
  m_h3d[in_key]=h3;
  return;
}

//__________________________________________________________________
//
void HistManager::BookTH3D(const string &name, const string &title, double xbinsize, double xlow, double xup,
                           int nybins, double* yedges, int nzbins, double* zedges,
                           const string &key, const string &xtitle, const string &ytitle, const string &ztitle, int lw, int lc){

  double dnxbins=(xup-xlow)/xbinsize +0.5;
  int nxbins=(int)(dnxbins);

  double xe[nxbins+1]; 
  for(int i=0; i<=nxbins; i++){xe[i]=xlow+i*xbinsize;}
  double* xedges=&xe[0];
  TH3D* h3=new TH3D(name.c_str(), title.c_str(), nxbins,xedges, nybins,yedges, nzbins,zedges);
  if(xtitle!="")h3->GetXaxis()->SetTitle(xtitle.c_str());
  if(ytitle!="")h3->GetYaxis()->SetTitle(ytitle.c_str());
  if(ztitle!="")h3->GetZaxis()->SetTitle(ztitle.c_str());
  h3->SetLineWidth(lw); h3->SetLineColor(lc);
  h3->Sumw2();
  h3->SetDirectory(0);
    
  const string &in_key = (key=="") ? name : key;
  if(m_h3d.find(in_key) != m_h3d.end()){
      cout<<"Warning: Replacing existing H3D "<<name<<endl;
  }
  m_h3d[in_key]=h3;
  return;
}

//__________________________________________________________________
//
void HistManager::FillTH1D( const string &hkey, double val, double wgt){
  map<string, TH1D*>::iterator h1_it = m_h1d.find(hkey);
  if(h1_it == m_h1d.end()){
      cout<<"Error: TH1D "<<hkey<<" not found to fill"<<endl;
  } else if(h1_it->second == NULL){
      cout<<"Error: TH1D "<<hkey<<" not found to fill"<<endl;
  } else {
      h1_it->second -> Fill(val, wgt);
  }
  return;
}

//__________________________________________________________________
//
void HistManager::FillTH2D( const string &hkey, double val1, double val2, double wgt){
  map<string, TH2D*>::iterator h2_it = m_h2d.find(hkey);
  if( h2_it == m_h2d.end() ){
      cout<<"Error: TH2D "<<hkey<<" not found to fill"<<endl;
  } else if( h2_it->second == NULL){
      cout<<"Error: TH2D "<<hkey<<" not found to fill"<<endl;
  } else {
      h2_it -> second -> Fill(val1, val2, wgt);
  }
  return;
}

//__________________________________________________________________
//
void HistManager::FillTH3D( const string &hkey, double val1, double val2, double val3, double wgt){
  map<string, TH3D*>::iterator h3_it = m_h3d.find(hkey);
  if( h3_it == m_h3d.end() ){
      cout<<"Error: TH3D "<<hkey<<" not found to fill"<<endl;
  } else if( h3_it->second == NULL){
      cout<<"Error: TH3D "<<hkey<<" not found to fill"<<endl;
  } else {
      h3_it -> second -> Fill(val1, val2, val3, wgt);
  }
  return;
}

//__________________________________________________________________
//
TH1D* HistManager::GetTH1D( const string &hname){
    map<string, TH1D*>::const_iterator h1_it = m_h1d.find(hname);
    if(h1_it == m_h1d.end()){
        std::cout<<"Error: TH1D "<<hname<<" not found"<<std::endl;
        return 0;
    }
    return h1_it->second;
}

//__________________________________________________________________
//
TH2D* HistManager::GetTH2D( const string &hname){
  map<string, TH2D*>::const_iterator h2_it = m_h2d.find(hname);
  if(h2_it == m_h2d.end()){
      std::cout<<"Error: TH2D "<<hname<<" not found"<<std::endl;
      return 0;
  }
  return h2_it->second;
}

//__________________________________________________________________
//
TH3D* HistManager::GetTH3D( const string &hname){
  map<string, TH3D*>::const_iterator h3_it = m_h3d.find(hname);
  if(h3_it == m_h3d.end()){
      std::cout<<"Error: TH3D "<<hname<<" not found"<<std::endl;
      return 0;
  }
  return h3_it->second;
}

//__________________________________________________________________
//
void HistManager::SetTH1D( const string &hkey, TH1D* h1d){
  if(m_h1d.find(hkey) != m_h1d.end()){cout<<"Error: TH1D "<<hkey<<" already exists"<<endl;}
  else{m_h1d[hkey] = h1d;}
  return;
}

//__________________________________________________________________
//
void HistManager::SetTH2D(const string &hkey, TH2D* h2d){
  if(m_h2d.find(hkey) != m_h2d.end()){cout<<"Error: TH2D "<<hkey<<" already exists"<<endl;}
  else{m_h2d[hkey] = h2d;}
  return;
}

//__________________________________________________________________
//
void HistManager::SetTH3D(const string &hkey, TH3D* h3d){
  if(m_h3d.find(hkey) != m_h3d.end()){cout<<"Error: TH3D "<<hkey<<" already exists"<<endl;}
  else{m_h3d[hkey] = h3d;}
  return;
}

//__________________________________________________________________
//
void HistManager::ReplaceTH1D(const string &hkey, TH1D* h1d){

  if(m_h1d.find(hkey) != m_h1d.end()){ delete m_h1d[hkey]; }
  m_h1d[hkey] = h1d;

  return;
}

//__________________________________________________________________
//
void HistManager::ReplaceTH2D(const string &hkey, TH2D* h2d){

  if(m_h2d.find(hkey) != m_h2d.end()){ delete m_h2d[hkey]; }
  m_h2d[hkey] = h2d;

  return;
}

//__________________________________________________________________
//
void HistManager::ReplaceTH3D(const string &hkey, TH3D* h3d){

  if(m_h3d.find(hkey) != m_h3d.end()){ delete m_h3d[hkey]; }
  m_h3d[hkey] = h3d;

  return;
}

//__________________________________________________________________
//
int HistManager::ReadTH1D( const string &name, TFile* f, const string &key){

  string s;
  if(key!=""){s=key;}
  else{ s=name; }

  if(m_h1d.find(s)!= m_h1d.end()){
    cout<<"TH1D "<<s.c_str()<<" already exists"<<endl;
    return 1;
  } //if hist already exists, do nothing
  m_h1d[s]=(TH1D*)(f->Get(name.c_str()));
  if(m_h1d[s]==0){cout<<name.c_str()<<" not found in file"<<endl; return -1;}
  //h1d[s]->Sumw2();
  m_h1d[s]->SetDirectory(0);
  m_h1d[s]->SetName(s.c_str());
  if(m_h1d[s]->GetSumw2N()==0){m_h1d[s]->Sumw2();}
  
  return 0;

}

//__________________________________________________________________
//
int HistManager::ReadTH2D(const string &name, TFile* f, const string &key){

  string s;
  if(key!=""){s=key;}
  else{ s=name; }

  if(m_h2d.find(s)!= m_h2d.end()){
    cout<<"TH2D "<<s.c_str()<<" already exists"<<endl;
    return 1;
  } //if hist already exists, do nothing
  m_h2d[s]=(TH2D*)(f->Get(name.c_str()));
  if(m_h2d[s]==0){cout<<name.c_str()<<" not found in file"<<endl; return -1;}
  //h2d[s]->Sumw2();
  m_h2d[s]->SetDirectory(0);
  m_h2d[s]->SetName(s.c_str());
  if(m_h2d[s]->GetSumw2N()==0){m_h2d[s]->Sumw2();}
  
  return 0;

}

//__________________________________________________________________
//
int HistManager::ReadTH3D(const string &name, TFile* f, const string &key){

  string s;
  if(key!=""){s=key;}
  else{ s=name; }

  if(m_h3d.find(s)!= m_h3d.end()){
    cout<<"TH3D "<<s.c_str()<<" already exists"<<endl;
    return 1;
  } //if hist already exists, do nothing
  m_h3d[s]=(TH3D*)(f->Get(name.c_str()));
  if(m_h3d[s]==0){cout<<name.c_str()<<" not found in file"<<endl; return -1;}
  //h3d[s]->Sumw2();
  m_h3d[s]->SetDirectory(0);
  m_h3d[s]->SetName(s.c_str());
  if(m_h3d[s]->GetSumw2N()==0){m_h3d[s]->Sumw2();}
  
  return 0;
}


