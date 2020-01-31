#include <fstream>

#include <IFAETopFramework/json.hpp>

#include "IFAETopFramework/OptionsBase.h"

#include "IFAETopFramework/SampleInfo.h"

#include "IFAETopFramework/AnalysisUtils.h"

#include <map>

using json = nlohmann::json;

//___________________________________________________________
//
SampleInfo::SampleInfo(  ):
  m_opt(NULL),
  m_nWeightedEvents(-1.),
  m_crossSection(-1.),
  m_sampleName(""),
  m_ready(false),
  m_systWeightFactorMap()
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
  m_systWeightFactorMap.clear();

  std::ifstream infile(configFile);
  if(!infile){
    std::cout << "<!> ERROR in SampleInfo::SampleInfo(): Cannot open the config file: " << configFile << std::endl;
    return;
  }

  json j;

  //
  // Reads a json formatting input config file following the structure:
  //    { <sample ID> : 
  //      { "nWeightedEvents" : <number of weighted events>, 
  //        "crossSection" : <cross-section in pb-1>,
  //        OPTIONAL: 
  //        "nWeightedEvents_<weight name> : <adjusted number of weighted events>
  //        ...
  //

  try {
    j = json::parse(infile);

    m_nWeightedEvents = j[sampleID]["nWeightedEvents"];
    m_crossSection    = j[sampleID]["crossSection"];

    // if( m_opt -> ComputeWeightSys() ){
      for (auto& [key,val] : j[sampleID].items() ){
        if( key.find("nWeightedEvents_") != std::string::npos ){
          std::string propname = key;
          propname.erase(0,16);
          propname.insert(0,"weight_pmg_");
          double factor = m_nWeightedEvents/(double)val;
          m_systWeightFactorMap.insert( {propname, factor} );
        }
      }
    // }
    m_ready = true;
  }

  //
  // Reads an input config file following the structure:
  //    <sample ID> <number of weighted events> <cross-section in pb-1> <sample name>
  //

  catch (json::parse_error& e) {
    std::cout << "Warning :: no proper json format configFile found in " << configFile << " - reverting to old configFile format..." << std::endl;

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
  }
}

//___________________________________________________________
//
SampleInfo::SampleInfo( const SampleInfo &q )
{
    m_opt             = q.m_opt;
    m_nWeightedEvents = q.m_nWeightedEvents;
    m_crossSection    = q.m_crossSection;
    m_sampleName      = q.m_sampleName;
    m_ready           = q.m_ready;
    m_systWeightFactorMap = q.m_systWeightFactorMap;
}

//___________________________________________________________
//
SampleInfo::~SampleInfo()
{}
