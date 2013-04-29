#include "conll_data.hpp"

namespace graphmod{
  using namespace std;
  ConllData::ConllData(string file_name){
    process_file(file_name);
  }  
  ConllData::ConllData(vector<string> file_names){
    for(auto file_name: file_names){
      process_file(file_name);
    }
  }

  void ConllData::process_file(string file_name){
    namespace io = boost::iostreams;
    ifstream file(file_name.c_str(), ios_base::in | ios_base::binary);
    io::filtering_stream<io::input> in;
    in.push(io::gzip_decompressor());
    in.push(file);
    vector<string> items;

    boost::regex expr("\\n\\s*\\n", boost::regex::perl);
    stringstream ss;
    string line;
    while(in){
      getline(in, line);
      ss << line << endl;
    }
    string all_lines = ss.str();
    boost::sregex_token_iterator t1(all_lines.begin(), all_lines.end(), expr, -1);
    boost::sregex_token_iterator t2;
    while(t1 != t2){
      string value = *t1;
      ConllSentence sent(value);
      if(sent.get_valid()){
	push_back(sent);
      }
      t1++;
    }    
  }

}
