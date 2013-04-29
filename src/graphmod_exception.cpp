#include "graphmod_exception.hpp"

namespace graphmod{
  GraphmodException::GraphmodException(){}

  GraphmodException::GraphmodException(std::string info) : _info(info){}

  GraphmodException::~GraphmodException() throw(){}

  const char* GraphmodException::what() const throw(){
    return _info.c_str();
  }
}
