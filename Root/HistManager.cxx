#include "IFAETopFramework/HistManager.h"

using namespace std;

//__________________________________________________________________
//
HistManager::HistManager(){
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
void HistManager::FinaliseTH1Bins(const string &s_hist, bool addUF){
    map<string, TH1D*>::iterator h1_it = m_h1d.find(s_hist);
    if( h1_it == m_h1d.end() ){
        cout<<"H1D "<<s_hist<<" not found "<<endl;
        return;
    }
    
    int nbins = h1_it->second->GetNbinsX();

    double v_of       = h1_it->second->GetBinContent(nbins+1);
    double v_last     = h1_it->second->GetBinContent(nbins);
    double e_of       = h1_it->second->GetBinError(nbins+1);
    double e_last     = h1_it->second->GetBinError(nbins);
    h1_it->second->SetBinContent(nbins, v_last + v_of);
    h1_it->second->SetBinError(nbins, sqrt(e_last*e_last + e_of*e_of) );
    h1_it->second->SetBinContent(nbins+1, 0.);
    h1_it->second->SetBinError(nbins+1, 0.);

    if(addUF){
      double v_uf       = h1_it->second->GetBinContent(0);
      double v_first    = h1_it->second->GetBinContent(1);
      double e_uf       = h1_it->second->GetBinError(0);
      double e_first    = h1_it->second->GetBinError(1);
      h1_it->second->SetBinContent(1, v_first + v_uf);
      h1_it->second->SetBinError(1, sqrt(e_first*e_first + e_uf*e_uf) );
    }
    //Always set the underflow to zero, whether or not it is included in the first bin
    h1_it->second->SetBinContent(0, 0.);
    h1_it->second->SetBinError(0, 0.);

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
void HistManager::ClearTProfile(const string &s_hist){
    map<string, TProfile*>::iterator prof_it = m_profile.find(s_hist);
    if( prof_it == m_profile.end() ){
        cout<<"TProfile: "<<s_hist<<" not found "<<endl;
        return;
    }
    delete prof_it->second;
    m_profile.erase(prof_it);
    return;
}


//__________________________________________________________________
//
TH1D* HistManager::BookTH1D( const string &key, const string &name, const string &title){
    TH1D* h1 = new TH1D();
    h1->SetDirectory(0);
    h1->SetName(name.c_str());
    h1->SetTitle(title.c_str());
    const string &in_key = (key=="") ? name : key;
    if(m_h1d.find(in_key) != m_h1d.end()){
        cout<<"Warning: Replacing existing H1D "<<name<<endl;
    }
    m_h1d[in_key]=h1;
    
    return h1;
}

//__________________________________________________________________
//
TH1D* HistManager::BookTH1D( const string &name, const string &title, double binsize, double xlow, double xup,
                            const string &key, const string &xtitle, const string &ytitle, int lw, int lc){
    
    double dnbins=(xup-xlow)/binsize +0.5;
    int nbins=(int)(dnbins);
    TH1D* h1=new TH1D(name.c_str(), title.c_str(), nbins,xlow,xup);
    h1->SetDirectory(0);
    if(xtitle!=""){h1->GetXaxis()->SetTitle(xtitle.c_str());}
    if(ytitle!=""){h1->GetYaxis()->SetTitle(ytitle.c_str());}
    h1->SetLineWidth(lw);
    h1->SetLineColor(lc);
    h1->Sumw2();
    
    const string &in_key = (key=="") ? name : key;
    if(m_h1d.find(in_key) != m_h1d.end()){
        cout<<"Warning: Replacing existing H1D "<<name<<endl;
    }
    m_h1d[in_key]=h1;
    return h1;
}

//__________________________________________________________________
//
TH1D* HistManager::BookTH1D( const string &name, const string &title, int nbins, const double* xedges,
                            const string &key, const string &xtitle, const string &ytitle, int lw, int lc){
    
    TH1D* h1=new TH1D(name.c_str(), title.c_str(), nbins,xedges);
    h1->SetDirectory(0);
    if(xtitle!="")h1->GetXaxis()->SetTitle(xtitle.c_str());
    if(ytitle!="")h1->GetYaxis()->SetTitle(ytitle.c_str());
    h1->SetLineWidth(lw);
    h1->SetLineColor(lc);
    h1->Sumw2();
    
    const string &in_key = (key=="") ? name : key;
    if(m_h1d.find(in_key) != m_h1d.end()){
        cout<<"Warning: Replacing existing H1D "<<name<<endl;
    }
    m_h1d[in_key]=h1;
    return h1;
}

//__________________________________________________________________
//
TH2D* HistManager::BookTH2D( const string &key, const string &name, const string &title ){
    TH2D* h2 = new TH2D();
    h2->SetDirectory(0);
    h2->SetName(name.c_str());
    h2->SetTitle(title.c_str());
    
    const string &in_key = (key=="") ? name : key;
    if(m_h2d.find(in_key) != m_h2d.end()){
        cout<<"Warning: Replacing existing H2D "<<name<<endl;
    }
    m_h2d[in_key]=h2;
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
    h2->SetDirectory(0);
    if(xtitle!="")h2->GetXaxis()->SetTitle(xtitle.c_str());
    if(ytitle!="")h2->GetYaxis()->SetTitle(ytitle.c_str());
    h2->SetLineWidth(lw);
    h2->SetLineColor(lc);
    h2->Sumw2();
    
    const string &in_key = (key=="") ? name : key;
    if(m_h2d.find(in_key) != m_h2d.end()){
        cout<<"Warning: Replacing existing H2D "<<name<<endl;
    }
    m_h2d[in_key]=h2;
    return h2;
}

//__________________________________________________________________
//
TH2D* HistManager::BookTH2D(const string &name, const string &title,
                            int nxbins, const double* xedges, int nybins, const double* yedges,
                            const string &key, const string &xtitle, const string &ytitle,
                            int lw, int lc){
    
    TH2D* h2=new TH2D(name.c_str(), title.c_str(), nxbins,xedges, nybins,yedges);
    h2->SetDirectory(0);
    if(xtitle!="")h2->GetXaxis()->SetTitle(xtitle.c_str());
    if(ytitle!="")h2->GetYaxis()->SetTitle(ytitle.c_str());
    h2->SetLineWidth(lw);
    h2->SetLineColor(lc);
    h2->Sumw2();
    
    const string &in_key = (key=="") ? name : key;
    if(m_h2d.find(in_key) != m_h2d.end()){
        cout<<"Warning: Replacing existing H2D "<<name<<endl;
    }
    m_h2d[in_key]=h2;
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
    h2->SetDirectory(0);
    if(xtitle!="")h2->GetXaxis()->SetTitle(xtitle.c_str());
    if(ytitle!="")h2->GetYaxis()->SetTitle(ytitle.c_str());
    h2->SetLineWidth(lw);
    h2->SetLineColor(lc);
    h2->Sumw2();
    
    const string &in_key = (key=="") ? name : key;
    if(m_h2d.find(in_key) != m_h2d.end()){
        cout<<"Warning: Replacing existing H2D "<<name<<endl;
    }
    m_h2d[in_key]=h2;
    return h2;
    
}

//__________________________________________________________________
//
TH3D* HistManager::BookTH3D(const string &key, const string &name, const string &title){
    TH3D* h3 = new TH3D();
    h3->SetDirectory(0);
    h3->SetName(name.c_str());
    h3->SetTitle(title.c_str());
    h3->Sumw2();
    
    const string &in_key = (key=="") ? name : key;
    if(m_h3d.find(in_key) != m_h3d.end()){
        cout<<"Warning: Replacing existing H3D "<<name<<endl;
    }
    m_h3d[in_key]=h3;
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
    h3->SetDirectory(0);
    if(xtitle!="")h3->GetXaxis()->SetTitle(xtitle.c_str());
    if(ytitle!="")h3->GetYaxis()->SetTitle(ytitle.c_str());
    if(ztitle!="")h3->GetZaxis()->SetTitle(ztitle.c_str());
    h3->SetLineWidth(lw);
    h3->SetLineColor(lc);
    h3->Sumw2();
    
    const string &in_key = (key=="") ? name : key;
    if(m_h3d.find(in_key) != m_h3d.end()){
        cout<<"Warning: Replacing existing H3D "<<name<<endl;
    }
    m_h3d[in_key]=h3;
    
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
    h3->SetDirectory(0);
    if(xtitle!="")h3->GetXaxis()->SetTitle(xtitle.c_str());
    if(ytitle!="")h3->GetYaxis()->SetTitle(ytitle.c_str());
    if(ztitle!="")h3->GetZaxis()->SetTitle(ztitle.c_str());
    h3->SetLineWidth(lw);
    h3->SetLineColor(lc);
    h3->Sumw2();
    
    const string &in_key = (key=="") ? name : key;
    if(m_h3d.find(in_key) != m_h3d.end()){
        cout<<"Warning: Replacing existing H3D "<<name<<endl;
    }
    m_h3d[in_key]=h3;
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
    h3->SetDirectory(0);
    if(xtitle!="")h3->GetXaxis()->SetTitle(xtitle.c_str());
    if(ytitle!="")h3->GetYaxis()->SetTitle(ytitle.c_str());
    if(ztitle!="")h3->GetZaxis()->SetTitle(ztitle.c_str());
    h3->SetLineWidth(lw); h3->SetLineColor(lc);
    h3->Sumw2();
    
    const string &in_key = (key=="") ? name : key;
    if(m_h3d.find(in_key) != m_h3d.end()){
        cout<<"Warning: Replacing existing H3D "<<name<<endl;
    }
    m_h3d[in_key]=h3;
    
    delete[] xe;
    return h3;
}

//__________________________________________________________________
//
TProfile* HistManager::BookTProfile( const string &name, const string &title, double binsize, double xlow, double xup,
                                    const string &key, const string &xtitle, const string &ytitle, int lw, int lc){
    
    double dnbins=(xup-xlow)/binsize +0.5;
    int nbins=(int)(dnbins);
    TProfile* prof = new TProfile(name.c_str(), title.c_str(), nbins,xlow,xup);
    prof->SetDirectory(0);
    if(xtitle!=""){prof->GetXaxis()->SetTitle(xtitle.c_str());}
    if(ytitle!=""){prof->GetYaxis()->SetTitle(ytitle.c_str());}
    prof->SetLineWidth(lw);
    prof->SetLineColor(lc);
    prof->Sumw2();
    
    const string &in_key = (key=="") ? name : key;
    if(m_profile.find(in_key) != m_profile.end()){
        cout<<"Warning: Replacing existing TProfile: "<<name<<endl;
    }
    m_profile[in_key]=prof;
    return prof;
}

//__________________________________________________________________
//
TProfile* HistManager::BookTProfile( const string &name, const string &title, int nbins, const double* xedges,
                                     const string &key, const string &xtitle, const string &ytitle, int lw, int lc){
    
    TProfile* prof = new TProfile( name.c_str(), title.c_str(), nbins, xedges );
    prof->SetDirectory(0);
    if(xtitle!="")prof->GetXaxis()->SetTitle(xtitle.c_str());
    if(ytitle!="")prof->GetYaxis()->SetTitle(ytitle.c_str());
    prof->SetLineWidth(lw);
    prof->SetLineColor(lc);
    prof->Sumw2();
    
    const string &in_key = (key=="") ? name : key;
    if(m_profile.find(in_key) != m_profile.end()){
        cout<<"Warning: Replacing existing TProfile "<<name<<endl;
    }
    m_profile[in_key]=prof;
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
TH1D* HistManager::CloneTH1D( const string &hkey, const string &origkey, bool reset){
    if(m_h1d.find(origkey) == m_h1d.end()){
        std::cout<<"Error: TH1D "<<origkey<<" not found to clone"<<std::endl;
        return NULL;
    }
    TH1D* hret = CloneTH1D(hkey, m_h1d[origkey], reset);
    return hret;
}

//__________________________________________________________________
//
TH2D* HistManager::CloneTH2D( const string &hkey, const string &origkey, bool reset){
    if(m_h2d.find(origkey) == m_h2d.end()){
        std::cout<<"Error: TH2D "<<origkey<<" not found to clone"<<std::endl;
        return NULL;
    }
    TH2D* hret = CloneTH2D(hkey, m_h2d[origkey], reset);
    return hret;
}

//__________________________________________________________________
//
TH3D* HistManager::CloneTH3D( const string &hkey, const string &origkey, bool reset){
    if(m_h3d.find(origkey) == m_h3d.end()){
        std::cout<<"Error: TH3D "<<origkey<<" not found to clone"<<std::endl;
        return NULL;
    }
    TH3D* hret = CloneTH3D(hkey, m_h3d[origkey], reset);
    return hret;
}

//__________________________________________________________________
//
TProfile* HistManager::CloneTProfile( const string &hkey, const string &origkey, bool reset){
    if(m_profile.find(origkey) == m_profile.end()){
        std::cout<<"Error: TProfile "<<origkey<<" not found to clone"<<std::endl;
        return NULL;
    }
    TProfile* hret = CloneTProfile(hkey, m_profile[origkey], reset);
    return hret;
}

//__________________________________________________________________
//
TH1D* HistManager::CloneTH1D( const string &hkey, TH1D* h1d, bool reset){
    
    TH1D* hclone = (TH1D*)(h1d->Clone(hkey.c_str()));
    hclone->SetDirectory(0);
    if(reset){hclone->Reset();}
    if(hclone->GetSumw2N()==0){hclone->Sumw2();}
    
    m_h1d[hkey] = hclone;
    return hclone;
}

//__________________________________________________________________
//
TH2D* HistManager::CloneTH2D( const string &hkey, TH2D* h2d, bool reset){
    
    TH2D* hclone = (TH2D*)(h2d->Clone(hkey.c_str()));
    hclone->SetDirectory(0);
    if(reset){hclone->Reset();}
    if(hclone->GetSumw2N()==0){hclone->Sumw2();}
    m_h2d[hkey] = hclone;
    return hclone;
}

//__________________________________________________________________
//
TH3D* HistManager::CloneTH3D( const string &hkey, TH3D* h3d, bool reset){
    
    TH3D* hclone = (TH3D*)(h3d->Clone(hkey.c_str()));
    hclone->SetDirectory(0);
    if(reset){hclone->Reset();}
    if(hclone->GetSumw2N()==0){hclone->Sumw2();}
    m_h3d[hkey] = hclone;
    return hclone;
}

//__________________________________________________________________
//
TProfile* HistManager::CloneTProfile( const string &hkey, TProfile* prof_ori, bool reset){
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
TH1D* HistManager::ReadTH1D( const string &name, TFile* f, const string &key){
    
    string s;
    if(key!=""){s=key;}
    else{ s=name; }
    
    if(m_h1d.find(s)!= m_h1d.end()){
        cout<<"TH1D "<<s.c_str()<<" already exists"<<endl;
        return NULL;
    } //if hist already exists, do nothing
    TH1D* forig = (TH1D*)(f->Get(name.c_str()));
    if(forig == 0){cout<<name.c_str()<<" not found in file"<<endl; return NULL;}
    TH1D* hret = CloneTH1D(s, forig);
    return hret;
}

//__________________________________________________________________
//
TH2D* HistManager::ReadTH2D(const string &name, TFile* f, const string &key){
    
    string s;
    if(key!=""){s=key;}
    else{ s=name; }
    
    if(m_h2d.find(s)!= m_h2d.end()){
        cout<<"TH2D "<<s.c_str()<<" already exists"<<endl;
        return NULL;
    } //if hist already exists, do nothing
    
    TH2D* forig = (TH2D*)(f->Get(name.c_str()));
    if(forig == 0){cout<<name.c_str()<<" not found in file"<<endl; return NULL;}
    TH2D* hret = CloneTH2D(s, forig);
    
    return hret;
    
}

//__________________________________________________________________
//
TH3D* HistManager::ReadTH3D(const string &name, TFile* f, const string &key){
    
    string s;
    if(key!=""){s=key;}
    else{ s=name; }
    
    if(m_h3d.find(s)!= m_h3d.end()){
        cout<<"TH3D "<<s.c_str()<<" already exists"<<endl;
        return NULL;
    } //if hist already exists, do nothing
    
    TH3D* forig = (TH3D*)(f->Get(name.c_str()));
    if(forig == 0){cout<<name.c_str()<<" not found in file"<<endl; return NULL;}
    TH3D* hret = CloneTH3D(s, forig);
    
    return hret;
}

//__________________________________________________________________
//
TProfile* HistManager::ReadTProfile( const string &name, TFile* f, const string &key){
    string s;
    if(key!=""){s=key;}
    else{ s=name; }
    
    if(m_profile.find(s)!= m_profile.end()){
        cout<<"TProfile "<<s.c_str()<<" already exists"<<endl;
        return NULL;
    } //if hist already exists, do nothing
    TProfile* forig = (TProfile*)(f->Get(name.c_str()));
    if(forig == 0){cout<<name.c_str()<<" not found in file"<<endl; return NULL;}
    TProfile* hret = CloneTProfile(s, forig);
    return hret;
}


