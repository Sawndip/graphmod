#include "conll_word.hpp"

namespace graphmod{

  using namespace std;

  ConllWord::ConllWord(){
  }

  ConllWord::ConllWord(string line, string sep){
    boost::char_separator<char> separator("\t");
    vector<string> fields;
    boost::tokenizer<boost::char_separator<char> > ltokenizer(line, separator);   
    for(auto token: ltokenizer){
      fields.push_back(token);
    }
    if(fields.size() != 10){
      _valid = false;
    }
    else{
      _index = atoi(fields[0].c_str()) - 1;
      _form = fields[1];
      _lemma = fields[2];
      _coarse_tag = fields[3];
      _fine_tag = fields[4];
      _feats = fields[5];
      _head = atoi(fields[6].c_str()) - 1;
      _relation = fields[7];
      _projective_head = atoi(fields[8].c_str()) - 1;
      _projective_relation = fields[9];
      _valid = true;
    }
  }

  bool ConllWord::get_valid() const{
    return _valid;
  }

  string ConllWord::get_form() const{
    return _form;
  }

  string ConllWord::get_lemma() const{
    return _lemma;
  }

  string ConllWord::get_fine_tag() const{
    return _fine_tag;
  }

  string ConllWord::get_relation() const{
    return _relation;
  }

  int ConllWord::get_index() const{
    return _index;
  }

  int ConllWord::get_head() const{
    return _head;
  }
  
  ostream& operator<<(ostream& out, const ConllWord& word){
    out << word.get_form();
    return out;
  }

}
