#ifndef GRAPHMOD_CATEGORICAL_HPP
#define GRAPHMOD_CATEGORICAL_HPP

#include <iostream>
#include <string>
#include <vector>
#include "alphabet.hpp"

namespace graphmod{
  template<class object_type>
  class Categorical{
  public:
    Categorical(){
      //std::cout << "shouldn't make cat this way!" << std::endl;
    }
    Categorical(Alphabet<object_type>& alphabet) : _alphabet(&alphabet){
      //std::cout << "C(alphabet) " << std::endl;
    }
    std::string get_domain_name() const{
      return _alphabet->get_name();
    }
    unsigned int get_domain_size() const{
      return _alphabet->get_size();
    }
    Alphabet<object_type>& get_alphabet(){
      return *_alphabet;
    }
    object_type lookup(int index) const{
      return _alphabet->get_object(index);
    }
  private:
    Alphabet<object_type>* _alphabet;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & _alphabet;
    }
  };
}

#endif
