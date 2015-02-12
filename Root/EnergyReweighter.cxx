#include <iostream>

#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/EnergyReweighter.h"


//__________________________________________________________
//
EnergyReweighter::EnergyReweighter( OptionsBase *opt, const PDFSet &pdfset ):
m_opt(opt),
m_rootFileTopRWpath("$ROOTCOREBIN/../IFAETopFramework/data/RwHstt.root"),
m_histTopRW8to14("ratio8to14"),
m_histTopRW14to8("ratio14to8"),
m_histTopRW(0),
m_rootFileTopRW(0),
m_origEnergy(0.),m_newEnergy(0.),
m_scaleEneryUp(true),m_useGeV(false),
m_pdf(0)
{
    //Necessary file for additionnal reweighting
    m_rootFileTopRW = new TFile(m_rootFileTopRWpath,"read");
    if(m_scaleEneryUp){
        m_histTopRW = (TH1F*)m_rootFileTopRW->Get(m_histTopRW8to14);
    } else {
        m_histTopRW = (TH1F*)m_rootFileTopRW->Get(m_histTopRW14to8);
    }
    m_histTopRW->SetDirectory(0);
    
    //Initialisation of pdf object
    m_pdf = LHAPDF::mkPDF(pdfset);
}

//__________________________________________________________
//
EnergyReweighter::~EnergyReweighter()
{}

//__________________________________________________________
//
EnergyReweighter::EnergyReweighter( const EnergyReweighter &q )
{
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in EnergyReweighter::EnergyReweighter( const EnergyReweighter &q )" << std::endl;
    
    m_opt               = q.m_opt;
    m_rootFileTopRWpath = q.m_rootFileTopRWpath;
    m_histTopRW8to14    = q.m_histTopRW8to14;
    m_histTopRW14to8    = q.m_histTopRW14to8;
    m_histTopRW         = q.m_histTopRW;
    m_rootFileTopRW     = q.m_rootFileTopRW;
    m_origEnergy        = q.m_origEnergy;
    m_newEnergy         = q.m_newEnergy;
    m_scaleEneryUp      = q.m_scaleEneryUp;
    m_useGeV            = q.m_useGeV;
    m_pdf               = q.m_pdf;
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Leaving EnergyReweighter::EnergyReweighter( const EnergyReweighter &q )" << std::endl;
}

//__________________________________________________________
//
double EnergyReweighter::getTopRWFactor( const double pT ) const {
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in EnergyReweighter::getTopRWFactor(" << pT << ")" << std::endl;
    
    const double used_pT = m_useGeV ? pT*1000 : pT;
    
    const int pTbin = m_histTopRW -> FindBin(used_pT);
    const int Nbins = m_histTopRW -> GetNbinsX();
    
    double rw = 1;
    if(pTbin>Nbins) rw = m_histTopRW -> GetBinContent(Nbins);
    else if(pTbin<1) rw = m_histTopRW -> GetBinContent(1);
    else rw = m_histTopRW -> GetBinContent(pTbin);
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Leaving EnergyReweighter::getTopRWFactor()" << std::endl;

    return rw;
}

//__________________________________________________________
//
double EnergyReweighter::getPDFRWFactor(const int pdf_id1, const int pdf_id2,
                      const double pdf_x1, const double pdf_x2,
                      const double pdf_scale){
    
    if(m_opt->msgLevel()==Debug::DEBUG)
        std::cout << "Entering in EnergyReweighter::getPDFRWFactor("<<pdf_id1<<","<<pdf_id2<<","<<pdf_x1<<","<<pdf_x2<<","<<pdf_scale<<")" << std::endl;
    
    
    if(m_origEnergy == 0){
        std::cerr << "<!> Error in getPDFRWFactor() function: new energy is 0 !!" << std::endl;
        return -1;
    }
    
    double ratioE = m_newEnergy/m_origEnergy;
    
    double f_1_orig = m_pdf -> xfxQ(pdf_id1, pdf_x1, pdf_scale) / pdf_x1;
    double f_2_orig = m_pdf -> xfxQ(pdf_id2, pdf_x2, pdf_scale) / pdf_x2;
    
    double f_1_new = m_pdf -> xfxQ(pdf_id1, pdf_x1/ratioE, pdf_scale) / (pdf_x1/ratioE);
    double f_2_new = m_pdf -> xfxQ(pdf_id2, pdf_x2/ratioE, pdf_scale) / (pdf_x2/ratioE);
    
    double rw = (f_1_new*f_2_new)/(f_1_orig*f_2_orig)/pow(ratioE, 2);
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Leaving EnergyReweighter::getPDFRWFactor()" << std::endl;
    
    return rw;
}


