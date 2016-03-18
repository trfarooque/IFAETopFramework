//Standard includes
#include <iostream>
#include <string>

//IFAETopFramework includes
#include "IFAETopFramework/OutputHistManager.h"
#include "IFAETopFramework/OutputData.h"
#include "IFAETopFramework/HistManager.h"
//ROOT libraries
#include "TSystem.h"

//______________________________________________________________________________________
//
OutputHistManager::OutputHistManager( OptionsBase* opt):
  OutputManager(opt),
  m_stdTH1Def(0),
  m_stdTProfileDef(0),
  m_stdTH2Def(0),
  m_histMngr(0),
  m_vecH2ToProfile(0)
{
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Entering in OutputHistManager constructor" << std::endl;
    
    m_stdTH1Def     = new StdTH1();
    m_stdTProfileDef= new StdTH1();
    m_stdTH2Def     = new StdTH2();
    m_histMngr      = new HistManager(  m_opt->AddUnderflow(), m_opt->AddOverflow() );
    m_vecH2ToProfile= new std::set < std::string >;
    m_vecH2ToProfile->clear();
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputHistManager constructor" << std::endl;
}

//______________________________________________________________________________________
//
OutputHistManager::OutputHistManager( const OutputHistManager &q ) : 
  OutputManager(q)
{
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputHistManager copy-constructor" << std::endl;
    
    m_stdTH1Def     = q.m_stdTH1Def;
    m_stdTProfileDef   = q.m_stdTProfileDef;
    m_stdTH2Def     = q.m_stdTH2Def;
    m_histMngr      = q.m_histMngr;
    m_vecH2ToProfile= q.m_vecH2ToProfile;
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputHistManager copy-constructor" << std::endl;
}

//______________________________________________________________________________________
//
OutputHistManager::~OutputHistManager()
{
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputHistManager destructor" << std::endl;
    delete m_stdTH1Def;
    delete m_stdTProfileDef;
    delete m_stdTH2Def;
    delete m_histMngr;
    delete m_vecH2ToProfile;
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputHistManager destructor" << std::endl;
}

//-----------------------------TH1-SPECIFIC METHODS-------------------------------
//______________________________________________________________________________________
//
bool OutputHistManager::AddStandardTH1(const std::string &name, const double width, const double min, const double max, const std::vector<double>* edges, const bool hasSyst, const int hopt){
    
    //
    // INTERNAL FUNCTION
    //
    
    if(m_opt -> MsgLevel() == Debug::DEBUG){
        std::cout << "In OutputHistManager::addStandardTH1" << std::endl;
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
    m_stdTH1Def -> insert( std::pair < std::string, h1Def* >( name, hist ) );
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputHistManager::addStandardTH1" << std::endl;
    
    return true;
}

//______________________________________________________________________________________
//
bool OutputHistManager::BookStandardTH1( const std::string &pattern, const bool hasSyst){
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputHistManager::bookStandardTH1" << std::endl;
    
    //
    // Don't do it if there is no systematics
    //
    if( m_opt -> OnlyDumpSystHistograms() && !hasSyst ){
        return false;
    }
    
    m_mapHasSyst -> insert( std::pair <std::string, bool>(pattern,hasSyst));
    
    //
    // Loop over the registered histograms (to be booked for nominal and weight systematics)
    //
    for ( const auto h1 : *m_stdTH1Def ){
        
      std::string histName = pattern + "_" + h1.second->var.Name();
      
      const double* ptr_edges = (h1.second -> edges != NULL) ? &( h1.second->edges->at(0) ) : NULL;
      int nbins = ( (h1.second -> edges != NULL) && (h1.second -> edges->size() > 0) )? h1.second->edges->size() - 1 : 0;
      
      if(nbins > 0){        
	  m_histMngr -> BookTH1D( histName, h1.second->var.Title(), nbins, ptr_edges);
        }
	else{
	  m_histMngr -> BookTH1D( histName, h1.second->var.Title(), h1.second->width, h1.second->min, h1.second->max);
	}
	if(h1.second->hopt > 0){ m_histMngr->SetTH1Opt(histName, h1.second->hopt); }

        if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << histName << std::endl;
        
        if(hasSyst && h1.second->hasSyst){
            if(!m_sysMap){
                std::cerr << "<!> ERROR in OutputHistManager::bookStandardTH1: You want to use systematics, but none is defined ... Please check !" << std::endl;
            } else {
                for (const auto &sys : *m_sysMap) {
		  std::string systHistName = histName + "_" + sys.second->Name();
		  if(nbins > 0){  
		    m_histMngr -> BookTH1D( systHistName, h1.second->var.Title(), nbins, ptr_edges);
		  }
		  else{
		    m_histMngr -> BookTH1D( systHistName, h1.second->var.Title(), h1.second->width, h1.second->min, h1.second->max);
		  }
		  if(h1.second->hopt > 0){ m_histMngr->SetTH1Opt( systHistName, h1.second->hopt ); }
		  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << systHistName << std::endl;
                }
            }
        }
    }
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputHistManager::bookStandardTH1" << std::endl;
    return true;
}

//________________________________________________________________________________________
//
bool OutputHistManager::FillStandardTH1( const std::string &pattern ){
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Entering in OutputHistManager::fillStandardTH1("<<pattern<<")" << std::endl;
    
    if(!m_data){
        std::cerr << "<!> ERROR in OutputHistManager::fillStandardTH1: We have big problems ... Please provide an OutputData object" << std::endl;
        return false;
    }
    
    for ( const std::pair < TString, OutputHistManager::h1Def* > h1 : *m_stdTH1Def ){
        
        //
        // Nominal histogram filling
        //
      std::string histName = pattern + "_" + h1.second->var.Name();
        
      if( !h1.second->var.IsVector() || (h1.second->var.VecInd() >= 0) ){
	h1.second->var.CalcDoubleValue();
	m_histMngr -> FillTH1D(histName, h1.second->var.GetDoubleValue(), m_data->o_eventWeight_Nom);
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
	if(!m_sysMap){
	  std::cerr << "<!> ERROR in OutputHistManager::bookStandardTH1: You want to use systematics, but none is defined ... Please check !" << std::endl;
	} else {
	  for (const auto sys : *m_sysMap) {
	    std::string systHistName = histName + "_" + sys.second->Name();
	    sys.second->CalcDoubleValue();

	    if( !h1.second->var.IsVector() || (h1.second->var.VecInd() >= 0) ){
	      m_histMngr -> FillTH1D(systHistName, h1.second->var.GetDoubleValue(), sys.second->GetDoubleValue());
	    } else {
	      FillTH1FromVector( h1.second->var.Address(),
				 h1.second->var.VarType(), systHistName, sys.second->GetDoubleValue(), h1.second->var.Moment());
	    }
	    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Filled histogram : " << systHistName << std::endl;
	  }
	}
      }
    }
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputHistManager::fillStandardTH1("<<pattern<<")" << std::endl;
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
  if( (type == VariableDef::VECDOUBLE) ){
    std::vector < double >* vec = (std::vector<double>*)t;
    for ( double value : *vec ){
      m_histMngr -> FillTH1D(histName, value, weight);
    }

  }
  else if(type == VariableDef::VECFLOAT){
    std::vector < float >* vec = (std::vector< float >*)t;
    for ( double value : *vec ){
      m_histMngr -> FillTH1D(histName, value, weight);
    }

  }
  else if(type == VariableDef::VECINT){
    std::vector < int >* vec = (std::vector< int >*)t;
    for ( double value : *vec ){
      m_histMngr -> FillTH1D(histName, value, weight);
    }

  }
  else if(type == VariableDef::VECAO){
    AOVector* vec = (AOVector*)t;
    for ( AnalysisObject* obj : *vec ){
      m_histMngr -> FillTH1D(histName, obj->GetMoment(moment), weight);
    }
  }
  else {
    std::cerr << "<!> ERROR in OutputHistManager::FillTH1FromVector: the object type is unknown. Please check." << std::endl;
  }
  return true;
}

//________________________________________________________________________________________
//
bool OutputHistManager::SaveStandardTH1( const std::string &outputName, const bool newFile){
   
  TFile *f = new TFile( outputName.c_str(), ( newFile ? "recreate" : "update" ) );
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
bool OutputHistManager::AddStandardTH2( const std::string &name, const double widthX, const double minX, const double maxX,
				    const double widthY, const double minY, const double maxY, 
				    const std::vector<double>* edgesX, const std::vector<double>* edgesY,
				    const bool hasSyst){
    
    //
    // INTERNAL FUNCTION
    //
    
    if(m_opt -> MsgLevel() == Debug::DEBUG){
        std::cout << "In OutputHistManager::addStandardTH2" << std::endl;
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
    
    m_stdTH2Def -> insert( std::pair < std::string, h2Def* >( name, hist ) );
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputHistManager::addStandardTH2" << std::endl;
    
    return true;
}

//______________________________________________________________________________________
//
bool OutputHistManager::BookStandardTH2( const std::string &pattern, const bool hasSyst){
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "In OutputHistManager::bookStandardTH2" << std::endl;
    
    //
    // Don't do it if there is no systematics
    //
    if( m_opt -> OnlyDumpSystHistograms() && !hasSyst ){
        return false;
    }
    
    m_mapHasSyst -> insert( std::pair <std::string, bool>(pattern,hasSyst));
    
    for ( const auto h2 : *m_stdTH2Def ){
        
      std::string histName = pattern + "_" + h2.second->varY.Name() + "_vs_" + h2.second->varX.Name();
      std::string histTitle = h2.second->varY.Title() + " vs " + h2.second->varX.Title();

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

        if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << histName << std::endl;
        
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
		  
		  
                    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Booked histogram : " << systHistName << std::endl;
                }
            }
        }
    }
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputHistManager::bookStandardTH2" << std::endl;
    return true;
}

//________________________________________________________________________________________
//
bool OutputHistManager::FillStandardTH2( const std::string &pattern ){
    
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Entering in OutputHistManager::fillStandardTH2("<<pattern<<")" << std::endl;
    if(!m_data){
        std::cerr << "<!> ERROR in OutputHistManager::fillStandardTH2: We have big problems ... Please provide an OutputData object" << std::endl;
        return false;
    }
    
    for ( const auto h2 : *m_stdTH2Def ){
        
      //
      // Nominal histogram filling
      //
      std::string histName = pattern + "_" + h2.second->varY.Name() + "_vs_" + h2.second->varX.Name();
      bool bX_flat =  !h2.second->varX.IsVector() || (h2.second->varX.VecInd() >= 0) ; 
      bool bY_flat =  !h2.second->varY.IsVector() || (h2.second->varY.VecInd() >= 0) ; 
      if( bX_flat ){ h2.second->varX.CalcDoubleValue(); }
      if( bY_flat ){ h2.second->varY.CalcDoubleValue(); }
      if( bX_flat && bY_flat ){
	m_histMngr -> FillTH2D(histName, h2.second->varX.GetDoubleValue(), h2.second->varY.GetDoubleValue(), m_data->o_eventWeight_Nom);
      }
      else if( bX_flat && !bY_flat){
	FillTH2FromOneVector(h2.second->varX.GetDoubleValue(), h2.second->varY.Address(), h2.second->varY.VarType() 
			     , histName, m_data->o_eventWeight_Nom, "Y", h2.second->varY.Moment());
      }
      else if( !bX_flat && bY_flat){
	FillTH2FromOneVector(h2.second->varY.GetDoubleValue(), h2.second->varX.Address(), h2.second->varX.VarType() 
			     , histName, m_data->o_eventWeight_Nom, "X", h2.second->varX.Moment());
      }
      else{

	FillTH2PairwiseFromVectors(h2.second->varX.Address(), h2.second->varY.Address()
				   , h2.second->varX.VarType() , h2.second->varY.VarType() 
				   , histName, m_data->o_eventWeight_Nom, h2.second->varX.Moment(), h2.second->varY.Moment());

      }

      if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Filled histogram : " << histName << std::endl;
        
      //
      // Now checking systematics (if needed and if existing)
      //
      if(h2.second->hasSyst && m_mapHasSyst->at(pattern)){
	if(!m_sysMap){
	  std::cerr << "<!> ERROR in OutputHistManager::bookStandardTH2: You want to use systematics, but none is defined ... Please check !" << std::endl;
	} else {
	  for(const auto sys : *m_sysMap) {
	    std::string systHistName = histName + "_" + sys.second->Name();
	    sys.second->CalcDoubleValue();

	    if( bX_flat && bY_flat ){
	      m_histMngr -> FillTH2D(systHistName, h2.second->varX.GetDoubleValue(), h2.second->varY.GetDoubleValue(), sys.second->GetDoubleValue());
	    }
	    else if( bX_flat && !bY_flat){
	      FillTH2FromOneVector(h2.second->varX.GetDoubleValue(), h2.second->varY.Address(), h2.second->varY.VarType() 
				   , systHistName, sys.second->GetDoubleValue(), "Y", h2.second->varY.Moment());
	    }
	    else if( !bX_flat && bY_flat){
	      FillTH2FromOneVector(h2.second->varY.GetDoubleValue(), h2.second->varX.Address(), h2.second->varX.VarType() 
				   , systHistName, sys.second->GetDoubleValue(), "X", h2.second->varX.Moment());
	    }
	    else{

	      FillTH2PairwiseFromVectors(h2.second->varX.Address(), h2.second->varY.Address()
					 , h2.second->varX.VarType() , h2.second->varY.VarType() 
					 , systHistName, sys.second->GetDoubleValue(), h2.second->varX.Moment(), h2.second->varY.Moment());

	    }

	    m_histMngr -> FillTH2D(systHistName, h2.second->varX.GetDoubleValue(), h2.second->varY.GetDoubleValue(), sys.second->GetDoubleValue());
	    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "  -> Filled histogram : " << systHistName << std::endl;
	  }
	}
      }
    }
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputHistManager::fillStandardTH2("<<pattern<<")" << std::endl;
    return true;
}

//__________________________________________________________________________________________
//
bool OutputHistManager::FillTH2PairwiseFromVectors( void* tX, void* tY
						    , VariableDef::VariableType typeX, VariableDef::VariableType typeY
						    , const std::string &histName, const double weight
						    , const std::string& momentX, const std::string& momentY){


  if( (typeX == VariableDef::VECDOUBLE) && (typeY == VariableDef::VECDOUBLE) ){
    FillTH2PairwiseFromPrimitiveVectors( (std::vector<double>*)tX, (std::vector<double>*)tY, histName, weight );
  }
  else if( (typeX == VariableDef::VECDOUBLE) && (typeY == VariableDef::VECFLOAT) ){
    FillTH2PairwiseFromPrimitiveVectors( (std::vector<double>*)tX, (std::vector<float>*)tY, histName, weight );
  }
  else if( (typeX == VariableDef::VECDOUBLE) && (typeY == VariableDef::VECINT) ){
    FillTH2PairwiseFromPrimitiveVectors( (std::vector<double>*)tX, (std::vector<int>*)tY, histName, weight );
  }
  if( (typeX == VariableDef::VECDOUBLE) && (typeY == VariableDef::VECAO) ){
    FillTH2PairwiseFromVectorsAOY( (std::vector<double>*)tX, (std::vector<AnalysisObject*>*)tY, histName, weight, momentY );
  }

  //-
  if( (typeX == VariableDef::VECFLOAT) && (typeY == VariableDef::VECDOUBLE) ){
    FillTH2PairwiseFromPrimitiveVectors( (std::vector<float>*)tX, (std::vector<double>*)tY, histName, weight );
  }
  else if( (typeX == VariableDef::VECFLOAT) && (typeY == VariableDef::VECFLOAT) ){
    FillTH2PairwiseFromPrimitiveVectors( (std::vector<float>*)tX, (std::vector<float>*)tY, histName, weight );
  }
  else if( (typeX == VariableDef::VECFLOAT) && (typeY == VariableDef::VECINT) ){
    FillTH2PairwiseFromPrimitiveVectors( (std::vector<float>*)tX, (std::vector<int>*)tY, histName, weight );
  }
  if( (typeX == VariableDef::VECFLOAT) && (typeY == VariableDef::VECAO) ){
    FillTH2PairwiseFromVectorsAOY( (std::vector<float>*)tX, (std::vector<AnalysisObject*>*)tY, histName, weight, momentY );
  }

  //-
  if( (typeX == VariableDef::VECINT) && (typeY == VariableDef::VECDOUBLE) ){
    FillTH2PairwiseFromPrimitiveVectors( (std::vector<int>*)tX, (std::vector<double>*)tY, histName, weight );
  }
  else if( (typeX == VariableDef::VECINT) && (typeY == VariableDef::VECFLOAT) ){
    FillTH2PairwiseFromPrimitiveVectors( (std::vector<int>*)tX, (std::vector<float>*)tY, histName, weight );
  }
  else if( (typeX == VariableDef::VECINT) && (typeY == VariableDef::VECINT) ){
    FillTH2PairwiseFromPrimitiveVectors( (std::vector<int>*)tX, (std::vector<int>*)tY, histName, weight );
  }
  if( (typeX == VariableDef::VECINT) && (typeY == VariableDef::VECAO) ){
    FillTH2PairwiseFromVectorsAOY( (std::vector<int>*)tX, (std::vector<AnalysisObject*>*)tY, histName, weight, momentY );
  }

  //-
  if( (typeX == VariableDef::VECAO) && (typeY == VariableDef::VECDOUBLE) ){
    FillTH2PairwiseFromVectorsAOX( (std::vector<AnalysisObject*>*)tX, (std::vector<double>*)tY, histName, weight, momentX  );
  }
  else if( (typeX == VariableDef::VECAO) && (typeY == VariableDef::VECFLOAT) ){
    FillTH2PairwiseFromVectorsAOX( (std::vector<AnalysisObject*>*)tX, (std::vector<float>*)tY, histName, weight, momentX  );
  }
  else if( (typeX == VariableDef::VECAO) && (typeY == VariableDef::VECINT) ){
    FillTH2PairwiseFromVectorsAOX( (std::vector<AnalysisObject*>*)tX, (std::vector<int>*)tY, histName, weight, momentX  );
  }
  if( (typeX == VariableDef::VECAO) && (typeY == VariableDef::VECAO) ){
    FillTH2PairwiseFromAOVectors( (std::vector<AnalysisObject*>*)tX, (std::vector<AnalysisObject*>*)tY, histName, weight, momentX, momentY  );
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

  if( (type == VariableDef::VECDOUBLE) ){
    std::vector < double >* vec = (std::vector<double>*)t;
    for ( double value : *vec ){
      if(vecAxis=="Y"){ m_histMngr -> FillTH2D(histName, flatVal, value, weight); }
      else if(vecAxis=="X"){ m_histMngr -> FillTH2D(histName, value, flatVal, weight); }
    }

  }
  else if(type == VariableDef::VECFLOAT){
    std::vector < float >* vec = (std::vector< float >*)t;
    for ( double value : *vec ){
      if(vecAxis=="Y"){ m_histMngr -> FillTH2D(histName, flatVal, value, weight); }
      else if(vecAxis=="X"){ m_histMngr -> FillTH2D(histName, value, flatVal, weight); }
    }

  }
  else if(type == VariableDef::VECINT){
    std::vector < int >* vec = (std::vector< int >*)t;
    for ( double value : *vec ){
      if(vecAxis=="Y"){ m_histMngr -> FillTH2D(histName, flatVal, value, weight); }
      else if(vecAxis=="X"){ m_histMngr -> FillTH2D(histName, value, flatVal, weight); }
    }

  }
  else if(type == VariableDef::VECAO){
    AOVector* vec = (AOVector*)t;
    for ( AnalysisObject* obj : *vec ){
      if(vecAxis=="Y"){ m_histMngr -> FillTH2D(histName, flatVal, obj->GetMoment(moment), weight); }
      else if(vecAxis=="X"){ m_histMngr -> FillTH2D(histName, obj->GetMoment(moment), flatVal, weight); }
    }
  }
  else {
    std::cerr << "<!> ERROR in OutputHistManager::FillTH1FromVector: the object type is unknown. Please check." << std::endl;
  }
  return true;
}

//________________________________________________________________________________________
//
bool OutputHistManager::SaveStandardTH2( const std::string &outputName, const bool newFile ){
    
  TFile *f = new TFile( outputName.c_str(), ( newFile ? "recreate" : "update" ) );
  //
  // Storing TH2 in the output file
  //
  std::vector<std::string> h2list = m_histMngr->GetTH2KeyList();
  for( const auto it_h2 : h2list ){
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



