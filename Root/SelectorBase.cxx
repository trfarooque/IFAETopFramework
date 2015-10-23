#include <iostream>

#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/SelectorBase.h"

//___________________________________________________________
//
SelectorBase::SelectorBase( OptionsBase* opt ):
m_opt(opt),
m_map_sel_string_int(0),
m_map_sel_int_string(0)
{
    m_map_sel_int_string = new std::map < int, std::string >;
    m_map_sel_string_int = new std::map < std::string, int >;
}

//___________________________________________________________
//
SelectorBase::SelectorBase( const SelectorBase &q ){
    m_opt = q.m_opt;
    m_map_sel_int_string = q.m_map_sel_int_string;
    m_map_sel_string_int = q.m_map_sel_string_int;
}

//___________________________________________________________
//
SelectorBase::~SelectorBase()
{}

//___________________________________________________________
//
void SelectorBase::AddSelection( const int index, const std::string &name ) {
    if(m_opt -> MsgLevel()==Debug::DEBUG) std::cout << "Entering in SelectorBase::AddSelection()" << std::endl;
    m_map_sel_string_int -> at(name) = index;
    m_map_sel_int_string -> at(index) = name;
}
