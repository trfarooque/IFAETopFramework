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
    NtupleReader(NtupleData* ntupData, OptionsBase* opt);
    NtupleReader(const NtupleReader &);
    virtual ~NtupleReader();

    //
    // Class specific functions
    //
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
    
    
    int getChainEntry(long entry) const ;
    int chainNEntries() const;
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


#endif //NTUPLEREADER_H
