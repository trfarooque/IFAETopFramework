#ifndef CUTFLOWTOOLS_H
#define CUTFLOWTOOLS_H

#include <string>
#include <map>
//#include <EventLoop/Algorithm.h>
#include <TH1F.h>
#include <iostream>
#include <sstream>  

class CutFlowTools
{
 
 public:
  CutFlowTools();
  ~CutFlowTools();

  std::map<std::string, std::vector<std::string> > m_vecCutFlowID; //!                      
  std::map<std::string, TH1F*> m_vecCutFlows; //!                                                
  void addCutFlow(std::string cutFlowID, std::string sCutIDs);
  void callAddCutFlow(std::string cutFlowID, std::string sCutIDs);
  void cutFlow(std::string cutFlowID, std::string cutID, double weight);
  void printCutFlow(std::string cutFlowID);
  void printAllCutFlow();
  std::string slength(std::string input, int size);
  std::string slength(double d, int size);
  
};

#endif 
