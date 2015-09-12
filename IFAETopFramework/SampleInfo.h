#ifndef SAMPLEINFO_H
#define SAMPLEINFO_H

#include <string>

class OptionsBase;

class SampleInfo {
    
public:
    
    //_______________________________________
    //
    SampleInfo( OptionsBase* opt );
    SampleInfo( OptionsBase* opt, const std::string &, const std::string & );
    SampleInfo( const SampleInfo & );
    ~SampleInfo();
    
    //_______________________________________
    //
    inline double NWeightEvents() const { return m_nWeightedEvents; }
    inline double CrossSection() const { return m_crossSection; }
    inline double NormFactor( const double lumi ) { return (m_crossSection * lumi / m_nWeightedEvents); }
    
    //_______________________________________
    //
    inline void SetCrossSection( const double xSec ){ m_crossSection = xSec; }
    inline void SetNWeightedEvents( const double evts ){ m_nWeightedEvents = evts; }
    
private:
    OptionsBase* m_opt;
    double m_nWeightedEvents;
    double m_crossSection;
    
};

#endif
