#ifndef NTUPLEREADER_H
#define NTUPLEREEADER_H

#include <string>
#include <map>
#include "TChain.h"

class OptionsBase;
class NtupleData;
class TBranch;

class NtupleReader{
    
public:
    NtupleReader(NtupleData* ntupData, OptionsBase* opt);
    virtual ~NtupleReader();
    
    void Init();
    void Finalise();
    virtual void EmptyBranches();
    
    virtual int setEventBranchAddresses();
    
    virtual int setJetBranchAddresses(const std::string &sj);
    virtual int setFatJetBranchAddresses(int sfj_key, const std::string &sfj);
    
    virtual int setLeptonBranchAddresses(const std::string &sj);
    virtual int setElectronBranchAddresses(const std::string &sj);
    virtual int setMuonBranchAddresses(const std::string &sj);
    
    virtual int setMETBranchAddresses(const std::string &sj);
    
    virtual int setWeightBranchAddresses(const std::string &sj);
    
    int getChainEntry(long entry){return m_chain->GetEntry(entry); }
    int chainNEntries(){return m_chain->GetEntries();}
    
    void ChainFromTextFile(TChain* ch, std::string infilename);
    void ChainFromStrList(TChain* ch, std::string infilelist);
    
    //NtupleReader needs to know:
    // the location and name of the chain to read (read here from given file name and tree name)
    // the type of input tree
    // the ntupledata which it must fill
    
protected:
    TChain* m_chain;    //!
    OptionsBase* m_opt;
    NtupleData* m_ntupData;
};


#endif
