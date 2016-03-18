#ifndef OUTPUTDATA_H
#define OUTPUTDATA_H

#include <map>
#include <string>

class OutputData {
    
public:
    OutputData();
    OutputData( const OutputData & );
    virtual ~OutputData();
    virtual void ClearOutputData();
    virtual void EmptyOutputData();

    //protected:
    std::map<std::string, double> *o_nominal_weight_components; 
    std::map<std::string, double> *o_syst_weight_components; 
    
    double o_eventWeight_Nom;
    std::map<std::string, double >* o_eventWeight_Systs;

    bool InsertNomComponent(const std::string& name) const;
    bool InsertSystComponent(const std::string& name) const;
    bool InsertSystWeight(const std::string& name) const;


};

#endif //OUTPUTDATA_H
