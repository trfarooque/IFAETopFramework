#include <fstream>

#include "IFAETopFramework/OptionsBase.h"

#include "IFAETopFramework/SampleInfo.h"


//___________________________________________________________
//
SampleInfo::SampleInfo( OptionsBase *opt ):
m_opt(opt),
m_nWeightedEvents(-1.),
m_crossSection(-1.),
m_ready(false)
{}

//___________________________________________________________
//
SampleInfo::SampleInfo( OptionsBase *opt, const std::string &configFile ):
m_opt(opt),
m_nWeightedEvents(-1.),
m_crossSection(-1.),
m_ready(false)
{
    //
    // Reads an input config file following the structure:
    //    <sample name> <number of weighted events> <cross-section in pb-1>
    //    
    std::ifstream infile(configFile);
    if(!infile){
        std::cout << "<!> ERROR in SampleInfo::SampleInfo(): Cannot open the config file: " << configFile << std::endl;
        return;
    }
    std::string dsid,nEvents,xSec;
    while ( infile >> dsid >> nEvents >> xSec ) {
        if( dsid == m_opt -> StrSampleID() ){
            m_nWeightedEvents = atof(nEvents.c_str());
            m_crossSection = atof(xSec.c_str());
            m_ready = true;
            break;
        }
    }
}

//___________________________________________________________
//
SampleInfo::SampleInfo( const SampleInfo &q )
{
    m_opt = q.m_opt;
    m_nWeightedEvents = q.m_nWeightedEvents;
    m_crossSection = q.m_crossSection;
    m_ready = q.m_ready;
}

//___________________________________________________________
//
SampleInfo::~SampleInfo()
{}