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
    //Selection decisions
    //
    std::map<int, bool> *o_sel_decisions;

};

#endif //OUTPUTDATA_H
