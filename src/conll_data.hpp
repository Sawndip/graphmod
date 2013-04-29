#ifndef GRAPHMOD_CONLL_DATA_HPP
#define GRAPHMOD_CONLL_DATA_HPP

#include <iostream>
#include <vector>
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/regex.hpp>
#include "conll_sentence.hpp"
namespace graphmod{
  class ConllData : public std::vector<ConllSentence>{
  public:
    ConllData(std::string);
    ConllData(std::vector<std::string>);
    void process_file(std::string);
  private:
    
  };
}

#endif
