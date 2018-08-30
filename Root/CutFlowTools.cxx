//#include "xAODRootAccess/tools/Message.h"
#include <IFAETopFramework/CutFlowTools.h>
#include <string>


CutFlowTools::CutFlowTools(){}


CutFlowTools::~CutFlowTools(){}


void CutFlowTools::addCutFlow(std::string cutFlowID, std::string sCutIDs)
{
  std::vector<std::string> vecCutIDs;
  std::string nameID = "";
  for (unsigned int i=0; i<sCutIDs.size(); i++) {
    if (sCutIDs[i]=='|') {
      vecCutIDs.push_back(nameID);
      nameID = "";
    } else
      nameID = nameID + sCutIDs[i];
  }
  if (nameID!="") vecCutIDs.push_back(nameID);

  std::map<std::string, TH1F* >::iterator icutflowItr = m_vecCutFlows.begin();
  for ( ; icutflowItr!=m_vecCutFlows.end(); icutflowItr++){
    if (icutflowItr->first == cutFlowID) {
      std::cout<<"Duplicate cutflow "<<std::endl;
      //return error status code
      exit(0);
    }
  }

  TH1F *hptr = new TH1F(("CutFlow_"+cutFlowID).c_str(), "Cut-Flow", vecCutIDs.size(), 0.0, vecCutIDs.size()+0.0 );
  m_vecCutFlows[cutFlowID] = hptr;

  for (unsigned int i=0; i<vecCutIDs.size(); i++){
    m_vecCutFlows[cutFlowID]->GetXaxis()->SetBinLabel(i+1, TString(vecCutIDs[i]));
  }

  m_vecCutFlowID[cutFlowID] = vecCutIDs;

  return;
}


void CutFlowTools::cutFlow(std::string cutFlowID, std::string cutID, double weight)
{
  bool cutFlowExists = false;
  int nCut = -1;
  std::map<std::string, TH1F* >::iterator icutflowItr = m_vecCutFlows.begin();
  for ( ; icutflowItr!=m_vecCutFlows.end(); icutflowItr++){
    if (icutflowItr->first == cutFlowID) {
      cutFlowExists = true;
    }
  }
  if (!cutFlowExists){
    std::cout<< "CutFlow does not exist"<<std::endl;
    //return error status code
    exit(0);
  }

  for (unsigned int i=0; i<m_vecCutFlowID[cutFlowID].size(); i++)
    if (m_vecCutFlowID[cutFlowID][i] == cutID) nCut=i;
  if (nCut==-1) {
    std::cout<<"Cut ID does not exist"<<std::endl;
    //return error status code
    exit(0);
  }

  m_vecCutFlows[cutFlowID]->Fill(nCut, weight);

  return;
}


void CutFlowTools::printCutFlow(std::string cutFlowID)
{
  // int nCutFlow = -1;                                                                                                                                    

  double nAll = m_vecCutFlows[cutFlowID]->GetBinContent(1);
  std::cout <<"------------------------------------------------------- "<< std::endl;
  std::cout <<"| CutFlow Information for "<< cutFlowID<< std::endl;
  std::cout <<"------------------------------------------------------- "<< std::endl;
  std::cout <<"| Applied Cut     | # Events   | Eff.[%] | Rel.Eff[%] | "<< std::endl;
  std::cout <<"------------------------------------------------------- "<< std::endl;
  for (unsigned int i=0; i<m_vecCutFlowID[cutFlowID].size(); i++) {
    double num      = m_vecCutFlows[cutFlowID]->GetBinContent(i+1);
    double prev     = m_vecCutFlows[cutFlowID]->GetBinContent(i);
    double ratioFull    = 100.0;
    double ratioPrevious  = 100.0;
    if (i==0) ratioPrevious =1.0;
    if (nAll!=0.0)    ratioFull = num/nAll;
    if (prev!=0.0)    ratioPrevious = num/prev;
    std::cout <<"| "<<slength(m_vecCutFlowID[cutFlowID][i], 15)<<" | "<<slength(num,10)<<" | "<<slength(ratioFull*100.0,5)<<"   | "<<slength(ratioPrevious*100.0,5)<<"      |"<<std::endl;
  }
  std::cout<<"-------------------------------------------------------"<<std::endl;
  return;
}

std::string CutFlowTools::slength(std::string input, int size) {
  input=input+"                    ";
  input.resize(size);
  return input;
}

std::string CutFlowTools::slength(double d, int size) {
  std::ostringstream Str;
  Str << d;
  std::string input(Str.str());
  return slength(input, size);
}


void CutFlowTools::printAllCutFlow()
{
  std::map<std::string, TH1F* >::iterator icutflowItr = m_vecCutFlows.begin();
  for ( ; icutflowItr!=m_vecCutFlows.end(); icutflowItr++){
    printCutFlow(icutflowItr->first);
  }
  return;  
}
