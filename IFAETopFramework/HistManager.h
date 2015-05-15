#ifndef HISTMANAGER_H
#define HISTMANAGER_H

#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TFile.h>
#include <map> 
#include <math.h>
#include <string>
#include <iostream>

using namespace std;

class HistManager{

 public:
  HistManager();

  void BookTH1D(string name, string title, string key="" );

  void BookTH1D(string name, string title, double binsize, double xlow, double xup,
		string key="", string xtitle="", string ytitle="", int lw=2, int lc=1);

  void BookTH1D(string name, string title, int nbins, double* xedges,
		string key="", string xtitle="", string ytitle="", int lw=2, int lc=1);

  void BookTH2D(string key, string name, string title);

  void BookTH2D(string name, string title, double xbinsize, double xlow, double xup,
		double ybinsize, double ylow, double yup,
		string key="", string xtitle="", string ytitle="", int lw=2, int lc=1);

  void BookTH2D(string name, string title,
		int nxbins, double* xedges, int nybins, double* yedges,
		string key="", string xtitle="", string ytitle="", int lw=2, int lc=1);

  void BookTH2D(string name, string title, double xbinsize, double xlow, double xup,
		int nybins, double* yedges,
		string key="", string xtitle="", string ytitle="", int lw=2, int lc=1);

  void BookTH3D(string key, string name, string title);

  void BookTH3D(string name, string title, double xbinsize, double xlow, double xup,
		double ybinsize, double ylow, double yup, 
		int nzbins, double* zedges, 
		string key="", string xtitle="", string ytitle="",string ztitle="", int lw=2, int lc=1);
  
  void BookTH3D(string name, string title, double xbinsize, double xlow, double xup,
		int nybins, double* yedges, int nzbins, double* zedges, 
		string key="", string xtitle="", string ytitle="",string ztitle="", int lw=2, int lc=1);
  
  void BookTH3D(string name, string title, double xbinsize, double xlow, double xup,
		double ybinsize, double ylow, double yup, 
		double zbinsize, double zlow, double zup, 
		string key="", string xtitle="", string ytitle="", string ztitle="", int lw=2, int lc=1);
  
  void FillTH1D(string hname, double val, double wgt=1.);
  void FillTH2D(string hname, double val1, double val2, double wgt=1.);
  void FillTH3D(string hname, double val1, double val2, double val3, double wgt=1.);

  TH1D* GetTH1D(string hname){ return m_h1d[hname]; }
  TH2D* GetTH2D(string hname){ return m_h2d[hname]; }
  TH3D* GetTH3D(string hname){ return m_h3d[hname]; }

  void SetTH1D(string hkey, TH1D* h1d);
  void SetTH2D(string hkey, TH2D* h2d);
  void SetTH3D(string hkey, TH3D* h3d);

  void ReplaceTH1D(string hkey, TH1D* h1d);
  void ReplaceTH2D(string hkey, TH2D* h2d);
  void ReplaceTH3D(string hkey, TH3D* h3d);

  int ReadTH1D(string name, TFile* f, string key);
  int ReadTH2D(string name, TFile* f, string key);
  int ReadTH3D(string name, TFile* f, string key);

  void FinaliseTH1Bins(string s_hist);

  vector<string> GetTH1KeyList();
  vector<string> GetTH2KeyList();
  vector<string> GetTH3KeyList();

  void ClearTH1(string s_hist);
  void ClearTH2(string s_hist);
  void ClearTH3(string s_hist);

 protected:
  map<string, TH1D*> m_h1d;
  map<string, TH2D*> m_h2d;
  map<string, TH3D*> m_h3d;


};




#endif
