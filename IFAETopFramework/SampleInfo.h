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
    inline double NWeightedEvents(const std::string& branch) const { return m_signalRWMap.at(branch); }
    inline double CrossSection() const { return m_crossSection; }
    inline double NormFactor( const double lumi = 1. ) { return ( m_ready ? (m_crossSection * lumi / m_nWeightedEvents) : 1. ); }
    inline double NormFactor( const std::string& branch, const double lumi = 1. ) { return ( m_ready ? (m_crossSection * lumi / m_signalRWMap.at(branch)) : 1. ); }
    inline const std::string& SampleName(){ return m_sampleName; }
    inline bool Ready() const { return m_ready; }
    inline std::map<std::string, double > SystWeightFactorMap() const { return m_systWeightFactorMap; }
    inline std::map<std::string, double> SignalRWMap() const {return m_signalRWMap; }

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
    std::map<std::string, double > m_systWeightFactorMap;
    std::map<std::string, double> m_signalRWMap;

};

#endif
