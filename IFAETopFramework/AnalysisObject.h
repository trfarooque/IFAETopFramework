#ifndef ANALYSISOBJECT_H
#define ANALYSISOBJECT_H

#include <map>
#include <string>
#include "TLorentzVector.h"

class AnalysisObject : public TLorentzVector {
    ClassDef(AnalysisObject, 1);
    
public:
    typedef std::vector<AnalysisObject*> AOVector;
    
    AnalysisObject();
    virtual ~AnalysisObject();
    AnalysisObject( const AnalysisObject& q );
    
    void Reset();
    void SetMoment(const std::string &name,const double value);
    double GetMoment(const std::string &name) const;
    bool IsKnownMoment(const std::string &name) const;
    AnalysisObject& operator=(const AnalysisObject& q);
    AnalysisObject operator+(const AnalysisObject& q) const;
    
private:
    std::map<std::string,double> m_moments;
    
};

#endif // ANALYSISOBJECT_H
