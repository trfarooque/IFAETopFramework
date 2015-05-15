#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H

//ROOT libraries
#include "TString.h"

//Standard libraries
#include <map>
#include <vector>

//IFAEFramework classes
#include "IFAETopFramework/SystManager.h"
#include "IFAETopFramework/VariableDef.h"
#include "IFAETopFramework/OptionsBase.h"


//class OptionsBase;
class HistManager;
class TreeManager;
class OutputData;
//class VariableDef;

class OutputManager {
    
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
      bool hasSyst;
    };

    struct h2Def{
      VariableDef varX;
      double widthX;
      double minX;
      double maxX;
      VariableDef varY;
      double widthY;
      double minY;
      double maxY;
      bool hasSyst;
    };

    //No additional structs for trees


    typedef std::map < TString, h1Def* > StdTH1;
    typedef std::map < TString, h2Def* > StdTH2;
    typedef std::map < TString, VariableDef* > StdBranches;


    //________________________
    // Member functions
    OutputManager(OptionsBase *opt, OutputType type);
    OutputManager( const OutputManager &q );
    ~OutputManager();
    
    //________________________
    // Inline functions
    bool setSystVector( SystManager::SystVector *sysVector ){ m_sysVector = sysVector; return true;}
    bool setData( OutputData *data ){ m_data = data; return true;}
    
    //
    //___________________________________________________________
    // TH1-SPECIFIC FUNCTIONS
    //
    //
    template< typename T > bool addStandardTH1( const TString &name, const double width, const double min,
                                                const double max, const TString &title, const TString &variableType,
                                                const bool hasSyst, T *t, const int vec_ind = -1) {
        
        if(!t) std::cerr << "<!> ERROR in OutputManager::addStandardTH1(template): I cannot access the pointer (" << t << "). Please check !" << std::endl;
        
        bool added = addStandardTH1(name, width, min, max, hasSyst);
        if(!added) {
            std::cerr << "<!> ERROR in OutputManager::addStandardTH1(template): could not add the variable !! Please check." << std::endl;
            return false;
        }

	VariableDef _var(name, title, variableType, t, vec_ind);

        std::map< TString , h1Def*>::iterator it = m_stdTH1Def -> find(name);
        if(it != m_stdTH1Def->end()){
	  m_stdTH1Def->at(name)->var = _var;
	}

        else std::cerr << "<!> ERROR in OutputManager::addStandardTH1(template): The searched variable has not been booked previously. Please check !" << std::endl;
        return true;
    }
    bool bookStandardTH1( const TString &pattern, const bool hasSyst = false);
    bool fillStandardTH1( const TString &name );
    bool saveStandardTH1( const TString& );


    //
    //___________________________________________________________
    // TH2-SPECIFIC FUNCTIONS
    //
    //
    template< typename TX, typename TY > bool addStandardTH2( const TString &nameX, const TString &nameY,
			   const double widthX, const double minX, const double maxX, 
			   const double widthY, const double minY, const double maxY, 
			   const TString &titleX, const TString &titleY, const TString &variableTypeX, const TString &variableTypeY,
			   const bool hasSyst, TX *tX, TY *tY, const int vec_indX = -1, const int vec_indY = -1) {
      
        if(!tX ) std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): I cannot access the pointer (" << tX << "). Please check !" << std::endl;
        if(!tY ) std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): I cannot access the pointer (" << tY << "). Please check !" << std::endl;

	TString name = nameY + "_vs_" + nameX;
        bool added = addStandardTH2(name, widthX, minX, maxX, widthY, minY, maxY, hasSyst);
        if(!added) {
            std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): could not add the variable !! Please check." << std::endl;
            return false;
        }

	VariableDef _varX(nameX,"", variableTypeX, tX, vec_indX);
	VariableDef _varY(nameY,"", variableTypeY, tY, vec_indY);

        std::map< TString , h2Def*>::iterator it = m_stdTH2Def -> find(name);
        if(it != m_stdTH2Def->end()){
	  m_stdTH2Def->at(name)->varX = _varX;
	  m_stdTH2Def->at(name)->varY = _varY;
	}

        else std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): The searched variable has not been booked previously. Please check !" << std::endl;
        return true;
    }
    bool bookStandardTH2( const TString &pattern, const bool hasSyst = false);
    bool fillStandardTH2( const TString &name );
    bool saveStandardTH2( const TString& );



    //--------------------------------------------------------------
    //
    //___________________________________________________________
    // TREE-SPECIFIC FUNCTIONS
    //
    //
    template< typename T > bool addStandardBranch( const TString &name,  const TString &title, const TString &variableType,
                                                T *t, const int vec_ind = -1) {

      if(m_opt -> msgLevel() == Debug::DEBUG){
        std::cout << "In OutputManager::addStandardBranch" << std::endl;
        std::cout << "Adding variable: "<< name << std::endl;
        std::cout << "  title  = " << title << std::endl;
      }
    
        
      if(!t) std::cerr << "<!> ERROR in OutputManager::addStandardBranch(template): I cannot access the pointer (" << t << "). Please check !" << std::endl;
    
      VariableDef *_var = new VariableDef(name, title, variableType, t, vec_ind);
	
      m_stdBranchDef -> insert( std::pair < TString, VariableDef* >( name, _var ) );
    
      if(m_opt -> msgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::addStandardTH1" << std::endl;
    
      return true;

        
    }
    bool bookStandardTree( const TString &pattern, const TString &title);
    bool fillStandardTree( const TString &name ); //probably do not need this one
    bool saveStandardTree( const TString& );



    
private:
    bool addStandardTH1(const TString name, const double width, const double min, const double max, const bool hasSyst);

    bool addStandardTH2(const TString name, const double widthX, const double minX, const double maxX,
			const double widthY, const double minY, const double maxY, const bool hasSyst);

private:
    OptionsBase *m_opt;
    OutputType m_type;
    StdTH1 *m_stdTH1Def;
    StdTH2 *m_stdTH2Def;
    StdBranches *m_stdBranchDef;

    HistManager *m_histMngr;
    TreeManager *m_treeMngr;


    SystManager::SystVector *m_sysVector;
    OutputData *m_data;
    std::map < TString, bool > *m_mapHasSyst;
};
#endif





