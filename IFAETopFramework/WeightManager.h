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

  //Create the maps of nominal and systematics weights from input command line options
  bool AddAllWeights();

  //Calculate nominal and systematics event weights from components
  bool ComputeAllWeights();
  bool ComputeNominalWeight();
  bool ComputeSystematicWeights();
  bool ComputeSystematicWeight(const std::string& name);

  //Set the nominal weight component in the OutputData map. Event weights are left unchanged.
  bool SetNominalComponent(const std::string& name, double value);
  //Set the systematic weight component in the OutputData map. Event weights are left unchanged.
  bool SetSystematicComponent(const std::string& name, double value);

  //Set the nominal weight component in the OutputData map and update all affected event weights.
  bool UpdateNominalComponent(const std::string& name, double value);
  //Set the systematic weight component in the OutputData map and update all affected event weights.
  bool UpdateSystematicComponent(const std::string& name, double value);

 protected:

  bool AddNominal( const std::string &name, bool isInput=true, const std::string& branchName="", const std::string& inputType="D" );
  bool AddSystematic( const std::string &name, const std::string& affected_component=""
		      , bool isInput=true, const std::string& branchName="", const std::string& inputType="D" );

  bool AddWeightsFromString(const std::string& inputStr, bool isNominal=false);
  bool AddWeightsFromConfig( const std::string& inputStr );

  bool ComputeSystematicWeight(WeightObject* sys);
  bool SetWeightComponent(const std::string& name, double value, bool nominal );

  OptionsBase* m_opt;
  const NtupleData* m_ntupData;
  OutputData* m_outData;
  WeightMap* m_nomMap;
  WeightMap* m_systMap;

};

#endif
