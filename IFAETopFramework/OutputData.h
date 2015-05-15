#ifndef OUTPUTDATA_H
#define OUTPUTDATA_H

#include "IFAETopFramework/CommonConstants.h"
#include <string>
#include <vector>
#include <map>
#include <TROOT.h>

using namespace std;
class OutputData {
    
public:
    OutputData();
    virtual ~OutputData(){};
    void ClearOutputData();
    void EmptyOutputData();
    
public:
    
 
    double finalEvent_weightNom;    
    
    
};

#endif
