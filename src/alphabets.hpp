#ifndef GRAPHMOD_ALPHABETS_HPP
#define GRAPHMOD_ALPHABETS_HPP

#include "alphabet.hpp"

namespace graphmod{
  template<class object_type>
  class Alphabets : public std::map<std::string, Alphabet<object_type> >{
  public:
    //using std::map<std::string, Alphabet<object_type> >::count;
    //using std::map<std::string, Alphabet<object_type> >::operator[];
    Alphabet<object_type>& operator()(std::string name){
      if(this->count(name) == 0){
	std::cout << "creating alphabet for " << name << std::endl;
	this->operator[](name) = Alphabet<object_type>(name);
      }
      return this->at(name);
    }
    Alphabet<object_type>& operator()(std::string name, int size){
      if(this->count(name) == 0){
	std::cout << "creating alphabet for " << name << std::endl;
	this->operator[](name) = Alphabet<object_type>(name, size);
      }
      return this->at(name);
    }
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<std::map<std::string, Alphabet<object_type> > >(*this);
    }
  };
}

#endif
