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
    std::map<std::string, double> *o_D_weight_components;
    std::map<std::string, float> *o_F_weight_components;
    std::map<std::string, std::vector<double>* > *o_vecD_weight_components;
    std::map<std::string, std::vector<float>* > *o_vecF_weight_components;

    double o_eventWeight_Nom;
    std::map<std::string, double >* o_eventWeight_Systs;

    //bool InsertNomComponent(const std::string& name) const;
    //bool InsertSystComponent(const std::string& name) const;
    //bool InsertSystWeight(const std::string& name) const;


};

#endif //OUTPUTDATA_H
