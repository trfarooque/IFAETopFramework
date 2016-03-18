#ifndef WEIGHTMANAGER_H
#define WEIGHTMANAGER_H

#include <string>
#include <map>
#include <iostream>
#include "IFAETopFramework/OutputData.h"
#include "IFAETopFramework/WeightObject.h"

class OptionsBase;
class NtupleData;

class WeightManager{

 public:

  typedef std::map< std::string, WeightObject* > WeightMap;

  WeightManager( OptionsBase* opt, const NtupleData* ntupData, OutputData* outData);
  WeightManager( const WeightManager& q); 
  ~WeightManager();

  inline WeightMap* NomMap(){ return m_nomMap; }
  inline WeightMap* SystMap(){ return m_systMap; }


  void Print() const;

  bool AddNominal( const std::string &name, bool isInput=true, const std::string& branchName="", const std::string& inputType="D" );
  bool AddSystematic( const std::string &name, const std::string& affected_component=""
		      , bool isInput=true, const std::string& branchName="", const std::string& inputType="D" );

  bool AddNominalsFromString();
  bool AddNominalsFromConfig();

  bool AddSystematicsFromString();
  bool AddSystematicsFromConfig();

  bool ComputeAllWeights();
  bool ComputeNominalWeights();
  bool ComputeSystematicWeights();


 protected:

  bool AddWeightsFromString(const std::string& inputStr, bool isNominal=false);
  bool AddWeightsFromConfig(const std::string& inputStr, bool isNominal=false);

  OptionsBase* m_opt;
  const NtupleData* m_ntupData;
  OutputData* m_outData;
  WeightMap* m_nomMap;
  WeightMap* m_systMap;
  
};

#endif
