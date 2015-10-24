#ifndef SELECTORBASE_H
#define SELECTORBASE_H

#include <string>
#include <map>

class OptionsBase;
class NtupleData;
class OutputData;

class SelectorBase {

public:
    
    SelectorBase( OptionsBase *opt );
    SelectorBase( const SelectorBase &q );
    ~SelectorBase();
    
    void AddSelection( const int index, const std::string &name );
    
    bool PassSelection( const std::string &) const;
    virtual bool PassSelection( const int ) const;
    
    inline std::map < int, std::string >* GetSelectionMap() const { return m_map_sel_int_string; }
    
protected:
    OptionsBase *m_opt;
    std::map < std::string, int > *m_map_sel_string_int;
    std::map < int, std::string > *m_map_sel_int_string;
};

#endif // SELECTORBASE_H
