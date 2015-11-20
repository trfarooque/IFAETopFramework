#ifndef IFAE_OBJECT_H
#define IFAE_OBJECT_H

#include <map>
#include <string>
#include "TLorentzVector.h"

class IFAE_Object : public TLorentzVector {
    ClassDef(IFAE_Object, 1);
    
public:
    typedef std::vector<IFAE_Object*> IFAE_ObjectVector;
    
    IFAE_Object();
    virtual ~IFAE_Object();
    IFAE_Object( const IFAE_Object& q );

    void Reset();
    void SetMoment(const std::string &name,const double value);
    double GetMoment(const std::string &name) const;
    bool IsKnownMoment(const std::string &name) const;
    IFAE_Object& operator=(const IFAE_Object& q);
    IFAE_Object operator+(const IFAE_Object& q) const;
    
private:
    std::map<std::string,double> m_moments;
    
};

#endif // IFAE_OBJECT_H
