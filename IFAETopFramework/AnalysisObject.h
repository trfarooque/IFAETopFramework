#ifndef ANALYSISOBJECT_H
#define ANALYSISOBJECT_H

#include <iostream>
#include <map>
#include <string>
#include "TLorentzVector.h"

class AnalysisObject : public TLorentzVector {
  ClassDef(AnalysisObject, 1);
    
public:
    
    //
    // Standard C++ functions
    //
    AnalysisObject();
    virtual ~AnalysisObject();
    AnalysisObject( const TLorentzVector& tlv );
    AnalysisObject( const AnalysisObject& q );
    
    //
    // Specific functions
    //
    void Reset();
    void SetMoment(const std::string &name,const double value);
    void UpdateMoment( const std::string &name, const double value );
    double GetMoment(const std::string &name) const;

    template<typename T> void SetGeneralMoment(const std::string &name, T* value){ 
      
      if(IsKnownGeneralMoment(name)){
	//The moment is already in the map ... you should use UpdateMoment function
	std::cout << "<!> WARNING in AnalysisObject::SetGeneralMoment(): The moment \"" << name 
		  << "\" is already used and set. Please use UpdateGeneralMoment() instead." << std::endl;
	return;
      }
      if(IsKinematicMoment(name)){ std::cerr<<" AnalysisObject::SetMoment() --> ERROR Please use TLorentzVector::SetPtEtaPhiM() to set kinematic moment "
					    <<name<<std::endl;}
      m_gen_moments.insert( std::pair<std::string,void*>(name,(void*)(value)) );
      
      return;
    }
    
    template<typename T> void UpdateGeneralMoment( const std::string &name, T* value ){
 
      if (!IsKnownGeneralMoment(name)){
	//The moment is not in the map ... you should use SetMoment function
	std::cout << "<!> WARNING in AnalysisObject::UpdateGeneralMoment(): The moment \"" 
		  << name << "\" is not known. Please use SetGeneralMoment() instead." << std::endl;
	return;
      }
      m_gen_moments[name] = (void*)value;
      
      return;
    }

    void* GetGeneralMoment(const std::string &name) const;

    bool IsKnownMoment(const std::string &name) const;
    bool IsKinematicMoment(const std::string &name) const;
    bool IsKnownGeneralMoment(const std::string &name) const;


    AnalysisObject& operator=(const AnalysisObject& q);
    AnalysisObject operator+(const AnalysisObject& q) const;

    std::map<std::string,double> m_moments;
    std::map<std::string, void*> m_gen_moments;
    
};

typedef std::vector<AnalysisObject*> AOVector;

#endif // ANALYSISOBJECT_H
