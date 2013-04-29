#ifndef GRAPHMOD_ALPHABET_HPP
#define GRAPHMOD_ALPHABET_HPP

#include <map>
#include <string>
#include "boost_libraries.hpp"

namespace graphmod{
  template<class object_type>
  class Alphabet : public std::map<object_type, int>{
  private:
    std::string _name;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<std::map<object_type, int> >(*this);
      ar & _name;
    }
  public:
    using std::map<object_type, int>::size;
    using std::map<object_type, int>::count;
    Alphabet() : _name(""){
    }
    Alphabet(std::string name) : _name(name){
      //std::cout << name << std::endl;
    }
    Alphabet(std::string name, int size) : _name(name){
      //std::cout << name << " " << size << std::endl;
      for(int i=0; i<size; i++){
	std::stringstream ss;
	ss << i;
	this->operator[](ss.str()) = i;
      }
    }
    std::string get_name() const{
      return _name;
    }
    void set_name(std::string name){
      _name = name;
    }
    int get_size(){
      return this->size();
    }
    object_type get_object(int value){
      for(auto si_pair: *this){
	if(si_pair.second == value){
	  return si_pair.first;
	}
      }
      return object_type();
    }
    int get_index(object_type object){
      if(count(object) == 0){
	int next_integer = size();
	//std::cout << object << " added with index " << next_integer << std::endl; 
	this->operator[](object) = next_integer;
      }
      return this->at(object);
    }    
  };
}

#endif
