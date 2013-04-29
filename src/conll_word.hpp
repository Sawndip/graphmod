#ifndef GRAPHMOD_CONLL_WORD_HPP
#define GRAPHMOD_CONLL_WORD_HPP

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

namespace graphmod{

  class ConllWord{
  private:
    bool _valid;
    int _index, _head, _projective_head;
    std::string _form, _lemma, _coarse_tag, _fine_tag, _feats, _relation, _projective_relation;
  public:
    ConllWord();
    ConllWord(std::string, std::string sep="\t");
    std::string get_form() const;
    std::string get_lemma() const;
    std::string get_relation() const;
    int get_head() const;
    int get_index() const;
    std::string get_fine_tag() const;
    bool get_valid() const;
  };
  std::ostream& operator<<(std::ostream& out, const ConllWord& word);
}

#endif
