#include <iomanip>

#include "IFAETopFramework/AnalysisObject.h"

//_______________________________________________________________________
//
AnalysisObject::AnalysisObject():
TLorentzVector()
{
    m_moments.clear();
    m_gen_moments.clear();
}

//_______________________________________________________________________
//
AnalysisObject::~AnalysisObject()
{
    m_moments.clear();
    m_gen_moments.clear();
}

//_______________________________________________________________________
//
AnalysisObject::AnalysisObject( const AnalysisObject& q ):
TLorentzVector(q),
m_moments(q.m_moments),
m_gen_moments(q.m_gen_moments)
{}

AnalysisObject::AnalysisObject( const TLorentzVector& tlv ) : 
  TLorentzVector(tlv)
{  
  m_moments.clear(); 
  m_gen_moments.clear(); 
}

//_______________________________________________________________________
//
void AnalysisObject::Reset(){
  SetPtEtaPhiE(0.,0.,0.,0.);
  m_moments.clear();
  m_gen_moments.clear();
}

//_______________________________________________________________________
//
void AnalysisObject::SetMoment(const std::string &name,const double value){

  if(IsKnownMoment(name)){
    //The moment is already in the map ... you should use UpdateMoment function
    std::cout << "<!> WARNING in AnalysisObject::SetMoment(): The moment \"" << name << "\" is already used and set. Please use UpdateMoment function instead." << std::endl;
    return;
  }
  if(IsKinematicMoment(name)){ std::cerr<<" AnalysisObject::SetMoment() --> ERROR Please use TLorentzVector::SetPtEtaPhiM() to set kinematic moment "<<name<<std::endl;}
  m_moments.insert(std::pair<std::string,double>(name,value));
}

//_______________________________________________________________________
//
void AnalysisObject::UpdateMoment(const std::string &name,const double value){
  if (!IsKnownMoment(name)){
    //The moment is not in the map ... you should use SetMoment function
    std::cout << "<!> WARNING in AnalysisObject::UpdateMoment(): The moment \"" << name << "\" is not known. Please use SetMoment function instead." << std::endl;
    return;
  }
  m_moments[name] = value;
}

//_______________________________________________________________________
//
double AnalysisObject::GetMoment(const std::string &name) const
{
    if(name == "Pt"){ return Pt(); }
    else if(name == "Eta"){ return Eta(); }
    else if(name == "Rapidity"){ return Rapidity(); }
    else if(name == "Phi"){ return Phi(); }
    else if(name == "M"){ return M(); }
    else if(name == "E"){ return E(); }
    else{
      std::map<std::string,double>::const_iterator it=m_moments.find(name);
      if (it!=m_moments.end()){ return it->second; }
    }
    std::cerr << " >>>>> FATAL in AnalysisObject::GetMoment(''" <<  name << "''): unknown moment !" << std::endl;
    return -1;
}

//_______________________________________________________________________
//
void* AnalysisObject::GetGeneralMoment(const std::string &name) const
{
  if(IsKinematicMoment(name)){
    std::cerr<<"AnalysisObject::GetGeneralMoment()-->ERROR: Kinematic moments can be accessed directly or through GetMoment()"<<std::endl;
    return NULL;
  }
  
  std::map<std::string,void*>::const_iterator it=m_gen_moments.find(name);
  if (it!=m_gen_moments.end()){ return it->second; }

  std::cerr << " >>>>> FATAL in AnalysisObject::GetMoment(''" <<  name << "''): unknown moment !" << std::endl;
  return NULL;

}

//_______________________________________________________________________
//
bool AnalysisObject::IsKinematicMoment(const std::string &name) const
{
  if( (name == "Pt") || (name == "Eta") || (name == "Rapidity") || (name == "Phi") || (name == "M") || (name == "E") ){ return true; }
    return false;
}

//_______________________________________________________________________
//
bool AnalysisObject::IsKnownMoment(const std::string &name) const
{
    std::map<std::string,double>::const_iterator it=m_moments.find(name);
    if (it!=m_moments.end()) return true;

    return false;
}

//_______________________________________________________________________
//
bool AnalysisObject::IsKnownGeneralMoment(const std::string &name) const
{
    std::map<std::string,void*>::const_iterator it=m_gen_moments.find(name);
    if (it!=m_gen_moments.end()) return true;

    return false;
}


//_______________________________________________________________________
//
AnalysisObject &AnalysisObject::operator=(const AnalysisObject& q) {
    if (this!=&q) {
        TLorentzVector::operator=(q);
        m_moments=q.m_moments;
        m_gen_moments=q.m_gen_moments;
    }
    return *this;
}

//_______________________________________________________________________
//
AnalysisObject AnalysisObject::operator+(const AnalysisObject& q) const{
    double x= Px() + q.Px();
    double y= Py() + q.Py();
    double z= Pz() + q.Pz();
    double e= E()  + q.E();
    AnalysisObject object;
    object.SetPxPyPzE(x,y,z,e);
    return object;
}
