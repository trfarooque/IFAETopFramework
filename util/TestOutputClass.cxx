#include <iostream>

#include "TRandom3.h"

#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/OutputData.h"
#include "IFAETopFramework/OutputManager.h"

int main() {
    
    //
    //
    // Creating dummy variables to illustrate
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
    // Creating the output standard histograms and steps
    //
    outputMngrHist->addStandardTH1( "jets_n", 1, -0.5, 15.5, "Number of jets", "I", false, &Njets);
    outputMngrHist->addStandardTH1( "jet1_pt", 25, 0, 1000, "leading jet pt", "VF", false, &jets_pt,0);
    outputMngrHist->addStandardTH1( "jet2_pt", 25, 0, 1000, "leading jet pt", "VF", false, &jets_pt,1);
    outputMngrHist->addStandardTH1( "jet3_pt", 25, 0, 1000, "leading jet pt", "VF", false, &jets_pt,2);
    outputMngrHist->addStandardTH1( "jet1_mv1", 0.05, -2, 2, "MV1", "VD", false, &jets_MV1,0);
    outputMngrHist->bookStandardTH1( "c0", false);
    outputMngrHist->bookStandardTH1( "c1", false);
    
    const unsigned int nentries = 1000;
    TRandom3 *rdm = new TRandom3();
    
    //
    //
    // Looping over fake events
    //
    //
    for ( unsigned int iEntry = 0; iEntry < nentries; ++iEntry){
        
        //
        // Filling the variables
        //
        jets_pt.clear();
        jets_isBtagged.clear();
        jets_MV1.clear();
        jets_truthMatched_index.clear();
        
        Njets = rdm->Poisson(5);
        for(unsigned int iJet = 0; iJet < Njets; ++iJet){
            jets_pt.push_back(rdm->Gaus(400.,100)/(iJet+1));
            double MV1 = rdm->Uniform(0,1);
            jets_MV1.push_back(MV1);
            if(MV1>0.7) jets_isBtagged.push_back(1);
            else jets_isBtagged.push_back(0);
            
            int parton_matched = rdm->Poisson(1);
            std::vector < int > matched;
            for (unsigned int iMatch = 0; iMatch<parton_matched; ++iMatch) {
                matched.push_back((int)rdm->Uniform(0,100));
            }
            jets_truthMatched_index.push_back(matched);
        }
        
        //
        // Filling the output
        //
        outputMngrHist->fillStandardTH1("c0");
        if(Njets>=6){
            outputMngrHist->fillStandardTH1("c1");
        }
    }
    outputMngrHist->saveStandardTH1("myNiceTest.root");
    return 0;
}
