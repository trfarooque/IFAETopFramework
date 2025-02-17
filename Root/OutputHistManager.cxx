//Standard includes
#include <iostream>

//IFAETopFramework includes
#include "IFAETopFramework/OutputHistManager.h"
#include "IFAETopFramework/OutputData.h"
#include "IFAETopFramework/HistManager.h"
#include "IFAETopFramework/WeightManager.h"
//ROOT libraries
#include "TSystem.h"

//______________________________________________________________________________________
//
OutputHistManager::OutputHistManager( OptionsBase* opt, OutputData* data, WeightManager::WeightMap *sysMap, bool _debug):
  OutputManager(opt, data),
  m_stdTH1Def(0),
  m_stdTProfileDef(0),
  m_stdTH2Def(0),
  m_histMngr(0),
  m_sysMap(sysMap),
  m_mapHasSyst(0),
  m_vecH2ToProfile(0),
  m_debug(_debug)
{
  m_debug = m_debug || (m_opt -> MsgLevel() == Debug::DEBUG);
    if(m_debug) std::cout << "Entering in OutputHistManager constructor" << std::endl;
    
    m_stdTH1Def     = new StdTH1();
    m_stdTProfileDef= new StdTH1();
    m_stdTH2Def     = new StdTH2();
    m_histMngr      = new HistManager(  m_opt->AddUnderflow(), m_opt->AddOverflow() );

    m_mapHasSyst    = new std::map <std::string,bool>();
    m_mapHasSyst    -> clear();

    m_vecH2ToProfile= new std::set < std::string >;
    m_vecH2ToProfile->clear();

    
    if(m_debug) std::cout << "Leaving OutputHistManager constructor" << std::endl;
}

//______________________________________________________________________________________
//
OutputHistManager::OutputHistManager( const OutputHistManager &q ) : 
  OutputManager(q)
{
    if(m_debug) std::cout << "In OutputHistManager copy-constructor" << std::endl;
    
    m_stdTH1Def        = q.m_stdTH1Def;
    m_stdTProfileDef   = q.m_stdTProfileDef;
    m_stdTH2Def        = q.m_stdTH2Def;
    m_histMngr         = q.m_histMngr;
    m_sysMap           = q.m_sysMap;
    m_mapHasSyst       = q.m_mapHasSyst;
    m_vecH2ToProfile   = q.m_vecH2ToProfile;
    
    if(m_debug) std::cout << "Leaving OutputHistManager copy-constructor" << std::endl;
}

//______________________________________________________________________________________
//
OutputHistManager::~OutputHistManager()
{
    if(m_debug) std::cout << "In OutputHistManager destructor" << std::endl;


    for( std::pair<std::string, h1Def*> hist : *m_stdTH1Def ){
      delete hist.second->var;
      delete hist.second;
    }
    delete m_stdTH1Def;

    for( std::pair<std::string, h1Def*> hist : *m_stdTProfileDef ){
      delete hist.second->var;
      delete hist.second;
    }
    delete m_stdTProfileDef;

    for( std::pair<std::string, h2Def*> hist : *m_stdTH2Def ){
      delete hist.second->varX;
      delete hist.second->varY;
      delete hist.second;
    }
    delete m_stdTH2Def;

    delete m_histMngr;
    delete m_mapHasSyst;
    delete m_vecH2ToProfile;
    if(m_debug) std::cout << "Leaving OutputHistManager destructor" << std::endl;
}

//-----------------------------TH1-SPECIFIC METHODS-------------------------------
//______________________________________________________________________________________
//
bool OutputHistManager::AddStandardTH1(const std::string &name, const double width, const double min, const double max, const std::vector<double>* edges, const bool hasSyst, const int hopt, const bool noWeight){
    
    //
    // INTERNAL FUNCTION
    //
  bool _hasSyst = hasSyst && !noWeight;

    if(m_debug){
        std::cout << "In OutputHistManager::addStandardTH1" << std::endl;
        std::cout << "Adding variable: "<< name << std::endl;
        std::cout << "  width    = " << width << std::endl;
        std::cout << "  min      = " << min << std::endl;
        std::cout << "  max      = " << max << std::endl;
	std::cout << "  edges    = " << edges << std::endl;
        std::cout << "  hasSyst  = " << hasSyst << std::endl;
        std::cout << "  hopt     = " << hopt << std::endl;
	std::cout << "  noWeight = " << noWeight << std::endl;
    }
    
    h1Def *hist = new h1Def();
    hist -> var = NULL;
    hist -> width = width;
    hist -> min = min;
    hist -> max = max;
    hist -> hasSyst = _hasSyst;
    hist -> edges = edges;    
    hist -> hopt = hopt;
    hist -> noWeight = noWeight;
    m_stdTH1Def -> insert( std::pair < std::string, h1Def* >( name, hist ) );
    
    if(m_debug) std::cout << "Leaving OutputHistManager::addStandardTH1" << std::endl;
    
    return true;
}

//______________________________________________________________________________________
//
bool OutputHistManager::BookStandardTH1( const std::string &pattern, const bool hasSyst){
    
    if(m_debug) std::cout << "In OutputHistManager::bookStandardTH1" << std::endl;
    m_mapHasSyst -> insert( std::pair <std::string, bool>(pattern,hasSyst));

    //
    // Don't do it if there is no systematics
    //
    if( m_opt -> OnlyDumpSystHistograms() && !hasSyst ){
        return false;
    }
    
    
    //
    // Loop over the registered histograms (to be booked for nominal and weight systematics)
    //
    for ( const auto h1 : *m_stdTH1Def ){
        
      std::string histName = pattern + "_" + h1.second->var->Name();
      
      const double* ptr_edges = (h1.second -> edges != NULL) ? &( h1.second->edges->at(0) ) : NULL;
      int nbins = ( (h1.second -> edges != NULL) && (h1.second -> edges->size() > 0) )? h1.second->edges->size() - 1 : 0;
      
      if(nbins > 0){        
	m_histMngr -> BookTH1D( histName, h1.second->var->Title(), nbins, ptr_edges);
      }
      else{
	m_histMngr -> BookTH1D( histName, h1.second->var->Title(), h1.second->width, h1.second->min, h1.second->max);
      }
      if(h1.second->hopt > 0){ m_histMngr->SetTH1Opt(histName, h1.second->hopt); }
      
      if(m_debug) std::cout << "  -> Booked histogram : " << histName << std::endl;

      if(hasSyst && h1.second->hasSyst){
	if(!m_sysMap){
	  std::cerr << "<!> ERROR in OutputHistManager::bookStandardTH1: You want to use systematics, but none is defined ... Please check !" << std::endl;
	} else {
	  for (const auto &sys : *m_sysMap) {
	    std::string systHistName = histName + "_" + sys.second->Name();
	    if(nbins > 0){  
	      m_histMngr -> BookTH1D( systHistName, h1.second->var->Title(), nbins, ptr_edges);
	    }
	    else{
	      m_histMngr -> BookTH1D( systHistName, h1.second->var->Title(), h1.second->width, h1.second->min, h1.second->max);
	    }
	    if(h1.second->hopt > 0){ m_histMngr->SetTH1Opt( systHistName, h1.second->hopt ); }
	    if(m_debug) std::cout << "  -> Booked histogram : " << systHistName << std::endl;
	  }
	}
      }
    }
    
    if(m_debug) std::cout << "Leaving OutputHistManager::bookStandardTH1" << std::endl;
    return true;
}

//________________________________________________________________________________________
//
bool OutputHistManager::FillStandardTH1( const std::string &pattern, const bool updateStores ){
    
  if(m_debug) std::cout << "Entering in OutputHistManager::fillStandardTH1("<<pattern<<")" << std::endl;
  
  if(!m_data){
    std::cerr << "<!> ERROR in OutputHistManager::fillStandardTH1: We have big problems ... Please provide an OutputData object" << std::endl;
    return false;
  }

  if( m_opt -> OnlyDumpSystHistograms() && !(m_mapHasSyst -> at(pattern)) ) return true; 
  
  for ( const std::pair < TString, OutputHistManager::h1Def* > h1 : *m_stdTH1Def ){

    //
    // Nominal histogram filling
    //
    std::string histName = pattern + "_" + h1.second->var->Name();
    double weight = (h1.second->noWeight) ? 1. : m_data->o_eventWeight_Nom;
    if( !h1.second->var->IsVector() || (h1.second->var->VecInd() >= 0) ){

      if(m_debug){
	std::cout<<" PATTERN = "<<pattern<<" NAME = "<<h1.second->var->Name()<<" VALUE = "<<h1.second->var->GetDoubleValue()<<std::endl;        
      }
      if(updateStores){ 
	h1.second->var->CalcDoubleValue();
      }

      ////////////////////////////////////////////////////////////////////////////////////////////////////////
      //
      // Do not fill histograms in cases of invalidity: 
      // Variable currently pointing to NULL, index larger than vector size, unknown moment for AnalysisObject
      //
      ////////////////////////////////////////////////////////////////////////////////////////////////////////
      if( !h1.second->var->IsValidValue() ) continue;    

      if(m_debug){
	std::cout<<" pattern = "<<pattern<<" varName = "<<h1.second->var->Name()<<" double_value = "<< h1.second->var->GetDoubleValue()<<std::endl;
      }
      //if( !h1.second->var->IsVector() || (h1.second->var->VecInd() < h1.second->var->GetVecSize()) ){
      m_histMngr -> FillTH1D(histName, h1.second->var->GetDoubleValue(), weight);
      //}
    } 
    else {

      // If the index provided when declaring the standard histogram is -1, the
      // histogram is filled with all the components of the vector
      // Otherwise, just fills the histogram with the given component

      FillTH1FromVector( h1.second->var->Address(),
			 h1.second->var->VarType(), histName, weight, h1.second->var->Moment() );
    }
    if(m_debug) std::cout << "  -> Filled histogram : " << histName << std::endl;
        
    //
    // Now checking systematics (if needed and if existing)
    //

    if(h1.second->hasSyst && m_mapHasSyst->at(pattern)){
      if(!m_sysMap){
	std::cerr << "<!> ERROR in OutputHistManager::bookStandardTH1: You want to use systematics, but none is defined ... Please check !" << std::endl;
      } else {
	for (const auto sys : *m_sysMap) {
	  std::string systHistName = histName + "_" + sys.second->Name();

	  if( !h1.second->var->IsVector() || (h1.second->var->VecInd() >= 0) ){ // && (h1.second->var->VecInd() < h1.second->var->GetVecSize()) ) ){
	    m_histMngr -> FillTH1D(systHistName, h1.second->var->GetDoubleValue(), sys.second->GetWeightValue());
	  } else {
	    FillTH1FromVector( h1.second->var->Address(),
			       h1.second->var->VarType(), systHistName, sys.second->GetWeightValue(), h1.second->var->Moment());
	  }
	  if(m_debug) std::cout << "  -> Filled histogram : " << systHistName << std::endl;
	}
      }
    }
  }
  if(m_debug) std::cout << "Leaving OutputHistManager::fillStandardTH1("<<pattern<<")" << std::endl;
  return true;
}

//________________________________________________________________________________________
//
bool OutputHistManager::FillTH1FromVector( void* t, const VariableDef::VariableType type, const std::string &histName, const double weight
					   , const std::string& moment ) {
    
    //
    // INTERNAL FUNCTION
    //   Used to fill a TH1 histogram based on an input vector. It checks the size of the vector
    //   before filling the histogram.
    //
  if( (type == VariableDef::PTRVECDOUBLE) || (type == VariableDef::VECDOUBLE) ){
    std::vector < double >* vec = (type == VariableDef::PTRVECDOUBLE) ? 
      *(std::vector<double>**)t : (std::vector<double>*)t;
    for ( double value : *vec ){ m_histMngr -> FillTH1D(histName, value, weight); }

  }
  else if( (type == VariableDef::PTRVECFLOAT) || (type == VariableDef::VECFLOAT) ){
    std::vector < float >* vec = (type == VariableDef::PTRVECFLOAT) ? 
      *(std::vector<float>**)t : (std::vector<float>*)t;
    for ( double value : *vec ){ m_histMngr -> FillTH1D(histName, value, weight); }
  }
  else if( (type == VariableDef::PTRVECINT) || (type == VariableDef::VECINT) ){
    std::vector < int >* vec = (type == VariableDef::PTRVECINT) ? 
      *(std::vector<int>**)t : (std::vector<int>*)t;
    for ( double value : *vec ){ m_histMngr -> FillTH1D(histName, value, weight); }
  }
  else if( (type == VariableDef::PTRVECBOOL) || (type == VariableDef::VECBOOL) ){
    std::vector < bool >* vec = (type == VariableDef::PTRVECBOOL) ? 
      *(std::vector<bool>**)t : (std::vector<bool>*)t;
    for ( double value : *vec ){ m_histMngr -> FillTH1D(histName, value, weight); }
  }
  else if( (type == VariableDef::PTRVECAO) || (type == VariableDef::VECAO) ){
    AOVector* vec = (type == VariableDef::PTRVECAO) ? 
      *(AOVector**)t : (AOVector*)t;
    for ( AnalysisObject* obj : *vec ){ 
      if( obj -> IsKnownMoment(moment) ){
	m_histMngr -> FillTH1D(histName, obj->GetMoment(moment), weight);
      }
    }

  }
  else {
    std::cerr << "<!> ERROR in OutputHistManager::FillTH1FromVector: the object type is unknown. Please check." << std::endl;
  }
  return true;
}

//________________________________________________________________________________________
//
bool OutputHistManager::SaveStandardTH1( const std::string &outputName, const bool newFile, const std::string& pattern){
   
  TFile *f = new TFile( outputName.c_str(), ( newFile ? "recreate" : "update" ) );
    //Storing TH1 in the output file
    vector<string> h1list = m_histMngr->GetTH1KeyList();
    for( const auto it_h1 : h1list ){
      if(!pattern.empty() && (it_h1.find(pattern) == std::string::npos)){ continue; }
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
bool OutputHistManager::AddStandardTH2( const std::string &name, const double widthX, const double minX, const double maxX,
					const double widthY, const double minY, const double maxY, 
					const std::vector<double>* edgesX, const std::vector<double>* edgesY,
					const bool hasSyst, const bool pairwise, const bool noWeight){
    //
    // INTERNAL FUNCTION
    //
  bool _hasSyst = hasSyst && !noWeight;

    if(m_debug){
        std::cout << "In OutputHistManager::addStandardTH2" << std::endl;
        std::cout << "Adding variable: "<< name << std::endl;
        std::cout << "  widthX  = " << widthX << std::endl;
        std::cout << "  minX    = " << minX << std::endl;
        std::cout << "  maxX    = " << maxX << std::endl;
        std::cout << "  widthY  = " << widthY << std::endl;
        std::cout << "  minY    = " << minY << std::endl;
        std::cout << "  maxY    = " << maxY << std::endl;
        std::cout << "  hasSyst = " << hasSyst << std::endl;
        std::cout << "  pairwise = " << pairwise << std::endl;
	std::cout << "  noWeight = " << noWeight << std::endl;
    }
    
    h2Def *hist = new h2Def();
    hist -> varX = NULL;
    hist -> widthX = widthX;
    hist -> minX = minX;
    hist -> maxX = maxX;
    hist -> edgesX = edgesX;
    hist -> varY = NULL;
    hist -> widthY = widthY;
    hist -> minY = minY;
    hist -> maxY = maxY;
    hist -> edgesY = edgesY;
    hist -> hasSyst = _hasSyst;
    hist -> pairwise = pairwise;
    hist -> noWeight = noWeight;

    m_stdTH2Def -> insert( std::pair < std::string, h2Def* >( name, hist ) );
    
    if(m_debug) std::cout << "Leaving OutputHistManager::addStandardTH2" << std::endl;
    
    return true;
}

//______________________________________________________________________________________
//
bool OutputHistManager::BookStandardTH2( const std::string &pattern, const bool hasSyst){
    
    if(m_debug) std::cout << "In OutputHistManager::bookStandardTH2" << std::endl;
    m_mapHasSyst -> insert( std::pair <std::string, bool>(pattern,hasSyst));
    
    //
    // Don't do it if there is no systematics
    //
    if( m_opt -> OnlyDumpSystHistograms() && !hasSyst ){
        return false;
    }
    
    for ( const auto h2 : *m_stdTH2Def ){
        
      std::string histName = pattern + "_" + h2.second->varY->Name() + "_vs_" + h2.second->varX->Name();
      std::string histTitle = h2.second->varY->Title() + " vs " + h2.second->varX->Title();

      const double* ptr_edgesX = (h2.second -> edgesX != NULL) ? &( h2.second->edgesX->at(0) ) : NULL;
      int nbinsX = ( (h2.second -> edgesX != NULL) && (h2.second -> edgesX->size() > 0) ) ? h2.second -> edgesX->size() - 1 : 0;
      const double* ptr_edgesY = (h2.second -> edgesY != NULL) ? &( h2.second->edgesY->at(0) ) : NULL;
      int nbinsY = ( (h2.second -> edgesY != NULL) && (h2.second -> edgesY->size() > 0) ) ? h2.second -> edgesY->size() - 1 : 0;

	if( (nbinsX > 0) && (nbinsY > 0) ){ 
	  m_histMngr -> BookTH2D( histName, histTitle,
				  nbinsX, ptr_edgesX, nbinsY, ptr_edgesY);
	}
	else if( (nbinsX <= 0) && (nbinsY > 0) ){
	  m_histMngr -> BookTH2D( histName, histTitle,
				  h2.second->widthX, h2.second->minX, h2.second->maxX, nbinsY, ptr_edgesY);
	}
	else if( (nbinsX > 0) && (nbinsY <= 0) ){
	  m_histMngr -> BookTH2D( histName, histTitle,
				  nbinsX, ptr_edgesX, h2.second->widthY, h2.second->minY, h2.second->maxY);
	}
	else{
	  m_histMngr -> BookTH2D( histName, histTitle,
				  h2.second->widthX, h2.second->minX, h2.second->maxX,
				  h2.second->widthY, h2.second->minY, h2.second->maxY);
	}

        if(m_debug) std::cout << "  -> Booked histogram : " << histName << std::endl;
        
        if(hasSyst && h2.second->hasSyst){
            if(!m_sysMap){
                std::cerr << "<!> ERROR in OutputHistManager::bookStandardTH2: You want to use systematics, but none is defined ... Please check !" << std::endl;
            } else {
                for (const auto sys : *m_sysMap) {
		  std::string systHistName = histName + "_" + sys.second->Name();

		  if( (nbinsX > 0) && (nbinsY > 0) ){ 
		    m_histMngr -> BookTH2D( systHistName, histTitle,
					    nbinsX, ptr_edgesX, nbinsY, ptr_edgesY);
		  }
		  else if( (nbinsX <= 0) && (nbinsY > 0) ){
		    m_histMngr -> BookTH2D( systHistName, histTitle,
					    h2.second->widthX, h2.second->minX, h2.second->maxX, nbinsY, ptr_edgesY);
		  }
		  else if( (nbinsX > 0) && (nbinsY <= 0) ){
		    //This point should never be reached
		    std::cerr << " IFAETopFramework::HistManager does not support variable x-bins and uniform y-bins. Consider switching the axes" << std::endl;
		    return false;
		  }
		  else{
		    m_histMngr -> BookTH2D( systHistName, histTitle,
					    h2.second->widthX, h2.second->minX, h2.second->maxX,
					    h2.second->widthY, h2.second->minY, h2.second->maxY);
		  }
		  
		  
                    if(m_debug) std::cout << "  -> Booked histogram : " << systHistName << std::endl;
                }
            }
        }
    }
    if(m_debug) std::cout << "Leaving OutputHistManager::bookStandardTH2" << std::endl;
    return true;
}

//________________________________________________________________________________________
//
bool OutputHistManager::FillStandardTH2( const std::string &pattern, const bool updateStores ){
    
    if(m_debug) std::cout << "Entering in OutputHistManager::fillStandardTH2("<<pattern<<")" << std::endl;
    if(!m_data){
        std::cerr << "<!> ERROR in OutputHistManager::fillStandardTH2: We have big problems ... Please provide an OutputData object" << std::endl;
        return false;
    }

    if( m_opt -> OnlyDumpSystHistograms() && !(m_mapHasSyst -> at(pattern)) ) return true; 
    
    for ( const auto h2 : *m_stdTH2Def ){
      /*
      //
      // Do not fill histogram if either X or Y variable is currently NULL
      //
      if( h2.second->varX->PointsToNull() ) continue;    
      if( h2.second->varY->PointsToNull() ) continue;    
      */

      if( m_opt -> OnlyDumpSystHistograms() && !(h2.second->hasSyst) ) continue; 

      double weight = h2.second->noWeight ? 1. : m_data -> o_eventWeight_Nom;        
      //
      // Nominal histogram filling
      //
      std::string histName = pattern + "_" + h2.second->varY->Name() + "_vs_" + h2.second->varX->Name();
      bool bX_flat =  !h2.second->varX->IsVector() || (h2.second->varX->VecInd() >= 0) ; 
      bool bY_flat =  !h2.second->varY->IsVector() || (h2.second->varY->VecInd() >= 0) ; 
      if( updateStores && bX_flat ){ h2.second->varX->CalcDoubleValue(); }
      if( updateStores && bY_flat ){ h2.second->varY->CalcDoubleValue(); }

      if( bX_flat && !h2.second->varX->IsValidValue() ) continue;
      if( bY_flat && !h2.second->varY->IsValidValue() ) continue;

      /*
      // TO BE REMOVED
      //Check whether the given indices for the variables are within range of the vectors
      //
      //if(bX_flat && h2.second->varX->IsVector() && (h2.second->varX->VecInd() >= h2.second->varX->GetVecSize()) ){ continue; }
      //if(bY_flat && h2.second->varY->IsVector() && (h2.second->varY->VecInd() >= h2.second->varY->GetVecSize()) ){ continue; }
      */

      if( bX_flat && bY_flat ){
	m_histMngr -> FillTH2D(histName, h2.second->varX->GetDoubleValue(), h2.second->varY->GetDoubleValue(), weight);
      }
      else if( bX_flat && !bY_flat){
	FillTH2FromOneVector(h2.second->varX->GetDoubleValue(), h2.second->varY->Address(), h2.second->varY->VarType() 
			     , histName, weight, "Y", h2.second->varY->Moment());
      }
      else if( !bX_flat && bY_flat){
	FillTH2FromOneVector(h2.second->varY->GetDoubleValue(), h2.second->varX->Address(), h2.second->varX->VarType() 
			     , histName, weight, "X", h2.second->varX->Moment());
      }
      else{

	FillTH2FromVectors(h2.second->varX->Address(), h2.second->varY->Address()
			   , h2.second->varX->VarType() , h2.second->varY->VarType() 
			   , histName, weight, h2.second->pairwise, h2.second->varX->Moment(), h2.second->varY->Moment());

      }

      if(m_debug) std::cout << "  -> Filled histogram : " << histName << std::endl;
        
      //
      // Now checking systematics (if needed and if existing)
      //
      if(h2.second->hasSyst && m_mapHasSyst->at(pattern)){
	if(!m_sysMap){
	  std::cerr << "<!> ERROR in OutputHistManager::bookStandardTH2: You want to use systematics, but none is defined ... Please check !" << std::endl;
	} else {
	  for(const auto sys : *m_sysMap) {
	    std::string systHistName = histName + "_" + sys.second->Name();

	    if( bX_flat && bY_flat ){
	      m_histMngr -> FillTH2D(systHistName, h2.second->varX->GetDoubleValue(), h2.second->varY->GetDoubleValue(), sys.second->GetWeightValue());
	    }
	    else if( bX_flat && !bY_flat){
	      FillTH2FromOneVector(h2.second->varX->GetDoubleValue(), h2.second->varY->Address(), h2.second->varY->VarType() 
				   , systHistName, sys.second->GetWeightValue(), "Y", h2.second->varY->Moment());
	    }
	    else if( !bX_flat && bY_flat){
	      FillTH2FromOneVector(h2.second->varY->GetDoubleValue(), h2.second->varX->Address(), h2.second->varX->VarType() 
				   , systHistName, sys.second->GetWeightValue(), "X", h2.second->varX->Moment());
	    }
	    else{

	      FillTH2FromVectors(h2.second->varX->Address(), h2.second->varY->Address()
				 , h2.second->varX->VarType() , h2.second->varY->VarType() 
				 , systHistName, sys.second->GetWeightValue(), h2.second->pairwise, h2.second->varX->Moment(), h2.second->varY->Moment());

	    }

	    if(m_debug) std::cout << "  -> Filled histogram : " << systHistName << std::endl;
	  }
	}
      }
    }
    if(m_debug) std::cout << "Leaving OutputHistManager::fillStandardTH2("<<pattern<<")" << std::endl;
    return true;
}

//__________________________________________________________________________________________
//
bool OutputHistManager::FillTH2FromVectors( void* tX, void* tY
					    , VariableDef::VariableType typeX, VariableDef::VariableType typeY
					    , const std::string &histName, const double weight, const bool pairwise
					    , const std::string& momentX, const std::string& momentY){
  bool same = (tX == tY);
  if( same && (typeX != typeY) ){
    std::cerr << " OutputHistManager::FillTH2FromVectors --> different types for same vector "<<tX << std::endl;
    return false;
  }

  std::vector<double>* vecD_X = NULL;
  std::vector<float>* vecF_X = NULL;
  std::vector<int>* vecI_X = NULL;
  std::vector<bool>* vecB_X = NULL;
  AOVector* vecAO_X = NULL;

  if(typeX == VariableDef::PTRVECDOUBLE){ vecD_X = *((std::vector<double>**)tX); } 
  else if(typeX == VariableDef::VECDOUBLE){ vecD_X = (std::vector<double>*)tX; } 
  else if(typeX == VariableDef::PTRVECFLOAT){ vecF_X = *((std::vector<float>**)tX); } 
  else if(typeX == VariableDef::VECFLOAT){ vecF_X = (std::vector<float>*)tX; } 
  else if(typeX == VariableDef::PTRVECINT){ vecI_X = *((std::vector<int>**)tX); } 
  else if(typeX == VariableDef::VECINT){ vecI_X = (std::vector<int>*)tX; } 
  else if(typeX == VariableDef::PTRVECBOOL){ vecB_X = *((std::vector<bool>**)tX); } 
  else if(typeX == VariableDef::VECBOOL){ vecB_X = (std::vector<bool>*)tX; } 
  else if(typeX == VariableDef::PTRVECAO){ vecAO_X = *((AOVector**)tX); } 
  else if(typeX == VariableDef::VECAO){ vecAO_X = (AOVector*)tX; } 

  //------------------------
  std::vector<double>* vecD_Y = NULL;
  std::vector<float>* vecF_Y = NULL;
  std::vector<int>* vecI_Y = NULL;
  std::vector<bool>* vecB_Y = NULL;
  AOVector* vecAO_Y = NULL;

  if(typeY == VariableDef::PTRVECDOUBLE){ vecD_Y = *((std::vector<double>**)tY); } 
  else if(typeY == VariableDef::VECDOUBLE){ vecD_Y = (std::vector<double>*)tY; } 
  else if(typeY == VariableDef::PTRVECFLOAT){ vecF_Y = *((std::vector<float>**)tY); } 
  else if(typeY == VariableDef::VECFLOAT){ vecF_Y = (std::vector<float>*)tY; } 
  else if(typeY == VariableDef::PTRVECINT){ vecI_Y = *((std::vector<int>**)tY); } 
  else if(typeY == VariableDef::VECINT){ vecI_Y = (std::vector<int>*)tY; } 
  else if(typeY == VariableDef::PTRVECBOOL){ vecB_Y = *((std::vector<bool>**)tY); } 
  else if(typeY == VariableDef::VECBOOL){ vecB_Y = (std::vector<bool>*)tY; } 
  else if(typeY == VariableDef::PTRVECAO){ vecAO_Y = *((AOVector**)tY); } 
  else if(typeY == VariableDef::VECAO){ vecAO_Y = (AOVector*)tY; } 
  //-----------------------------------------

  if( vecD_X && vecD_Y ){
    FillTH2FromPrimitiveVectors( vecD_X, vecD_Y, histName, weight, pairwise, same);
  }
  else if( vecD_X && vecF_Y ){
    FillTH2FromPrimitiveVectors( vecD_X, vecF_Y, histName, weight, pairwise  );
  }
  else if( vecD_X && vecI_Y ){
    FillTH2FromPrimitiveVectors( vecD_X, vecI_Y, histName, weight, pairwise  );
  }
  else if( vecD_X && vecB_Y ){
    FillTH2FromPrimitiveVectors( vecD_X, vecB_Y, histName, weight, pairwise  );
  }
  else if( vecD_X && vecAO_Y ){
    FillTH2FromFlatAndAOVectors(  vecAO_Y, vecD_X, histName, weight, pairwise, momentY, "Y" );
  }
  //---------------
  if( vecF_X && vecD_Y ){
    FillTH2FromPrimitiveVectors( vecF_X, vecD_Y, histName, weight, pairwise  );
  }
  else if( vecF_X && vecF_Y ){
    FillTH2FromPrimitiveVectors( vecF_X, vecF_Y, histName, weight, pairwise, same  );
  }
  else if( vecF_X && vecI_Y ){
    FillTH2FromPrimitiveVectors( vecF_X, vecI_Y, histName, weight, pairwise  );
  }
  else if( vecF_X && vecB_Y ){
    FillTH2FromPrimitiveVectors( vecF_X, vecB_Y, histName, weight, pairwise  );
  }
  else if( vecF_X && vecAO_Y ){
    FillTH2FromFlatAndAOVectors( vecAO_Y, vecF_X, histName, weight, pairwise , momentY, "Y" );
  }
  //---------------
  if( vecI_X && vecD_Y ){
    FillTH2FromPrimitiveVectors( vecI_X, vecD_Y, histName, weight, pairwise  );
  }
  else if( vecI_X && vecF_Y ){
    FillTH2FromPrimitiveVectors( vecI_X, vecF_Y, histName, weight, pairwise  );
  }
  else if( vecI_X && vecI_Y ){
    FillTH2FromPrimitiveVectors( vecI_X, vecI_Y, histName, weight, pairwise, same );
  }
  else if( vecI_X && vecB_Y ){
    FillTH2FromPrimitiveVectors( vecI_X, vecB_Y, histName, weight, pairwise, same );
  }
  else if( vecI_X && vecAO_Y ){
    FillTH2FromFlatAndAOVectors( vecAO_Y, vecI_X, histName, weight, pairwise , momentY, "Y" );
  }
  //---------------
  if( vecB_X && vecD_Y ){
    FillTH2FromPrimitiveVectors( vecB_X, vecD_Y, histName, weight, pairwise  );
  }
  else if( vecB_X && vecF_Y ){
    FillTH2FromPrimitiveVectors( vecB_X, vecF_Y, histName, weight, pairwise  );
  }
  else if( vecB_X && vecI_Y ){
    FillTH2FromPrimitiveVectors( vecB_X, vecI_Y, histName, weight, pairwise, same );
  }
  else if( vecB_X && vecB_Y ){
    FillTH2FromPrimitiveVectors( vecB_X, vecB_Y, histName, weight, pairwise, same );
  }
  else if( vecB_X && vecAO_Y ){
    FillTH2FromFlatAndAOVectors( vecAO_Y, vecB_X, histName, weight, pairwise , momentY, "Y" );
  }
  //-----------
  if( vecAO_X && vecD_Y ){
    FillTH2FromFlatAndAOVectors( vecAO_X, vecD_Y, histName, weight, pairwise , momentX, "X" );
  }
  else if( vecAO_X && vecF_Y ){
    FillTH2FromFlatAndAOVectors( vecAO_X, vecF_Y, histName, weight, pairwise , momentX, "X" );
  }
  else if( vecAO_X && vecI_Y ){
    FillTH2FromFlatAndAOVectors( vecAO_X, vecI_Y, histName, weight, pairwise , momentX, "X" );
  }
  else if( vecAO_X && vecB_Y ){
    FillTH2FromFlatAndAOVectors( vecAO_X, vecB_Y, histName, weight, pairwise , momentX, "X" );
  }
  else if( vecAO_X && vecAO_Y ){
    FillTH2FromAOVectors( vecAO_X, vecAO_Y, histName, weight, pairwise , momentX, momentY );
  }

  return true;
}


//________________________________________________________________________________________
//
bool OutputHistManager::FillTH2FromOneVector(const double& flatVal, void* t, const VariableDef::VariableType type
					     , const std::string &histName, const double weight,  const std::string& vecAxis, const std::string& moment){

  //
  // INTERNAL FUNCTION
  //   Used to fill a TH2 histogram from input vectors for both axes. The values are filled **pairwise**. It checks that both vectors
  //   have the same size before filling the histogram.
  //
  //if( (vecAxis != "X") && (vecAxis != "Y") ){ std::cout<<"ERROR in OutputHistManager::FillTH2FromOneVector : Axis "<<vecAxis<<" for a TH2D is not known. "<<std::endl; return false;}

  if( (type == VariableDef::PTRVECDOUBLE) || (type == VariableDef::VECDOUBLE) ){
    std::vector < double >* vec = (type == VariableDef::PTRVECDOUBLE) 
      ? *(std::vector<double>**)t : (std::vector<double>*)t;
    for ( double value : *vec ){
      if(vecAxis=="Y"){ m_histMngr -> FillTH2D(histName, flatVal, value, weight); }
      else if(vecAxis=="X"){ m_histMngr -> FillTH2D(histName, value, flatVal, weight); }
    }
  }
  else if( (type == VariableDef::PTRVECFLOAT) || (type == VariableDef::VECFLOAT) ){
    std::vector < float >* vec = (type == VariableDef::PTRVECFLOAT) 
      ? *(std::vector<float>**)t : (std::vector<float>*)t;
    for ( double value : *vec ){
      if(vecAxis=="Y"){ m_histMngr -> FillTH2D(histName, flatVal, value, weight); }
      else if(vecAxis=="X"){ m_histMngr -> FillTH2D(histName, value, flatVal, weight); }
    }
  }
  else if( (type == VariableDef::PTRVECINT) || (type == VariableDef::VECINT) ){
    std::vector < int >* vec = (type == VariableDef::PTRVECINT) 
      ? *(std::vector<int>**)t : (std::vector<int>*)t;
    for ( double value : *vec ){
      if(vecAxis=="Y"){ m_histMngr -> FillTH2D(histName, flatVal, value, weight); }
      else if(vecAxis=="X"){ m_histMngr -> FillTH2D(histName, value, flatVal, weight); }
    }
  }
  else if( (type == VariableDef::PTRVECBOOL) || (type == VariableDef::VECBOOL) ){
    std::vector < bool >* vec = (type == VariableDef::PTRVECBOOL) 
      ? *(std::vector<bool>**)t : (std::vector<bool>*)t;
    for ( double value : *vec ){
      if(vecAxis=="Y"){ m_histMngr -> FillTH2D(histName, flatVal, value, weight); }
      else if(vecAxis=="X"){ m_histMngr -> FillTH2D(histName, value, flatVal, weight); }
    }
  }
  else if( (type == VariableDef::PTRVECAO) || (type == VariableDef::VECAO) ){
    AOVector* vec = (type == VariableDef::PTRVECAO) ? *(AOVector**)t : (AOVector*)t;
    for ( AnalysisObject* obj : *vec ){
      if( !obj-> IsKnownMoment(moment) ) continue;
      if(vecAxis=="Y"){ m_histMngr -> FillTH2D(histName, flatVal, obj->GetMoment(moment), weight); }
      else if(vecAxis=="X"){ m_histMngr -> FillTH2D(histName, obj->GetMoment(moment), flatVal, weight); }
    }
  }
  else {
    std::cerr << "<!> ERROR in OutputHistManager::FillTH1FromVector: the object type is unknown. Please check." << std::endl;
  }

  return true;
}

bool OutputHistManager::FillTH2FromAOVectors( AOVector* tX, AOVector* tY
					      , const std::string &histName, const double weight, const bool pairwise
					      , const std::string& momentX, const std::string& momentY){
  if( pairwise && (tX->size() != tY->size()) ){ 
    std::cout << "ERROR in OutputHistManager::FillTH2FromAOVectors - histograms cannot filled pairwise from vectors of different sizes" <<std::endl; 
    return false;
  }

  double valX = 0.; double valY = 0.;
  int iX = 0;
  for(AnalysisObject* compX : *tX){
    if(!compX->IsKnownMoment(momentX)) continue;
    valX = compX->GetMoment(momentX); 
    int iY = 0;
    for(AnalysisObject* compY : *tY){
      if(!compY->IsKnownMoment(momentY)) continue;
      if( ( (tX == tY) && !pairwise && (iY <= iX) ) || ( pairwise && (iX != iY) ) ){ iY++; continue; }
      valY = compY->GetMoment(momentY);
      m_histMngr -> FillTH2D(histName, valX, valY, weight );
      iY++;
    }
    iX++;
  }
  return true;

}


//________________________________________________________________________________________
//
bool OutputHistManager::SaveStandardTH2( const std::string &outputName, const bool newFile, const std::string& pattern ){
    
  TFile *f = new TFile( outputName.c_str(), ( newFile ? "recreate" : "update" ) );
  //
  // Storing TH2 in the output file
  //
  std::vector<std::string> h2list = m_histMngr->GetTH2KeyList();
  for( const auto it_h2 : h2list ){

    if(!pattern.empty() && (it_h2.find(pattern) == std::string::npos)){ continue; }

    f->cd();
    TH2D* h_temp = m_histMngr->GetTH2D(it_h2);
    h_temp -> Write();
        
    //Need to profile this TH2 ?
    bool toProfile = false;
    const std::string h_tempName = it_h2;
    for( const std::string prof_name : *m_vecH2ToProfile ){
      if(h_tempName.find(prof_name) != std::string::npos){
	toProfile = true;
	break;
      }
    }
    if(toProfile){
      std::string hname = h_tempName + "_pfx"; 
      TProfile* temp_profile = (TProfile*) h_temp -> ProfileX( hname.c_str() );
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
bool OutputHistManager::StoreTProfile( const std::string &nameX, const std::string &nameY, const bool hasSyst ){
  std::string name = nameY + "_vs_" + nameX;
    if(!hasSyst){
        if( m_vecH2ToProfile -> find(name) != m_vecH2ToProfile -> end() ){
            std::cout << " !! WARNING: In OutputHistManager::StoreTProfile(" << nameX << ", " << nameY << "): the TProfile is already in the vector !" << std::endl;
            return false;
        } else {
            m_vecH2ToProfile -> insert(name);
            return true;
        }
    } else {
        std::cout << "In OutputHistManager::StoreTProfile: The systematic version of this is not yet implemented !" << std::endl;
        return false;
    }
    return false;
}


bool OutputHistManager::UpdateStores(){

  for ( const std::pair < TString, OutputHistManager::h1Def* > h1 : *m_stdTH1Def ){
    if( !h1.second->var->IsVector() || (h1.second->var->VecInd() >= 0) ){ h1.second->var->CalcDoubleValue(); }
  }

  for ( const auto h2 : *m_stdTH2Def ){
    if( !h2.second->varX->IsVector() || (h2.second->varX->VecInd() >= 0) ){ h2.second->varX->CalcDoubleValue(); }
    if( !h2.second->varY->IsVector() || (h2.second->varY->VecInd() >= 0) ){ h2.second->varY->CalcDoubleValue(); }
  }

  return true;
}
