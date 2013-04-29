#include "conll_sentence.hpp"

namespace graphmod{
  
  using namespace std;

  ConllSentence::ConllSentence(){
  }

  ConllSentence::ConllSentence(string lines){
    boost::char_separator<char> separator("\n");
    boost::tokenizer<boost::char_separator<char> > ltokenizer(lines, separator);   
    _valid = true;
    for(auto line: ltokenizer){
      push_back(ConllWord(line));
      if(not back().get_valid()){
	_valid = false;
      }
    }  
  }

  vector<ConllWord> ConllSentence::get_related(ConllWord word){
    vector<ConllWord> retval;
    for(ConllWord other: *this){
      if(other.get_head() == word.get_index()){
	retval.push_back(other);
      }
    }
    return retval;
  }

  bool ConllSentence::get_valid(){
    return _valid;
  }

  vector<ConllWord> ConllSentence::get_near(ConllWord word, int window){
    vector<ConllWord> retval;
    for(ConllWord other: *this){
      int diff = abs(other.get_index() - word.get_index());
      if(diff > 0 and diff <= window){
	retval.push_back(other);
      }
    }
    return retval;
  }

}
