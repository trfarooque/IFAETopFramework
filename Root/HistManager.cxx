#include "IFAETopFramework/HistManager.h"

using namespace std;

const int HistManager::FCHECK  = 0x1 << 0;
const int HistManager::UFLOW  = 0x1 << 1;
const int HistManager::OFLOW  = 0x1 << 2;

//__________________________________________________________________
//
HistManager::HistManager( bool addUF, bool addOF) : 
  m_addUF(addUF),
  m_addOF(addOF)
{
  m_h1d.clear();
  m_h2d.clear();
  m_h3d.clear();
  m_profile.clear();
}

//__________________________________________________________________
//
HistManager::HistManager( const HistManager &q, bool make_new){
    m_h1d.clear();
    m_profile.clear();
    m_h2d.clear();
    m_h3d.clear();

    m_addUF = q.m_addUF;
    m_addOF = q.m_addOF;

    if(make_new){
        //Loop over old HistManager, clone each histogram, and add it to the new HistManager
        map<string, TH1D*>::const_iterator h1_it = (q.m_h1d).begin();
        for(; h1_it != q.m_h1d.end(); ++h1_it){ CloneTH1D(h1_it->first, h1_it->second); }
        
        map<string, TH2D*>::const_iterator h2_it = (q.m_h2d).begin();
        for(; h2_it != q.m_h2d.end(); ++h2_it){ CloneTH2D(h2_it->first, h2_it->second); }
        
        map<string, TH3D*>::const_iterator h3_it = (q.m_h3d).begin();
        for(; h3_it != q.m_h3d.end(); ++h3_it){ CloneTH3D(h3_it->first, h3_it->second); }
        
        map<string, TProfile*>::const_iterator prof_it = (q.m_profile).begin();
        for(; prof_it != q.m_profile.end(); ++prof_it){ CloneTProfile(prof_it->first, prof_it->second); }
    }
    else{
        m_h1d = q.m_h1d;
        m_h2d = q.m_h2d;
        m_h3d = q.m_h3d;
        m_profile = q.m_profile;
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
vector<string> HistManager::GetTProfileKeyList(){
    vector<string> v_key; v_key.clear();
    for(map<string, TProfile*>::iterator h_it = m_profile.begin(); h_it != m_profile.end(); ++h_it){
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
    int hopt = m_h1d_opt[s_hist];
    bool addUF = (hopt & FCHECK) ? (hopt & UFLOW) : m_addUF;
    bool addOF = (hopt & FCHECK) ? (hopt & OFLOW) : m_addOF;

    int nbins = h1_it->second->GetNbinsX();
    if(addOF){
      double v_of       = h1_it->second->GetBinContent(nbins+1);
      double v_last     = h1_it->second->GetBinContent(nbins);
      double e_of       = h1_it->second->GetBinError(nbins+1);
      double e_last     = h1_it->second->GetBinError(nbins);
      h1_it->second->SetBinContent(nbins, v_last + v_of);
      h1_it->second->SetBinError(nbins, sqrt(e_last*e_last + e_of*e_of) );
    }
    if(addUF){
      double v_uf       = h1_it->second->GetBinContent(0);
      double v_first    = h1_it->second->GetBinContent(1);
      double e_uf       = h1_it->second->GetBinError(0);
      double e_first    = h1_it->second->GetBinError(1);
      h1_it->second->SetBinContent(1, v_first + v_uf);
      h1_it->second->SetBinError(1, sqrt(e_first*e_first + e_uf*e_uf) );
    }
    //Always set the underflow/overflow to zero, whether or not it is included in the first/last bin
    h1_it->second->SetBinContent(0, 0.);
    h1_it->second->SetBinError(0, 0.);
    h1_it->second->SetBinContent(nbins+1, 0.);
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
void HistManager::ClearAllTProfile(){
    for(map<string, TProfile*>::iterator prof_it = m_profile.begin(); prof_it != m_profile.end(); ++prof_it){
        delete prof_it->second;
        m_profile.erase(prof_it);
    }
}

//__________________________________________________________________
//
void HistManager::ClearTH1(const string &s_hist){
    map<string, TH1D*>::iterator h1_it = m_h1d.find(s_hist);
    if( h1_it == m_h1d.end() ){
        cout<<"HistManager::ClearTH1  Warning: H1D "<<s_hist<<" not found "<<endl;
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
        cout<<"HistManager::ClearTH2  Warning: H2D "<<s_hist<<" not found "<<endl;
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
        cout<<"HistManager::ClearTH3  Warning: H3D "<<s_hist<<" not found "<<endl;
        return;
    }
    delete h3_it->second;
    m_h3d.erase(h3_it);
    return;
}

//__________________________________________________________________
//
void HistManager::ClearTProfile(const string &s_hist){
    map<string, TProfile*>::iterator prof_it = m_profile.find(s_hist);
    if( prof_it == m_profile.end() ){
        cout<<"HistManager::ClearTProfile  Warning: TProfile: "<<s_hist<<" not found "<<endl;
        return;
    }
    delete prof_it->second;
    m_profile.erase(prof_it);
    return;
}

void HistManager::InitTH1(const string &name, TH1D* h1d, const string &title, const string &key, const string& xtitle, const string& ytitle, int opt, int lw, int lc){

  h1d->SetDirectory(0);
  h1d->SetName(name.c_str());
  h1d->SetTitle(title.c_str());

  if(xtitle!=""){h1d->GetXaxis()->SetTitle(xtitle.c_str());}
  if(ytitle!=""){h1d->GetYaxis()->SetTitle(ytitle.c_str());}
  h1d->SetLineWidth(lw);
  h1d->SetLineColor(lc);
  h1d->Sumw2();
    
  const string &in_key = (key=="") ? name : key;
  if(m_h1d.find(in_key) != m_h1d.end()){
    cout<<"HistManager::InitTH1  Warning: Replacing existing H1D "<<name<<endl;
  }
  m_h1d[in_key]=h1d;
  m_h1d_opt[in_key]=opt;

  return;
}

//__________________________________________________________________
//
TH1D* HistManager::BookTH1D( const string &key, const string &name, const string &title){
    TH1D* h1 = new TH1D();
    InitTH1(name, h1, title, key);
    
    return h1;
}

//__________________________________________________________________
//
TH1D* HistManager::BookTH1D( const string &name, const string &title, double binsize, double xlow, double xup,
			     const string &key, const string &xtitle, const string &ytitle, int opt, int lw, int lc){
    
    double dnbins=(xup-xlow)/binsize +0.5;
    int nbins=(int)(dnbins);
    TH1D* h1=new TH1D(name.c_str(), title.c_str(), nbins,xlow,xup);
    InitTH1(name, h1, title, key, xtitle, ytitle, opt, lw, lc);
    return h1;
}

//__________________________________________________________________
//
TH1D* HistManager::BookTH1D( const string &name, const string &title, int nbins, const double* xedges,
			     const string &key, const string &xtitle, const string &ytitle, int opt, int lw, int lc){
    
    TH1D* h1=new TH1D(name.c_str(), title.c_str(), nbins,xedges);
    InitTH1(name, h1, title, key, xtitle, ytitle, opt, lw, lc);
    return h1;
}



//__________________________________________________________________________________________________________________
//
void HistManager::InitTH2(const string &name, TH2D* h2d, const string &title, const string &key, const string& xtitle, const string& ytitle, int lw, int lc){
  h2d->SetDirectory(0);
  h2d->SetName(name.c_str());
  h2d->SetTitle(title.c_str());
  if(xtitle!="")h2d->GetXaxis()->SetTitle(xtitle.c_str());
  if(ytitle!="")h2d->GetYaxis()->SetTitle(ytitle.c_str());
  h2d->SetLineWidth(lw);
  h2d->SetLineColor(lc);
  h2d->Sumw2();
  
  const string &in_key = (key=="") ? name : key;
  if(m_h2d.find(in_key) != m_h2d.end()){
    cout<<"HistManager::InitTH2  Warning: Replacing existing H2D "<<name<<endl;
  }
  m_h2d[in_key]=h2d;
  
}

//__________________________________________________________________
//
TH2D* HistManager::BookTH2D( const string &key, const string &name, const string &title ){
    TH2D* h2 = new TH2D();
    InitTH2(name, h2, title, key);
    return h2;
}

//__________________________________________________________________
//
TH2D* HistManager::BookTH2D( const string &name, const string &title, double xbinsize, double xlow, double xup,
                            double ybinsize, double ylow, double yup, const string &key, const string &xtitle,
                            const string &ytitle, int lw, int lc){
    
    double dnxbins=(xup-xlow)/xbinsize +0.5;
    double dnybins=(yup-ylow)/ybinsize +0.5;
    
    int nxbins=(int)(dnxbins);
    int nybins=(int)(dnybins);
    
    TH2D* h2=new TH2D(name.c_str(), title.c_str(), nxbins,xlow,xup, nybins,ylow,yup);
    InitTH2(name, h2, title, key, xtitle, ytitle, lw, lc);

    return h2;
}

//__________________________________________________________________
//
TH2D* HistManager::BookTH2D(const string &name, const string &title,
                            int nxbins, const double* xedges, int nybins, const double* yedges,
                            const string &key, const string &xtitle, const string &ytitle,
                            int lw, int lc){
    
    TH2D* h2=new TH2D(name.c_str(), title.c_str(), nxbins,xedges, nybins,yedges);
    InitTH2(name, h2, title, key, xtitle, ytitle, lw, lc);

    return h2;
}

//__________________________________________________________________
//
TH2D* HistManager::BookTH2D( const string &name, const string &title, double xbinsize, double xlow, double xup,
                            int nybins, const double* yedges,
                            const string &key, const string &xtitle, const string &ytitle, int lw, int lc){
    
    double dnxbins=(xup-xlow)/xbinsize +0.5;
    int nxbins=(int)(dnxbins);
    
    TH2D* h2=new TH2D(name.c_str(), title.c_str(), nxbins,xlow,xup, nybins,yedges);
    InitTH2(name, h2, title, key, xtitle, ytitle, lw, lc);

    return h2;
    
}

//__________________________________________________________________
//
TH2D* HistManager::BookTH2D( const string &name, const string &title, int nxbins, const double* xedges,
			     double ybinsize, double ylow, double yup,
			     const string &key, const string &xtitle, const string &ytitle, int lw, int lc){
    
    double dnybins=(yup-ylow)/ybinsize +0.5;
    int nybins=(int)(dnybins);
    
    TH2D* h2=new TH2D(name.c_str(), title.c_str(), nxbins,xedges, nybins,ylow,yup);
    InitTH2(name, h2, title, key, xtitle, ytitle, lw, lc);

    return h2;
    
}

//__________________________________________________________________________________________________________________
//
void HistManager::InitTH3(const string &name, TH3D* h3d, const string &title, const string &key, const string& xtitle, const string& ytitle, const string& ztitle, int lw, int lc){
  h3d->SetName(name.c_str());
  h3d->SetTitle(title.c_str());
  h3d->SetDirectory(0);
  if(xtitle!="")h3d->GetXaxis()->SetTitle(xtitle.c_str());
  if(ytitle!="")h3d->GetYaxis()->SetTitle(ytitle.c_str());
  if(ztitle!="")h3d->GetZaxis()->SetTitle(ztitle.c_str());
  h3d->SetLineWidth(lw);
  h3d->SetLineColor(lc);
  h3d->Sumw2();
  
  const string &in_key = (key=="") ? name : key;
  if(m_h3d.find(in_key) != m_h3d.end()){
    cout<<"HistManager::InitTH3  Warning: Replacing existing H3D "<<name<<endl;
  }
  m_h3d[in_key]=h3d;
  
}
//__________________________________________________________________
//
TH3D* HistManager::BookTH3D(const string &key, const string &name, const string &title){
    TH3D* h3 = new TH3D();
    InitTH3(name, h3, title, key);
    return h3;
}

//__________________________________________________________________
//
TH3D* HistManager::BookTH3D(const string &name, const string &title, double xbinsize, double xlow, double xup,
                            double ybinsize, double ylow, double yup, int nzbins, const double* zedges,
                            const string &key, const string &xtitle, const string &ytitle, const string &ztitle, int lw, int lc){
    
    double dnxbins=(xup-xlow)/xbinsize + 0.5;
    double dnybins=(yup-ylow)/ybinsize + 0.5;
    
    const int nxbins = (int)(dnxbins);
    const int nybins = (int)(dnybins);
    
    double *xe = new double[nxbins+1];
    double *ye = new double[nybins+1];
    
    for(int i=0; i<=nxbins; i++){xe[i]=xlow+i*xbinsize;}
    for(int i=0; i<=nybins; i++){ye[i]=ylow+i*ybinsize;}
    
    double* xedges=&xe[0]; double* yedges=&ye[0];
    TH3D* h3=new TH3D(name.c_str(), title.c_str(), nxbins,xedges, nybins,yedges, nzbins,zedges);
    InitTH3(name, h3, title, key, xtitle, ytitle, ztitle, lw, lc);
    
    delete[] xe;
    delete[] ye;
    return h3;
}

//__________________________________________________________________
//
TH3D* HistManager::BookTH3D( const string &name, const string &title, double xbinsize, double xlow, double xup,
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
    InitTH3(name, h3, title, key, xtitle, ytitle, ztitle, lw, lc);
    return h3;
}

//__________________________________________________________________
//
TH3D* HistManager::BookTH3D(const string &name, const string &title, double xbinsize, double xlow, double xup,
                            int nybins, const double* yedges, int nzbins, const double* zedges,
                            const string &key, const string &xtitle, const string &ytitle, const string &ztitle, int lw, int lc){
    
    double dnxbins=(xup-xlow)/xbinsize +0.5;
    int nxbins=(int)(dnxbins);
    
    double *xe = new double[nxbins+1];
    
    for(int i=0; i<=nxbins; i++){xe[i]=xlow+i*xbinsize;}
    double* xedges=&xe[0];
    TH3D* h3=new TH3D(name.c_str(), title.c_str(), nxbins,xedges, nybins,yedges, nzbins,zedges);
    InitTH3(name, h3, title, key, xtitle, ytitle, ztitle, lw, lc);
    
    delete[] xe;
    return h3;
}

//__________________________________________________________________
//
TH3D* HistManager::BookTH3D(const string &name, const string &title, int nxbins, const double* xedges,
                            int nybins, const double* yedges, int nzbins, const double* zedges,
                            const string &key, const string &xtitle, const string &ytitle, const string &ztitle, int lw, int lc){
    
    TH3D* h3=new TH3D(name.c_str(), title.c_str(), nxbins,xedges, nybins,yedges, nzbins,zedges);
    InitTH3(name, h3, title, key, xtitle, ytitle, ztitle, lw, lc);
    
    return h3;
}

//
//_________________________________________________________________________________
//
void HistManager::InitTProfile(const string &name, TProfile* prof, const string &title, const string &key, const string& xtitle, const string& ytitle, int lw, int lc){

    prof->SetDirectory(0);
    prof->SetName(name.c_str());
    prof->SetTitle(title.c_str());
    if(xtitle!=""){prof->GetXaxis()->SetTitle(xtitle.c_str());}
    if(ytitle!=""){prof->GetYaxis()->SetTitle(ytitle.c_str());}
    prof->SetLineWidth(lw);
    prof->SetLineColor(lc);
    prof->Sumw2();
    
    const string &in_key = (key=="") ? name : key;
    if(m_profile.find(in_key) != m_profile.end()){
        cout<<"HistManager::InitTProfile  Warning: Replacing existing TProfile: "<<name<<endl;
    }
    m_profile[in_key]=prof;

    return;
}


//__________________________________________________________________
//
TProfile* HistManager::BookTProfile( const string &name, const string &title, double binsize, double xlow, double xup,
                                    const string &key, const string &xtitle, const string &ytitle, int lw, int lc){
    
    double dnbins=(xup-xlow)/binsize +0.5;
    int nbins=(int)(dnbins);
    TProfile* prof = new TProfile(name.c_str(), title.c_str(), nbins,xlow,xup);
    InitTProfile(name, prof, title, key, xtitle, ytitle, lw, lc);

    return prof;
}

//__________________________________________________________________
//
TProfile* HistManager::BookTProfile( const string &name, const string &title, int nbins, const double* xedges,
                                     const string &key, const string &xtitle, const string &ytitle, int lw, int lc){
    
    TProfile* prof = new TProfile( name.c_str(), title.c_str(), nbins, xedges );
    InitTProfile(name, prof, title, key, xtitle, ytitle, lw, lc);
    return prof;
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
void HistManager::FillTH2D( const string &hkey, double val1, double val2, double wgt ){
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
void HistManager::FillTProfile( const string &hkey, double val1, double val2, double wgt ){
    map<string, TProfile*>::iterator prof_it = m_profile.find(hkey);
    if(prof_it == m_profile.end()){
        cout<<"Error: TProfile "<<hkey<<" not found to fill"<<endl;
    } else if(prof_it->second == NULL){
        cout<<"Error: TProfile "<<hkey<<" not found to fill"<<endl;
    } else {
        prof_it->second -> Fill(val1, val2, wgt);
    }
    return;
}

//__________________________________________________________________
//
TH1D* HistManager::GetTH1D( const string &hname){
    map<string, TH1D*>::const_iterator h1_it = m_h1d.find(hname);
    if(h1_it == m_h1d.end()){
        return 0;
    }
    return h1_it->second;
}

//__________________________________________________________________
//
TH2D* HistManager::GetTH2D( const string &hname){
    map<string, TH2D*>::const_iterator h2_it = m_h2d.find(hname);
    if(h2_it == m_h2d.end()){
        return 0;
    }
    return h2_it->second;
}

//__________________________________________________________________
//
TH3D* HistManager::GetTH3D( const string &hname){
    map<string, TH3D*>::const_iterator h3_it = m_h3d.find(hname);
    if(h3_it == m_h3d.end()){
        return 0;
    }
    return h3_it->second;
}

//__________________________________________________________________
//
TProfile* HistManager::GetTProfile( const string &hname){
    map<string, TProfile*>::const_iterator prof_it = m_profile.find(hname);
    if(prof_it == m_profile.end()){
        return 0;
    }
    return prof_it->second;
}

//__________________________________________________________________
//
TH1D* HistManager::CloneTH1D( const string &hkey, const string &origkey, bool reset, bool force_replace){
    if(m_h1d.find(origkey) == m_h1d.end()){
        std::cout<<"Error: TH1D "<<origkey<<" not found to clone"<<std::endl;
        return NULL;
    }
    TH1D* hret = CloneTH1D(hkey, m_h1d[origkey], reset, force_replace);
    return hret;
}

//__________________________________________________________________
//
TH2D* HistManager::CloneTH2D( const string &hkey, const string &origkey, bool reset, bool force_replace){
    if(m_h2d.find(origkey) == m_h2d.end()){
        std::cout<<"Error: TH2D "<<origkey<<" not found to clone"<<std::endl;
        return NULL;
    }
    TH2D* hret = CloneTH2D(hkey, m_h2d[origkey], reset, force_replace);
    return hret;
}

//__________________________________________________________________
//
TH3D* HistManager::CloneTH3D( const string &hkey, const string &origkey, bool reset, bool force_replace){
    if(m_h3d.find(origkey) == m_h3d.end()){
        std::cout<<"Error: TH3D "<<origkey<<" not found to clone"<<std::endl;
        return NULL;
    }
    TH3D* hret = CloneTH3D(hkey, m_h3d[origkey], reset, force_replace);
    return hret;
}

//__________________________________________________________________
//
TProfile* HistManager::CloneTProfile( const string &hkey, const string &origkey, bool reset, bool force_replace){
    if(m_profile.find(origkey) == m_profile.end()){
        std::cout<<"Error: TProfile "<<origkey<<" not found to clone"<<std::endl;
        return NULL;
    }
    TProfile* hret = CloneTProfile(hkey, m_profile[origkey], reset, force_replace);
    return hret;
}

//__________________________________________________________________
//
TH1D* HistManager::CloneTH1D( const string &hkey, TH1D* h1d, bool reset, bool force_replace){

  if(  m_h1d.find(hkey) != m_h1d.end() ){ 
    if(!force_replace){
      std::cout<<"HistManager::CloneTH1D  ERROR: TH1 "<<hkey<<" already exists. Set force_replace=true to force replacement"<<std::endl; 
      return NULL;
    }
    delete m_h1d[hkey];
  }
  TH1D* hclone = (TH1D*)(h1d->Clone(hkey.c_str()));
  hclone->SetDirectory(0);
  if(reset){hclone->Reset();}
  if(hclone->GetSumw2N()==0){hclone->Sumw2();}
    
  m_h1d[hkey] = hclone;
  return hclone;
}

//__________________________________________________________________
//
TH2D* HistManager::CloneTH2D( const string &hkey, TH2D* h2d, bool reset, bool force_replace){
    
  if(  m_h2d.find(hkey) != m_h2d.end() ){ 
    if(!force_replace){
      std::cout<<"HistManager::CloneTH2D  ERROR: TH2 "<<hkey<<" already exists. Set force_replace=true to force replacement"<<std::endl; 
      return NULL;
    }
    delete m_h2d[hkey];
  }
  TH2D* hclone = (TH2D*)(h2d->Clone(hkey.c_str()));
  hclone->SetDirectory(0);
  if(reset){hclone->Reset();}
  if(hclone->GetSumw2N()==0){hclone->Sumw2();}
  m_h2d[hkey] = hclone;
  return hclone;
}

//__________________________________________________________________
//
TH3D* HistManager::CloneTH3D( const string &hkey, TH3D* h3d, bool reset, bool force_replace){
    
  if(  m_h3d.find(hkey) != m_h3d.end() ){ 
    if(!force_replace){
      std::cout<<"HistManager::CloneTH3D  ERROR: TH3 "<<hkey<<" already exists. Set force_replace=true to force replacement"<<std::endl; 
      return NULL;
    }
    delete m_h3d[hkey];
  }

  TH3D* hclone = (TH3D*)(h3d->Clone(hkey.c_str()));
  hclone->SetDirectory(0);
  if(reset){hclone->Reset();}
  if(hclone->GetSumw2N()==0){hclone->Sumw2();}
  m_h3d[hkey] = hclone;
  return hclone;
}

//__________________________________________________________________
//
TProfile* HistManager::CloneTProfile( const string &hkey, TProfile* prof_ori, bool reset, bool force_replace){

  if(  m_profile.find(hkey) != m_profile.end() ){ 
    if(!force_replace){
      std::cout<<"HistManager::CloneTProfile  ERROR: TProfile "<<hkey<<" already exists. Set force_replace=true to force replacement"<<std::endl; 
      return NULL;
    }
    delete m_profile[hkey];
  }

    TProfile* hclone = (TProfile*)(prof_ori->Clone(hkey.c_str()));
    hclone->SetDirectory(0);
    if(reset){
        hclone->Reset();
    }
    if(hclone->GetSumw2N()==0){
        hclone->Sumw2();
    }
    m_profile[hkey] = hclone;
    return hclone;
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
void HistManager::SetTProfile( const string &hkey, TProfile* prof){
    if(m_profile.find(hkey) != m_profile.end()){
        cout<<"Error: TProfile "<<hkey<<" already exists"<<endl;
    } else {
        m_profile[hkey] = prof;
    }
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
void HistManager::ReplaceTProfile(const string &hkey, TProfile* prof){
    if(m_profile.find(hkey) != m_profile.end()){
        delete m_profile[hkey];
    }
    m_profile[hkey] = prof;
    return;
}

//__________________________________________________________________
//
TH1D* HistManager::ReadTH1D( const string &name, TFile* f, const string &key, bool force_replace){
    
    string s;
    if(key!=""){s=key;}
    else{ s=name; }
    
    if( !force_replace && (m_h1d.find(s)!= m_h1d.end()) ){
        cout<<"HistManager::ReadTH1D  Warning : TH1D "<<s.c_str()<<" already exists. Ignoring."<<endl;
        return NULL;
    } //if hist already exists, do nothing
    TH1D* forig = (TH1D*)(f->Get(name.c_str()));
    if(forig == 0){cout<<"HistManager::ReadTH1D  Warning : TH1D "<<name.c_str()<<" not found in file"<<endl; return NULL;}
    TH1D* hret = CloneTH1D(s, forig, force_replace);
    return hret;
}

//__________________________________________________________________
//
TH2D* HistManager::ReadTH2D(const string &name, TFile* f, const string &key, bool force_replace){
    
    string s;
    if(key!=""){s=key;}
    else{ s=name; }
    
    if( !force_replace && (m_h2d.find(s)!= m_h2d.end()) ){
        cout<<"HistManager::ReadTH2D  Warning : TH2D "<<s.c_str()<<" already exists. Ignoring."<<endl;
        return NULL;
    } //if hist already exists, do nothing
    
    TH2D* forig = (TH2D*)(f->Get(name.c_str()));
    if(forig == 0){cout<<"HistManager::ReadTH1D  Warning : TH2D "<<name.c_str()<<" not found in file"<<endl; return NULL;}
    TH2D* hret = CloneTH2D(s, forig, force_replace);
    
    return hret;
    
}

//__________________________________________________________________
//
TH3D* HistManager::ReadTH3D(const string &name, TFile* f, const string &key, bool force_replace){
    
    string s;
    if(key!=""){s=key;}
    else{ s=name; }
    
    if( !force_replace && (m_h3d.find(s)!= m_h3d.end()) ){
        cout<<"HistManager::ReadTH3D  Warning : TH3D "<<s.c_str()<<" already exists. Ignoring."<<endl;
        cout<<"TH3D "<<s.c_str()<<" already exists"<<endl;
        return NULL;
    } //if hist already exists, do nothing
    
    TH3D* forig = (TH3D*)(f->Get(name.c_str()));
    if(forig == 0){cout<<"HistManager::ReadTH1D  Warning : TH1D "<<name.c_str()<<" not found in file"<<endl; return NULL;}
    TH3D* hret = CloneTH3D(s, forig, force_replace);
    
    return hret;
}

//__________________________________________________________________
//
TProfile* HistManager::ReadTProfile( const string &name, TFile* f, const string &key, bool force_replace){
    string s;
    if(key!=""){s=key;}
    else{ s=name; }
    
    if( !force_replace && (m_profile.find(s)!= m_profile.end()) ){
        cout<<"HistManager::ReadTProfile  Warning : TProfile "<<s.c_str()<<" already exists. Ignoring."<<endl;
        return NULL;
    } //if hist already exists, do nothing
    TProfile* forig = (TProfile*)(f->Get(name.c_str()));
    if(forig == 0){cout<<"HistManager::ReadTH1D  Warning : TH1D "<<name.c_str()<<" not found in file"<<endl; return NULL;}
    TProfile* hret = CloneTProfile(s, forig, force_replace);
    return hret;
}


