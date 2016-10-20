#ifndef OUTPUTHISTMANAGER_H
#define OUTPUTHISTMANAGER_H

//Standard libraries
#include <map>
#include <set>
#include <vector>
#include <string>

//IFAEFramework classes
#include "IFAETopFramework/WeightManager.h"
#include "IFAETopFramework/VariableDef.h"
#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/HistManager.h"
#include "IFAETopFramework/OutputManager.h"
#include "IFAETopFramework/AnalysisObject.h"

class OutputData;

class OutputHistManager : public OutputManager {
    
public:
    
    //________________________
    // Structs to make the code readable
    struct h1Def{
      VariableDef* var;
      double width;
      double min;
      double max;
      const std::vector<double>* edges;
      bool hasSyst;
      bool noWeight;
      int hopt;
    };
    
    struct h2Def{
      VariableDef* varX;
      double widthX;
      double minX;
      double maxX;
      const std::vector<double>* edgesX;
      VariableDef* varY;
      double widthY;
      double minY;
      double maxY;
      const std::vector<double>* edgesY;
      bool hasSyst;
      bool noWeight;
      bool pairwise;
    };
    
    //_________________________
    // Typedefs
    typedef std::map < std::string, h1Def* > StdTH1;
    typedef std::map < std::string, h2Def* > StdTH2;
    
    //________________________
    // Member functions
    OutputHistManager(OptionsBase *opt, OutputData* data=NULL, WeightManager::WeightMap *sysMap=NULL);
    OutputHistManager( const OutputHistManager &q );
    ~OutputHistManager();
    
    //________________________
    // Inline functions
    inline HistManager* HistMngr(){ return m_histMngr; }
    inline void SetSystMap( WeightManager::WeightMap *sysMap ){ m_sysMap = sysMap; }
    
    //
    //___________________________________________________________
    // TH1-SPECIFIC FUNCTIONS
    //
    //
    
    //___________________________________________________
    //
    template< typename T > bool AddStandardTH1( const std::string &name, const double width, const double min, const double max,
						const std::string &title,
						const bool hasSyst, T *t,  const int vec_ind = -1, const std::string& moment="", const int hopt=0, const bool noWeight=false) {

      if(noWeight && hasSyst){ std::cout << "Warning in OutputHistManager::AddStandardTH1 --> Unweighted histogram "<< name << " requested with systematics. Please check"<<std::endl;}
      bool _hasSyst = hasSyst && !noWeight;
      return AddStandardTH1(name, width, min, max, NULL, title, _hasSyst, t, vec_ind, moment, hopt, noWeight);
    }
    
    template< typename T > bool AddStandardTH1( const std::string &name, const std::vector<double>* edges,
						const std::string &title,
						const bool hasSyst, T *t, const int vec_ind = -1, const std::string& moment="", const int hopt=0, const bool noWeight=false) {
      if(noWeight && hasSyst){ std::cout << "Warning in OutputHistManager::AddStandardTH1 --> Unweighted histogram "<< name << " requested with systematics. Please check"<<std::endl;}
      bool _hasSyst = hasSyst && !noWeight;
      return AddStandardTH1(name, 0., 0., 0., edges, title, _hasSyst, t, vec_ind, moment, hopt, noWeight);

    }
    
    bool BookStandardTH1( const std::string &pattern, const bool hasSyst = false);
    bool FillStandardTH1( const std::string &name, const bool updateStores = true );
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
							      const std::string &titleX, const std::string &titleY, 
							      const bool hasSyst, TX *tX, TY *tY, 
							      const int vec_indX = -1, const int vec_indY = -1, const bool pairwise=true, 
							      const std::string& momentX = "", const std::string& momentY = "", const bool noWeight=false) {
      if(noWeight && hasSyst){ std::cout << "Warning in OutputHistManager::AddStandardTH2 --> Unweighted histogram "<< nameY << "_vs_" << nameX << " requested with systematics. Please check"<<std::endl;}
      bool _hasSyst = hasSyst && !noWeight;
      return AddStandardTH2(nameX, nameY, widthX, minX, maxX, widthY, minY, maxY, NULL, NULL, 
			    titleX, titleY, _hasSyst, tX, tY, vec_indX, vec_indY, pairwise, momentX, momentY, noWeight);
    }
    
    template< typename TX, typename TY > bool AddStandardTH2( const std::string &nameX, const std::string &nameY,
							      const double widthX, const double minX, const double maxX,
							      const std::vector<double>* edgesY,
							      const std::string &titleX, const std::string &titleY,
							      const bool hasSyst, TX *tX, TY *tY,
							      const int vec_indX = -1, const int vec_indY = -1, const bool pairwise=true, 
							      const std::string& momentX = "", const std::string& momentY = "", const bool noWeight=false) {
      if(noWeight && hasSyst){ std::cout << "Warning in OutputHistManager::AddStandardTH2 --> Unweighted histogram "<< nameY << "_vs_" << nameX << " requested with systematics. Please check"<<std::endl;}
      bool _hasSyst = hasSyst && !noWeight;
      return AddStandardTH2(nameX, nameY, widthX, minX, maxX, 0., 0., 0., NULL, edgesY,
                              titleX, titleY, _hasSyst, tX, tY,
			      vec_indX, vec_indY, pairwise, momentX, momentY, noWeight);
    }
    
    template< typename TX, typename TY > bool AddStandardTH2( const std::string &nameX, const std::string &nameY,
							      const std::vector<double>* edgesX, const std::vector<double>* edgesY,
							      const std::string &titleX, const std::string &titleY, 
							      const bool hasSyst, TX *tX, TY *tY,
							      const int vec_indX = -1, const int vec_indY = -1, const bool pairwise=true, 
							      const std::string& momentX = "", const std::string& momentY = "", const bool noWeight=false) {
      if(noWeight && hasSyst){ std::cout << "Warning in OutputHistManager::AddStandardTH2 --> Unweighted histogram "<< nameY << "_vs_" << nameX << " requested with systematics. Please check"<<std::endl;}
      bool _hasSyst = hasSyst && !noWeight;
      return AddStandardTH2(nameX, nameY, 0., 0., 0., 0., 0., 0., edgesX, edgesY,
                              titleX, titleY, _hasSyst, tX, tY, 
			      vec_indX, vec_indY, pairwise, momentX, momentY, noWeight);
        
    }
    bool BookStandardTH2( const std::string &pattern, const bool hasSyst = false);
    bool FillStandardTH2( const std::string &name, const bool updateStores = true );
    bool SaveStandardTH2( const std::string&, const bool newFile = true );
    
    //
    // Light TProfile interface
    //
    bool StoreTProfile( const std::string &nameX, const std::string &nameY, const bool hasSyst = false );

    //
    //Update all the VariableDef stores associated to standard histograms. Use judiciously - 
    //this is most useful when the number of selection regions far exceeds the actual number of distributions
    bool UpdateStores();

    
private:
    
    template< typename T > bool AddStandardTH1( const std::string &name, const double width, const double min, const double max,
						const std::vector<double>* edges, 
						const std::string &title,
						const bool hasSyst, T *t, 
						const int vec_ind = -1, const std::string& moment="", const int hopt = 0, const bool noWeight=false) {
      bool _hasSyst = hasSyst && !noWeight;

        //
        // Don't do it if there is no systematics
        //
        if( m_opt -> OnlyDumpSystHistograms() && !_hasSyst ){
            return false;
        }
        
        if(!t) std::cerr << "<!> ERROR in OutputManager::addStandardTH1(template): I cannot access the pointer (" << t << "). Please check !" << std::endl;
        
        bool added = AddStandardTH1(name, width, min, max, edges, _hasSyst, hopt, noWeight);
        if(!added) {
            std::cerr << "<!> ERROR in OutputManager::addStandardTH1(template): could not add the variable !! Please check." << std::endl;
            return false;
        }
        
        VariableDef* _var = new VariableDef(name, title, t, vec_ind, moment);
        
        std::map< std::string , h1Def*>::iterator it = m_stdTH1Def -> find(name);
        if(it != m_stdTH1Def->end()){
            m_stdTH1Def->at(name)->var = _var;
        }
        else {
            std::cerr << "<!> ERROR in OutputManager::addStandardTH1(template): The searched variable has not been booked previously. Please check !" << std::endl;
        }
        
        return true;
    }
    
    bool AddStandardTH1(const std::string &name, const double width, const double min, const double max, const std::vector<double>* edges, const bool hasSyst = false, const int hopt = 0, const bool noWeight=false);
    
    template< typename TX, typename TY > bool AddStandardTH2( const std::string &nameX, const std::string &nameY,
							      const double widthX, const double minX, const double maxX,
							      const double widthY, const double minY, const double maxY,
							      const std::vector<double>* edgesX, const std::vector<double>* edgesY,
							      const std::string &titleX, const std::string &titleY,
							      const bool hasSyst, TX *tX, TY *tY,
							      const int vec_indX = -1, const int vec_indY = -1, const bool pairwise = true,  
							      const std::string& momentX = "", const std::string& momentY = "", const bool noWeight=false) {
      bool _hasSyst = hasSyst && !noWeight;

        //
        // Don't do it if there is no systematics
        //
        if( m_opt -> OnlyDumpSystHistograms() && !_hasSyst ){
            return false;
        }
        
        if( !tX ) std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): I cannot access the tX pointer (" << tX << "). Please check !" << std::endl;
        if( !tY ) std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): I cannot access the tY pointer (" << tY << "). Please check !" << std::endl;
        
        std::string name = nameY + "_vs_" + nameX;
        bool added = AddStandardTH2(name, widthX, minX, maxX, widthY, minY, maxY, edgesX, edgesY, _hasSyst, pairwise, noWeight);
        if(!added) {
            std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): could not add the variable !! Please check." << std::endl;
            return false;
        }
        
        VariableDef*  _varX = new VariableDef(nameX,titleX, tX, vec_indX, momentX);
        VariableDef* _varY = new VariableDef(nameY,titleY, tY, vec_indY, momentY);
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
                        const std::vector<double>* edgesX, const std::vector<double>* edgesY, const bool hasSyst = false, const bool pairwise = true, const bool noWeight=false);
    
    bool FillTH1FromVector( void* t, const VariableDef::VariableType type, 
			    const std::string &histName, const double weight, const std::string& moment="");
    bool FillTH2FromOneVector(const double& flatVal, void* t, const VariableDef::VariableType type, 
			      const std::string &histName, const double weight,  const std::string& vecAxis="Y", const std::string& moment="");

    bool FillTH2FromVectors( void* tX, void* tY, const VariableDef::VariableType typeX, const VariableDef::VariableType typeY
			     , const std::string &histName, const double weight, const bool pairwise=true
			     , const std::string& momentX="", const std::string& momentY="");
    

    template<typename TX, typename TY> bool FillTH2FromPrimitiveVectors( vector<TX>* tX, vector<TY>* tY
									 , const std::string &histName, const double weight, bool pairwise=true, bool same=false){
      if(pairwise && ( tX->size() != tY->size() ) ){ 
	std::cout << "ERROR in OutputHistManager::FillTH2FromPrimitiveVectors - histogram cannot be filled pairwise using vectors of different sizes" <<std::endl; 
	return false;
      }

      //Looping over two vectors
      //If they are the same vectors, then diagonal elements should not be filled
      //If filling pairwise only, off-diagonal elements are not to be filled

      int iX = 0;
      for(auto valX : *tX){ 
	int iY = 0;
	for(auto valY : *tY){
	  if( (pairwise && (iX!=iY) ) || ( same && (iY <= iX) ) ){ iY++; continue; }
	  m_histMngr -> FillTH2D(histName, (double)valX, (double)valY, weight );
	  iY++;
	}
	iX++;
      }
      return true;

    }

    template<typename T> bool FillTH2FromFlatAndAOVectors( AOVector* tAO, vector<T>* tPrim
							   , const std::string &histName, const double weight, bool pairwise=true
							   , const std::string& moment="Pt", const std::string& ao_axis = "Y"){
      int iPrim = 0;
      for(auto val : *tPrim){ 
	int iAO = 0; 
	for(auto compAO : *tAO){
	  if( (pairwise && (iAO != iPrim) ) ){ iAO++; continue; }
	  double valAO = ((AnalysisObject*)(compAO))->GetMoment(moment);
	  if(ao_axis=="Y") m_histMngr -> FillTH2D(histName, (double)val, valAO, weight );
	  else if(ao_axis =="X") m_histMngr -> FillTH2D(histName, valAO, (double)val, weight );
	  else{ std::cerr << " OutputHistManager::FillTH2FromFlatAndAOVectors --> ERROR : Axis "<<ao_axis<<" is not recognised "<<std::endl; }
	  iAO++;
	}
	iPrim++;
      }
      return true;
      
    }
    
    bool FillTH2FromAOVectors( AOVector* tX, AOVector* tY
			       , const std::string &histName, const double weight, const bool pairwise=true
			       , const std::string& momentX="Pt", const std::string& momentY="Pt");
    

private:
    StdTH1 *m_stdTH1Def;
    StdTH1 *m_stdTProfileDef;
    StdTH2 *m_stdTH2Def;
    
    HistManager *m_histMngr;
    WeightManager::WeightMap *m_sysMap;
    std::map < std::string, bool > *m_mapHasSyst;
    
    std::set < std::string > *m_vecH2ToProfile;
    
};

#endif // OUTPUTMANAGER_H





