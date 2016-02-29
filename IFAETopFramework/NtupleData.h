#ifndef NTUPLEDATA_H
#define NTUPLEDATA_H

class NtupleData {
    
public:
    
    //
    // Standard C++ functions
    //
    NtupleData();
    virtual ~NtupleData(){}
    
    //
    // Specific functions
    //
    virtual void ClearNtupleData(){}
    virtual void EmptyNtupleData(){}
    
};

#endif
