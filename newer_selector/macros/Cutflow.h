#ifndef CUTFLOW_H
#define CUTFLOW_H

#include <vector>;

class SelectorBase;

struct cutflow_node{
  const int index;
  const std::string name;
  const std::string bin;
}

class Cutflow {

 public:
  Cutflow(const int index, SelectorBase* selector);
  ~Cutflow;


 protected:
  TH1D* m_hist_cutflow;
  std::vector<cutflow_node>;
};

#endif
