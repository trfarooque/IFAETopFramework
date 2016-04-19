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

  //This function will read in the correct b-tag weight components, corresponding to the required b-jet multiplicity, into the 
  //OutputData map. It will then call the UpdateNominalComponent and UpdateSystematicComponent functions of WeightManager to 
  //properly update all event weights.  
  virtual bool UpdateTRFWeights(const int /*njets*/, const bool /*isIncl*/, const int /*req_nbjets*/);


 protected:
  OutputData* m_outData;
  const NtupleData* m_ntupData;
  OptionsBase* m_opt;
  WeightManager* m_wgtMngr;

};

#endif
