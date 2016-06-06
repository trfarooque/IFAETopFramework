#ifndef NTUPLEDATA_H
#define NTUPLEDATA_H

#include <vector>
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
    mutable std::map<std::string, double> *d_D_weight_components; 
    mutable std::map<std::string, float> *d_F_weight_components; 
    mutable std::map<std::string, std::vector<double>* > *d_vecD_weight_components; 
    mutable std::map<std::string, std::vector<float>* > *d_vecF_weight_components; 

    /*
    mutable std::map<std::string, double> *d_syst_D_weight_components; 
    mutable std::map<std::string, float> *d_syst_F_weight_components; 
    mutable std::map<std::string, std::vector<double>* > *d_syst_vecD_weight_components; 
    mutable std::map<std::string, std::vector<float>* > *d_syst_vecF_weight_components; 
    */

    //bool InsertNomComponent(const std::string& name) const;
    //bool InsertSystComponent(const std::string& name) const;
};

#endif
