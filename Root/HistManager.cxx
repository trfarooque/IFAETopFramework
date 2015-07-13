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
void HistManager::FinaliseTH1Bins(string s_hist){
  if(m_h1d.find(s_hist) == m_h1d.end() ){cout<<"H1D "<<s_hist<<" not found "<<endl; return;}
  int nbins = m_h1d[s_hist]->GetNbinsX();
  double v_uf = m_h1d[s_hist]->GetBinContent(0);
  double v_of = m_h1d[s_hist]->GetBinContent(nbins+1);

  double v_first = m_h1d[s_hist]->GetBinContent(1);
  double v_last = m_h1d[s_hist]->GetBinContent(nbins);

  double e_uf = m_h1d[s_hist]->GetBinError(0);
  double e_of = m_h1d[s_hist]->GetBinError(nbins+1);
  double e_first = m_h1d[s_hist]->GetBinError(1);
  double e_last = m_h1d[s_hist]->GetBinError(nbins);

  m_h1d[s_hist]->SetBinContent(1, v_first + v_uf);
  m_h1d[s_hist]->SetBinContent(nbins, v_last + v_of);

  m_h1d[s_hist]->SetBinError(1, sqrt(e_first*e_first + e_uf*e_uf) );
  m_h1d[s_hist]->SetBinError(nbins, sqrt(e_last*e_last + e_of*e_of) );
    
  //Since the over/underflows are set in the last/first bins, remove the under/over flow component
  m_h1d[s_hist]->SetBinContent(0, 0.);
  m_h1d[s_hist]->SetBinContent(nbins+1, 0.);
  m_h1d[s_hist]->SetBinError(0, 0. );
  m_h1d[s_hist]->SetBinError(nbins+1, 0. );

  return;
}

//__________________________________________________________________
//
void HistManager::ClearTH1(string s_hist){

  if(m_h1d.find(s_hist) == m_h1d.end() ){cout<<"H1D "<<s_hist<<" not found "<<endl; return;}

  delete m_h1d[s_hist];
  m_h1d.erase(m_h1d.find(s_hist));
  return;

}

//__________________________________________________________________
//
void HistManager::ClearTH2(string s_hist){

  if(m_h2d.find(s_hist) == m_h2d.end() ){cout<<"H2D "<<s_hist<<" not found "<<endl;}

  delete m_h2d[s_hist];
  m_h2d.erase(m_h2d.find(s_hist));
  return;

}

//__________________________________________________________________
//
void HistManager::ClearTH3(string s_hist){

  if(m_h3d.find(s_hist) == m_h3d.end() ){cout<<"H3D "<<s_hist<<" not found "<<endl;}

  delete m_h3d[s_hist];
  m_h3d.erase(m_h3d.find(s_hist));
  return;

}

//__________________________________________________________________
//
void HistManager::BookTH1D(string key, string name, string title){
  TH1D* h1 = new TH1D();
  h1->SetName(name.c_str());
  h1->SetTitle(title.c_str());

  if(key == ""){key = name;}
  if(m_h1d.find(key) != m_h1d.end()){cout<<"Warning: Replacing existing H1D "<<name<<endl;}
  m_h1d[key]=h1;

  return;

}

//__________________________________________________________________
//
void HistManager::BookTH2D(string key, string name, string title){
  TH2D* h2 = new TH2D();
  h2->SetName(name.c_str());
  h2->SetTitle(title.c_str());

  if(key == ""){key = name;}
  if(m_h2d.find(key) != m_h2d.end()){cout<<"Warning: Replacing existing H2D "<<name<<endl;}
  m_h2d[key]=h2;

  return;

}

//__________________________________________________________________
//
void HistManager::BookTH3D(string key, string name, string title){
  TH3D* h3 = new TH3D();
  h3->SetName(name.c_str());
  h3->SetTitle(title.c_str());

  if(key == ""){key = name;}
  if(m_h3d.find(key) != m_h3d.end()){cout<<"Warning: Replacing existing H3D "<<name<<endl;}
  m_h3d[key]=h3;

  return;

}

//__________________________________________________________________
//
void HistManager::BookTH1D(string name, string title, double binsize, double xlow, double xup, 
			   string key, string xtitle, string ytitle, int lw, int lc){

  double dnbins=(xup-xlow)/binsize +0.5;
  int nbins=(int)(dnbins);
  TH1D* h1=new TH1D(name.c_str(), title.c_str(), nbins,xlow,xup);
  h1->GetXaxis()->SetTitle(xtitle.c_str());
  if(ytitle!=""){h1->GetYaxis()->SetTitle(ytitle.c_str());}
  //else{h1->GetYaxis()->SetTitle("Events/");}
  h1->SetLineWidth(lw);
  h1->SetLineColor(lc);
  h1->Sumw2();

  if(key == ""){key = name;}
  if(m_h1d.find(key) != m_h1d.end()){cout<<"Warning: Replacing existing H1D "<<name<<endl;}
  m_h1d[key]=h1;
  return;
}

//__________________________________________________________________
//
void HistManager::BookTH1D(string name, string title, int nbins, double* xedges, 
	      string key, string xtitle, string ytitle, int lw, int lc){
  TH1D* h1=new TH1D(name.c_str(), title.c_str(), nbins,xedges);
  h1->GetXaxis()->SetTitle(xtitle.c_str());
  h1->GetYaxis()->SetTitle(ytitle.c_str());
  h1->SetLineWidth(lw);
  h1->SetLineColor(lc);
  h1->Sumw2();

  if(key == ""){key = name;}
  if(m_h1d.find(key) != m_h1d.end()){cout<<"Warning: Replacing existing H1D "<<name<<endl;}
  m_h1d[key]=h1;
  return;
}

//__________________________________________________________________
//
void HistManager::BookTH2D(string name, string title, double xbinsize, double xlow, double xup,
	      double ybinsize, double ylow, double yup, 
	      string key, string xtitle, string ytitle, int lw, int lc){

  double dnxbins=(xup-xlow)/xbinsize +0.5;
  double dnybins=(yup-ylow)/ybinsize +0.5;

  int nxbins=(int)(dnxbins); 
  int nybins=(int)(dnybins);

  TH2D* h2=new TH2D(name.c_str(), title.c_str(), nxbins,xlow,xup, nybins,ylow,yup);
  h2->GetXaxis()->SetTitle(xtitle.c_str());
  h2->GetYaxis()->SetTitle(ytitle.c_str());
  h2->SetLineWidth(lw);
  h2->SetLineColor(lc);
  h2->Sumw2();

  if(key == ""){key = name;}
  if(m_h2d.find(key) != m_h2d.end()){cout<<"Warning: Replacing existing H2D "<<name<<endl;}
  m_h2d[key]=h2;
  return;
}

//__________________________________________________________________
//
void HistManager::BookTH2D(string name, string title, 
	      int nxbins, double* xedges, int nybins, double* yedges, 
	      string key, string xtitle, string ytitle, int lw, int lc){

  TH2D* h2=new TH2D(name.c_str(), title.c_str(), nxbins,xedges, nybins,yedges);
  h2->GetXaxis()->SetTitle(xtitle.c_str());
  h2->GetYaxis()->SetTitle(ytitle.c_str());
  h2->SetLineWidth(lw);
  h2->SetLineColor(lc);
  h2->Sumw2();

  if(key == ""){key = name;}
  if(m_h2d.find(key) != m_h2d.end()){cout<<"Warning: Replacing existing H2D "<<name<<endl;}
  m_h2d[key]=h2;
  return;
}

//__________________________________________________________________
//
void HistManager::BookTH2D(string name, string title, double xbinsize, double xlow, double xup,
	      int nybins, double* yedges, 
	      string key, string xtitle, string ytitle, int lw, int lc){

  double dnxbins=(xup-xlow)/xbinsize +0.5;
  int nxbins=(int)(dnxbins);

  TH2D* h2=new TH2D(name.c_str(), title.c_str(), nxbins,xlow,xup, nybins,yedges);
  h2->GetXaxis()->SetTitle(xtitle.c_str());
  h2->GetYaxis()->SetTitle(ytitle.c_str());
  h2->SetLineWidth(lw);
  h2->SetLineColor(lc);
  h2->Sumw2();

  if(key == ""){key = name;}
  if(m_h2d.find(key) != m_h2d.end()){cout<<"Warning: Replacing existing H2D "<<name<<endl;}
  m_h2d[key]=h2;
  return;

}

//__________________________________________________________________
//
void HistManager::BookTH3D(string name, string title, double xbinsize, double xlow, double xup,
                           double ybinsize, double ylow, double yup, int nzbins, double* zedges,
                           string key, string xtitle, string ytitle, string ztitle, int lw, int lc){
    
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
  h3->GetXaxis()->SetTitle(xtitle.c_str());
  h3->GetYaxis()->SetTitle(ytitle.c_str());
  h3->GetZaxis()->SetTitle(ztitle.c_str());
  h3->SetLineWidth(lw);
  h3->SetLineColor(lc);
  h3->Sumw2();

  if(key == ""){key = name;}
  if(m_h3d.find(key) != m_h3d.end()){cout<<"Warning: Replacing existing H3D "<<name<<endl;}
  m_h3d[key]=h3;
  return;
}

//__________________________________________________________________
//
void HistManager::BookTH3D(string name, string title, double xbinsize, double xlow, double xup,
                           double ybinsize, double ylow, double yup,
                           double zbinsize, double zlow, double zup,
                           string key, string xtitle, string ytitle, string ztitle, int lw, int lc){

  double dnxbins=(xup-xlow)/xbinsize +0.5;
  double dnybins=(yup-ylow)/ybinsize +0.5;
  double dnzbins=(zup-zlow)/zbinsize +0.5;

  int nxbins=(int)(dnxbins);
  int nybins=(int)(dnybins);
  int nzbins=(int)(dnzbins);

  TH3D* h3=new TH3D(name.c_str(), title.c_str(), nxbins,xlow,xup, nybins,ylow,yup, nzbins,zlow,zup);
  h3->GetXaxis()->SetTitle(xtitle.c_str());
  h3->GetYaxis()->SetTitle(ytitle.c_str());
  h3->GetZaxis()->SetTitle(ztitle.c_str());
  h3->SetLineWidth(lw);
  h3->SetLineColor(lc);
  h3->Sumw2();

  if(key == ""){key = name;}
  if(m_h3d.find(key) != m_h3d.end()){cout<<"Warning: Replacing existing H3D "<<name<<endl;}
  m_h3d[key]=h3;
  return;
}

//__________________________________________________________________
//
void HistManager::BookTH3D(string name, string title, double xbinsize, double xlow, double xup,
                           int nybins, double* yedges, int nzbins, double* zedges,
                           string key, string xtitle, string ytitle, string ztitle, int lw, int lc){

  double dnxbins=(xup-xlow)/xbinsize +0.5;
  int nxbins=(int)(dnxbins);

  double xe[nxbins+1]; 
  for(int i=0; i<=nxbins; i++){xe[i]=xlow+i*xbinsize;}
  double* xedges=&xe[0];
  TH3D* h3=new TH3D(name.c_str(), title.c_str(), nxbins,xedges, nybins,yedges, nzbins,zedges);
  h3->GetXaxis()->SetTitle(xtitle.c_str());
  h3->GetYaxis()->SetTitle(ytitle.c_str());
  h3->GetZaxis()->SetTitle(ztitle.c_str());
  h3->SetLineWidth(lw); h3->SetLineColor(lc);
  h3->Sumw2();

  if(key == ""){key = name;}
  if(m_h3d.find(key) != m_h3d.end()){cout<<"Warning: Replacing existing H3D "<<name<<endl;}
  m_h3d[key]=h3;
  return;
}

//__________________________________________________________________
//
void HistManager::FillTH1D(string hkey, double val, double wgt){

  if(m_h1d.find(hkey) == m_h1d.end()){cout<<"Error: TH1D "<<hkey<<" not found to fill"<<endl;}
  else if(m_h1d[hkey] == NULL){cout<<"Error: TH1D "<<hkey<<" not found to fill"<<endl;}
  else {m_h1d[hkey]->Fill(val, wgt);}

  return;
}

//__________________________________________________________________
//
void HistManager::FillTH2D(string hkey, double val1, double val2, double wgt){

  if(m_h2d.find(hkey) == m_h2d.end()){cout<<"Error: TH2D "<<hkey<<" not found to fill"<<endl;}
  else if(m_h2d[hkey] == NULL){cout<<"Error: TH2D "<<hkey<<" not found to fill"<<endl;}
  else {m_h2d[hkey]->Fill(val1, val2, wgt);}

  return;
}

//__________________________________________________________________
//
void HistManager::FillTH3D(string hkey, double val1, double val2, double val3, double wgt){

  if(m_h3d.find(hkey) == m_h3d.end()){cout<<"Error: TH3D "<<hkey<<" not found to fill"<<endl;}
  else if(m_h3d[hkey] == NULL){cout<<"Error: TH3D "<<hkey<<" not found to fill"<<endl;}
  else {m_h3d[hkey]->Fill(val1, val2, val3, wgt);}

  return;

}

TH1D* HistManager::GetTH1D(string hname){
  if(m_h1d.find(hname) == m_h1d.end()){std::cout<<"Error: TH1D "<<hname<<" not found"<<std::endl; return NULL;} 
  return m_h1d[hname];
}

TH2D* HistManager::GetTH2D(string hname){
  if(m_h2d.find(hname) == m_h2d.end()){std::cout<<"Error: TH2D "<<hname<<" not found"<<std::endl; return NULL;} 
  return m_h2d[hname];
}

TH3D* HistManager::GetTH3D(string hname){
  if(m_h3d.find(hname) == m_h3d.end()){std::cout<<"Error: TH3D "<<hname<<" not found"<<std::endl; return NULL;} 
  return m_h3d[hname];
}
//__________________________________________________________________
//
void HistManager::SetTH1D(string hkey, TH1D* h1d){

  if(m_h1d.find(hkey) != m_h1d.end()){cout<<"Error: TH1D "<<hkey<<" already exists"<<endl;}
  else{m_h1d[hkey] = h1d;}

  return;
}

//__________________________________________________________________
//
void HistManager::SetTH2D(string hkey, TH2D* h2d){

  if(m_h2d.find(hkey) != m_h2d.end()){cout<<"Error: TH2D "<<hkey<<" already exists"<<endl;}
  else{m_h2d[hkey] = h2d;}
  return;
}

//__________________________________________________________________
//
void HistManager::SetTH3D(string hkey, TH3D* h3d){

  if(m_h3d.find(hkey) != m_h3d.end()){cout<<"Error: TH3D "<<hkey<<" already exists"<<endl;}
  else{m_h3d[hkey] = h3d;}

  return;
}

//__________________________________________________________________
//
void HistManager::ReplaceTH1D(string hkey, TH1D* h1d){

  if(m_h1d.find(hkey) != m_h1d.end()){ delete m_h1d[hkey]; }
  m_h1d[hkey] = h1d;

  return;
}

//__________________________________________________________________
//
void HistManager::ReplaceTH2D(string hkey, TH2D* h2d){

  if(m_h2d.find(hkey) != m_h2d.end()){ delete m_h2d[hkey]; }
  m_h2d[hkey] = h2d;

  return;
}

//__________________________________________________________________
//
void HistManager::ReplaceTH3D(string hkey, TH3D* h3d){

  if(m_h3d.find(hkey) != m_h3d.end()){ delete m_h3d[hkey]; }
  m_h3d[hkey] = h3d;

  return;
}

//__________________________________________________________________
//
int HistManager::ReadTH1D(string name, TFile* f, string key){

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
int HistManager::ReadTH2D(string name, TFile* f, string key){

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
int HistManager::ReadTH3D(string name, TFile* f, string key){

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


