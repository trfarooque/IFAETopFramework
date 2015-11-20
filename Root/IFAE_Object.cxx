#include <iostream>
#include <iomanip>

#include "IFAETopFramework/IFAE_Object.h"

//_______________________________________________________________________
//
IFAE_Object::IFAE_Object():
TLorentzVector()
{
    m_moments.clear();
}

//_______________________________________________________________________
//
IFAE_Object::~IFAE_Object()
{
    m_moments.clear();
}

//_______________________________________________________________________
//
IFAE_Object::IFAE_Object( const IFAE_Object& q ):
TLorentzVector(q),
m_moments(q.m_moments)
{}

//_______________________________________________________________________
//
void IFAE_Object::Reset(){
    m_moments.clear();
}

//_______________________________________________________________________
//
void IFAE_Object::SetMoment(const std::string &name,const double value){
    m_moments.insert(std::pair<std::string,double>(name,value));
}

//_______________________________________________________________________
//
double IFAE_Object::GetMoment(const std::string &name) const
{
    std::map<std::string,double>::const_iterator it=m_moments.find(name);
    if (it!=m_moments.end()) return it->second;
    std::cout << " >>>>> FATAL in IFAE_Object::GetMoment(''" <<  name << "''): unknown moment !" << std::endl;
    return -1;
}

//_______________________________________________________________________
//
bool IFAE_Object::IsKnownMoment(const std::string &name) const
{
    std::map<std::string,double>::const_iterator it=m_moments.find(name);
    if (it!=m_moments.end()) return true;
    return false;
}

//_______________________________________________________________________
//
IFAE_Object &IFAE_Object::operator=(const IFAE_Object& q) {
    if (this!=&q) {
        TLorentzVector::operator=(q);
        m_moments=q.m_moments;
    }
    return *this;
}

//_______________________________________________________________________
//
IFAE_Object IFAE_Object::operator+(const IFAE_Object& q) const{
    double x= Px() + q.Px();
    double y= Py() + q.Py();
    double z= Pz() + q.Pz();
    double e= E()  + q.E();
    IFAE_Object object;
    object.SetPxPyPzE(x,y,z,e);
    return object;
}