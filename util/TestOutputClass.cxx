#include <iostream>

#include "TRandom3.h"

#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/OutputData.h"
#include "IFAETopFramework/OutputManager.h"

int main() {
    
    //
    //
    // Creating ummy variables to illustrate
    //
    //
    int Njets;
    std::vector < float > jets_pt;
    std::vector < int > jets_isBtagged;
    std::vector < double > jets_MV1;
    std::vector < std::vector < int > > jets_truthMatched_index;
    
    //
    //
    // Creation of the OptionsBase object
    //     Needed to propagate possible options (debug, ...)
    //
    //
    OptionsBase *optBase = new OptionsBase();
    
    //
    //
    // Creation of the OutputManager class object
    //     For now only histograms
    //
    //
    OutputManager *outputMngrHist = new OutputManager(optBase,OutputManager::HISTOS);
    
    //
    //
    // Creation of the OutputData object
    //     Typically containing the variables we want to store (njets, ...) but also the weights (NECESSARILY !!)
    //
    //
    OutputData *data = new OutputData();
    data -> finalEvent_weightNom = 1;//This is the variable used for filling the nominal histograms
    outputMngrHist -> setData(data);//Associates the "data" object to the OutputManager one
    
    //
    // Creating the output histograms
    //
    outputMngrHist->addStandardTH1( "jets_n", 1, -0.5, 15.5, "Number of jets", "I", false, &Njets);
    outputMngrHist->bookStandardTH1( "c0", false);
    outputMngrHist->bookStandardTH1( "c1", false);
    
    const unsigned int nentries = 100;
    TRandom3 *rdm = new TRandom3();
    
    //
    //
    // Looping over fake events
    //
    //
    for ( unsigned int iEntry = 0; iEntry < nentries; ++iEntry){
        Njets = rdm->Poisson(5);
        outputMngrHist->fillStandardTH1("c0");
        
        if(Njets>=6){
            outputMngrHist->fillStandardTH1("c1");
        }
    }
    outputMngrHist->saveStandardTH1("myNiceTest.root");
    
    return 0;
}
