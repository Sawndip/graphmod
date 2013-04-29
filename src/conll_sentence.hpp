#ifndef GRAPHMOD_CONLL_SENTENCE_HPP
#define GRAPHMOD_CONLL_SENTENCE_HPP

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
#include "conll_word.hpp"

namespace graphmod{

  class ConllSentence : public std::vector<ConllWord>{
  private:
    bool _valid;
  public:
    ConllSentence();
    ConllSentence(std::string);
    //void print(std::ostream& out) const;
    std::vector<ConllWord> get_related(ConllWord);
    std::vector<ConllWord> get_near(ConllWord, int);
    bool get_valid();
  };
}

#endif
