#include "IFAETopFramework/TriggerInfo.h"


TriggerInfo::TriggerInfo(const std::string& name, int type, int period) : 
  m_name(name),
  m_type(type),
  m_period(period),
  m_pass(false)
{}
