//Standard includes
#include <iostream>
#include <string>

//IFAETopFrameowrk includes
#include "IFAETopFramework/OutputClass.h"
#include "IFAETopFramework/NtupleData.h"
#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/HistManager.h"

//ROOT libraries
#include "TString.h"

//_________________________________________________________________
//
OutputClass::OutputClass( OptionsBase* opt, OutputType type ):
m_opt(opt),
m_type(type),
m_stdh1Def(0),
m_histMngr(0),
m_sysVector(0),
m_data(0),
m_mapHasSyst(0)
{
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Entering in OutputClass constructor" << std::endl;
    
    m_stdh1Def = new StdHistos();
    m_histMngr = new HistManager();
    m_mapHasSyst = new std::map <TString,bool>();
    m_mapHasSyst->clear();
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputClass constructor" << std::endl;
}

//_________________________________________________________________
//
OutputClass::OutputClass( const OutputClass &q ){
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "In OutputClass copy-constructor" << std::endl;
    
    m_opt = q.m_opt;
    m_type = q.m_type;
    m_stdh1Def = q.m_stdh1Def;
    m_histMngr = q.m_histMngr;
    m_sysVector = q.m_sysVector;
    m_data = q.m_data;
    m_mapHasSyst = q.m_mapHasSyst;
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputClass copy-constructor" << std::endl;
}

//_________________________________________________________________
//
OutputClass::~OutputClass()
{
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "In OutputClass destructor" << std::endl;
}

//_________________________________________________________________
//
bool OutputClass::addStandardTH1( const TString &name, const double width, const double min,
                                  const double max, const TString &title,const TString &variableType,
                                  const bool hasSyst, const int component ){
    
    if(m_opt -> msgLevel() == Debug::DEBUG){
        std::cout << "In OutputClass::addStandardTH1" << std::endl;
        std::cout << "Adding variable: "<< name << std::endl;
        std::cout << "  width  = " << width << std::endl;
        std::cout << "  min    = " << min << std::endl;
        std::cout << "  max    = " << max << std::endl;
        std::cout << "  hasSyst= " << hasSyst << std::endl;
        std::cout << "  title  = " << title << std::endl;
        std::cout << "  type   = " << variableType << std::endl;
        std::cout << "  component   = " << component << std::endl;
    }
    
    h1Def *hist = new h1Def();
    hist -> name = name;
    hist -> width = width;
    hist -> min = min;
    hist -> max = max;
    hist -> title = title;
    hist -> hasSyst = hasSyst;
    hist -> component = component;
    if(variableType=="I") hist -> varType = VariableType::INT;
    else if(variableType=="D") hist -> varType = VariableType::DOUBLE;
    else if(variableType=="F") hist -> varType = VariableType::FLOAT;
    else if(variableType=="VI") hist -> varType = VariableType::VECINT;
    else if(variableType=="VF") hist -> varType = VariableType::VECFLOAT;
    else if(variableType=="VD") hist -> varType = VariableType::VECDOUBLE;
    else { std::cerr << "<!> In OutputClass::addStandardTH1: Variable type is not recognised ... : "<<variableType << std::endl; }
    
    m_stdh1Def -> insert( std::pair < TString, h1Def* >( name, hist ) );
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputClass::addStandardTH1" << std::endl;
    
    return true;
}

//_________________________________________________________________
//
bool OutputClass::bookStandardTH1( const TString &pattern, const bool hasSyst){
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "In OutputClass::bookStandardTH1" << std::endl;
    
    m_mapHasSyst -> insert( std::pair <TString, bool>(pattern,hasSyst));
        
    for ( std::map< TString, h1Def* >::iterator it = m_stdh1Def->begin(); it != m_stdh1Def->end(); ++it){

        TString histName = pattern;
        histName += "_";
        histName += it->second->name;
        m_histMngr -> BookTH1D((std::string)histName,
                               (std::string)it->second->title,
                                it->second->width,
                                it->second->min,
                                it->second->max);

        if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << histName << std::endl;
        
        if(hasSyst && it->second->hasSyst){
            if(!m_sysVector){
                std::cerr << "<!> ERROR in OutputClass::bookStandardTH1: You want to use systematics, but none is defined ... Please check !" << std::endl;
            } else {
                for (unsigned int iSys = 0; iSys < m_sysVector->size(); ++iSys) {
                    TString systHistName = histName;
                    systHistName += "_";
                    systHistName += m_sysVector->at(iSys)->name;
                    m_histMngr -> BookTH1D((std::string)systHistName,
                                           (std::string)it->second->title,
                                           it->second->width,
                                           it->second->min,
                                           it->second->max);
                                           
                    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << systHistName << std::endl;
                }
            }
        }
    }
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputClass::bookStandardTH1" << std::endl;
    
    return true;
}

//________________________________________________________________________________________
//
bool OutputClass::fillStandardTH1( const TString &pattern ){
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Entering in OutputClass::fillStandardTH1("<<pattern<<")" << std::endl;

    if(!m_data){
        std::cerr << "<!> ERROR in OutputClass::fillStandardTH1: We have big problems ... Please provide a NtupleData object" << std::endl;
        return false;
    }
    
    for ( std::map< TString, h1Def* >::iterator it = m_stdh1Def->begin(); it != m_stdh1Def->end(); ++it){
        
        //Nominal histogram filling
        TString histName = pattern;
        histName += "_";
        histName += it->second->name;
        
        if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Before filling histogram : " << histName << std::endl;
        
        if(it->second->varType==INT && it->second->value) m_histMngr -> FillTH1D((std::string)histName, *((int*)(it->second->value)), m_data->finalEvent_weightNom);
        else if(it->second->varType==DOUBLE && it->second->value) m_histMngr -> FillTH1D((std::string)histName, *((double*)(it->second->value)), m_data->finalEvent_weightNom);
        else if(it->second->varType==FLOAT && it->second->value) m_histMngr -> FillTH1D((std::string)histName, *((float*)(it->second->value)), m_data->finalEvent_weightNom);
        else if(it->second->varType==VECFLOAT && it->second->value){
            if(((std::vector<float>*)(it->second->value))->size() > it->second->component){
                m_histMngr -> FillTH1D((std::string)histName, ((std::vector<float>*)(it->second->value))->at(it->second->component), m_data->finalEvent_weightNom);
            }
        } else if(it->second->varType==VECINT && it->second->value){
            if(((std::vector<int>*)(it->second->value))->size() > it->second->component){
                m_histMngr -> FillTH1D((std::string)histName, ((std::vector<int>*)(it->second->value))->at(it->second->component), m_data->finalEvent_weightNom);
            }
        } else if(it->second->varType==VECDOUBLE && it->second->value){
            if(((std::vector<double>*)(it->second->value))->size() > it->second->component){
                m_histMngr -> FillTH1D((std::string)histName, ((std::vector<double>*)(it->second->value))->at(it->second->component), m_data->finalEvent_weightNom);
            }
        }
        
        if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Filled histogram : " << histName << std::endl;
        
        //Now checking systematics (if needed and if existing)
        if(it->second->hasSyst && m_mapHasSyst->at(pattern)){
            if(!m_sysVector){
                std::cerr << "<!> ERROR in OutputClass::bookStandardTH1: You want to use systematics, but none is defined ... Please check !" << std::endl;
            } else {
                for (unsigned int iSys = 0; iSys < m_sysVector->size(); ++iSys) {
                    TString systHistName = histName;
                    systHistName += "_";
                    systHistName += m_sysVector->at(iSys)->name;
                    
                    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Before filling histogram : " << systHistName << std::endl;
 
                    if(it->second->varType==INT && it->second->value) m_histMngr -> FillTH1D((std::string)histName, *((int*)(it->second->value)), m_sysVector->at(iSys)->value);
                    else if(it->second->varType==DOUBLE && it->second->value) m_histMngr -> FillTH1D((std::string)histName, *((double*)(it->second->value)), m_sysVector->at(iSys)->value);
                    else if(it->second->varType==FLOAT && it->second->value) m_histMngr -> FillTH1D((std::string)histName, *((float*)(it->second->value)), m_sysVector->at(iSys)->value);
                    else if(it->second->varType==VECFLOAT && it->second->value){
                        if(((std::vector<float>*)(it->second->value))->size() > it->second->component){
                            m_histMngr -> FillTH1D((std::string)histName, ((std::vector<float>*)(it->second->value))->at(it->second->component), m_sysVector->at(iSys)->value);
                        }
                    } else if(it->second->varType==VECINT && it->second->value){
                        if(((std::vector<int>*)(it->second->value))->size() > it->second->component){
                            m_histMngr -> FillTH1D((std::string)histName, ((std::vector<int>*)(it->second->value))->at(it->second->component), m_sysVector->at(iSys)->value);
                        }
                    } else if(it->second->varType==VECDOUBLE && it->second->value){
                        if(((std::vector<double>*)(it->second->value))->size() > it->second->component){
                            m_histMngr -> FillTH1D((std::string)histName, ((std::vector<double>*)(it->second->value))->at(it->second->component), m_sysVector->at(iSys)->value);
                        }
                    }
                   
                    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << systHistName << std::endl;
                }
            }
        }
    }
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputClass::fillStandardTH1("<<pattern<<")" << std::endl;
    
    return true;
}

//________________________________________________________________________________________
//
bool OutputClass::saveStandardTH1( const TString &outputName ){
    
    TFile *f = new TFile(outputName,"recreate");
    
    //Storing TH1 in the output file
    vector<string> h1list = m_histMngr->GetTH1KeyList();
    for(vector<string>::iterator hit = h1list.begin(); hit != h1list.end(); hit++){
        m_histMngr->FinaliseTH1Bins(*hit);
        f->cd();
        m_histMngr->GetTH1D(*hit)->Write();
        m_histMngr->ClearTH1(*hit);
    }
    f -> Close();
    
    return true;
}




