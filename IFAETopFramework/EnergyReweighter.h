#ifndef ENERGYREWEIGHTER_H
#define ENERGYREWEIGHTER_H

#include "TString.h"
#include "TH1F.h"
#include "TFile.h"
#include "LHAPDF/LHAPDF.h"

class OptionsBase;

class EnergyReweighter {
    
public:
    
    enum PDFSet
    {
        MSTW2008LO = 21000,//VLT,
        CTEQ6L1 = 10042,//w,z+jets, dibosons,ttV,Gluino
        CT10 = 10800,//ttbar,singleTop,ttH
    };
    
    EnergyReweighter( OptionsBase *opt, const PDFSet &pdfset );
    ~EnergyReweighter();
    EnergyReweighter( const EnergyReweighter &q );
    
    //___________________________________________________
    //inline functions to set the parameters
    inline void scaleEnergyUp( const bool up ){ m_scaleEneryUp = up; } //default is true
    inline void useGeV( const bool GeV){ m_useGeV = GeV; } //default is false (input pT are MeV)
    inline void setOriginEnergy( const double energy ){ m_origEnergy = energy; }
    inline void setNewEnergy( const double energy ){ m_newEnergy = energy; }
    
    //___________________________________________________
    //functions to get the reweighting
    double getTopRWFactor( const double pT ) const;
    double getPDFRWFactor( const int pdf_id1, const int pdf_id2,
                           const double pdf_x1, const double pdf_x2,
                           const double pdf_scale);
    
    
private:
    OptionsBase *m_opt;
    TString m_rootFileTopRWpath, m_histTopRW8to14, m_histTopRW14to8;
    TH1F* m_histTopRW;
    TFile* m_rootFileTopRW;
    double m_origEnergy, m_newEnergy;
    bool m_scaleEneryUp,m_useGeV;
    LHAPDF::PDF *m_pdf;
};


#endif
