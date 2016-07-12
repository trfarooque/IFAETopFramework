#ifndef ANALYSISUTILS_H
#define ANALYSISUTILS_H

#include "IFAETopFramework/AnalysisObject.h"

//class AnalysisObject;

#include <vector>
#include <map>
#include <string>
#include <utility>
#include <algorithm>

class AnalysisUtils{

 public:
  AnalysisUtils();
  ~AnalysisUtils();

  static std::vector<size_t> SortVectorIndices(const std::vector<double>& vec_value, int nsort=-1, bool b_descending=true  );
  static std::vector<double> SortVectorValues(const std::vector<double>& vec_value, int nsort=-1, bool b_descending=true );
  static std::vector<size_t> SortObjectIndices(const AOVector& vec_obj, const std::string& sort_by="Pt", int nsort=-1, bool b_descending=true );
  static AOVector SortObjectValues(const AOVector& vec_obj, const std::string& sort_by="Pt", int nsort=-1, bool b_descending=true );

  static void TrimString(std::string& str, const std::string& whitespace=" \t");
  static std::string::size_type ParseString(std::string& base, std::string& piece, const std::string& delim);
  static bool BoolValue(std::string& arg_val, bool& bin_val);
  static bool BoolValue(std::string& arg_val, const std::string& arg_name="");
  static bool FileExists(const std::string& filename);

  static std::string ReplaceString(const std::string& inputStr, const std::string& orig, const std::string& replacement);
  static int CountSubstring(const std::string& str, const std::string& sub);

  static int ParseConfigFile(const std::string& config_file, std::vector<std::map<std::string, 
			     std::string> >& ret_map, const std::string& delim=" : ", 
			     const bool blockformat=false, 
			     const std::string& flagList="", const std::string& vetoList="");
  static int ParseConfigFile_Blocks(const std::string& config_file, std::vector<std::map<std::string, 
				    std::string> >& ret_map, const std::string& delim=" : ", 
				    const std::string& flagList="", const std::string& vetoList="");
  static int ParseConfigFile_Lines(const std::string& config_file, std::vector<std::map<std::string, 
				   std::string> >& ret_map, const std::string& delim=" : ",
				   const std::string& /*flagList*/="", const std::string& /*vetoList*/="");



 protected:
  static  bool orderAscend(std::pair<int, double> p1, std::pair<int, double> p2 ){
    return valOrderAscend(p1.second, p2.second);
  }

  static bool orderDescend(std::pair<int, double> p1, std::pair<int, double> p2 ){
    return valOrderDescend(p1.second, p2.second);
  }
  static bool valOrderAscend(double val1, double val2){ return (val1 < val2); }
  static bool valOrderDescend(double val1, double val2){ return (val1 > val2); }

  static void SortVectorPairs(std::vector<std::pair<int, double> >& vec_pair, int nsort=-1, bool b_descending=true );


}; 

#endif
