#ifndef TRIGGERINFO_H
#define TRIGGERINFO_H

#include <map>
#include <vector>
#include <string>

class TriggerInfo{

 public:

  TriggerInfo(const std::string& name, int type=0, int period=0);
  TriggerInfo(const TriggerInfo& q);
  ~TriggerInfo(){}
  void SetDecision(const bool dec);

  inline const std::string& Name() const{ return m_name;}
  inline int Type() const{ return m_type;}
  inline int Period() const{ return m_period; }
  inline bool Pass() const{ return m_pass; }

  inline void SetPass(const bool pass){ m_pass = pass; }

 private:
  std::string m_name;
  int m_type;
  int m_period;
  bool m_pass;

};

typedef std::vector<TriggerInfo*> TriggerList;
typedef std::map<std::string, TriggerInfo*> TriggerMap;

#endif 
