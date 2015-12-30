#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H

//ROOT libraries
#include "TString.h"

//Standard libraries
#include <map>
#include <set>
#include <vector>

//IFAEFramework classes
#include "IFAETopFramework/SystManager.h"
#include "IFAETopFramework/VariableDef.h"
#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/HistManager.h"

class TreeManager;
class OutputData;

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
    typedef std::map < TString, h1Def* > StdTH1;
    typedef std::map < TString, h2Def* > StdTH2;
    typedef std::map < TString, VariableDef* > StdBranches;
    
    //________________________
    // Member functions
    OutputManager(OptionsBase *opt, OutputType type);
    OutputManager( const OutputManager &q );
    ~OutputManager();
    
    bool SetSystVector( SystManager::SystVector *sysVector );
    
    
    //________________________
    // Inline functions
    inline bool SetData( OutputData *data ){ m_data = data; return true;}
    inline HistManager* HistMngr(){ return m_histMngr; }
    inline void SetWeightBranchName( const std::string &name ) { m_weightVarName = name; }
    
    //
    //___________________________________________________________
    // TH1-SPECIFIC FUNCTIONS
    //
    //
    
    //___________________________________________________
    //
    template< typename T > bool AddStandardTH1( const TString &name, const double width, const double min, const double max,
						const TString &title, const TString &variableType,
						const bool hasSyst, T *t,  const int vec_ind = -1, const int hopt=0) {

      return AddStandardTH1(name, width, min, max, NULL, title, variableType, hasSyst, t, vec_ind, hopt);
    }
    
    template< typename T > bool AddStandardTH1( const TString &name, const std::vector<double>* edges,
						const TString &title, const TString &variableType,
						const bool hasSyst, T *t, const int vec_ind = -1, const int hopt=0) {
      return AddStandardTH1(name, 0., 0., 0., edges, title, variableType, hasSyst, t, vec_ind, hopt);

    }
    
    bool BookStandardTH1( const TString &pattern, const bool hasSyst = false);
    bool FillStandardTH1( const TString &name );
    bool SaveStandardTH1( const TString&, const bool newFile = true);
    
    //
    //___________________________________________________________
    // TH2-SPECIFIC FUNCTIONS
    //
    //
    //___________________________________________________
    //
    template< typename TX, typename TY > bool AddStandardTH2( const TString &nameX, const TString &nameY,
							      const double widthX, const double minX, const double maxX,
							      const double widthY, const double minY, const double maxY,
							      const TString &titleX, const TString &titleY, const TString &variableTypeX, const TString &variableTypeY,
							      const bool hasSyst, TX *tX, TY *tY, 
							      const int vec_indX = -1, const int vec_indY = -1) {
      return AddStandardTH2(nameX, nameY, widthX, minX, maxX, widthY, minY, maxY, NULL, NULL, 
			    titleX, titleY, variableTypeX, variableTypeY, hasSyst, tX, tY, vec_indX, vec_indY);
    }
    
    template< typename TX, typename TY > bool AddStandardTH2( const TString &nameX, const TString &nameY,
                                                             const double widthX, const double minX, const double maxX,
                                                             const std::vector<double>* edgesY,
                                                             const TString &titleX, const TString &titleY,
                                                             const TString &variableTypeX, const TString &variableTypeY,
                                                             const bool hasSyst, TX *tX, TY *tY,
                                                             const int vec_indX = -1, const int vec_indY = -1) {
        return AddStandardTH2(nameX, nameY, widthX, minX, maxX, 0., 0., 0., NULL, edgesY,
                              titleX, titleY, variableTypeX, variableTypeY, hasSyst, tX, tY, vec_indX, vec_indY);
        
    }
    
    template< typename TX, typename TY > bool AddStandardTH2( const TString &nameX, const TString &nameY,
                                                             const std::vector<double>* edgesX, const std::vector<double>* edgesY,
                                                             const TString &titleX, const TString &titleY, const TString &variableTypeX, const TString &variableTypeY,
                                                             const bool hasSyst, TX *tX, TY *tY,
                                                             const int vec_indX = -1, const int vec_indY = -1) {
        return AddStandardTH2(nameX, nameY, 0., 0., 0., 0., 0., 0., edgesX, edgesY,
                              titleX, titleY, variableTypeX, variableTypeY, hasSyst, tX, tY, vec_indX, vec_indY);
        
    }
    bool BookStandardTH2( const TString &pattern, const bool hasSyst = false);
    bool FillStandardTH2( const TString &name );
    bool SaveStandardTH2( const TString&, const bool newFile = true );
    
    //
    // Light TProfile interface
    //
    bool StoreTProfile( const TString &nameX, const TString &nameY, const bool hasSyst = false );
    
    //
    //___________________________________________________________
    // TREE-SPECIFIC FUNCTIONS
    //
    //
    template< typename T > bool AddStandardBranch( const TString &name,  const TString &title, const TString &variableType,
                                                  T *t, const int vec_ind = -1) {
        
        if(m_type==HISTOS){
            std::cout << "<!> ERROR in OutputManager::addStandardBranch : You requested the booking of a branch, but OutputManager is in HISTOS mode." << std::endl;
            return false;
        }
        
        if(m_opt -> MsgLevel() == Debug::DEBUG){
            std::cout << "In OutputManager::addStandardBranch" << std::endl;
            std::cout << "Adding variable: "<< name << std::endl;
            std::cout << "  title  = " << title << std::endl;
            std::cout << "  address  = " << t << std::endl;
        }
        if(!t) std::cerr << "<!> ERROR in OutputManager::addStandardBranch(template): I cannot access the pointer (" << t << "). Please check !" << std::endl;
        VariableDef *_var = new VariableDef(name, title, variableType, t, vec_ind);
        m_stdBranchDef -> insert( std::pair < TString, VariableDef* >( name, _var ) );
        
        if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving OutputManager::addStandardTH1" << std::endl;
        
        return true;
    }
    bool BookStandardTree( const TString &pattern, const TString &title);
    bool FillStandardTree( const TString &name );
    bool SaveStandardTree( const TString& );
    
    
private:
    
    template< typename T > bool AddStandardTH1( const TString &name, const double width, const double min, const double max,
						const std::vector<double>* edges, 
						const TString &title, const TString &variableType,
						const bool hasSyst, T *t, 
						const int vec_ind = -1, const int hopt = 0) {
        //
        // Checks if the mode is correct
        //
        if(m_type==TREES){
            std::cout << "<!> ERROR in OutputManager::addStandardTH1 : You requested the booking of a histogram, but OutputManager is in TREE mode." << std::endl;
            return false;
        }
        
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
        
        VariableDef _var(name, title, variableType, t, vec_ind);
        
        std::map< TString , h1Def*>::iterator it = m_stdTH1Def -> find(name);
        if(it != m_stdTH1Def->end()){
            m_stdTH1Def->at(name)->var = _var;
        }
        else {
            std::cerr << "<!> ERROR in OutputManager::addStandardTH1(template): The searched variable has not been booked previously. Please check !" << std::endl;
        }
        
        return true;
    }
    
    bool AddStandardTH1(const TString &name, const double width, const double min, const double max, const std::vector<double>* edges, const bool hasSyst = false, const int hopt = 0);
    
    template< typename TX, typename TY > bool AddStandardTH2( const TString &nameX, const TString &nameY,
                                                             const double widthX, const double minX, const double maxX,
                                                             const double widthY, const double minY, const double maxY,
                                                             const std::vector<double>* edgesX, const std::vector<double>* edgesY,
                                                             const TString &titleX, const TString &titleY, const TString &variableTypeX, const TString &variableTypeY,
                                                             const bool hasSyst, TX *tX, TY *tY,
                                                             const int vec_indX = -1, const int vec_indY = -1) {
        //
        // Checks if the mode is correct
        //
        if(m_type==TREES){
            std::cout << "<!> ERROR in OutputManager::addStandardTH2 : You requested the booking of a histogram, but OutputManager is in TREE mode." << std::endl;
            return false;
        }
        
        //
        // Don't do it if there is no systematics
        //
        if( m_opt -> OnlyDumpSystHistograms() && !hasSyst ){
            return false;
        }
        
        if( !tX ) std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): I cannot access the tX pointer (" << tX << "). Please check !" << std::endl;
        if( !tY ) std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): I cannot access the tY pointer (" << tY << "). Please check !" << std::endl;
        
        TString name = nameY + "_vs_" + nameX;
        bool added = AddStandardTH2(name, widthX, minX, maxX, widthY, minY, maxY, edgesX, edgesY, hasSyst);
        if(!added) {
            std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): could not add the variable !! Please check." << std::endl;
            return false;
        }
        
        VariableDef _varX(nameX,titleX, variableTypeX, tX, vec_indX);
        VariableDef _varY(nameY,titleY, variableTypeY, tY, vec_indY);
        
        std::map< TString , h2Def*>::iterator it = m_stdTH2Def -> find(name);
        if(it != m_stdTH2Def->end()){
            m_stdTH2Def->at(name)->varX = _varX;
            m_stdTH2Def->at(name)->varY = _varY;
        }
        
        else std::cerr << "<!> ERROR in OutputManager::addStandardTH2(template): The searched variable has not been booked previously. Please check !" << std::endl;
        return true;
    }
    
    bool AddStandardTH2(const TString &name, const double widthX, const double minX, const double maxX,
                        const double widthY, const double minY, const double maxY, 
                        const std::vector<double>* edgesX, const std::vector<double>* edgesY, const bool hasSyst = false);
    
    bool FillTH1FromVector( void* t, const VariableDef::VariableType type, const TString &histName, const double weight, const int index = -1 );
    
private:
    OptionsBase *m_opt;
    OutputType m_type;
    StdTH1 *m_stdTH1Def;
    StdTH1 *m_stdTProfileDef;
    StdTH2 *m_stdTH2Def;
    StdBranches *m_stdBranchDef;
    
    HistManager *m_histMngr;
    TreeManager *m_treeMngr;
    
    SystManager::SystVector *m_sysVector;
    OutputData *m_data;
    std::map < TString, bool > *m_mapHasSyst;
    std::set < TString > *m_vecH2ToProfile;
    
    std::string m_weightVarName;
};

#endif // OUTPUTMANAGER_H





