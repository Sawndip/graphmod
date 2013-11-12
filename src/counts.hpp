#ifndef GRAPHMOD_COUNTS_HPP
#define GRAPHMOD_COUNTS_HPP

#include <vector>
#include <string>
#include "boost_libraries.hpp"

namespace graphmod{
  template<class Implementation>
  class Counts{
  public:    
    typedef std::vector<std::string> vector_key_type;
    typedef std::vector<std::string> matrix_key_type;
    typedef std::vector<std::string> cube_key_type;    
    typedef std::vector<int> vector_type;
    typedef std::vector<vector_type> matrix_type;
    typedef std::vector<matrix_type> cube_type;
    typedef std::vector<std::string> name_list_type;
    typedef std::vector<int> value_list_type;
    typedef std::vector<unsigned int> size_list_type;
    typedef std::vector<unsigned int> index_list_type;

    typedef std::vector<name_list_type> name_changes_type;
    typedef std::vector<value_list_type> value_changes_type;
    typedef std::vector<int> increment_changes_type;

    void add_target(const name_list_type names, const size_list_type sizes){
      static_cast<Implementation*>(this)->add_target_implementation(names, sizes);
    }

    vector_type& operator()(std::string key){
      return static_cast<Implementation*>(this)->parentheses_operator_implementation(key);
    }

    matrix_type& operator()(std::string keyA, std::string keyB){
      return static_cast<Implementation*>(this)->parentheses_operator_implementation(keyA, keyB);
    }

    cube_type& operator()(std::string keyA, std::string keyB, std::string keyC){
      return static_cast<Implementation*>(this)->parentheses_operator_implementation(keyA, keyB, keyC);
    }

    int operator()(const name_list_type names, const index_list_type indices) const{
      return static_cast<const Implementation*>(this)->parentheses_operator_implementation(names, indices);
    }

    std::vector<name_list_type> get_targets() const{
      return static_cast<const Implementation*>(this)->get_targets_implementation();
    }

    std::string str() const{
      return static_cast<const Implementation*>(this)->str_implementation();
    }

    void increment(name_list_type names, value_list_type values, int weight){
      static_cast<Implementation*>(this)->increment_implementation(names, values, weight);
    }

    name_changes_type& get_name_changes(){
      return static_cast<Implementation*>(this)->get_name_changes_implementation();
    }
    value_changes_type& get_value_changes(){
      return static_cast<Implementation*>(this)->get_value_changes_implementation();
    }

    increment_changes_type& get_increment_changes(){
      return static_cast<Implementation*>(this)->get_increment_changes_implementation();
    }

    name_changes_type get_name_changes_copy(){
      return static_cast<const Implementation*>(this)->get_name_changes_copy_implementation();
    }
    value_changes_type get_value_changes_copy(){
      return static_cast<const Implementation*>(this)->get_value_changes_copy_implementation();
    }

    increment_changes_type get_increment_changes_copy(){
      return static_cast<const Implementation*>(this)->get_increment_changes_copy_implementation();
    }
    void clear_changes(){
      static_cast<Implementation*>(this)->clear_changes_implementation();
    }
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
    }
  };
}

#endif
