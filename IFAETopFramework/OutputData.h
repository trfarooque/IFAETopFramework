#ifndef OUTPUTDATA_H
#define OUTPUTDATA_H


class OutputData {
    
public:
    OutputData();
    virtual ~OutputData();
    virtual void ClearOutputData();
    virtual void EmptyOutputData();
    
public:
    double finalEvent_weightNom;
};

#endif //OUTPUTDATA_H
