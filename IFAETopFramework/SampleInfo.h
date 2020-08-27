#ifndef SAMPLEINFO_H
#define SAMPLEINFO_H

#include <string>
#include <map>

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
    double NWeightedEvents(const std::string& wgt_name="nominal") const;
    inline double CrossSection() const { return m_crossSection; }
    double NormFactor( const std::string& wgt_name="nominal", double lumi = 1. ) const;
    inline const std::string& SampleName(){ return m_sampleName; }
    inline bool Ready() const { return m_ready; }
    //inline std::map<std::string, double > SystWeightFactorMap() const { return m_systWeightFactorMap; }

    //_______________________________________
    //
    inline void SetCrossSection( const double xSec ){ m_crossSection = xSec; }
    void SetNWeightedEvents( const double evts, const std::string& wgt_name );
    inline void SetSampleName( const std::string& sampleName){ m_sampleName = sampleName; }

private:
    const OptionsBase* m_opt;
    std::map<std::string, double>* m_nWeightedEvents;
    double m_crossSection;
    std::string m_sampleName;
    bool m_ready;
    //std::map<std::string, double > m_systWeightFactorMap;
    
};

#endif
