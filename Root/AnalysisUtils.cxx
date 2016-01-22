#include <sys/stat.h>
#include <iostream>
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
