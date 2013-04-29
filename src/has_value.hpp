#ifndef GRAPHMOD_HAS_VALUE_HPP
#define GRAPHMOD_HAS_VALUE_HPP

#include <sstream>
#include "boost_libraries.hpp"

namespace graphmod{
  template<class data_type>
  class HasValue{
  public:
    HasValue() : _value(){
    }
    HasValue(data_type value) : _value(value){
    }
    data_type& get_value(){
      return _value;
    }
    data_type get_value_copy() const{
      return _value;
    }
    void set_value(data_type value){
      _value = value;
    }
    std::string str() const{
      std::stringstream ss;
      ss << _value;
      return ss.str();
    }
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & _value;
    }
    data_type _value;
  };
}

#endif
