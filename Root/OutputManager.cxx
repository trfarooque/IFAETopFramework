//Standard includes
#include <iostream>
#include <string>

//IFAETopFrameowrk includes
#include "IFAETopFramework/OutputManager.h"
#include "IFAETopFramework/OutputData.h"
#include "IFAETopFramework/HistManager.h"
#include "IFAETopFramework/TreeManager.h"
#include "IFAETopFramework/AnalysisObject.h"
//ROOT libraries
#include "TString.h"
#include "TSystem.h"

//______________________________________________________________________________________
//
OutputManager::OutputManager( OptionsBase* opt, OutputType type ):
m_opt(opt),
m_type(type),
m_stdTH1Def(0),
m_stdTProfileDef(0),
m_stdTH2Def(0),
m_stdBranchDef(0),
m_histMngr(0),
m_treeMngr(0),
m_sysVector(0),
m_data(0),
m_mapHasSyst(0),
m_vecH2ToProfile(0),
m_weightVarName("weight")
{
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Entering in OutputManager constructor" << std::endl;
    
    m_stdTH1Def     = new StdTH1();
    m_stdTProfileDef= new StdTH1();
    m_stdTH2Def     = new StdTH2();
    m_stdBranchDef  = new StdBranches();
    m_histMngr      = new HistManager(  m_opt->AddUnderflow(), m_opt->AddOverflow() );
    m_treeMngr      = new TreeManager();
    m_mapHasSyst    = new std::map <TString,bool>();
    m_mapHasSyst    -> clear();
    m_vecH2ToProfile= new std::set < TString >;
    m_vecH2ToProfile->clear();
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager constructor" << std::endl;
}

//______________________________________________________________________________________
//
OutputManager::OutputManager( const OutputManager &q )
{
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputManager copy-constructor" << std::endl;
    
    m_opt           = q.m_opt;
    m_type          = q.m_type;
    m_stdTH1Def     = q.m_stdTH1Def;
    m_stdTProfileDef   = q.m_stdTProfileDef;
    m_stdTH2Def     = q.m_stdTH2Def;
    m_stdBranchDef  = q.m_stdBranchDef;
    m_histMngr      = q.m_histMngr;
    m_treeMngr      = q.m_treeMngr;
    m_sysVector     = q.m_sysVector;
    m_data          = q.m_data;
    m_mapHasSyst    = q.m_mapHasSyst;
    m_vecH2ToProfile= q.m_vecH2ToProfile;
    m_weightVarName = q.m_weightVarName;
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager copy-constructor" << std::endl;
}

//______________________________________________________________________________________
//
OutputManager::~OutputManager()
{
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputManager destructor" << std::endl;
}

//______________________________________________________________________________________
//
bool OutputManager::SetSystVector( SystManager::SystVector *sysVector ){
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputManager::SetSystVector()" << std::endl;
    
    m_sysVector = sysVector;
    if(m_type==HISTOS){
        return true;
    } else {
        AddStandardBranch( m_weightVarName, "Nominal weight", "D", &(m_data -> o_eventWeight_Nom) );
        if(m_sysVector){
            for( const auto &sys : *m_sysVector ){
                if(sys.second -> IsPrimitive()){
                    std::string branchName = m_weightVarName + "_" + std::string(sys.second -> Name());
                    AddStandardBranch( branchName, sys.second -> Title(), sys.second -> VarTypeString(), sys.second -> Address() );
                } else {
                    std::cout << "<!> In OutputManager::SetSystVector(): you are not allowed to use non-primitive types for systematic weights yet ... You can kindly ask :-)" << std::endl;
                }
            }
        }
    }
    return true;
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::SetSystVector()" << std::endl;
}

//-----------------------------TH1-SPECIFIC METHODS-------------------------------
//______________________________________________________________________________________
//
bool OutputManager::AddStandardTH1(const TString &name, const double width, const double min, const double max, const std::vector<double>* edges, const bool hasSyst, const int hopt){
    
    //
    // INTERNAL FUNCTION
    //
    
    if(m_opt -> MsgLevel() == Debug::DEBUG){
        std::cout << "In OutputManager::addStandardTH1" << std::endl;
        std::cout << "Adding variable: "<< name << std::endl;
        std::cout << "  width  = " << width << std::endl;
        std::cout << "  min    = " << min << std::endl;
        std::cout << "  max    = " << max << std::endl;
	std::cout << "  edges  = " << edges << std::endl;
        std::cout << "  hasSyst= " << hasSyst << std::endl;
        std::cout << "  hopt   = " << hopt << std::endl;
    }
    
    h1Def *hist = new h1Def();
    hist -> width = width;
    hist -> min = min;
    hist -> max = max;
    hist -> hasSyst = hasSyst;
    hist -> edges = edges;    
    hist -> hopt = hopt;
    m_stdTH1Def -> insert( std::pair < TString, h1Def* >( name, hist ) );
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::addStandardTH1" << std::endl;
    
    return true;
}

//______________________________________________________________________________________
//
bool OutputManager::BookStandardTH1( const TString &pattern, const bool hasSyst){
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputManager::bookStandardTH1" << std::endl;
    
    //
    // Check the mode is correct
    //
    if(m_type==TREES){
        std::cout << "<!> ERROR in OutputManager::bookStandardTH1(): histogram booking has been requested in TREES mode. Please check." << std::endl;
        return false;
    }
    
    //
    // Don't do it if there is no systematics
    //
    if( m_opt -> OnlyDumpSystHistograms() && !hasSyst ){
        return false;
    }
    
    m_mapHasSyst -> insert( std::pair <TString, bool>(pattern,hasSyst));
    
    //
    // Loop over the registered histograms (to be booked for nominal and weight systematics)
    //
    for ( const auto h1 : *m_stdTH1Def ){
        
        TString histName = pattern;
        histName += "_" ;
        histName += h1.second->var.Name();

	const double* ptr_edges = (h1.second -> edges != NULL) ? &( h1.second->edges->at(0) ) : NULL;
	int nbins = ( (h1.second -> edges != NULL) && (h1.second -> edges->size() > 0) )? h1.second->edges->size() - 1 : 0;

	if(nbins > 0){        
	  m_histMngr -> BookTH1D( (std::string)histName, (std::string)(h1.second->var.Title()), nbins, ptr_edges);
        }
	else{
	  m_histMngr -> BookTH1D( (std::string)histName, (std::string)(h1.second->var.Title()), h1.second->width, h1.second->min, h1.second->max);
	}
	if(h1.second->hopt > 0){ m_histMngr->SetTH1Opt((std::string)histName, h1.second->hopt); }

        if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << histName << std::endl;
        
        if(hasSyst && h1.second->hasSyst){
            if(!m_sysVector){
                std::cerr << "<!> ERROR in OutputManager::bookStandardTH1: You want to use systematics, but none is defined ... Please check !" << std::endl;
            } else {
                for (const auto &sys : *m_sysVector) {
                    TString systHistName = histName;
                    systHistName += "_";
                    systHistName += sys.second->Name();

		    if(nbins > 0){  
		      m_histMngr -> BookTH1D( (std::string)systHistName, (std::string)(h1.second->var.Title()), nbins, ptr_edges);
		    }
		    else{
		      m_histMngr -> BookTH1D((std::string)systHistName, (std::string)(h1.second->var.Title()), h1.second->width, h1.second->min, h1.second->max);
		    }
		    if(h1.second->hopt > 0){ m_histMngr->SetTH1Opt((std::string)systHistName, h1.second->hopt); }
                    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << systHistName << std::endl;
                }
            }
        }
    }
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::bookStandardTH1" << std::endl;
    return true;
}

//________________________________________________________________________________________
//
bool OutputManager::FillStandardTH1( const TString &pattern ){
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Entering in OutputManager::fillStandardTH1("<<pattern<<")" << std::endl;
    
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
    
    for ( const std::pair < TString, OutputManager::h1Def* > h1 : *m_stdTH1Def ){
        
        //
        // Nominal histogram filling
        //
        TString histName = pattern;
        histName += "_";
        histName += h1.second->var.Name();
	//if(h1.second->var.IsAnaObject()){h1.second->var.SetAOValues();}
        
        if( !h1.second->var.IsVector() || (h1.second->var.VecInd() >= 0) ){
	  h1.second->var.CalcDoubleValue();
	  m_histMngr -> FillTH1D((std::string)histName, h1.second->var.GetDoubleValue(), m_data->o_eventWeight_Nom);
        } 
	else {

            // If the index provided when declaring the standard histogram is -1, the
            // histogram is filled with all the components of the vector
            // Otherwise, just fills the histogram with the given component

            FillTH1FromVector( h1.second->var.Address(),
                               h1.second->var.VarType(), histName, m_data->o_eventWeight_Nom, h1.second->var.Moment() );
        }
        if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Filled histogram : " << histName << std::endl;
        
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
                    systHistName += sys.second->Name();
                    if( !h1.second->var.IsVector() || (h1.second->var.VecInd() >= 0) ){
		      h1.second->var.CalcDoubleValue();
		      m_histMngr -> FillTH1D((std::string)systHistName, h1.second->var.GetDoubleValue(), sys.second->GetDoubleValue());
                    } else {
                        FillTH1FromVector( h1.second->var.Address(),
                                           h1.second->var.VarType(), (std::string)systHistName, sys.second->GetDoubleValue(), h1.second->var.Moment());
                    }
                    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Filled histogram : " << systHistName << std::endl;
                }
            }
        }
    }
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::fillStandardTH1("<<pattern<<")" << std::endl;
    return true;
}

//________________________________________________________________________________________
//
bool OutputManager::FillTH1FromVector( void* t, const VariableDef::VariableType type, const TString &histName, const double weight, const std::string& moment ) {
    
    //
    // INTERNAL FUNCTION
    //   Used to fill a TH1 histogram based on an input vector. It checks the size of the vector
    //   before filling the histogram.
    //
  if( (type == VariableDef::VECDOUBLE) ){
    std::vector < double >* vec = (std::vector<double>*)t;
    for ( double value : *vec ){
      m_histMngr -> FillTH1D((std::string)histName, value, weight);
    }

  }
  else if(type == VariableDef::VECFLOAT){
    std::vector < float >* vec = (std::vector< float >*)t;
    for ( double value : *vec ){
      m_histMngr -> FillTH1D((std::string)histName, value, weight);
    }

  }
  else if(type == VariableDef::VECINT){
    std::vector < int >* vec = (std::vector< int >*)t;
    for ( double value : *vec ){
      m_histMngr -> FillTH1D((std::string)histName, value, weight);
    }

  }
  else if(type == VariableDef::VECAO){
    AOVector* vec = (AOVector*)t;
    for ( AnalysisObject* obj : *vec ){
      m_histMngr -> FillTH1D((std::string)histName, obj->GetMoment(moment), weight);
    }
  }
  else {
    std::cerr << "<!> ERROR in OutputManager::FillTH1FromVector: the object type is unknown. Please check." << std::endl;
  }
  return true;
}

//________________________________________________________________________________________
//
bool OutputManager::SaveStandardTH1( const TString &outputName, const bool newFile){
   
    TFile *f = new TFile( outputName, ( newFile ? "recreate" : "update" ) );
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
bool OutputManager::AddStandardTH2( const TString &name, const double widthX, const double minX, const double maxX,
				    const double widthY, const double minY, const double maxY, 
				    const std::vector<double>* edgesX, const std::vector<double>* edgesY,
				    const bool hasSyst){
    
    //
    // INTERNAL FUNCTION
    //
    
    //
    // Check the mode is correct
    //
    if(m_type==TREES){
        std::cout << "<!> ERROR in OutputManager::addStandardTH2(): histogram booking has been requested in TREES mode. Please check." << std::endl;
        return false;
    }
    
    if(m_opt -> MsgLevel() == Debug::DEBUG){
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
    hist -> edgesX = edgesX;
    hist -> widthY = widthY;
    hist -> minY = minY;
    hist -> maxY = maxY;
    hist -> edgesY = edgesY;
    hist -> hasSyst = hasSyst;
    
    m_stdTH2Def -> insert( std::pair < TString, h2Def* >( name, hist ) );
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::addStandardTH2" << std::endl;
    
    return true;
}

//______________________________________________________________________________________
//
bool OutputManager::BookStandardTH2( const TString &pattern, const bool hasSyst){
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputManager::bookStandardTH2" << std::endl;
    
    //
    // Check the mode is correct
    //
    if(m_type==TREES){
        std::cout << "<!> ERROR in OutputManager::bookStandardTH2(): histogram booking has been requested in TREES mode. Please check." << std::endl;
        return false;
    }
    
    //
    // Don't do it if there is no systematics
    //
    if( m_opt -> OnlyDumpSystHistograms() && !hasSyst ){
        return false;
    }
    
    m_mapHasSyst -> insert( std::pair <TString, bool>(pattern,hasSyst));
    
    for ( const auto h2 : *m_stdTH2Def ){
        
        TString histName = pattern;
        histName += "_";
        histName += h2.second->varY.Name();
        histName += "_vs_";
        histName += h2.second->varX.Name();
        TString histTitle = h2.second->varY.Title();
        histTitle += " vs ";
        histTitle += h2.second->varX.Title();

	const double* ptr_edgesX = (h2.second -> edgesX != NULL) ? &( h2.second->edgesX->at(0) ) : NULL;
	int nbinsX = ( (h2.second -> edgesX != NULL) && (h2.second -> edgesX->size() > 0) ) ? h2.second -> edgesX->size() - 1 : 0;
	const double* ptr_edgesY = (h2.second -> edgesY != NULL) ? &( h2.second->edgesY->at(0) ) : NULL;
	int nbinsY = ( (h2.second -> edgesY != NULL) && (h2.second -> edgesY->size() > 0) ) ? h2.second -> edgesY->size() - 1 : 0;

	if( (nbinsX > 0) && (nbinsY > 0) ){ 
	  m_histMngr -> BookTH2D( (std::string)histName, (std::string)histTitle,
				  nbinsX, ptr_edgesX, nbinsY, ptr_edgesY);
	}
	else if( (nbinsX <= 0) && (nbinsY > 0) ){
	  m_histMngr -> BookTH2D( (std::string)histName, (std::string)histTitle,
				  h2.second->widthX, h2.second->minX, h2.second->maxX, nbinsY, ptr_edgesY);
	}
	else if( (nbinsX > 0) && (nbinsY <= 0) ){
	  m_histMngr -> BookTH2D( (std::string)histName, (std::string)histTitle,
				  nbinsX, ptr_edgesX, h2.second->widthY, h2.second->minY, h2.second->maxY);
	}
	else{
	  m_histMngr -> BookTH2D( (std::string)histName, (std::string)histTitle,
				  h2.second->widthX, h2.second->minX, h2.second->maxX,
				  h2.second->widthY, h2.second->minY, h2.second->maxY);
	}

        if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << histName << std::endl;
        
        if(hasSyst && h2.second->hasSyst){
            if(!m_sysVector){
                std::cerr << "<!> ERROR in OutputManager::bookStandardTH2: You want to use systematics, but none is defined ... Please check !" << std::endl;
            } else {
                for (const auto sys : *m_sysVector) {
                    TString systHistName = histName;
                    systHistName += "_";
                    systHistName += sys.second->Name();

		    if( (nbinsX > 0) && (nbinsY > 0) ){ 
		      m_histMngr -> BookTH2D( (std::string)systHistName, (std::string)histTitle,
					      nbinsX, ptr_edgesX, nbinsY, ptr_edgesY);
		    }
		    else if( (nbinsX <= 0) && (nbinsY > 0) ){
		      m_histMngr -> BookTH2D( (std::string)systHistName, (std::string)histTitle,
					      h2.second->widthX, h2.second->minX, h2.second->maxX, nbinsY, ptr_edgesY);
		    }
		    else if( (nbinsX > 0) && (nbinsY <= 0) ){
		      //This point should never be reached
		      std::cerr << " IFAETopFramework::HistManager does not support variable x-bins and uniform y-bins. Consider switching the axes" << std::endl;
		      return false;
		    }
		    else{
		      m_histMngr -> BookTH2D( (std::string)systHistName, (std::string)histTitle,
					      h2.second->widthX, h2.second->minX, h2.second->maxX,
					      h2.second->widthY, h2.second->minY, h2.second->maxY);
		    }


                    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << systHistName << std::endl;
                }
            }
        }
    }
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::bookStandardTH2" << std::endl;
    return true;
}

//________________________________________________________________________________________
//
bool OutputManager::FillStandardTH2( const TString &pattern ){
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Entering in OutputManager::fillStandardTH2("<<pattern<<")" << std::endl;
    
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
        histName += h2.second->varY.Name();
        histName += "_vs_";
        histName += h2.second->varX.Name();

        h2.second->varX.GetDoubleValue();
        h2.second->varY.GetDoubleValue();
        m_histMngr -> FillTH2D((std::string)histName, h2.second->varX.GetDoubleValue(), h2.second->varY.GetDoubleValue(), m_data->o_eventWeight_Nom);
        
        if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Filled histogram : " << histName << std::endl;
        
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
                    systHistName += sys.second->Name();

                    m_histMngr -> FillTH2D((std::string)systHistName, h2.second->varX.GetDoubleValue(), h2.second->varY.GetDoubleValue(), sys.second->GetDoubleValue());
                    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Filled histogram : " << systHistName << std::endl;
                }
            }
        }
    }
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::fillStandardTH2("<<pattern<<")" << std::endl;
    return true;
}

//________________________________________________________________________________________
//
bool OutputManager::SaveStandardTH2( const TString &outputName, const bool newFile ){
    
    TFile *f = new TFile( outputName, ( newFile ? "recreate" : "update" ) );
    //
    // Storing TH2 in the output file
    //
    vector<string> h2list = m_histMngr->GetTH2KeyList();
    for( const auto it_h2 : h2list ){
        f->cd();
        TH2D* h_temp = m_histMngr->GetTH2D(it_h2);
        h_temp -> Write();
        
        //Need to profile this TH2 ?
        bool toProfile = false;
        const TString h_tempName = it_h2;
        for( const TString prof_name : *m_vecH2ToProfile ){
            if(h_tempName.Contains(prof_name)){
                toProfile = true;
                break;
            }
        }
        if(toProfile){
            TProfile* temp_profile = (TProfile*) h_temp -> ProfileX( h_tempName + "_pfx" );
            temp_profile -> Write();
            delete temp_profile;
        }
        m_histMngr->ClearTH2(it_h2);
    }
    f -> Close();
    return true;
}

//________________________________________________________________________________________
//
bool OutputManager::StoreTProfile( const TString &nameX, const TString &nameY, const bool hasSyst ){
    TString name = nameY + "_vs_" + nameX;
    if(!hasSyst){
        if( m_vecH2ToProfile -> find(name) != m_vecH2ToProfile -> end() ){
            std::cout << " !! WARNING: In OutputManager::StoreTProfile(" << nameX << ", " << nameY << "): the TProfile is already in the vector !" << std::endl;
            return false;
        } else {
            m_vecH2ToProfile -> insert(name);
            return true;
        }
    } else {
        std::cout << "In OutputManager::StoreTProfile: The systematic version of this is not yet implemented !" << std::endl;
        return false;
    }
    return false;
}

//-----------------------------TREE-SPECIFIC METHODS-------------------------------
//______________________________________________________________________________________
//
bool OutputManager::BookStandardTree( const TString &pattern, const TString &title){
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputManager::bookStandardTree" << std::endl;
    
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
        if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Added branch : " << branch.first << std::endl;
    }
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::bookStandardTree" << std::endl;
    return true;
}

//______________________________________________________________________________________
//
bool OutputManager::FillStandardTree( const TString &pattern ){
    
    //
    // Check the mode is correct
    //
    if(m_type==HISTOS){
        std::cout << "<!> ERROR in OutputManager::fillStandardTree(): tree booking has been requested in HISTOS mode. Please check." << std::endl;
        return false;
    }
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputManager::fillStandardTree" << std::endl;
    m_treeMngr->FillTree((std::string)pattern);
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::fillStandardTree" << std::endl;
    return true;
}

//______________________________________________________________________________________
//
bool OutputManager::SaveStandardTree( const TString &outputName ){
    TFile *f = new TFile(outputName,"recreate");
    vector<string> treeList = m_treeMngr->TreeKeyList();
    for( const auto treeName : treeList ){
        f -> cd();
        m_treeMngr -> Tree(treeName) -> Write();
    }
    f -> Close();
    return true;
}




