#ifndef GRAPHMOD_UTILS_HPP
#define GRAPHMOD_UTILS_HPP

#include <exception>
#include <random>
#include <ostream>
#include <vector>
#include <map>
#include <string>
#include <cctype>
#include <set>
#include <deque>
#include "boost_libraries.hpp"
#include "instances.hpp"
#include "conll_word.hpp"
#include "conll_sentence.hpp"
#include "conll_data.hpp"
#include "conll_loader.hpp"

namespace graphmod{

  template<class object_type>
  std::string get_id(object_type* object){
    std::stringstream ss;
    ss << object;
    std::string retval = ss.str();
    std::string from("0123456789");
    std::string to("ABCDEFGHIJ");
    for(unsigned int i=0; i<retval.size(); i++){
      for(unsigned int j=0; j<from.size(); j++){
        if(retval[i] == from[j]){
          retval[i] = to[j];
        }
      }
    }
    return retval;
  }

  std::mt19937_64 get_random(unsigned int seed);


  std::map<std::string, std::string> collect_properties(boost::property_tree::ptree&);

  std::string indent(std::string, int);

  int find_range(double, std::vector<double>);

  graphmod::Instances from_conll(std::vector<std::string>, std::vector<std::string>, int, unsigned int, unsigned int);

  graphmod::Instances from_valex_conll(std::vector<std::string>, std::vector<std::string>, int, unsigned int, unsigned int);

  graphmod::Instances from_lines(std::string, std::vector<std::string>, unsigned int);

  double add_logs(double, double);
  double add_logs(std::vector<double>, double=0.0);

  template<class key_type, class value_type>
  std::ostream& operator<<(std::ostream& stream, const std::map<key_type, value_type>& object){
    for(auto kv_pair: object){
      stream << kv_pair.first << "=" << kv_pair.second << " ";
    }
    return stream;
  }

  template<class data_type>
  std::ostream& operator<<(std::ostream& out, const std::vector<data_type>& object){
    out << "[";
    for(unsigned int i=0; i<object.size(); i++){
      out << object[i];
      if(i < object.size() - 1){
	out << ", ";
      }
    }
    out << "]";
    return out;
  }

  template<class first_type, class second_type>
  std::ostream& operator<<(std::ostream& out, const std::pair<first_type, second_type>& object){
    out << object.first << "=" << object.second;
    return out;
  }

  template<class data_type>
  std::ostream& operator<<(std::ostream& out, const std::deque<data_type>& object){
    out << "[";
    for(int i=0; i<object.size(); i++){
      out << object[i];
      if(i < object.size() - 1){
	out << ", ";
      }
    }
    out << "]";
    return out;
  }

  template<class data_type>
  std::vector<std::vector<data_type> > transpose(const std::vector<std::vector<data_type> > matrix){
    int nrows = matrix.size(), ncols = matrix[0].size();
    std::vector<int> row_indices(nrows), col_indices(ncols);
    std::iota(row_indices.begin(), row_indices.end(), 0);
    std::iota(col_indices.begin(), col_indices.end(), 0);
    std::vector<std::vector<data_type> > retval;
    std::transform(col_indices.begin(), col_indices.end(), std::back_inserter(retval), [&matrix,&row_indices](int old_col){
	std::vector<data_type> new_row;
	std::transform(row_indices.begin(), row_indices.end(), std::back_inserter(new_row), [&matrix,old_col](int old_row){
	    return matrix[old_row][old_col];
	  }); 
	return new_row;
      });
    return retval;
  }

  template<class data_type>
  data_type matrix_sum(const std::vector<std::vector<data_type> > matrix){
    return std::accumulate(matrix.begin(), matrix.end(), 0.0, [](const data_type total, const std::vector<data_type> row){
	return total + std::accumulate(row.begin(), row.end(), 0);
      });
  }

}

#endif
