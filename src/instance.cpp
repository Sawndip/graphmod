#include "instance.hpp"

namespace graphmod{

  using namespace std;

  vector<int> Instance::get(string name){
    return operator[](name);
  }

  string Instance::str() const{
    stringstream ss;
    ss << "Instance: ";
    for(auto pair: *this){
      ss << pair.first << "[" << pair.second.size() << "] ";
    }
    return ss.str();
  }
}
