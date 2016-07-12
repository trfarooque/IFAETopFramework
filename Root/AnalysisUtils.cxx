#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include "IFAETopFramework/AnalysisUtils.h"


AnalysisUtils::AnalysisUtils(){}
AnalysisUtils::~AnalysisUtils(){}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Sorting utilities

void AnalysisUtils::SortVectorPairs(std::vector<std::pair<int, double> >& vec_pair, int nsort, bool b_descending){

  if(nsort <= 0){
    if(b_descending){
      std::sort( vec_pair.begin(), vec_pair.end(), orderDescend );
    }
    else{
      std::sort( vec_pair.begin(), vec_pair.end(), orderAscend );
    }
  }//full sort
  else{
    if(b_descending){
      std::partial_sort( vec_pair.begin(), vec_pair.begin()+nsort, vec_pair.end(), orderDescend );
    }
    else{
      std::partial_sort( vec_pair.begin(), vec_pair.begin()+nsort, vec_pair.end(), orderAscend );
    }

    vec_pair.erase(vec_pair.begin()+nsort, vec_pair.end());

  }//partial sort

  return;
}

std::vector<size_t> AnalysisUtils::SortVectorIndices(const std::vector<double>& vec_value, int nsort, bool b_descending ){ 

  std::vector<size_t> ret_ind; ret_ind.clear();
  std::vector< std::pair<int, double> > ind_val_pair; ind_val_pair.clear();
  for(size_t i = 0; i < vec_value.size(); i++){ //ret_ind.push_back(i); }
    ind_val_pair.push_back( std::make_pair(i, vec_value.at(i)) );
  }

  SortVectorPairs(ind_val_pair, nsort, b_descending); 
  for(size_t s = 0; s != ind_val_pair.size(); s++){ ret_ind.push_back(ind_val_pair.at(s).first); }
  return ret_ind;

}

std::vector<double> AnalysisUtils::SortVectorValues(const std::vector<double>& vec_value, int nsort, bool b_descending  ){

  std::vector<double> ret_vals(vec_value);

  if(nsort <= 0){
    if(b_descending){ std::sort( ret_vals.begin(), ret_vals.end(), valOrderDescend  ); }
    else{ std::sort( ret_vals.begin(), ret_vals.end(), valOrderAscend  ); }
  }//full sort
  else{
    if(b_descending){ std::partial_sort( ret_vals.begin(), ret_vals.begin()+nsort, ret_vals.end(), valOrderDescend  ); }
    else{ std::partial_sort( ret_vals.begin(), ret_vals.begin()+nsort, ret_vals.end(), valOrderAscend  ); }

    ret_vals.erase(ret_vals.begin()+nsort, ret_vals.end());

  }//partial sort

  return ret_vals;

}

std::vector<size_t> AnalysisUtils::SortObjectIndices(const AOVector& vec_obj, const std::string& sort_by, int nsort, bool b_descending ){

  std::vector<size_t> ret_ind; ret_ind.clear();
  std::vector< std::pair<int, double> > ind_val_pair; ind_val_pair.clear();
  for(size_t i = 0; i < vec_obj.size(); i++){ 

    double value = 0.;
    if(sort_by == "Pt"){ value = vec_obj.at(i)->Pt(); }
    else if(sort_by == "Eta"){ value = vec_obj.at(i)->Eta(); }
    else if(sort_by == "Phi"){ value = vec_obj.at(i)->Phi(); }
    else if(sort_by == "M"){ value = vec_obj.at(i)->M(); }
    else{ value = vec_obj.at(i)->GetMoment(sort_by); }
    ind_val_pair.push_back( std::make_pair(i, value ) ); 

  }

  SortVectorPairs(ind_val_pair, nsort, b_descending); 

  for(size_t s = 0; s != ind_val_pair.size(); s++){ ret_ind.push_back(ind_val_pair.at(s).first); }
  return ret_ind;

}

AOVector AnalysisUtils::SortObjectValues(const AOVector& vec_obj, const std::string& sort_by, int nsort, bool b_descending ){

  //Using SortVectorIndices for now. Will change later for efficiency. 

  AOVector ret_vec; ret_vec.clear();


  std::vector< std::pair<int, double> > ind_val_pair; ind_val_pair.clear();
  for(size_t i = 0; i < vec_obj.size(); i++){ 

    double value = 0.;
    if(sort_by == "Pt"){ value = vec_obj.at(i)->Pt(); }
    else if(sort_by == "Eta"){ value = vec_obj.at(i)->Eta(); }
    else if(sort_by == "Phi"){ value = vec_obj.at(i)->Phi(); }
    else if(sort_by == "M"){ value = vec_obj.at(i)->M(); }
    else{ value = vec_obj.at(i)->GetMoment(sort_by); }
    ind_val_pair.push_back( std::make_pair(i, value ) ); 

  }
  SortVectorPairs(ind_val_pair, nsort, b_descending); 

  for(size_t s = 0; s != ind_val_pair.size(); s++){ ret_vec.push_back( vec_obj.at( ind_val_pair.at(s).first ) ); }
  return ret_vec;

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//String utilities

void AnalysisUtils::TrimString(std::string& str, const std::string& whitespace){
  const auto strBegin = str.find_first_not_of(whitespace);
  if(strBegin == std::string::npos){ str = ""; }  // no content
  else{
    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;
    str = str.substr(strBegin, strRange);
  }
  return;
}


std::string::size_type AnalysisUtils::ParseString(std::string& base, std::string& piece, const std::string& delim){

  std::string::size_type pos = base.find(delim);
  if(pos != std::string::npos ){
    piece = base.substr(0, pos);
    base = base.substr(pos + delim.size());
  }
  else {piece = base;}
  return pos;

}

std::string AnalysisUtils::ReplaceString(const std::string& inputStr, const std::string& orig, const std::string& replacement){

  std::string newStr = inputStr;
  for( std::string::size_type  pos = 0; ; pos += replacement.length() ){
    pos = newStr.find(orig, pos);
    if(pos == std::string::npos) break;
    newStr.erase( pos, orig.length() );
    newStr.insert( pos, replacement );
  }

  return newStr;

}

//From RosettaCode
int AnalysisUtils::CountSubstring(const std::string& str, const std::string& sub){
  if (sub.length() == 0) return 0;
  int count = 0;
  for (size_t offset = str.find(sub); offset != std::string::npos;
       offset = str.find(sub, offset + sub.length())){ ++count; }
  return count;
}


bool AnalysisUtils::BoolValue(std::string& arg_val, bool& bin_val){

  std::transform(arg_val.begin(), arg_val.end(), arg_val.begin(), ::toupper);
  if( arg_val.find("TRUE") != std::string::npos ){ bin_val = true; return true; }
  else if( arg_val.find("FALSE") != std::string::npos ){ bin_val = false; return true; }
  else{std::cout<<"Error : Unknown value "<<arg_val<<" for binary option "<<std::endl; return false; }

}

bool AnalysisUtils::BoolValue(std::string& arg_val, const std::string& arg_name){

  std::transform(arg_val.begin(), arg_val.end(), arg_val.begin(), ::toupper);
  if( arg_val.find("TRUE") != std::string::npos ){ return true; }
  else if( arg_val.find("FALSE") != std::string::npos ){ return false; }
  else{std::cout<<"Error : Unknown value "<<arg_val<<" for binary option "<<arg_name<<std::endl; return false; }

}

//Copied from StackOverflow -
bool AnalysisUtils::FileExists(const std::string& filename){
  struct stat buf;
  if (stat(filename.c_str(), &buf) != -1){ return true; }
  return false;
}

//______________________________________________________________________________________________________________________
int AnalysisUtils::ParseConfigFile(const std::string& config_file, std::vector<std::map<std::string, 
				   std::string> >& ret_map, const std::string& delim, const bool blockformat, 
				   const std::string& flagList, const std::string& vetoList ){
  int stat = 1;
  if(blockformat){ stat = ParseConfigFile_Blocks(config_file, ret_map,  delim, flagList, vetoList); }
  else{ stat = ParseConfigFile_Lines(config_file, ret_map,  delim, flagList, vetoList); }

  return stat;

}

int AnalysisUtils::ParseConfigFile_Blocks(const std::string& config_file, std::vector<std::map<std::string, 
					  std::string> >& ret_map, const std::string& delim, 
					  const std::string& flagList, const std::string& vetoList){
  ret_map.clear();
  std::ifstream conf_stream(config_file);
  if(!conf_stream){
    std::cout<<"Error : configuration file "<<config_file<<" not found. Exiting."<<std::endl;
    return -1;
  }

  std::string conf_line; 
  bool begun = false;
  int nset = -1;
  bool cond_skip = false;
  std::string cond_term = "";
  std::string s_if    = "--if ";
  std::string s_ifnot = "--ifnot ";
  std::string s_endif = "--endif ";

  while( getline(conf_stream, conf_line) ){
    TrimString(conf_line);
    if(conf_line == "BEGIN"){begun = true; continue;}
    if(!begun){continue;}
    if(conf_line == "END") break;
    if( conf_line.empty() || (conf_line.find("#") == 0) ) continue;

    if( conf_line.find("--if") == 0 ){
      if(cond_skip){ continue; } //an outer if statement is active
      if( conf_line.find(s_if) == 0 ){
	cond_term = conf_line.substr( s_if.size());
	if( flagList.find("__"+cond_term+"__") == std::string::npos ){ cond_skip = true; }
      }
      else if( conf_line.find(s_ifnot) == 0 ){
	cond_term = conf_line.substr( s_ifnot.size());
	if( vetoList.find("__"+cond_term+"__") == std::string::npos ){ cond_skip = true; }
      }
      continue;
    }//start a conditional
    else if( conf_line.find(s_endif) == 0 ){
      if( conf_line.find( cond_term ) == std::string::npos ){ continue; }
      else{
	cond_skip = false;
	cond_term = "";
      }
      continue;
    }//end a conditional

    if(cond_skip){ continue; }
    if(conf_line == "BREAK"){ break; }

    if(conf_line == "NEW"){ 
      nset++; 
      std::map<std::string, std::string> varset;
      ret_map.push_back(varset);
    }
    else{
      std::string param; param.clear();
      std::string paramString = conf_line;
      std::string::size_type pos = ParseString(paramString, param, delim);
      if(pos == std::string::npos){ 
	std::cout<<" Could not read parameter value from line "<<conf_line<<std::endl;
	std::cout<<" Format should be [PARAM] "<<delim<<" [VALUE]"<<std::endl;
	continue;
      }
      TrimString(param);
      ret_map.at(nset)[param] = paramString;
    }
  }

  return nset+1;

}

int AnalysisUtils::ParseConfigFile_Lines(const std::string& config_file, std::vector<std::map<std::string, 
					 std::string> >& ret_map, const std::string& delim,
					 const std::string& /*flagList*/, const std::string& /*vetoList*/){
  
  ret_map.clear();

  std::map<int, std::string> paramSeq; paramSeq.clear();
  std::ifstream conf_stream(config_file);
  if(!conf_stream){
    std::cout<<"Error : configuration file "<<config_file<<" not found. Exiting."<<std::endl;
    return -1;
  }

  std::string conf_line;
  while( conf_line != "BEGIN" ){  
    getline(conf_stream, conf_line); 
    TrimString(conf_line);
  }
  getline(conf_stream, conf_line);

  //Header gives the sequence of configuration variables
  std::string param; param.clear();
  std::string paramString = conf_line;
  int nparam = 0;
  std::string::size_type pos = 0;
  do{ 
    pos = ParseString(paramString, param, delim);
    TrimString(param);
    paramSeq[nparam] = param;
    nparam++;
  }while(pos != std::string::npos);


  int nkey = nparam;
  int nline = 0;

  std::map<std::string, std::string> keymap;
  while( getline(conf_stream, conf_line) ){
    TrimString(conf_line);
    if(conf_line == "END") break;
    if( conf_line.empty() || (conf_line.find("#") == 0) ) continue;

    paramString = conf_line;
    param = "";
    nparam = 0;
    pos = 0;
    keymap.clear();

    do{
      pos = ParseString(paramString, param, delim);
      TrimString(param);
      if(nparam > nkey){
	std::cout<<"Error: Number of parameters on line "<<nline<<" exceeds number of keys given in header"<<std::endl; 
	break;
      }
      if(paramSeq.find(nparam) == paramSeq.end()){
	std::cout<<"WARNING: "<<nline<<" : param "<<nparam<<" not specified"<<std::endl;
	continue;
      } 
      keymap[ paramSeq[nparam] ] = param;
      nparam++;
    }while(pos != std::string::npos);//read all given parameters

    ret_map.push_back(keymap);
    nline++;
  }//line loop

  return nline;

}
