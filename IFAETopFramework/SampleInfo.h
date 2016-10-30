#ifndef SAMPLEINFO_H
#define SAMPLEINFO_H

#include <string>

class OptionsBase;

class SampleInfo {
    
public:
    
    //_______________________________________
    //
  SampleInfo();
  SampleInfo( const OptionsBase* opt );
  SampleInfo( const std::string& dsID, const std::string& configFile);
  SampleInfo( const OptionsBase* opt, const std::string& configFile);
  SampleInfo( const SampleInfo& q );
  ~SampleInfo();

  void ReadSample( const std::string& dsID, const std::string& configFile);
    //_______________________________________
    //
    inline double NWeightedEvents() const { return m_nWeightedEvents; }
    inline double CrossSection() const { return m_crossSection; }
    inline double NormFactor( const double lumi = 1. ) { return ( m_ready ? (m_crossSection * lumi / m_nWeightedEvents) : 1. ); }
    inline const std::string& SampleName(){ return m_sampleName; }
    inline bool Ready() const { return m_ready; }
    
    //_______________________________________
    //
    inline void SetCrossSection( const double xSec ){ m_crossSection = xSec; }
    inline void SetNWeightedEvents( const double evts ){ m_nWeightedEvents = evts; }
    inline void SetSampleName( const std::string& sampleName){ m_sampleName = sampleName; }

private:
    const OptionsBase* m_opt;
    double m_nWeightedEvents;
    double m_crossSection;
    std::string m_sampleName;
    bool m_ready;
    
};

#endif
