#include "IFAETopFramework/TriggerInfo.h"


TriggerInfo::TriggerInfo(const std::string& name, int type, int period) : 
  m_name(name),
  m_type(type),
  m_period(period),
  m_pass(false)
{}

TriggerInfo::TriggerInfo(const TriggerInfo& q){

  m_name = q.m_name;
  m_type = q.m_type;
  m_period = q.m_period;
  m_pass = q.m_pass;
}
