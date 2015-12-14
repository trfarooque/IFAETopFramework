#ifndef HISTMANAGER_H
#define HISTMANAGER_H

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TProfile.h"
#include "TFile.h"

#include <map>
#include <math.h>
#include <string>
#include <iostream>

using namespace std;

class HistManager{
    
public:
    
    //
    // Standard C++ functions
    //
    HistManager();
    HistManager( const HistManager &q, bool make_new=false );
    
    //
    // Class-specific functions
    //
    vector<string> GetTH1KeyList();
    vector<string> GetTProfileKeyList();
    vector<string> GetTH2KeyList();
    vector<string> GetTH3KeyList();
    
    void FinaliseTH1Bins( const string &s_hist, bool addUF=true);
    
    void ClearAllTH1();
    void ClearAllTProfile();
    void ClearAllTH2();
    void ClearAllTH3();
    
    void ClearTH1( const string &s_hist );
    void ClearTProfile( const string &s_hist );
    void ClearTH2( const string &s_hist );
    void ClearTH3( const string &s_hist );
    
    TH1D* BookTH1D( const string &, const string &, const string &title = ""  );
    TH1D* BookTH1D( const string &name, const string &title, double binsize, double xlow, double xup,
                   const string &key="", const string &xtitle="", const string &ytitle="", int lw=2, int lc=1);
    TH1D* BookTH1D( const string &name, const string &title, int nbins, const double* xedges,
                   const string &key="", const string &xtitle="", const string &ytitle="", int lw=2, int lc=1);
    
    TH2D* BookTH2D( const string &key, const string &name, const string &title);
    TH2D* BookTH2D( const string &name, const string &title, double xbinsize, double xlow, double xup,
                   double ybinsize, double ylow, double yup, const string &key="", const string &xtitle="",
                   const string &ytitle="", int lw=2, int lc=1);
    TH2D* BookTH2D( const string &name, const string &title,
                   int nxbins, const double* xedges, int nybins, const double* yedges,
                   const string &key="", const string &xtitle="", const string &ytitle="",
                   int lw=2, int lc=1);
    TH2D* BookTH2D( const string &name, const string &title, double xbinsize, double xlow, double xup,
                  int nybins, const double* yedges,
                  const string &key="", const string &xtitle="", const string &ytitle="", int lw=2, int lc=1);
    
    TH3D* BookTH3D( const string &key, const string &name, const string &title);
    TH3D* BookTH3D( const string &name, const string &title, double xbinsize, double xlow, double xup,
                   double ybinsize, double ylow, double yup,
                   int nzbins, const double* zedges,
                   const string &key="", const string &xtitle="", const string &ytitle="",
                   const string &ztitle="", int lw=2, int lc=1);
    TH3D* BookTH3D( const string &name, const string &title, double xbinsize, double xlow, double xup,
                   int nybins, const double* yedges, int nzbins, const double* zedges,
                   const string &key="", const string &xtitle="", const string &ytitle="",
                   const string &ztitle="", int lw=2, int lc=1);
    TH3D* BookTH3D( const string &name, const string &title, double xbinsize, double xlow, double xup,
                   double ybinsize, double ylow, double yup,
                   double zbinsize, double zlow, double zup,
                   const string &key="", const string &xtitle="", const string &ytitle="", const string &ztitle="",
                   int lw=2, int lc=1);
    
    TProfile* BookTProfile( const string &name, const string &title, double binsize, double xlow, double xup,
                            const string &key="", const string &xtitle="", const string &ytitle="", int lw=2, int lc=1);
    TProfile* BookTProfile( const string &name, const string &title, int nbins, const double* xedges,
                            const string &key="", const string &xtitle="", const string &ytitle="", int lw=2, int lc=1);
    
    void FillTH1D( const string &hname, double val, double wgt=1. );
    void FillTH2D( const string &hname, double val1, double val2, double wgt=1. );
    void FillTH3D( const string &hname, double val1, double val2, double val3, double wgt=1. );
    void FillTProfile( const string &hname, double val1, double val2, double wgt=1. );
    
    TH1D* GetTH1D( const string &hname);
    TProfile* GetTProfile( const string &hname );
    TH2D* GetTH2D( const string &hname);
    TH3D* GetTH3D( const string &hname);

    
    TH1D* CloneTH1D( const string &hkey, const string &origkey, bool reset = false);
    TProfile* CloneTProfile( const string &hkey, const string &origkey, bool reset = false);
    TH2D* CloneTH2D( const string &hkey, const string &origkey, bool reset = false);
    TH3D* CloneTH3D( const string &hkey, const string &origkey, bool reset = false);
    
    TH1D* CloneTH1D( const string &hkey, TH1D* h1d, bool reset = false);
    TProfile* CloneTProfile( const string &hkey, TProfile* hprof, bool reset = false);
    TH2D* CloneTH2D( const string &hkey, TH2D* h2d, bool reset = false);
    TH3D* CloneTH3D( const string &hkey, TH3D* h3d, bool reset = false);

    
    void SetTH1D( const string &hkey, TH1D* h1d);
    void SetTProfile( const string &hkey, TProfile* h1d);
    void SetTH2D( const string &hkey, TH2D* h2d);
    void SetTH3D( const string &hkey, TH3D* h3d);
    
    void ReplaceTH1D( const string &hkey, TH1D* h1d);
    void ReplaceTProfile( const string &hkey, TProfile* h1d);
    void ReplaceTH2D( const string &hkey, TH2D* h2d);
    void ReplaceTH3D( const string &hkey, TH3D* h3d);
    
    TH1D* ReadTH1D( const string &name, TFile* f, const string &key);
    TProfile* ReadTProfile( const string &name, TFile* f, const string &key);
    TH2D* ReadTH2D( const string &name, TFile* f, const string &key);
    TH3D* ReadTH3D( const string &name, TFile* f, const string &key);
    
protected:
    map<string, TH1D*> m_h1d;
    map<string, TProfile*> m_profile;
    map<string, TH2D*> m_h2d;
    map<string, TH3D*> m_h3d;
};


#endif //HISTMANAGER_H
