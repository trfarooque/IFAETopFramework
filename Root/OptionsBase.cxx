#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/AnalysisUtils.h"

//_____________________________________________________________________________________________
//
OptionsBase::OptionsBase():
m_optStr(""),
m_msgLevel(Debug::STANDARD),
m_isData(false),
m_computeWeightSys(false),//do you want to run the weight systematics
m_textFileList(false),//tells if you use a text file or a string line to define the input files (nature of m_inputFile)
m_onlyDumpSystHistograms(false),
m_doBlind(false),
m_add_underflow(true),
m_add_overflow(true),

m_sampleName(SampleName::TTH),//enum
m_sysName(SysName::NOMINAL),//enum
m_anaType(AnaType::HSG8),//enum
m_leptonChannel(LeptonChannel::ELEC),//enum

m_str_sampleName(""),//used to define the enum
m_str_sampleID("UNDEFINED"),
m_str_nomName(""),//used to define dthe enum (object systematics) or the list of weight syst. to run
m_str_sysName(""),//used to define dthe enum (object systematics) or the list of weight syst. to run
m_str_wgt_configList(""),
m_str_anaType(""),
m_str_leptonChannel(""),
m_inputTree(""),
m_outputTree(""),
m_inputFile(""),
m_outputFile(""),
m_outputFolder(""),

m_skipEvents(-1),
m_nEvents(-1),
m_pickEvent(-1)
{}

//_____________________________________________________________________________________________
//
OptionsBase::OptionsBase( const OptionsBase& q )
{
    m_optStr            = q.m_optStr;

    m_msgLevel          = q.m_msgLevel;

    m_isData                    = q.m_isData;
    m_computeWeightSys          = q.m_computeWeightSys;
    m_textFileList              = q.m_textFileList;
    m_onlyDumpSystHistograms    = q.m_onlyDumpSystHistograms;
    m_doBlind                   = q.m_doBlind;
    m_add_underflow             = q.m_add_underflow;
    m_add_overflow              = q.m_add_overflow;

    m_sampleName        = q.m_sampleName;
    m_sysName           = q.m_sysName;
    m_anaType           = q.m_anaType;
    m_leptonChannel     = q.m_leptonChannel;

    m_str_sampleName     = q.m_str_sampleName;
    m_str_sampleID       = q.m_str_sampleID;
    m_str_nomName        = q.m_str_nomName;
    m_str_sysName        = q.m_str_sysName;
    m_str_wgt_configList = q.m_str_wgt_configList;
    m_str_anaType        = q.m_str_anaType;
    m_str_leptonChannel  = q.m_str_leptonChannel;

    m_inputTree         = q.m_inputTree;
    m_outputTree        = q.m_outputTree;

    m_inputFile         = q.m_inputFile;
    m_outputFile        = q.m_outputFile;

    m_outputFolder      = q.m_outputFolder;

    m_skipEvents        = q.m_skipEvents;
    m_nEvents           = q.m_nEvents;
    m_pickEvent         = q.m_pickEvent;
}

//_____________________________________________________________________________________________
//
OptionsBase::~OptionsBase()
{}

//_____________________________________________________________________________________________
//
void OptionsBase::ParseUserOpts(int argc, char** argv){

    m_argc = argc;
    m_argv = argv;

    for (int i=1; i < argc; i++){// the 0th component is the name of the executable

        //
        //Converting the argument to string format
        //
        std::string opt(argv[i]);
        std::string argument,value;
        size_t pos = opt.find("=");
        if(pos == std::string::npos){//the sign = is not found, skip the argument with a warning message
            std::cout << "<!> In OptionsBase::parseUserOpts, the argument has no '=' sign, skip it : " << opt << std::endl;
            continue;
        }
        argument = opt.substr(0, pos);
        std::transform(argument.begin(), argument.end(), argument.begin(), toupper);//converts to upper letters to avoid case problems
        value=opt.erase(0, pos + 1);

        //
        //Search for know argument names
        //
        if(!IdentifyOption(argument, value)){
            std::cout << "<!> Problem identifying the option : " << argument << "   -> value is " << value << std::endl;
        }
        else{ m_optStr += argument; }
    }

    //
    //If necessary, prints out the value of the data members
    //
    if(m_msgLevel>Debug::SILENT) this -> PrintOptions();

}

//_____________________________________________________________________________________________
//
bool OptionsBase::IdentifyOption ( const std::string &argument, const std::string &value )
{
    std::string temp_arg = argument;
    std::string temp_val = value;

    if( temp_arg.find("--MSGLEVEL") != std::string::npos ){
        std::transform(temp_val.begin(), temp_val.end(), temp_val.begin(), toupper);
        if ( temp_val.find("SILENT") != std::string::npos) 	m_msgLevel = Debug::SILENT;
        else if ( temp_val.find("STANDARD") != std::string::npos) 	m_msgLevel = Debug::STANDARD;
        else if ( temp_val.find("VERBOSE") != std::string::npos) 	m_msgLevel = Debug::VERBOSE;
        else if ( temp_val.find("DEBUG") != std::string::npos)      m_msgLevel = Debug::DEBUG;
        else{std::cout<<"Unknown debug option"<<std::endl;}
    }
    else if( temp_arg.find("--ISDATA") != std::string::npos ){
      m_isData = AnalysisUtils::BoolValue(temp_val, "ISDATA");
    }
    else if( temp_arg.find("--COMPUTEWEIGHTSYS") != std::string::npos ){
      m_computeWeightSys = AnalysisUtils::BoolValue(temp_val, "COMPUTEWEIGHTSYS");
    }
    else if( temp_arg.find("--ONLYDUMPSYSTHISTOGRAMS") != std::string::npos ){
      m_onlyDumpSystHistograms = AnalysisUtils::BoolValue(temp_val, "ONLYDUMPSYSTHISTOGRAMS");
    }

    else if( temp_arg.find("--DOBLIND") != std::string::npos ){
      m_doBlind = AnalysisUtils::BoolValue(temp_val, "DOBLIND");
    }

    else if( temp_arg.find("--ADDUNDERFLOW") != std::string::npos ){
      m_add_underflow = AnalysisUtils::BoolValue(temp_val, "ADDUNDERFLOW");
    }

    else if( temp_arg.find("--ADDOVERFLOW") != std::string::npos ){
      m_add_overflow = AnalysisUtils::BoolValue(temp_val, "ADDOVERFLOW");
    }

    else if( temp_arg.find("--TEXTFILELIST") != std::string::npos ){
      m_textFileList = AnalysisUtils::BoolValue(temp_val, "TEXTFILELIST");
    }
    else if( temp_arg.find("--SAMPLENAME") != std::string::npos ){
        std::transform(temp_val.begin(), temp_val.end(), temp_val.begin(), toupper);
        //DATA
        if ( temp_val.find("DATA") != std::string::npos){              m_sampleName = SampleName::DATA;        }
        //BACKGROUNDS
        else if ( temp_val.find("TTBARBB") != std::string::npos){      m_sampleName = SampleName::TTBARBB;     }
        else if ( temp_val.find("TTBARCC") != std::string::npos){      m_sampleName = SampleName::TTBARCC;     }
        else if ( temp_val.find("TTBARLIGHT") != std::string::npos){   m_sampleName = SampleName::TTBARLIGHT;  }
        else if ( temp_val.find("TTBAR") != std::string::npos){        m_sampleName = SampleName::TTBAR;       }
        else if ( temp_val.find("DIBOSONS") != std::string::npos){     m_sampleName = SampleName::DIBOSONS;    }
        else if ( temp_val.find("SINGLETOP") != std::string::npos){    m_sampleName = SampleName::SINGLETOP;   }
        else if ( temp_val.find("WJETS") != std::string::npos){        m_sampleName = SampleName::WJETS;       }
        else if ( temp_val.find("ZJETS") != std::string::npos){        m_sampleName = SampleName::ZJETS;       }
        else if ( temp_val.find("TTH") != std::string::npos){          m_sampleName = SampleName::TTH;         }
        else if ( temp_val.find("TTZ") != std::string::npos){          m_sampleName = SampleName::TTZ;         }
        else if ( temp_val.find("TTW") != std::string::npos){          m_sampleName = SampleName::TTW;         }
        else if ( temp_val.find("TTV") != std::string::npos){          m_sampleName = SampleName::TTV;         }
        else if ( temp_val.find("QCDMU") != std::string::npos){        m_sampleName = SampleName::QCDMU;       }
        else if ( temp_val.find("QCDE") != std::string::npos){         m_sampleName = SampleName::QCDE;        }
        else if ( temp_val.find("QCD") != std::string::npos){          m_sampleName = SampleName::QCD;         }
        //SIGNALS
        else if ( temp_val.find("VLQ") != std::string::npos){          m_sampleName = SampleName::VLQ;         }
        else if ( temp_val.find("GLUINO") != std::string::npos){       m_sampleName = SampleName::GLUINO;      }
        else if ( temp_val.find("SGLUON") != std::string::npos){       m_sampleName = SampleName::SGLUON;      }
        else if ( temp_val.find("HPLUS") != std::string::npos){        m_sampleName = SampleName::HPLUS;      }
        else if ((temp_val.find("UHBW") != std::string::npos) or
                 (temp_val.find("UHCW") != std::string::npos)) {       m_sampleName = SampleName::FCNC;       }
        else {
            std::cout<<"Unknown sample name '"<<temp_val<<"'"<<std::endl;
        }
        m_str_sampleName = temp_val;
    }
    else if( temp_arg.find("--SAMPLEID") != std::string::npos ){
      m_str_sampleID = temp_val;
    }
    else if( temp_arg.find("--NOMINALNAME") != std::string::npos ){
      std::transform(temp_val.begin(), temp_val.end(), temp_val.begin(), toupper);
      m_str_nomName = temp_val;
    }

    else if( temp_arg.find("--SYSNAME") != std::string::npos ){
      std::transform(temp_val.begin(), temp_val.end(), temp_val.begin(), toupper);
      m_str_sysName = temp_val;
    }

    else if( temp_arg.find("--WEIGHTCONFIGS") != std::string::npos ){
      m_str_wgt_configList = temp_val;
    }
    else if( temp_arg.find("--ANATYPE") != std::string::npos ){
      std::transform(temp_val.begin(), temp_val.end(), temp_val.begin(), toupper);
      if ( temp_val.find("HSG8") != std::string::npos) m_anaType = AnaType::HSG8;
      else if ( temp_val.find("SUSY") != std::string::npos) m_anaType = AnaType::SUSY;
      else if ( temp_val.find("VLQ") != std::string::npos) m_anaType = AnaType::VLQ;
      else{ std::cout<<"Unknown analysis type"<<std::endl; }
      m_str_anaType = temp_val;
    }
    else if( temp_arg.find("--LEPTONCHANNEL") != std::string::npos ){
      std::transform(temp_val.begin(), temp_val.end(), temp_val.begin(), toupper);
      if ( temp_val.find("ELECTRON") != std::string::npos) m_leptonChannel = LeptonChannel::ELEC;
      else if ( temp_val.find("MUON") != std::string::npos) m_leptonChannel = LeptonChannel::MUON;
      else{ std::cout<<"Unknown lepton type"<<std::endl; }
      m_str_leptonChannel = temp_val;
    }
    else if( temp_arg.find("--INPUTTREE") != std::string::npos ){
      m_inputTree = temp_val;
    }
    else if( temp_arg.find("--OUTPUTTREE") != std::string::npos ){
      m_outputTree = temp_val;
    }
    else if( temp_arg.find("--INPUTFILE") != std::string::npos ){
      m_inputFile = temp_val;
    }
    else if( temp_arg.find("--OUTPUTFILE") != std::string::npos ){
      m_outputFile = temp_val;
    }
    else if( temp_arg.find("--OUTPUTFOLDER") != std::string::npos ){
      m_outputFolder = temp_val;
    }
    else if( temp_arg.find("--NEVENTS") != std::string::npos ){
      m_nEvents = atoi(temp_val.c_str());
    }
    else if( temp_arg.find("--SKIPEVENTS") != std::string::npos ){
      m_skipEvents = atoi(temp_val.c_str());
    }
    else if( temp_arg.find("--PICKEVENT") != std::string::npos ){
      m_pickEvent = atoi(temp_val.c_str());
    }
    else {
      return false;
    }
    return true;
}

//_____________________________________________________________________________________________
//
void OptionsBase::PrintOptions()
{
    std::cout << "============== OptionsBase =================" << std::endl;
    std::cout << " m_msgLevel           = " << m_msgLevel << std::endl;

    std::cout << " m_isData             = "     << m_isData << std::endl;
    std::cout << " m_computeWeightSys   = "     << m_computeWeightSys << std::endl;
    std::cout << " m_textFileList       = "     << m_textFileList << std::endl;
    std::cout << " m_onlyDumpSystHistograms = " << m_onlyDumpSystHistograms << std::endl;
    std::cout << " m_doBlind            = "     << m_doBlind << std::endl;
    std::cout << " m_add_underflow      = "     << m_add_underflow << std::endl;
    std::cout << " m_add_overflow       = "     << m_add_overflow << std::endl;

    std::cout << " m_sampleName         = " << m_sampleName << std::endl;
    std::cout << " m_str_sampleName     = " << m_str_sampleName << std::endl;
    std::cout << " m_str_sampleID       = " << m_str_sampleID << std::endl;
    std::cout << " m_sysName            = " << m_sysName << std::endl;
    std::cout << " m_str_nomName        = " << m_str_nomName << std::endl;
    std::cout << " m_str_sysName        = " << m_str_sysName << std::endl;
    std::cout << " m_str_wgt_configList = " << m_str_wgt_configList << std::endl;
    std::cout << " m_anaType            = " << m_anaType << std::endl;
    std::cout << " m_str_anaType        = " << m_str_anaType << std::endl;
    std::cout << " m_leptonChannel      = " << m_leptonChannel << std::endl;
    std::cout << " m_str_leptonChannel  = " << m_str_leptonChannel << std::endl;

    std::cout << " m_inputTree          = " << m_inputTree << std::endl;
    std::cout << " m_outputTree         = " << m_outputTree << std::endl;
    std::cout << " m_inputFile          = " << m_inputFile << std::endl;
    std::cout << " m_outputFile         = " << m_outputFile << std::endl;
    std::cout << " m_outputFolder       = " << m_outputFolder << std::endl;

    std::cout << " m_skipEvents         = " << m_skipEvents << std::endl;
    std::cout << " m_nEvents            = " << m_nEvents << std::endl;
    std::cout << " m_pickEvent          = " << m_pickEvent << std::endl;
}
