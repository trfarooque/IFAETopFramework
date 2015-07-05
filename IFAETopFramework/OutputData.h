#ifndef OUTPUTDATA_H
#define OUTPUTDATA_H


class OutputData {
    
public:
    OutputData();
    OutputData( const OutputData & );
    virtual ~OutputData();
    virtual void ClearOutputData();
    virtual void EmptyOutputData();
    
public:
    double o_eventWeight_Nom;
};

#endif //OUTPUTDATA_H
