#include "IFAETopFramework/TRFManager.h"

#include "IFAETopFramework/NtupleData.h"
#include "IFAETopFramework/OutputData.h"
#include "IFAETopFramework/WeightManager.h"
#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/AnalysisObject.h"

TRFManager::TRFManager() : 
  m_outData(NULL),
  m_ntupData(NULL),
  m_opt(NULL),
  m_wgtMngr(NULL)
{}

TRFManager::TRFManager(OptionsBase* opt, WeightManager* wgtMngr, OutputData* outData, const NtupleData* ntupData) : 
  m_outData(outData),
  m_ntupData(ntupData),
  m_opt(opt),
  m_wgtMngr(wgtMngr)
{}

bool TRFManager::ComputeTRFWeights(){
  return true; 
}
bool TRFManager::ComputeTRFWeights(const std::vector<double>* /*jet_pt*/, const std::vector<double>* /*jet_eta*/
				   , const std::vector<double>* /*jet_btagw*/, const std::vector<int>* /*jet_truthlabel*/){
  return true;
}

bool TRFManager::ComputeTRFWeights(const AOVector &jets, const std::string& btagw_name, const std::string& truthlabel_name){ 

  std::vector<double> jets_pt; jets_pt.clear();
  std::vector<double> jets_eta; jets_eta.clear();
  std::vector<double> jets_btagw; jets_btagw.clear();
  std::vector<int> jets_truthlabel; jets_truthlabel.clear();

  for(AnalysisObject* obj : jets){
    jets_pt.push_back(obj->Pt());
    jets_eta.push_back(obj->Eta());
    jets_btagw.push_back(obj->GetMoment(btagw_name));
    jets_truthlabel.push_back( (int)(obj->GetMoment(truthlabel_name)) );
  }
  bool stat = ComputeTRFWeights(&jets_pt, &jets_eta, &jets_btagw, &jets_truthlabel);

 jets_pt.clear();
 jets_eta.clear();
 jets_btagw.clear();

 return stat;
}

bool TRFManager::UpdateTRFWeights(const int /*njets*/, const bool /*isIncl*/, const int /*req_nbjets*/){

  return true;

}
