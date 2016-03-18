#ifndef NTUPLEDATA_H
#define NTUPLEDATA_H

#include <map>
#include <string>
class NtupleData {
    
public:
    
    //
    // Standard C++ functions
    //
    NtupleData();
    virtual ~NtupleData();
    
    //
    // Specific functions
    //
    virtual void ClearNtupleData(){}
    virtual void EmptyNtupleData(){}

    //Member variables
    mutable std::map<std::string, double> *d_nominal_weight_components; 
    mutable std::map<std::string, double> *d_syst_weight_components; 

    bool InsertNomComponent(const std::string& name) const;
    bool InsertSystComponent(const std::string& name) const;
};

#endif
