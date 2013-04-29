#ifndef GRAPHMOD_CONLL_LOADER_HPP
#define GRAPHMOD_CONLL_LOADER_HPP
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "conll_data.hpp"
#include "boost_libraries.hpp"

namespace graphmod{
  class ConllLoader{
  public:
    ConllLoader(std::string);
    ConllLoader(std::vector<std::string>);
    //ConllData operator()();
    bool eof();
    ConllSentence next();
  private:
    std::vector<std::string> _file_names;
    std::ifstream _fd;
    boost::iostreams::filtering_stream<boost::iostreams::input> _in;
  };
}

#endif
