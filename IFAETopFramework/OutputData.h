#ifndef OUTPUTDATA_H
#define OUTPUTDATA_H

#include <map>
#include <vector>
#include <string>

class OutputData {
    
public:
    OutputData();
    OutputData( const OutputData & );
    virtual ~OutputData();
    virtual void ClearOutputData();
    virtual void EmptyOutputData();

    //protected:

    //____________________________________________________________
    //Weights
    //
    std::map<std::string, double> *o_D_weight_components;
    std::map<std::string, float> *o_F_weight_components;
    std::map<std::string, std::vector<double>* > *o_vecD_weight_components;
    std::map<std::string, std::vector<float>* > *o_vecF_weight_components;
    double o_eventWeight_Nom;
    std::map<std::string, double > *o_eventWeight_Systs;

    //____________________________________________________________
    //Selection decision
    //
    std::map<int, bool> *o_sel_decisions;

    //____________________________________________________________
    //TRF weights, permutations and current configurations
    //
    int o_TRF_bjets_n;
    bool o_TRF_isIncl;

};

#endif //OUTPUTDATA_H
