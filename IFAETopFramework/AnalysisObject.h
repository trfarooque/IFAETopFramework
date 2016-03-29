#ifndef ANALYSISOBJECT_H
#define ANALYSISOBJECT_H

#include <map>
#include <string>
#include "TLorentzVector.h"

class AnalysisObject : public TLorentzVector {
    ClassDef(AnalysisObject, 1);
    
public:
    //Standard C++ functions
    AnalysisObject();
    virtual ~AnalysisObject();
    AnalysisObject( const TLorentzVector& tlv );
    AnalysisObject( const AnalysisObject& q );
    
    //Specific functions
    void Reset();
    void SetMoment(const std::string &name,const double value);
    void UpdateMoment( const std::string &key, const double value );
    double GetMoment(const std::string &name) const;
    bool IsKnownMoment(const std::string &name) const;
    bool IsKinematicMoment(const std::string &name) const;
    AnalysisObject& operator=(const AnalysisObject& q);
    AnalysisObject operator+(const AnalysisObject& q) const;
    
private:
    std::map<std::string,double> m_moments;
    
};

typedef std::vector<AnalysisObject*> AOVector;

#endif // ANALYSISOBJECT_H
