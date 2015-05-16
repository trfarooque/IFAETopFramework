//Standard includes
#include <iostream>
#include <string>

//IFAETopFrameowrk includes
#include "IFAETopFramework/OutputManager.h"
#include "IFAETopFramework/OutputData.h"
#include "IFAETopFramework/HistManager.h"
#include "IFAETopFramework/TreeManager.h"

//ROOT libraries
#include "TString.h"
#include "TSystem.h"

//_________________________________________________________________
//
OutputManager::OutputManager( OptionsBase* opt, OutputType type ):
m_opt(opt),
m_type(type),
m_stdTH1Def(0),
m_stdTH2Def(0),
m_stdBranchDef(0),
m_histMngr(0),
m_treeMngr(0),
m_sysVector(0),
m_data(0),
m_mapHasSyst(0)
{
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Entering in OutputManager constructor" << std::endl;
    
    m_stdTH1Def = new StdTH1();
    m_stdTH2Def = new StdTH2();
    m_stdBranchDef = new StdBranches();
    m_histMngr = new HistManager();
    m_treeMngr = new TreeManager();
    m_mapHasSyst = new std::map <TString,bool>();
    m_mapHasSyst->clear();
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager constructor" << std::endl;
}

//_________________________________________________________________
//
OutputManager::OutputManager( const OutputManager &q ){
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "In OutputManager copy-constructor" << std::endl;
    
    m_opt = q.m_opt;
    m_type = q.m_type;
    m_stdTH1Def = q.m_stdTH1Def;
    m_stdTH2Def = q.m_stdTH2Def;
    m_stdBranchDef = q.m_stdBranchDef;
    m_histMngr = q.m_histMngr;
    m_treeMngr = q.m_treeMngr;
    m_sysVector = q.m_sysVector;
    m_data = q.m_data;
    m_mapHasSyst = q.m_mapHasSyst;
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager copy-constructor" << std::endl;
}

//_________________________________________________________________
//
OutputManager::~OutputManager()
{
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "In OutputManager destructor" << std::endl;
}

//-----------------------------TH1-SPECIFIC METHODS-------------------------------
//_________________________________________________________________
//
bool OutputManager::addStandardTH1(const TString name, const double width, const double min, const double max, const bool hasSyst){
    
    if(m_opt -> msgLevel() == Debug::DEBUG){
        std::cout << "In OutputManager::addStandardTH1" << std::endl;
        std::cout << "Adding variable: "<< name << std::endl;
        std::cout << "  width  = " << width << std::endl;
        std::cout << "  min    = " << min << std::endl;
        std::cout << "  max    = " << max << std::endl;
        std::cout << "  hasSyst= " << hasSyst << std::endl;
    }
    
    h1Def *hist = new h1Def();
    hist -> width = width;
    hist -> min = min;
    hist -> max = max;
    hist -> hasSyst = hasSyst;
    
    m_stdTH1Def -> insert( std::pair < TString, h1Def* >( name, hist ) );
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::addStandardTH1" << std::endl;
    
    return true;
}

//_________________________________________________________________
//
bool OutputManager::bookStandardTH1( const TString &pattern, const bool hasSyst){
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "In OutputManager::bookStandardTH1" << std::endl;
    
    m_mapHasSyst -> insert( std::pair <TString, bool>(pattern,hasSyst));
    
    for ( std::map< TString, h1Def* >::iterator it = m_stdTH1Def->begin(); it != m_stdTH1Def->end(); ++it){
        
        TString histName = pattern;
        histName += "_" ;
        
        histName += it->second->var.name();
        m_histMngr -> BookTH1D((std::string)histName,
                               (std::string)(it->second->var.title()),
                               it->second->width,
                               it->second->min,
                               it->second->max);
        
        if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << histName << std::endl;
        
        if(hasSyst && it->second->hasSyst){
            if(!m_sysVector){
                std::cerr << "<!> ERROR in OutputManager::bookStandardTH1: You want to use systematics, but none is defined ... Please check !" << std::endl;
            } else {
                for (unsigned int iSys = 0; iSys < m_sysVector->size(); ++iSys) {
                    TString systHistName = histName;
                    systHistName += "_";
                    systHistName += m_sysVector->at(iSys)->name;
                    m_histMngr -> BookTH1D((std::string)systHistName,
                                           (std::string)(it->second->var.title()),
                                           it->second->width,
                                           it->second->min,
                                           it->second->max);
                    
                    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << systHistName << std::endl;
                }
            }
        }
    }
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::bookStandardTH1" << std::endl;
    
    return true;
}

//________________________________________________________________________________________
//
bool OutputManager::fillStandardTH1( const TString &pattern ){
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Entering in OutputManager::fillStandardTH1("<<pattern<<")" << std::endl;
    
    if(!m_data){
        std::cerr << "<!> ERROR in OutputManager::fillStandardTH1: We have big problems ... Please provide an OutputData object" << std::endl;
        return false;
    }
    
    for ( std::map< TString, h1Def* >::iterator it = m_stdTH1Def->begin(); it != m_stdTH1Def->end(); ++it){
        
        //Nominal histogram filling
        TString histName = pattern;
        histName += "_";
        histName += it->second->var.name();
        
        if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Before filling histogram : " << histName << std::endl;
        
        m_histMngr -> FillTH1D((std::string)histName, it->second->var.GetDoubleValue(), m_data->finalEvent_weightNom);
        
        if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Filled histogram : " << histName << std::endl;
        
        //Now checking systematics (if needed and if existing)
        if(it->second->hasSyst && m_mapHasSyst->at(pattern)){
            if(!m_sysVector){
                std::cerr << "<!> ERROR in OutputManager::bookStandardTH1: You want to use systematics, but none is defined ... Please check !" << std::endl;
            } else {
                for (unsigned int iSys = 0; iSys < m_sysVector->size(); ++iSys) {
                    TString systHistName = histName;
                    systHistName += "_";
                    systHistName += m_sysVector->at(iSys)->name;
                    
                    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Before filling histogram : " << systHistName << std::endl;
                    m_histMngr -> FillTH1D((std::string)histName, it->second->var.GetDoubleValue(), m_sysVector->at(iSys)->value);
                    
                    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << systHistName << std::endl;
                }
            }
        }
    }
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::fillStandardTH1("<<pattern<<")" << std::endl;
    
    return true;
}

//________________________________________________________________________________________
//
bool OutputManager::saveStandardTH1( const TString &outputName ){
    
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

//-----------------------------TH2-SPECIFIC METHODS-------------------------------
//_________________________________________________________________
//
bool OutputManager::addStandardTH2( const TString name, const double widthX, const double minX, const double maxX,
                                   const double widthY, const double minY, const double maxY, const bool hasSyst){
    
    if(m_opt -> msgLevel() == Debug::DEBUG){
        std::cout << "In OutputManager::addStandardTH2" << std::endl;
        std::cout << "Adding variable: "<< name << std::endl;
        std::cout << "  widthX  = " << widthX << std::endl;
        std::cout << "  minX    = " << minX << std::endl;
        std::cout << "  maxX    = " << maxX << std::endl;
        std::cout << "  widthY  = " << widthY << std::endl;
        std::cout << "  minY    = " << minY << std::endl;
        std::cout << "  maxY    = " << maxY << std::endl;
        std::cout << "  hasSyst= " << hasSyst << std::endl;
        
    }
    
    h2Def *hist = new h2Def();
    hist -> widthX = widthX;
    hist -> minX = minX;
    hist -> maxX = maxX;
    hist -> widthY = widthY;
    hist -> minY = minY;
    hist -> maxY = maxY;
    hist -> hasSyst = hasSyst;
    
    m_stdTH2Def -> insert( std::pair < TString, h2Def* >( name, hist ) );
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::addStandardTH2" << std::endl;
    
    return true;
}

//_________________________________________________________________
//
bool OutputManager::bookStandardTH2( const TString &pattern, const bool hasSyst){
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "In OutputManager::bookStandardTH2" << std::endl;
    
    m_mapHasSyst -> insert( std::pair <TString, bool>(pattern,hasSyst));
    
    for ( std::map< TString, h2Def* >::iterator it = m_stdTH2Def->begin(); it != m_stdTH2Def->end(); ++it){
        
        TString histName = pattern;
        histName += "_";
        histName += it->second->varY.name();
        histName += "_vs_";
        histName += it->second->varX.name();
        TString histTitle = it->second->varY.title();
        histTitle += " vs ";
        histTitle += it->second->varX.title();
        
        m_histMngr -> BookTH2D((std::string)histName,
                               (std::string)histTitle,
                               it->second->widthX,
                               it->second->minX,
                               it->second->maxX,
                               it->second->widthY,
                               it->second->minY,
                               it->second->maxY);
        
        if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << histName << std::endl;
        
        if(hasSyst && it->second->hasSyst){
            if(!m_sysVector){
                std::cerr << "<!> ERROR in OutputManager::bookStandardTH2: You want to use systematics, but none is defined ... Please check !" << std::endl;
            } else {
                for (unsigned int iSys = 0; iSys < m_sysVector->size(); ++iSys) {
                    TString systHistName = histName;
                    systHistName += "_";
                    systHistName += m_sysVector->at(iSys)->name;
                    m_histMngr -> BookTH2D((std::string)systHistName,
                                           (std::string)histTitle,
                                           it->second->widthX,
                                           it->second->minX,
                                           it->second->maxX,
                                           it->second->widthY,
                                           it->second->minY,
                                           it->second->maxY);
                    
                    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << systHistName << std::endl;
                }
            }
        }
    }
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::bookStandardTH2" << std::endl;
    
    return true;
}

//________________________________________________________________________________________
//
bool OutputManager::fillStandardTH2( const TString &pattern ){
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Entering in OutputManager::fillStandardTH2("<<pattern<<")" << std::endl;
    
    if(!m_data){
        std::cerr << "<!> ERROR in OutputManager::fillStandardTH2: We have big problems ... Please provide an OutputData object" << std::endl;
        return false;
    }
    
    for ( std::map< TString, h2Def* >::iterator it = m_stdTH2Def->begin(); it != m_stdTH2Def->end(); ++it){
        
        //Nominal histogram filling
        TString histName = pattern;
        histName += "_";
        histName += it->second->varY.name();
        histName += "_vs_";
        histName += it->second->varX.name();
        
        if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Before filling histogram : " << histName << std::endl;
        
        m_histMngr -> FillTH2D((std::string)histName, it->second->varX.GetDoubleValue(), it->second->varY.GetDoubleValue(), m_data->finalEvent_weightNom);
        
        if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Filled histogram : " << histName << std::endl;
        
        //Now checking systematics (if needed and if existing)
        if(it->second->hasSyst && m_mapHasSyst->at(pattern)){
            if(!m_sysVector){
                std::cerr << "<!> ERROR in OutputManager::bookStandardTH2: You want to use systematics, but none is defined ... Please check !" << std::endl;
            } else {
                for (unsigned int iSys = 0; iSys < m_sysVector->size(); ++iSys) {
                    TString systHistName = histName;
                    systHistName += "_";
                    systHistName += m_sysVector->at(iSys)->name;
                    
                    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Before filling histogram : " << systHistName << std::endl;
                    m_histMngr -> FillTH2D((std::string)histName, it->second->varX.GetDoubleValue(), it->second->varY.GetDoubleValue(), m_sysVector->at(iSys)->value);
                    
                    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << systHistName << std::endl;
                }
            }
        }
    }
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::fillStandardTH2("<<pattern<<")" << std::endl;
    
    return true;
}

//________________________________________________________________________________________
//
bool OutputManager::saveStandardTH2( const TString &outputName ){
    
    TFile *f = new TFile(outputName,"recreate");
    
    //Storing TH2 in the output file
    vector<string> h2list = m_histMngr->GetTH2KeyList();
    for(vector<string>::iterator hit = h2list.begin(); hit != h2list.end(); hit++){
        f->cd();
        m_histMngr->GetTH2D(*hit)->Write();
        m_histMngr->ClearTH2(*hit);
    }
    f -> Close();
    
    return true;
}



//-----------------------------TREE-SPECIFIC METHODS-------------------------------
//_________________________________________________________________
//
bool OutputManager::bookStandardTree( const TString &pattern, const TString &title){
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "In OutputManager::bookStandardTree" << std::endl;
    
    //Book a tree with the given name
    m_treeMngr->BookTree((std::string)pattern, (std::string)title);
    
    //Loop over the list of standard branches and add those branches to the tree
    for ( std::map< TString, VariableDef* >::iterator it = m_stdBranchDef->begin(); it != m_stdBranchDef->end(); ++it){
        m_treeMngr->AddBranchToTree((std::string)pattern, *(it->second));
        if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Added branch : " << it->first << std::endl;
    }
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::bookStandardTree" << std::endl;
    
    return true;
}

//_________________________________________________________________
//
bool OutputManager::fillStandardTree( const TString &pattern ){
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "In OutputManager::fillStandardTrees" << std::endl;
    m_treeMngr->FillTree((std::string)pattern);
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::fillStandardTrees" << std::endl;
    return true;
}

//_________________________________________________________________
//
bool OutputManager::saveStandardTree( const TString &outputName ){
    
    TFile *f = new TFile(outputName,"recreate");
    vector<string> treeList = m_treeMngr->GetTreeKeyList();
    for(vector<string>::iterator it = treeList.begin(); it != treeList.end(); it++){
        f->cd();
        m_treeMngr->GetTree(*it)->Write();
    }
    f -> Close();
    return true;
}




