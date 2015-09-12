#include <fstream>

#include "IFAETopFramework/OptionsBase.h"

#include "IFAETopFramework/SampleInfo.h"


//___________________________________________________________
//
SampleInfo::SampleInfo( OptionsBase *opt ):
m_opt(opt),
m_nWeightedEvents(-1),
m_crossSection(-1)
{}

//___________________________________________________________
//
SampleInfo::SampleInfo( OptionsBase *opt, const std::string &id, const std::string &configFile ):
m_opt(opt)
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
        if(dsid==id){
            m_nWeightedEvents = atof(nEvents.c_str());
            m_crossSection = atof(xSec.c_str());
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
}

//___________________________________________________________
//
SampleInfo::~SampleInfo()
{}