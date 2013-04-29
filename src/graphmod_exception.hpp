#ifndef GRAPHMOD_EXCEPTION_HPP
#define GRAPHMOD_EXCEPTION_HPP

#include <exception>
#include <string>

namespace graphmod{
  class GraphmodException : public std::exception{
  public:
    GraphmodException();
    GraphmodException(std::string info);
    ~GraphmodException() throw();
    const char* what() const throw();
  protected:
    std::string _info;
  };
}

#endif
