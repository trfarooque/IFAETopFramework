/*
 
  OptionsBase class
    * Allows to access all the user-specific options
    * Way of usage (in main function):
        OptionsBase opt;
        opt.parseUser(argc, argv);
    * All data members are private: can access their values through the getter:
        Convention: getter as the same naming as the data member without m_
 
*/


#ifndef OPTIONSBASE_H
#define OPTIONSBASE_H

#include <string>
#include "IFAETopFramework/CommonConstants.h"
#include <iostream>
#include <algorithm>

class OptionsBase{
    
    
public:
    
    OptionsBase();
    OptionsBase( const OptionsBase& );
    virtual ~OptionsBase();
    
    //_____________________________________________________________
    // Informations about options
    void ParseUserOpts(int argc, char** argv);
    virtual void PrintOptions();
    
    //_____________________________________________________________
    // Data access and modification
    inline const std::string& OptStr() const{ return m_optStr; }

    inline Debug::debug MsgLevel() const { return m_msgLevel; }
    
    inline bool IsData() const { return m_isData; }
    inline bool ComputeWeightSys() const { return m_computeWeightSys; }
    inline bool TextFileList() const { return m_textFileList; }
    inline bool OnlyDumpSystHistograms() const { return m_onlyDumpSystHistograms; }
    inline bool DoBlind() const { return m_doBlind; }
    
    inline bool AddUnderflow() const { return m_add_underflow; }
    inline bool AddOverflow() const { return m_add_overflow; }

    inline const std::string& StrSampleName() const { return m_str_sampleName; }
    inline const std::string& StrSampleID() const { return m_str_sampleID; }
    inline const std::string& StrNominalName() const { return m_str_nomName; }
    inline const std::string& StrSysName() const { return m_str_sysName; }
    inline const std::string& WeightsConfigList() const { return m_str_wgt_configList; }
    inline const std::string& WeightFlags() const { return m_str_wgt_flags; }
    inline const std::string& WeightVetoFlags() const { return m_str_wgt_vetoFlags; }
    inline const std::string& StrAnaType() const { return m_str_anaType; }

    inline SampleName::sample SampleName() const { return m_sampleName; }
    inline SysName::sys SysName() const { return m_sysName; }
    inline AnaType::ana AnaType() const { return m_anaType; }
    inline LeptonChannel::lepch LeptonChannel() const { return m_leptonChannel; }
    
    inline const std::string& InputTree() const { return m_inputTree; }
    inline const std::string& OutputTree() const { return m_outputTree; }
    
    inline const std::string& InputFile() const { return m_inputFile; }
    inline const std::string& OutputFile() const { return m_outputFile; }
    
    inline const std::string& OutputFolder() const { return m_outputFolder; }
    
    inline int SkipEvents() const { return m_skipEvents; }
    inline int NEvents() const { return m_nEvents; }
    inline int PickEvent() const { return m_pickEvent; }
    
protected:
    virtual bool IdentifyOption ( const std::string &, const std::string & );
    
protected:
    int m_argc;
    char** m_argv;
    std::string m_optStr;    
    
    Debug::debug m_msgLevel;
    bool m_isData,m_computeWeightSys,m_textFileList,m_onlyDumpSystHistograms, m_doBlind, m_add_underflow, m_add_overflow;
    SampleName::sample m_sampleName;
    SysName::sys m_sysName;
    AnaType::ana m_anaType;
    LeptonChannel::lepch m_leptonChannel;

    std::string m_str_sampleName,m_str_sampleID,m_str_nomName,m_str_sysName
      ,m_str_wgt_configList,m_str_wgt_flags, m_str_wgt_vetoFlags
      ,m_str_anaType,m_str_leptonChannel;

    std::string m_inputTree,m_outputTree,m_inputFile,m_outputFile,m_outputFolder;
    int m_skipEvents, m_nEvents, m_pickEvent;
    
};

#endif  // OPTIONSBASE
