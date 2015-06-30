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

//______________________________________________________________________________________
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

//______________________________________________________________________________________
//
OutputManager::OutputManager( const OutputManager &q )
{
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

//______________________________________________________________________________________
//
OutputManager::~OutputManager()
{
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "In OutputManager destructor" << std::endl;
}

//-----------------------------TH1-SPECIFIC METHODS-------------------------------
//______________________________________________________________________________________
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

//______________________________________________________________________________________
//
bool OutputManager::bookStandardTH1( const TString &pattern, const bool hasSyst){
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "In OutputManager::bookStandardTH1" << std::endl;
    
    //
    // Check the mode is correct
    //
    if(m_type==TREES){
        std::cout << "<!> ERROR in OutputManager::bookStandardTH1(): histogram booking has been requested in TREES mode. Please check." << std::endl;
        return false;
    }
    
    m_mapHasSyst -> insert( std::pair <TString, bool>(pattern,hasSyst));
    
    //
    // Loop over the registered histograms (to be booked for nominal and weight systematics)
    //
    for ( const auto h1 : *m_stdTH1Def ){
        
        TString histName = pattern;
        histName += "_" ;
        histName += h1.second->var.name();
        
        m_histMngr -> BookTH1D( (std::string)histName, (std::string)(h1.second->var.title()), h1.second->width, h1.second->min, h1.second->max);
        
        if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << histName << std::endl;
        
        if(hasSyst && h1.second->hasSyst){
            if(!m_sysVector){
                std::cerr << "<!> ERROR in OutputManager::bookStandardTH1: You want to use systematics, but none is defined ... Please check !" << std::endl;
            } else {
                for (const auto &sys : *m_sysVector) {
                    TString systHistName = histName;
                    systHistName += "_";
                    systHistName += sys.second->name();
                    m_histMngr -> BookTH1D((std::string)systHistName, (std::string)(h1.second->var.title()), h1.second->width, h1.second->min, h1.second->max);
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
    
    //
    // Check the mode is correct
    //
    if(m_type==TREES){
        std::cout << "<!> ERROR in OutputManager::fillStandardTH1(): histogram booking has been requested in TREES mode. Please check." << std::endl;
        return false;
    }
    
    if(!m_data){
        std::cerr << "<!> ERROR in OutputManager::fillStandardTH1: We have big problems ... Please provide an OutputData object" << std::endl;
        return false;
    }
    
    for ( const auto h1 : *m_stdTH1Def ){
        
        //
        // Nominal histogram filling
        //
        TString histName = pattern;
        histName += "_";
        histName += h1.second->var.name();
        m_histMngr -> FillTH1D((std::string)histName, h1.second->var.GetDoubleValue(), m_data->o_eventWeight_Nom);
        if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Filled histogram : " << histName << std::endl;
        
        //
        // Now checking systematics (if needed and if existing)
        //
        if(h1.second->hasSyst && m_mapHasSyst->at(pattern)){
            if(!m_sysVector){
                std::cerr << "<!> ERROR in OutputManager::bookStandardTH1: You want to use systematics, but none is defined ... Please check !" << std::endl;
            } else {
                for (const auto sys : *m_sysVector) {
                    TString systHistName = histName;
                    systHistName += "_";
                    systHistName += sys.second->name();
                    m_histMngr -> FillTH1D((std::string)systHistName, h1.second->var.GetDoubleValue(), sys.second->GetDoubleValue());
                    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Filled histogram : " << systHistName << std::endl;
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
    for( const auto it_h1 : h1list ){
        m_histMngr->FinaliseTH1Bins(it_h1);
        f->cd();
        m_histMngr->GetTH1D(it_h1)->Write();
        m_histMngr->ClearTH1(it_h1);
    }
    f -> Close();
    return true;
}

//-----------------------------TH2-SPECIFIC METHODS-------------------------------
//______________________________________________________________________________________
//
bool OutputManager::addStandardTH2( const TString name, const double widthX, const double minX, const double maxX,
                                   const double widthY, const double minY, const double maxY, const bool hasSyst){
    
    //
    // Check the mode is correct
    //
    if(m_type==TREES){
        std::cout << "<!> ERROR in OutputManager::addStandardTH2(): histogram booking has been requested in TREES mode. Please check." << std::endl;
        return false;
    }
    
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

//______________________________________________________________________________________
//
bool OutputManager::bookStandardTH2( const TString &pattern, const bool hasSyst){
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "In OutputManager::bookStandardTH2" << std::endl;
    
    //
    // Check the mode is correct
    //
    if(m_type==TREES){
        std::cout << "<!> ERROR in OutputManager::bookStandardTH2(): histogram booking has been requested in TREES mode. Please check." << std::endl;
        return false;
    }
    
    m_mapHasSyst -> insert( std::pair <TString, bool>(pattern,hasSyst));
    
    for ( const auto h2 : *m_stdTH2Def ){
        
        TString histName = pattern;
        histName += "_";
        histName += h2.second->varY.name();
        histName += "_vs_";
        histName += h2.second->varX.name();
        TString histTitle = h2.second->varY.title();
        histTitle += " vs ";
        histTitle += h2.second->varX.title();
        m_histMngr -> BookTH2D( (std::string)histName, (std::string)histTitle,
                                h2.second->widthX, h2.second->minX, h2.second->maxX,
                                h2.second->widthY, h2.second->minY, h2.second->maxY);
        if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << histName << std::endl;
        
        if(hasSyst && h2.second->hasSyst){
            if(!m_sysVector){
                std::cerr << "<!> ERROR in OutputManager::bookStandardTH2: You want to use systematics, but none is defined ... Please check !" << std::endl;
            } else {
                for (const auto sys : *m_sysVector) {
                    TString systHistName = histName;
                    systHistName += "_";
                    systHistName += sys.second->name();
                    m_histMngr -> BookTH2D((std::string)systHistName,
                                           (std::string)histTitle,
                                           h2.second->widthX, h2.second->minX, h2.second->maxX,
                                           h2.second->widthY, h2.second->minY, h2.second->maxY);
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
    
    //
    // Check the mode is correct
    //
    if(m_type==TREES){
        std::cout << "<!> ERROR in OutputManager::fillStandardTH2(): histogram booking has been requested in TREES mode. Please check." << std::endl;
        return false;
    }
    
    if(!m_data){
        std::cerr << "<!> ERROR in OutputManager::fillStandardTH2: We have big problems ... Please provide an OutputData object" << std::endl;
        return false;
    }
    
    for ( const auto h2 : *m_stdTH2Def ){

        //
        // Nominal histogram filling
        //
        TString histName = pattern;
        histName += "_";
        histName += h2.second->varY.name();
        histName += "_vs_";
        histName += h2.second->varX.name();
        
        m_histMngr -> FillTH2D((std::string)histName, h2.second->varX.GetDoubleValue(), h2.second->varY.GetDoubleValue(), m_data->o_eventWeight_Nom);
        
        if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Filled histogram : " << histName << std::endl;
        
        //
        // Now checking systematics (if needed and if existing)
        //
        if(h2.second->hasSyst && m_mapHasSyst->at(pattern)){
            if(!m_sysVector){
                std::cerr << "<!> ERROR in OutputManager::bookStandardTH2: You want to use systematics, but none is defined ... Please check !" << std::endl;
            } else {
                for(const auto sys : *m_sysVector) {
                    TString systHistName = histName;
                    systHistName += "_";
                    systHistName += sys.second->name();
                    m_histMngr -> FillTH2D((std::string)systHistName, h2.second->varX.GetDoubleValue(), h2.second->varY.GetDoubleValue(), sys.second->GetDoubleValue());
                    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Filled histogram : " << systHistName << std::endl;
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
    //
    // Storing TH2 in the output file
    //
    vector<string> h2list = m_histMngr->GetTH2KeyList();
    for( const auto it_h2 : h2list ){
        f->cd();
        m_histMngr->GetTH2D(it_h2)->Write();
        m_histMngr->ClearTH2(it_h2);
    }
    f -> Close();
    return true;
}

//-----------------------------TREE-SPECIFIC METHODS-------------------------------
//______________________________________________________________________________________
//
bool OutputManager::bookStandardTree( const TString &pattern, const TString &title){
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "In OutputManager::bookStandardTree" << std::endl;
    
    //
    // Check the mode is correct
    //
    if(m_type==HISTOS){
        std::cout << "<!> ERROR in OutputManager::bookStandardTree(): tree booking has been requested in HISTOS mode. Please check." << std::endl;
        return false;
    }
    
    //Book a tree with the given name
    m_treeMngr->BookTree((std::string)pattern, (std::string)title);
    
    //Loop over the list of standard branches and add those branches to the tree
    for ( const auto branch : *m_stdBranchDef ){
        m_treeMngr->AddBranchToTree((std::string)pattern, *(branch.second));
        if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "  -> Added branch : " << branch.first << std::endl;
    }
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::bookStandardTree" << std::endl;
    return true;
}

//______________________________________________________________________________________
//
bool OutputManager::fillStandardTree( const TString &pattern ){
    
    //
    // Check the mode is correct
    //
    if(m_type==HISTOS){
        std::cout << "<!> ERROR in OutputManager::fillStandardTree(): tree booking has been requested in HISTOS mode. Please check." << std::endl;
        return false;
    }
    
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "In OutputManager::fillStandardTree" << std::endl;
    m_treeMngr->FillTree((std::string)pattern);
    if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::fillStandardTree" << std::endl;
    return true;
}

//______________________________________________________________________________________
//
bool OutputManager::saveStandardTree( const TString &outputName ){
    TFile *f = new TFile(outputName,"recreate");
    vector<string> treeList = m_treeMngr->GetTreeKeyList();
    for( const auto treeName : treeList ){
        f -> cd();
        m_treeMngr -> GetTree(treeName) -> Write();
    }
    f -> Close();
    return true;
}




