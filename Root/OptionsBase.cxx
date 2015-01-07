#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "IFAETopFramework/OptionsBase.h"


//_____________________________________________________________________________________________
//
OptionsBase::OptionsBase():
m_msgLevel(Debug::STANDARD),

m_isData(false),
m_computeWeightSys(false),//do you want to run the weight systematics
m_textFileList(false),//tells if you use a text file or a string line to define the input files (nature of m_inputFile)
m_useTRF(false),

m_sampleName(SampleName::TTH),//enum
m_sysName(SysName::NOMINAL),//enum
m_anaType(AnaType::HSG8),//enum

m_str_sampleName(""),//used to define the enum
m_str_sysName(""),//used to define dthe enum (object systematics) or the list of weight syst. to run
m_str_anaType(""),
m_inputTree(""),
m_outputTree(""),
m_inputFile(""),
m_outputFile(""),
m_outputFolder("")
{}

//_____________________________________________________________________________________________
//
OptionsBase::OptionsBase( const OptionsBase& q )
{
    m_msgLevel  = q.m_msgLevel;
    
    m_isData            = q.m_isData;
    m_computeWeightSys  = q.m_computeWeightSys;
    m_textFileList      = q.m_textFileList;
    m_useTRF            = q.m_useTRF;
    
    m_sampleName        = q.m_sampleName;
    m_sysName           = q.m_sysName;
    
    m_str_sampleName    = q.m_str_sampleName;
    m_str_sysName       = q.m_str_sysName;
    
    m_inputTree         = q.m_inputTree;
    m_outputTree        = q.m_outputTree;
    
    m_inputFile         = q.m_inputFile;
    m_outputFile        = q.m_outputFile;
    
    m_outputFolder      = q.m_outputFolder;
    
}

//_____________________________________________________________________________________________
//
OptionsBase::~OptionsBase()
{}

//_____________________________________________________________________________________________
//
void OptionsBase::parseUserOpts(int argc, char** argv){
    
    m_argc = argc;
    m_argv = argv;
    
    for (int i=0; i< argc; i++){// the 0 component is the name of the executable
        
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
        if( argument.find("--MSGLEVEL") != std::string::npos ){
            std::transform(value.begin(), value.end(), value.begin(), toupper);
            if ( value.find("SILENT") != std::string::npos) 	m_msgLevel = Debug::SILENT;
            else if ( value.find("STANDARD") != std::string::npos) 	m_msgLevel = Debug::STANDARD;
            else if ( value.find("VERBOSE") != std::string::npos) 	m_msgLevel = Debug::VERBOSE;
            else if ( value.find("DEBUG") != std::string::npos)      m_msgLevel = Debug::DEBUG;
            else{std::cout<<"Unknown debug option"<<std::endl;}
        }
        else if( argument.find("--ISDATA") != std::string::npos ){
            std::transform(value.begin(), value.end(), value.begin(), toupper);
            if ( value.find("TRUE") != std::string::npos) 	m_isData = true;
            else if ( value.find("FALSE") != std::string::npos) 	m_isData = false;
            else{std::cout<<"Unknown ISDATA option"<<std::endl;}
        }
        else if( argument.find("--COMPUTEWEIGHTSYS") != std::string::npos ){
            std::transform(value.begin(), value.end(), value.begin(), toupper);
            if ( value.find("TRUE") != std::string::npos) 	m_computeWeightSys = true;
            else if ( value.find("FALSE") != std::string::npos) 	m_computeWeightSys = false;
            else{std::cout<<"Unknown COMPUTEWEIGHTSYS option"<<std::endl;}
        }
        else if( argument.find("--TEXTFILELIST") != std::string::npos ){
            std::transform(value.begin(), value.end(), value.begin(), toupper);
            if ( value.find("TRUE") != std::string::npos) 	m_textFileList = true;
            else if ( value.find("FALSE") != std::string::npos) 	m_textFileList = false;
            else{std::cout<<"Unknown TEXTFILELIST option"<<std::endl;}
        }
        else if( argument.find("--USETRF") != std::string::npos ){
            std::transform(value.begin(), value.end(), value.begin(), toupper);
            if ( value.find("TRUE") != std::string::npos) 	m_useTRF = true;
            else if ( value.find("FALSE") != std::string::npos) 	m_useTRF = false;
            else{std::cout<<"Unknown USETRF option"<<std::endl;}
        }
        else if( argument.find("--SAMPLENAME") != std::string::npos ){
            std::transform(value.begin(), value.end(), value.begin(), toupper);
            //DATA
            if ( value.find("DATA") != std::string::npos) 	m_sampleName = SampleName::DATA;
            //BACKAGROUNDS
            else if ( value.find("TTBAR") != std::string::npos) m_sampleName = SampleName::TTBAR;
            else if ( value.find("TTBARBB") != std::string::npos) m_sampleName = SampleName::TTBARBB;
            else if ( value.find("TTBARCC") != std::string::npos) m_sampleName = SampleName::TTBARCC;
            else if ( value.find("TTBARLIGHT") != std::string::npos) m_sampleName = SampleName::TTBARLIGHT;
            else if ( value.find("DIBOSONS") != std::string::npos) m_sampleName = SampleName::DIBOSONS;
            else if ( value.find("SINGLETOP") != std::string::npos) m_sampleName = SampleName::SINGLETOP;
            else if ( value.find("WJETS") != std::string::npos) m_sampleName = SampleName::WJETS;
            else if ( value.find("ZJETS") != std::string::npos) m_sampleName = SampleName::ZJETS;
            else if ( value.find("TTH") != std::string::npos) 	m_sampleName = SampleName::TTH;
            else if ( value.find("TTZ") != std::string::npos) 	m_sampleName = SampleName::TTZ;
            else if ( value.find("TTW") != std::string::npos) 	m_sampleName = SampleName::TTW;
            else if ( value.find("TTV") != std::string::npos) m_sampleName = SampleName::TTV;
            else if ( value.find("QCD") != std::string::npos) m_sampleName = SampleName::QCD;
            //SIGNALS
            else if ( value.find("VLQ") != std::string::npos) m_sampleName = SampleName::VLQ;
            else if ( value.find("GUINO") != std::string::npos) m_sampleName = SampleName::GLUINO;
            else{std::cout<<"Unknown sample name"<<std::endl;}
            m_str_sampleName = value;
        }
        else if( argument.find("--SYSNAME") != std::string::npos ){
            std::transform(value.begin(), value.end(), value.begin(), toupper);
            if ( value.find("NOMINAL") != std::string::npos) 	m_sysName = SysName::NOMINAL;
            else { std::cout<<"Unknown systematics name"<<std::endl; }
            m_str_sysName = value;
        }
        else if( argument.find("--ANATYPE") != std::string::npos ){
            std::transform(value.begin(), value.end(), value.begin(), toupper);
            if ( value.find("HSG8") != std::string::npos) m_anaType = AnaType::HSG8;
            else if ( value.find("SUSY") != std::string::npos) m_anaType = AnaType::SUSY;
            else if ( value.find("VLQ") != std::string::npos) m_anaType = AnaType::VLQ;
            else{ std::cout<<"Unknown analysis type"<<std::endl; }
            m_str_anaType = value;
        }
        else if( argument.find("--INPUTTREE") != std::string::npos ){
            m_inputTree = value;
        }
        else if( argument.find("--OUTPUTTREE") != std::string::npos ){
            m_outputTree = value;
        }
        else if( argument.find("--INPUTFILE") != std::string::npos ){
            m_inputFile = value;
        }
        else if( argument.find("--OUTPUTFILE") != std::string::npos ){
            m_outputFile = value;
        }
        else if( argument.find("--OUTPUTFOLDER") != std::string::npos ){
            m_outputFolder = value;
        }
        else {
            std::cerr << "<!> The argument '" << argument << "' is not known ... Please check." << std::endl;
        }
    }
    
    //
    //If necessary, prints out the value of the data members
    //
    if(m_msgLevel>Debug::SILENT) this -> printOptions();
    
}

//_____________________________________________________________________________________________
//
void OptionsBase::printOptions()
{
    std::cout << "============== OptionsBase =================" << std::endl;
    std::cout << " m_msgLevel           = " << m_msgLevel << std::endl;
    
    std::cout << " m_isData             = " << m_isData << std::endl;
    std::cout << " m_computeWeightSys   = " << m_computeWeightSys << std::endl;
    std::cout << " m_textFileList       = " << m_textFileList << std::endl;
    std::cout << " m_useTRF             = " << m_useTRF << std::endl;
    
    std::cout << " m_sampleName         = " << m_sampleName << std::endl;
    std::cout << " m_str_sampleName     = " << m_str_sampleName << std::endl;
    std::cout << " m_sysName            = " << m_sysName << std::endl;
    std::cout << " m_str_sysName        = " << m_str_sysName << std::endl;
    std::cout << " m_str_anaType        = " << m_str_anaType << std::endl;
    std::cout << " m_inputTree          = " << m_inputTree << std::endl;
    std::cout << " m_outputTree         = " << m_outputTree << std::endl;
    std::cout << " m_inputFile          = " << m_inputFile << std::endl;
    std::cout << " m_outputFile         = " << m_outputFile << std::endl;
    std::cout << " m_outputFolder       = " << m_outputFolder << std::endl;
    std::cout << "===========================================" << std::endl;
}
