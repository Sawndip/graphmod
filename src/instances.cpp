#include "instances.hpp"


namespace graphmod{
  using namespace std;

  typedef pair<string, vector<string> > FeaturePair;

  Instances::Instances(){}

  Instances::Instances(string file_name){
    namespace io = boost::iostreams;
    namespace fs = boost::filesystem;
    io::filtering_istream in;
    ifstream file(file_name.c_str(), ios_base::in | ios_base::binary);
  
    fs::path first = fs::extension(file_name), second = fs::extension(fs::path(file_name).stem());
  
    if(first == ".gz"){
      in.push(io::gzip_decompressor());
    }
    in.push(file);
    if(first == ".xml" or second == ".xml"){
      boost::archive::xml_iarchive arch(in);
      arch >> BOOST_SERIALIZATION_NVP(*this);
    }
    else if(first == ".txt" or second == ".txt"){
      boost::archive::text_iarchive arch(in);
      arch >> BOOST_SERIALIZATION_NVP(*this);
    }
    else{
      boost::archive::binary_iarchive arch(in);
      arch >> BOOST_SERIALIZATION_NVP(*this);
    }

  }

  vector<string> Instances::get_lookup_names() const{
    vector<string> retval;
    for(auto x: _lookup){
      retval.push_back(x.first);
    }
    return retval;
  }

  void Instances::save(string file_name){
    namespace io = boost::iostreams;
    namespace fs = boost::filesystem;
    fs::path first = fs::extension(file_name), second = fs::extension(fs::path(file_name).stem());

    io::filtering_ostream out;
    if(first == ".gz"){
      out.push(io::gzip_compressor());
    }
    out.push(io::file_descriptor_sink(file_name.c_str()));
    if(first == ".xml" or second == ".xml"){
      boost::archive::xml_oarchive arch(out);
      arch << BOOST_SERIALIZATION_NVP(*this);
    }
    else if(first == ".txt" or second == ".txt"){
      boost::archive::text_oarchive arch(out);
      arch << BOOST_SERIALIZATION_NVP(*this);
    }
    else{
      boost::archive::binary_oarchive arch(out);
      arch << BOOST_SERIALIZATION_NVP(*this);
    }
  }

  string Instances::str() const{
    stringstream ss;
    ss << size();
    for(auto p: _lookup){
      ss << " " << p.first << "[" << p.second.size() << "]";
    }
    return ss.str();
  }


  void Instances::add(map<string, vector<string> > string_instance){
    Instance new_instance;
    for(auto fp: string_instance){
      string name = fp.first;
      if(_lookup.count(name) == 0){
	_lookup[name].clear();
      }
      vector<int> int_rep;
      for(string value: fp.second){
	if(_lookup[name].left.count(value) == 0){
	  _lookup[name].insert(boost::bimap<string, int>::value_type(value, _lookup[name].size()));
	}
	int_rep.push_back(_lookup[name].left.at(value));
      }
      new_instance[name] = int_rep;
    }
    push_back(new_instance);
  }

  /*
  void Instances::print(ostream& out) const{
    out << size();
    for(auto p: _lookup){
      out << " " << p.first << "[" << p.second.size() << "]";
    }
  }
  */
  int Instances::get_size(string name){
    if(name == "instance"){
      return size();
    }
    else{
      return _lookup[name].size();
    }
  }

  string Instances::get_name(string n, int val){
    return _lookup[n].right.at(val);
  }

  bool Instances::exists(string n){
    return _lookup.count(n) > 0 or n == "instance";
  }

  int Instances::get_size(){
    return size();
  }

  string Instances::xml(){
    return "";
  }

  vector<int> Instances::get_counts(string name){
    vector<int> retval(get_size(name), 0);
    for(auto instance: *this){
      for(auto value: instance[name]){
	retval[value]++;
      }
    }
    return retval;
  }

  Instance& Instances::get_instance(int i){
    return operator[](i);
  }
}
