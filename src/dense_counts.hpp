#ifndef GRAPHMOD_DENSE_COUNTS_HPP
#define GRAPHMOD_DENSE_COUNTS_HPP

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include "boost_libraries.hpp"
#include "counts.hpp"

namespace graphmod{
  class DenseCounts : public Counts<DenseCounts>{
  public:
    typedef Counts<DenseCounts>::vector_key_type vector_key_type;
    typedef Counts<DenseCounts>::vector_key_type matrix_key_type;
    typedef Counts<DenseCounts>::vector_key_type cube_key_type;
    typedef Counts<DenseCounts>::vector_type vector_type;
    typedef Counts<DenseCounts>::matrix_type matrix_type;
    typedef Counts<DenseCounts>::cube_type cube_type;
    typedef Counts<DenseCounts>::name_list_type name_list_type;
    typedef Counts<DenseCounts>::value_list_type value_list_type;
    typedef Counts<DenseCounts>::size_list_type size_list_type;
    typedef Counts<DenseCounts>::index_list_type index_list_type;
    typedef Counts<DenseCounts>::name_changes_type name_changes_type;
    typedef Counts<DenseCounts>::value_changes_type value_changes_type;
    typedef Counts<DenseCounts>::increment_changes_type increment_changes_type;
    
    void add_target_implementation(const name_list_type names, const size_list_type sizes){
      if(sizes.size() == 1 and _vectors.count(names) == 0){
	_vectors[names].resize(sizes[0], 0);	
      }
      else if(sizes.size() == 2 and _matrices.count(names) == 0){
	_matrices.insert({names, matrix_type()});
	_matrices[names].resize(sizes[0]);
	for(auto& row: _matrices[names]){
	  row.resize(sizes[1], 0);
	}
      }
      else if(sizes.size() == 3 and _cubes.count(names) == 0){
	_cubes[names].resize(sizes[0]);
	for(auto& row: _cubes[names]){
	  row.resize(sizes[1]);
	  for(auto& column: row){
	    column.resize(sizes[2], 0);
	  }
	}
      }
      else if(sizes.size() > 3 or sizes.size() < 1){
	std::stringstream ss;
	ss << "wrong number of dimensions to a Count object:" << std::endl;
	throw GraphmodException(ss.str());
      }
    }

    vector_type parentheses_operator_implementation(std::string key) const{
      return _vectors.at(vector_key_type({key}));
    }
    matrix_type parentheses_operator_implementation(std::string keyA, std::string keyB) const{
      return _matrices.at(matrix_key_type({keyA, keyB}));
    }
    cube_type& parentheses_operator_implementation(std::string keyA, std::string keyB, std::string keyC){
      return _cubes.at(cube_key_type({keyA, keyB, keyC}));
    }

    int parentheses_operator_implementation(name_list_type names, index_list_type indices) const{
      if(names.size() == 3){
	//return _cubes[names][indices[0]][indices[1]][indices[2]];
	return _cubes.at(names)[indices[0]][indices[1]][indices[2]];
      }
      else if(names.size() == 2){
	//return _matrices[names][indices[0]][indices[1]];
	return _matrices.at(names)[indices[0]][indices[1]];
      }
      else if(names.size() == 1){
	//return _vectors[names][indices[0]];
	return _vectors.at(names)[indices[0]];
      }
      else{
	throw GraphmodException("DenseCounts does not handle >3 dimensions!");
      }
    }
    std::vector<name_list_type> get_targets_implementation() const{
      std::vector<name_list_type> retval;
      for(auto x: _vectors){
	retval.push_back({x.first});
      }
      for(auto x: _matrices){
	retval.push_back(x.first);
      }
      for(auto x: _cubes){
	retval.push_back(x.first);
      }
      return retval;
    }
    std::string str_implementation() const{
      std::stringstream ss;
      ss << "DenseCounts: ";      
      for(auto x: _vectors){
	ss << x.first << " " << x.second.size() << std::endl;
      }
      for(auto x: _matrices){
	ss << x.first[0] << "X" << x.first[1] << " " << x.second.size() << " " << x.second[0].size() << std::endl;
      }
      for(auto x: _cubes){
	ss << x.first[0] << "X" << x.first[1] << "X" << x.first[2] << " " << x.second.size() << " " << x.second[0].size() << " " << x.second[0][0].size() << std::endl;
      }
      return ss.str();
    }
    void increment_implementation(name_list_type names, value_list_type values, int weight){
      _name_changes.push_back(names);
      _value_changes.push_back(values);
      _increment_changes.push_back(weight);
      //_changes.push_back(std::make_tuple(names, values, weight));
      if(names.size() == 1){
	_vectors[names][values[0]] += weight;
      }
      else if(names.size() == 2){
	_matrices[names][values[0]][values[1]] += weight;
      }
      else if(names.size() == 3){
	_cubes[names][values[0]][values[1]][values[2]] += weight;
      }
      else{
	throw GraphmodException("DenseCounts does not store > 3 dimensions");
      }
    }
    void clear_changes_implementation(){
      _name_changes.clear();
      _value_changes.clear();
      _increment_changes.clear();
    }
    name_changes_type& get_name_changes_implementation(){
      return _name_changes;
    }    
    value_changes_type& get_value_changes_implementation(){
      return _value_changes;
    }    
    increment_changes_type& get_increment_changes_implementation(){
      return _increment_changes;
    }    
    name_changes_type get_name_changes_copy_implementation() const{
      return _name_changes;
    }    
    value_changes_type get_value_changes_copy_implementation() const{
      return _value_changes;
    }    
    increment_changes_type get_increment_changes_copy_implementation() const{
      return _increment_changes;
    }    
  private:
    std::map<vector_key_type, vector_type> _vectors;
    std::map<matrix_key_type, matrix_type> _matrices;
    std::map<cube_key_type, cube_type> _cubes;    
    name_changes_type _name_changes;
    value_changes_type _value_changes;
    increment_changes_type _increment_changes;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<Counts<DenseCounts> >(*this);
      ar & _vectors & _matrices & _cubes;
    }
  };

}

#endif
