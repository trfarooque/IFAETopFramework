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

class TChain;
class OptionsBase;
class NtupleData;

class NtupleReader{
    
public:
    
    //
    // Standard C++ classes
    //
    NtupleReader(OptionsBase* opt);
    NtupleReader(const NtupleReader &);
    virtual ~NtupleReader();//{ delete m_ntupData; }

    //
    // Class specific functions
    //
    void Init();
    void Finalise();
    
    virtual int SetEventBranchAddresses();
    
    virtual int SetJetBranchAddresses(const std::string &sj);
    virtual int SetFatJetBranchAddresses(int sfj_key, const std::string &sfj);
    
    virtual int SetLeptonBranchAddresses(const std::string &sj);
    virtual int SetElectronBranchAddresses(const std::string &sj);
    virtual int SetMuonBranchAddresses(const std::string &sj);
    
    virtual int SetMETBranchAddresses(const std::string &sj);
    
    virtual int SetWeightBranchAddresses(const std::string &sj);
    virtual int SetTruthParticleBranchAddresses(const std::string &sj);
    
    int GetChainEntry(long entry) const ;
    int ChainNEntries() const;
    void ChainFromTextFile(TChain* ch, std::string infilename);
    void ChainFromStrList(TChain* ch, std::string infilelist);
    
    ///virtual const NtupleData* Data() = 0;//{ return m_ntupData; }

    //NtupleReader needs to know:
    // the location and name of the chain to read (read here from given file name and tree name)
    // the type of input tree
    // the ntupledata which it must fill
    
 protected:
    
    int SetVariableToChain(const std::string& name, void* variable);
    
    OptionsBase* m_opt;
    TChain* m_chain;    //!
    NtupleData* m_ntupData;
    
};


#endif //NTUPLEREADER_H
