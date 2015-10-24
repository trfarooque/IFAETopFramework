#include <iostream>

#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/SelectorBase.h"
#include "IFAETopFramework/NtupleData.h"
#include "IFAETopFramework/OutputData.h"

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
SelectorBase::~SelectorBase(){
    m_map_sel_string_int -> clear(); delete m_map_sel_string_int;
    m_map_sel_int_string -> clear(); delete m_map_sel_int_string;
}

//___________________________________________________________
//
void SelectorBase::AddSelection( const int index, const std::string &name ) {
    m_map_sel_string_int -> insert ( std::pair < std::string, int >( name, index ) );
    m_map_sel_int_string -> insert ( std::pair < int, std::string >( index, name ) );
}

//___________________________________________________________
//
bool SelectorBase::PassSelection( const std::string &selection ) const {
    return PassSelection( m_map_sel_string_int -> at(selection) );
}

//___________________________________________________________
//
bool SelectorBase::PassSelection( const int ) const {
    std::cout << "SelectorBase::PassSelection() is empty ... You should not be there !!" << std::endl;
    return false;
}