#include <fstream>

#include "IFAETopFramework/OptionsBase.h"

#include "IFAETopFramework/SampleInfo.h"

#include "IFAETopFramework/AnalysisUtils.h"

//___________________________________________________________
//
SampleInfo::SampleInfo(  ):
  m_opt(NULL),
  m_nWeightedEvents(-1.),
  m_crossSection(-1.),
  m_sampleName(""),
  m_ready(false)
{}


SampleInfo::SampleInfo( const OptionsBase *opt ):
  SampleInfo()
{ m_opt = opt; }


//___________________________________________________________
//
SampleInfo::SampleInfo( const std::string& sampleID, const std::string &configFile ):
  SampleInfo() 
{ ReadSample(sampleID, configFile); }


SampleInfo::SampleInfo( const OptionsBase *opt, const std::string &configFile ):
  SampleInfo(opt -> StrSampleID(), configFile)
{ m_opt = opt; }


void SampleInfo::ReadSample( const std::string& sampleID, const std::string &configFile ){
  
  m_nWeightedEvents = -1.;
  m_crossSection = -1.;
  m_sampleName = "";
  m_ready = false;

  //
  // Reads an input config file following the structure:
  //    <sample ID> <number of weighted events> <cross-section in pb-1> <sample name>
  //    
  std::ifstream infile(configFile);
  if(!infile){
    std::cout << "<!> ERROR in SampleInfo::SampleInfo(): Cannot open the config file: " << configFile << std::endl;
    return;
  }
  std::string fLine="",paramString="",param="",dsid="",nEvents="",xSec="",sampleName="";
  int nparam = 0; std::string::size_type pos = 0;

  while( getline(infile, fLine) ){
    AnalysisUtils::TrimString(fLine);
    if( fLine.empty() || (fLine.find(sampleID) == std::string::npos) ) continue;
    paramString = fLine;
    do{
      pos = AnalysisUtils::ParseString(paramString, param, " ");
      AnalysisUtils::TrimString(param); if(param.empty()) continue;
      if( nparam == 0 ){
	if(param.find(dsid) == std::string::npos) { std::cout<< "Warning :: Line << "<<fLine<<" does not begin with the sample ID"<<std::endl; }
      }
      else if(nparam == 1) m_nWeightedEvents = atof(param.c_str()); //nEvents = param;
      else if(nparam == 2) m_crossSection = atof(param.c_str()); //xSec = param;
      else if(nparam == 3) m_sampleName = param;
      else{ 
	std::cout<<" Too many parameters on line "<<fLine<<std::endl;
	break;
      }
      nparam++;
    }while( (!paramString.empty()) && (pos != std::string::npos) );//read all given parameters

    if( (nparam < 3) || (nparam > 4) ){std::cout<<" ERROR : "<<nparam<<" tokens in line : "<<fLine<<std::endl;}
    else{ m_ready = true; }
    break;
  } 

  /*
    while ( infile >> dsid >> nEvents >> xSec >> sampleName ) {
    if( dsid == sampleID ){
    m_nWeightedEvents = atof(nEvents.c_str());
    m_crossSection = atof(xSec.c_str());
    m_sampleName = sampleName;
    m_ready = true;
    break;
    }
    }
  */
}

//___________________________________________________________
//

//___________________________________________________________
//
SampleInfo::SampleInfo( const SampleInfo &q )
{
    m_opt             = q.m_opt;
    m_nWeightedEvents = q.m_nWeightedEvents;
    m_crossSection    = q.m_crossSection;
    m_sampleName      = q.m_sampleName;
    m_ready           = q.m_ready;
}

//___________________________________________________________
//
SampleInfo::~SampleInfo()
{}
