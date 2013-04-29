#include "conll_loader.hpp"

namespace graphmod{
  using namespace std;
  using std::string;
  using std::vector;

  ConllLoader::ConllLoader(string file_name) : _file_names({file_name}){
    //_in.push(boost::iostreams::gzip_decompressor());
    //std::ifstream f(file_name.c_str(), std::ios_base::in | std::ios_base::binary);
    //_in.push(f);
  }

  ConllLoader::ConllLoader(vector<string> file_names) : _file_names(file_names){
    //_in.push(boost::iostreams::gzip_decompressor());
    //for(auto file_name: file_names){
      //std::ifstream f(file_name.c_str(), std::ios_base::in | std::ios_base::binary);
      //_in.push(f);
      //_in.push(std::ifstream(file_name.c_str(), std::ios_base::in | std::ios_base::binary));
    //}
  }

  //ConllData ConllLoader::operator()(){
  //}

  bool ConllLoader::eof(){
    //return ((not _in) && (_file_names.size() == 0));
    //if(not _in){
    //cout << ((not _in) and (_file_names.size() == 0)) << endl;
    return ((not _in) and (_file_names.size() == 0));
    //return true;
  }

  ConllSentence ConllLoader::next(){
    if(not _in and _file_names.size() > 0){
      if(_fd.is_open()){
	_fd.close();
      }
      _in.reset();
      _in.push(boost::iostreams::gzip_decompressor());
      string file_name = _file_names.back();
      _fd.open(file_name.c_str(), std::ios_base::in | std::ios_base::binary);
      _in.push(_fd);
      _file_names.pop_back();
    }


    std::stringstream ss;
    std::string line;
    boost::cmatch what;
    boost::regex emp("^\\s*$", boost::regex::perl);
    while(_in){
      getline(_in, line);
      //cout << line << endl;
      if(boost::regex_match(line.c_str(), what, emp)){
	//cout << "one match" << endl;
	break;
      }
      ss << line << endl;
    }
    //cout << ss.str() << endl;
    return ConllSentence(ss.str());
    /*
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
    */
  }
}
