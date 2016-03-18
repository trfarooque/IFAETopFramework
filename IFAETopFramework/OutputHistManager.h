#ifndef OUTPUTHISTMANAGER_H
#define OUTPUTHISTMANAGER_H

//Standard libraries
#include <map>
#include <set>
#include <vector>
#include <string>

//IFAEFramework classes
//#include "IFAETopFramework/WeightManager.h"
#include "IFAETopFramework/VariableDef.h"
#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/HistManager.h"
#include "IFAETopFramework/OutputManager.h"
#include "IFAETopFramework/AnalysisObject.h"

class OutputData;

class OutputHistManager : public OutputManager {
    
public:
    
    //________________________
    //Some enums
    enum OutputType{
        HISTOS=1,
        TREES,
        ALL
    };
    
    //________________________
    // Structs to make the code readable
    struct h1Def{
      VariableDef var;
      double width;
      double min;
      double max;
      const std::vector<double>* edges;
      bool hasSyst;
      int hopt;
    };
    
    struct h2Def{
      VariableDef varX;
      double widthX;
      double minX;
      double maxX;
      const std::vector<double>* edgesX;
      VariableDef varY;
      double widthY;
      double minY;
      double maxY;
      const std::vector<double>* edgesY;
      bool hasSyst;
    };
    
    //_________________________
    // Typedefs
    typedef std::map < std::string, h1Def* > StdTH1;
    typedef std::map < std::string, h2Def* > StdTH2;
    
    //________________________
    // Member functions
    OutputHistManager(OptionsBase *opt);
    OutputHistManager( const OutputHistManager &q );
    ~OutputHistManager();
    
    //________________________
    // Inline functions
    inline HistManager* HistMngr(){ return m_histMngr; }
    
    //
    //___________________________________________________________
    // TH1-SPECIFIC FUNCTIONS
    //
    //
    
    //___________________________________________________
    //
    template< typename T > bool AddStandardTH1( const std::string &name, const double width, const double min, const double max,
						const std::string &title, const std::string &variableType,
						const bool hasSyst, T *t,  const int vec_ind = -1, const std::string& moment="", const int hopt=0) {

      return AddStandardTH1(name, width, min, max, NULL, title, variableType, hasSyst, t, vec_ind, moment, hopt);
    }
    
    template< typename T > bool AddStandardTH1( const std::string &name, const std::vector<double>* edges,
						const std::string &title, const std::string &variableType,
						const bool hasSyst, T *t, const int vec_ind = -1, const std::string& moment="", const int hopt=0) {
      return AddStandardTH1(name, 0., 0., 0., edges, title, variableType, hasSyst, t, vec_ind, moment, hopt);

    }
    
    bool BookStandardTH1( const std::string &pattern, const bool hasSyst = false);
    bool FillStandardTH1( const std::string &name );
    bool SaveStandardTH1( const std::string&, const bool newFile = true);
    
    //
    //___________________________________________________________
    // TH2-SPECIFIC FUNCTIONS
    //
    //
    //___________________________________________________
    //
    template< typename TX, typename TY > bool AddStandardTH2( const std::string &nameX, const std::string &nameY,
							      const double widthX, const double minX, const double maxX,
							      const double widthY, const double minY, const double maxY,
							      const std::string &titleX, const std::string &titleY, const std::string &variableTypeX, const std::string &variableTypeY,
							      const bool hasSyst, TX *tX, TY *tY, 
							      const int vec_indX = -1, const int vec_indY = -1, 
							      const std::string& momentX = "", const std::string& momentY = "") {
      return AddStandardTH2(nameX, nameY, widthX, minX, maxX, widthY, minY, maxY, NULL, NULL, 
			    titleX, titleY, variableTypeX, variableTypeY, hasSyst, tX, tY, vec_indX, vec_indY, momentX, momentY);
    }
    
    template< typename TX, typename TY > bool AddStandardTH2( const std::string &nameX, const std::string &nameY,
							      const double widthX, const double minX, const double maxX,
							      const std::vector<double>* edgesY,
							      const std::string &titleX, const std::string &titleY,
							      const std::string &variableTypeX, const std::string &variableTypeY,
							      const bool hasSyst, TX *tX, TY *tY,
							      const int vec_indX = -1, const int vec_indY = -1,
							      const std::string& momentX = "", const std::string& momentY = "") {
        return AddStandardTH2(nameX, nameY, widthX, minX, maxX, 0., 0., 0., NULL, edgesY,
                              titleX, titleY, variableTypeX, variableTypeY, hasSyst, tX, tY, vec_indX, vec_indY, momentX, momentY);
    }
    
    template< typename TX, typename TY > bool AddStandardTH2( const std::string &nameX, const std::string &nameY,
							      const std::vector<double>* edgesX, const std::vector<double>* edgesY,
							      const std::string &titleX, const std::string &titleY, const std::string &variableTypeX, const std::string &variableTypeY,
							      const bool hasSyst, TX *tX, TY *tY,
							      const int vec_indX = -1, const int vec_indY = -1,
							      const std::string& momentX = "", const std::string& momentY = "") {
        return AddStandardTH2(nameX, nameY, 0., 0., 0., 0., 0., 0., edgesX, edgesY,
                              titleX, titleY, variableTypeX, variableTypeY, hasSyst, tX, tY, vec_indX, vec_indY, momentX, momentY);
        
    }
    bool BookStandardTH2( const std::string &pattern, const bool hasSyst = false);
    bool FillStandardTH2( const std::string &name );
    bool SaveStandardTH2( const std::string&, const bool newFile = true );
    
    //
    // Light TProfile interface
    //
    bool StoreTProfile( const std::string &nameX, const std::string &nameY, const bool hasSyst = false );
    
private:
    
    template< typename T > bool AddStandardTH1( const std::string &name, const double width, const double min, const double max,
						const std::vector<double>* edges, 
						const std::string &title, const std::string &variableType,
						const bool hasSyst, T *t, 
						const int vec_ind = -1, const std::string& moment="", const int hopt = 0) {
        
        //
        // Don't do it if there is no systematics
        //
        if( m_opt -> OnlyDumpSystHistograms() && !hasSyst ){
            return false;
        }
        
        if(!t) std::cerr << "<!> ERROR in OutputManager::addStandardTH1(template): I cannot access the pointer (" << t << "). Please check !" << std::endl;
        
        bool added = AddStandardTH1(name, width, min, max, edges, hasSyst, hopt);
        if(!added) {
            std::cerr << "<!> ERROR in OutputManager::addStandardTH1(template): could not add the variable !! Please check." << std::endl;
            return false;
        }
        
        VariableDef _var(name, title, variableType, t, vec_ind, moment);
        
        std::map< std::string , h1Def*>::iterator it = m_stdTH1Def -> find(name);
        if(it != m_stdTH1Def->end()){
            m_stdTH1Def->at(name)->var = _var;
        }
        else {
            std::cerr << "<!> ERROR in OutputManager::addStandardTH1(template): The searched variable has not been booked previously. Please check !" << std::endl;
        }
        
        return true;
    }
    
    bool AddStandardTH1(const std::string &name, const double width, const double min, const double max, const std::vector<double>* edges, const bool hasSyst = false, const int hopt = 0);
    
    template< typename TX, typename TY > bool AddStandardTH2( const std::string &nameX, const std::string &nameY,
							      const double widthX, const double minX, const double maxX,
							      const double widthY, const double minY, const double maxY,
							      const std::vector<double>* edgesX, const std::vector<double>* edgesY,
							      const std::string &titleX, const std::string &titleY, const std::string &variableTypeX, const std::string &variableTypeY,
							      const bool hasSyst, TX *tX, TY *tY,
							      const int vec_indX = -1, const int vec_indY = -1,
							      const std::string& momentX = "", const std::string& momentY = "") {
        //
        // Don't do it if there is no systematics
        //
        if( m_opt -> OnlyDumpSystHistograms() && !hasSyst ){
            return false;
        }
        
        if( !tX ) std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): I cannot access the tX pointer (" << tX << "). Please check !" << std::endl;
        if( !tY ) std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): I cannot access the tY pointer (" << tY << "). Please check !" << std::endl;
        
        std::string name = nameY + "_vs_" + nameX;
        bool added = AddStandardTH2(name, widthX, minX, maxX, widthY, minY, maxY, edgesX, edgesY, hasSyst);
        if(!added) {
            std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): could not add the variable !! Please check." << std::endl;
            return false;
        }
        
        VariableDef _varX(nameX,titleX, variableTypeX, tX, vec_indX, momentX);
        VariableDef _varY(nameY,titleY, variableTypeY, tY, vec_indY, momentY);
        std::map< std::string , h2Def*>::iterator it = m_stdTH2Def -> find(name);
        if(it != m_stdTH2Def->end()){
            m_stdTH2Def->at(name)->varX = _varX;
            m_stdTH2Def->at(name)->varY = _varY;
        }
        
        else std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): The searched variable has not been booked previously. Please check !" << std::endl;
        return true;
    }
    
    bool AddStandardTH2(const std::string &name, const double widthX, const double minX, const double maxX,
                        const double widthY, const double minY, const double maxY, 
                        const std::vector<double>* edgesX, const std::vector<double>* edgesY, const bool hasSyst = false);
    
    bool FillTH1FromVector( void* t, const VariableDef::VariableType type, const std::string &histName, const double weight, const std::string& moment="");
    bool FillTH2FromOneVector(const double& flatVal, void* t, const VariableDef::VariableType type
			    , const std::string &histName, const double weight,  const std::string& vecAxis="Y", const std::string& moment="");

    bool FillTH2PairwiseFromVectors( void* tX, void* tY
				     , VariableDef::VariableType typeX, VariableDef::VariableType typeY
				     , const std::string &histName, const double weight
				     , const std::string& momentX="", const std::string& momentY="");


    template<typename TX, typename TY> bool FillTH2PairwiseFromPrimitiveVectors( vector<TX>* tX, vector<TY>* tY
									, const std::string &histName, const double weight){
      if( tX->size() != tY->size() ){ 
	std::cout << "ERROR in OutputHistManager::FillTH2PairwiseFromPrimitiveVectors - the given vectors are of different sizes" <<std::endl; 
	return false;
      }

      for(auto valX : *tX){ 
	for(auto valY : *tY){ 
	  m_histMngr -> FillTH2D(histName, (double)valX, (double)valY, weight );
	}
      }
      return true;

    }

    template<typename TX, typename TY> bool FillTH2PairwiseFromAOVectors( vector<TX>* tX, vector<TY>* tY
									, const std::string &histName, const double weight
									, const std::string& momentX="Pt", const std::string& momentY="Pt"){
      if( tX->size() != tY->size() ){ 
	std::cout << "ERROR in OutputHistManager::FillTH2PairwiseFromAOVectors - the given vectors are of different sizes" <<std::endl; 
	return false;
      }

      double valX = 0.; double valY = 0.;
      for(auto compX : *tX){
	valX = ((AnalysisObject*)compX)->GetMoment(momentX); 
	for(auto compY : *tY){
	  valY = ((AnalysisObject*)compY)->GetMoment(momentY);
	  m_histMngr -> FillTH2D(histName, valX, valY, weight );
	}
      }
      return true;

    }

    template<typename TX, typename TY> bool FillTH2PairwiseFromVectorsAOX( vector<TX>* tX, vector<TY>* tY
									   , const std::string &histName, const double weight
									   , const std::string& momentX="Pt"){
      if( tX->size() != tY->size() ){ 
	std::cout << "ERROR in OutputHistManager::FillTH2PairwiseFromAOVectors - the given vectors are of different sizes" <<std::endl; 
	return false;
      }

      double valX = 0.;
      for(auto compX : *tX){
	valX = ((AnalysisObject*)compX)->GetMoment(momentX); 
	for(auto valY : *tY){
	  m_histMngr -> FillTH2D(histName, valX, (double)valY, weight );
	}
      }
      return true;

    }

    template<typename TX, typename TY> bool FillTH2PairwiseFromVectorsAOY( vector<TX>* tX, vector<TY>* tY
									   , const std::string &histName, const double weight
									   , const std::string& momentY="Pt"){
      if( tX->size() != tY->size() ){ 
	std::cout << "ERROR in OutputHistManager::FillTH2PairwiseFromAOVectors - the given vectors are of different sizes" <<std::endl; 
	return false;
      }

      double valY = 0.;
      for(auto valX : *tX){
	for(auto compY : *tY){
	  valY = ((AnalysisObject*)compY)->GetMoment(momentY); 
	  m_histMngr -> FillTH2D(histName, (double)valX, valY, weight );
	}
      }
      return true;

    }


private:
    StdTH1 *m_stdTH1Def;
    StdTH1 *m_stdTProfileDef;
    StdTH2 *m_stdTH2Def;
    
    HistManager *m_histMngr;
    
    std::set < std::string > *m_vecH2ToProfile;
    
};

#endif // OUTPUTMANAGER_H





