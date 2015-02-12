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
    void parseUserOpts(int argc, char** argv);
    void printOptions();
    
    //_____________________________________________________________
    // Data access and modification
    inline Debug::debug msgLevel() const { return m_msgLevel; }
    
    inline bool isData() const { return m_isData; }
    inline bool computeWeightSys() const { return m_computeWeightSys; }
    inline bool textFileList() const { return m_textFileList; }
    inline bool useTRF() const { return m_useTRF; }
    inline bool recomputeBtag() const { return m_recomputeBtag; }
    inline bool usePDFRW() const { return m_usePDFRW; }
    
    inline std::string sampleNameStr() const { return m_str_sampleName; }
    inline std::string sysNameStr() const { return m_str_sysName; }
    inline std::string anaTypeStr() const { return m_str_anaType; }

    inline SampleName::sample sampleName() const { return m_sampleName; }
    inline SysName::sys sysName() const { return m_sysName; }
    inline AnaType::ana anaType() const { return m_anaType; }
    inline LeptonChannel::lepch leptonChannel() const { return m_leptonChannel; }
    
    inline std::string inputTree() const { return m_inputTree; }
    inline std::string outputTree() const { return m_outputTree; }
    
    inline std::string inputFile() const { return m_inputFile; }
    inline std::string outputFile() const { return m_outputFile; }
    
    inline std::string outputFolder() const { return m_outputFolder; }
    
private:
    int m_argc;
    char** m_argv;
    
    Debug::debug m_msgLevel;
    bool m_isData,m_computeWeightSys,m_textFileList,m_useTRF,m_recomputeBtag,m_usePDFRW;
    SampleName::sample m_sampleName;
    SysName::sys m_sysName;
    AnaType::ana m_anaType;
    LeptonChannel::lepch m_leptonChannel;

    std::string m_str_sampleName,m_str_sysName,m_str_anaType,m_str_leptonChannel;
    std::string m_inputTree,m_outputTree,m_inputFile,m_outputFile,m_outputFolder;
    
};

#endif  // OPTIONSBASE
