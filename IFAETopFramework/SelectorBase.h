#ifndef SELECTORBASE_H
#define SELECTORBASE_H

#include <string>
#include <map>

class OptionsBase;

class SelectorBase {

public:
    SelectorBase( OptionsBase *opt );
    SelectorBase( const SelectorBase &q );
    ~SelectorBase();
    
    void AddSelection( const int index, const std::string &name );
    
    virtual bool PassSelection( const int index ) const;
    virtual bool PassSelection( const std::string &name ) const;
    
protected:
    OptionsBase *m_opt;
    std::map < std::string, int > *m_map_sel_string_int;
    std::map < int, std::string > *m_map_sel_int_string;
};

#endif // SELECTORBASE_H
