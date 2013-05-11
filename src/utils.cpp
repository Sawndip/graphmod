#include "utils.hpp"


namespace graphmod{

  using namespace std;
  using namespace boost;

  std::string indent(std::string s, int d){
    return s;
  }

  bool function_tag(char c){
    if(c == 'I' || c == 'D' || c == 'P' || c == 'T'){
      return true;
    }
    else{
      return false;
    }
  }

  map<string, string> collect_properties(property_tree::ptree& pt){
    map<string, string> retval;

    for(auto ptp: pt){
      string tag = ptp.first;
      auto next_pt = ptp.second;
      if(tag == "data"){
	string key = next_pt.get<string>("<xmlattr>.key"), value = next_pt.get("<xmlattr>.value", next_pt.data());
	retval[key] = value;
      }
      else if(tag == "<xmlattr>"){
	for(auto ptpB: next_pt){
	  retval[ptpB.first] = ptpB.second.data();
	}
      }
      else{
	//cout << tag << endl;
	//_collect_properties_helper(next_pt, object);
      }
    }
    //_collect_properties_helper(pt, retval);
    return retval;
  }




  int find_range(double val, vector<double> ranges){
    double total = 0.0;
    for(unsigned int i=0; i<ranges.size(); i++){
      total += ranges[i];
      if(val < total){
	return i;
      }
    }
    return ranges.size() - 1;
  }

  graphmod::Instances from_conll(vector<string> file_names, vector<string> _keep_verbs, int window, unsigned int limit=0){
    Instances instances;
    set<string> keep_verbs;
    for(string verb: _keep_verbs){
      keep_verbs.insert(verb);
    }
    ConllLoader sentences(file_names);
    //boost::regex expr("\\n\\s*\\n", boost::regex::perl);
    //boost::regex_iterator 
    //int total = 0;
    while(not sentences.eof() and (limit == 0 or instances.size() < limit)){
      auto cs = sentences.next();
      for(ConllWord cw: cs){
	if(cw.get_fine_tag()[0] == 'V' and (keep_verbs.count(cw.get_lemma()) > 0 or keep_verbs.size() == 0)){
	  map<string, vector<string> > instance;
	  instance["verb"] = {cw.get_lemma()};

	  instance["verb_tag"] = {cw.get_fine_tag()};
	  instance["tag"].resize(0);
	  instance["gr"].resize(0);
	  instance["lemma"].resize(0);
	  if(window > 0){
	    for(ConllWord ow: cs.get_near(cw, window)){
	      instance["tag"].push_back(ow.get_fine_tag());
	      instance["lemma"].push_back(ow.get_lemma());
	    }
	  }
	  else{
	    //instance["relation"].resize(0);
	    for(ConllWord ow: cs.get_related(cw)){
	      stringstream ss;
	      string tag = ow.get_fine_tag(), gr = ow.get_relation();
	      if(function_tag(tag[0]) == true){
		ss << gr << "(" << tag << "-" << ow.get_lemma() << "," << cw.get_fine_tag() << ")";
	      }
	      else{
		ss << gr << "(" << tag << "," << cw.get_fine_tag() << ")";
	      }

	      instance["tag"].push_back(tag);
	      //instance["gr"].push_back(ss.str());
	      instance["gr"].push_back(gr);
	      instance["lemma"].push_back(ow.get_lemma());
	    }
	  }
	  instances.add(instance);
	}
      }
    }
    return instances;
    //cout << total << endl;
  }

  graphmod::Instances from_lines(string file_name, vector<string> stopword_list, unsigned int min_size){
    Instances instances;
    set<string> stopwords;
    for(string w: stopword_list){
      stopwords.insert(w);
    }
    namespace io = boost::iostreams;
    ifstream file(file_name.c_str(), ios_base::in);
    io::filtering_stream<io::input> in;
    in.push(file);
    string line;
    boost::regex expr("\\s+", boost::regex::perl);
    while(in){
      getline(in, line);
      map<string, vector<string> > instance;
      boost::sregex_token_iterator t1(line.begin(), line.end(), expr, -1);
      boost::sregex_token_iterator t2;
      while(t1 != t2){
	string value = *t1;
	for(auto& c: value){
	  c = tolower(c);
	}
	if(stopwords.count(value) == 0 and value.size() > min_size){
	  instance["token"].push_back(value);
	}
	t1++;
      }
      if(instance["token"].size() > 0){
	instances.add(instance);
      }
    }
    return instances;
  }

  double add_logs(double log_a, double log_b){
    double small, large;
    if(log_a < log_b){
      small = log_a;
      large = log_b;
    }
    else{
      small = log_b;
      large = log_a;
    }
    double neg_diff = small - large;
    if(neg_diff < -20){
      return large;
    }
    return large + std::log(1.0 + std::exp(neg_diff));
  }

  double add_logs(std::vector<double> log_probs, double total){
    if(log_probs.size() == 0){
      return total;
    }
    else{
      vector<double> temp(log_probs);
      double next = temp.back();
      temp.pop_back();
      if(total == 0.0){
	return add_logs(temp, next);
      }
      else{
	double new_total = add_logs(total, next);
	return add_logs(temp, new_total);
	  //std::accumulate(log_probs.begin(), log_probs.end(), 0.0);
      }
    }
  }

}
