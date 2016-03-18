/*
 
 NtupleReader class
 ==================
 Allows to do the interface between the input ntuples and the variables used in the users' analysis
 code.
 Most functions in the implementation file of this class are empty: they have to be implemented in the
 inherited classes !!
 
 Contacts
 --------
 Trisha Farooque <trisha.farooque@cern.ch>
 Loic Valery <lvalery@cern.ch>
 
*/


#ifndef NTUPLEREADER_H
#define NTUPLEREADER_H

#include <string>
#include <map>

#include "TChain.h"
#include "IFAETopFramework/OptionsBase.h"
class NtupleData;
class WeightObject;

class NtupleReader{
    
public:
    
    //
    // Standard C++ classes
    //
  NtupleReader( OptionsBase *opt, std::map<std::string, WeightObject*>* nomMap=NULL, std::map<std::string, WeightObject*>* sysMap=NULL);
  NtupleReader(const NtupleReader &);
  virtual ~NtupleReader();//{ delete m_ntupData; }

  //
  // Class specific functions
  //
  void Init(std::map<std::string, WeightObject*> *nomMap = NULL, std::map<std::string, WeightObject*> *sysMap = NULL);
  void Finalise();
  inline void SetNomComponentsMap(std::map<std::string, WeightObject*> *nomMap){ m_nomMap = nomMap; }
  inline void SetSystComponentsMap(std::map<std::string, WeightObject*> *sysMap){ m_sysMap = sysMap; }

  int SetAllBranchAddresses();    
  int SetWeightBranchAddresses(const std::string &sj);

    
  int GetChainEntry(long entry) const ;
  int ChainNEntries() const;
  void ChainFromTextFile(TChain* ch, std::string infilename);
  void ChainFromStrList(TChain* ch, std::string infilelist);
    
  //virtual const NtupleData* Data() const = 0;//{ return m_ntupData; }

    //NtupleReader needs to know:
    // the location and name of the chain to read (read here from given file name and tree name)
    // the type of input tree
    // the ntupledata which it must fill
    
 protected:
    
    OptionsBase* m_opt;
    TChain* m_chain;    //!
    NtupleData* m_ntupData;
    std::map<std::string, WeightObject*>* m_nomMap;
    std::map<std::string, WeightObject*>* m_sysMap;

    int SetVariableToChain(const std::string& name, void* variable);
    
    template<typename T>int SetVariableToChain(const std::string& name, T** variable){

      if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<"NtupleReader::SetVariableToChain(T**) :: Setting "<<name<<" to "<<variable<<std::endl; }
      m_chain->SetBranchStatus(name.c_str(), 1);
      
      TBranch* branch = 0;
      int stat = m_chain->SetBranchAddress(name.c_str(), variable, &branch);
      return stat;
    }
    

};


#endif //NTUPLEREADER_H
