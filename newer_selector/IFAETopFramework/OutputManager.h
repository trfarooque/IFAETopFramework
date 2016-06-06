#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H

//IFAEFramework classes

class OptionsBase;
class OutputData;

class OutputManager {
    
public:
    
    // Member functions
  OutputManager( OptionsBase *opt, OutputData *data=NULL );
  OutputManager( const OutputManager &q );
  virtual ~OutputManager();
  inline void SetData(OutputData* data){ m_data = data; }
    
protected:
    OptionsBase *m_opt;
    OutputData *m_data;
};

#endif // OUTPUTMANAGER_H





