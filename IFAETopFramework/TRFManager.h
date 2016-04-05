#ifndef TRFMANAGER_H
#define TRFMANAGER_H

#include <vector>
#include <string>

class NtupleData;
class OutputData;
class WeightManager;
class OptionsBase;
class AnalysisObject;

class TRFManager{
 public:
  TRFManager();
  TRFManager(OptionsBase* opt, WeightManager* wgtMngr, OutputData* outData, const NtupleData* ntupData);
  virtual ~TRFManager(){} 

  virtual bool ComputeTRFWeights();
  virtual bool ComputeTRFWeights(const std::vector<double>* /*jet_pt*/, const std::vector<double>* /*jet_eta*/
				 , const std::vector<double>* /*jet_btagw*/, const std::vector<int>* /*jet_truthlabel*/);
  virtual bool ComputeTRFWeights(const std::vector<AnalysisObject*> &jets, const std::string& btagw_name, const std::string& truthlabel_name);
  
  bool PassBtagRequirement(const int /*evt_DT_nbjets*/, const bool /*isIncl*/, const int /*req_nbjets*/, std::vector<AnalysisObject*>& /*jets*/);


 protected:
  OutputData* m_outData;
  const NtupleData* m_ntupData;
  OptionsBase* m_opt;
  WeightManager* m_wgtMngr;

};

#endif
