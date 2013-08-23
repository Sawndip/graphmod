#ifndef GRAPHMOD_FACTOR_GRAPH_HPP
#define GRAPHMOD_FACTOR_GRAPH_HPP

#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include "factor_interface.hpp"
#include "dirichlet_categorical_factor.hpp"
#include "double_dirichlet_categorical_factor.hpp"
#include "hierarchical_dirichlet_categorical_factor.hpp"
#include "beta_bernoulli_factor.hpp"
#include "truncated_beta_bernoulli_factor.hpp"

#include "variable_interface.hpp"
#include "categorical_variable.hpp"
#include "mapped_categorical_variable.hpp"
#include "continuous_vector_variable.hpp"
#include "continuous_matrix_variable.hpp"

#include "optimizer_interface.hpp"
#include "minka_dirichlet_optimizer.hpp"
#include "minka_double_dirichlet_optimizer.hpp"
#include "minka_beta_optimizer.hpp"

#include "boost_libraries.hpp"
#include "alphabets.hpp"
#include "probability_vector.hpp"

namespace graphmod{
  template<class counts_type>
  class FactorGraph{
  public:

    FactorGraph(){
    }

    FactorGraph(const FactorGraph& other){
      std::cout << "copying..." << std::endl;      
      _counts = other._counts;
      _alphabets = other._alphabets;

      std::map<VariableInterface<counts_type>*, VariableInterface<counts_type>*> old_to_new;
      for(auto v: other._variables){
	_variables.push_back(v->clone(_alphabets));
	_variables.back()->set_observed(v->get_observed());
	old_to_new[v] = _variables.back();
      }
      
      /*
      for(auto f: other._factors){
	_factors.push_back(f->clone(old_to_new));
      }
      
      for(auto o: other._optimizers){
	_optimizers.push_back(o->clone(old_to_new));
      }
      */
    }

    FactorGraph(std::string file_name){
      namespace io = boost::iostreams;
      namespace fs = boost::filesystem;
      io::filtering_istream in;
      std::ifstream file(file_name.c_str(), std::ios_base::in | std::ios_base::binary);
  
      fs::path first = fs::extension(file_name), second = fs::extension(fs::path(file_name).stem());
  
      if(first == ".gz"){
	in.push(io::gzip_decompressor());
      }
      in.push(file);
      if(first == ".xml" or second == ".xml"){
	throw GraphmodException("XML serialization is not supported");
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

    ~FactorGraph(){
    }
    
    template<class node_type, class... Args>
    node_type* add_variable(Args... args){
      _variables.push_back(new node_type(args...));
      return dynamic_cast<node_type*>(_variables.back());
    }
    
    template<class node_type, class... Args>
    node_type* add_factor(Args... args){
      _factors.push_back(new node_type(args...));
      return dynamic_cast<node_type*>(_factors.back());
    }
    
    template<class node_type, class... Args>
    node_type* add_optimizer(Args... args){
      _optimizers.push_back(new node_type(args...));
      return dynamic_cast<node_type*>(_optimizers.back());
    }

    //
    // Factors
    //
    DirichletCategoricalFactor<counts_type>* add_dirichlet_categorical(ContinuousVectorVariable<counts_type>* prior, 
							  CategoricalVariable<counts_type>* index, 
							  CategoricalVariable<counts_type>* observation){
      return add_factor<DirichletCategoricalFactor<counts_type> >(prior, index, observation);
    }

    DoubleDirichletCategoricalFactor<counts_type>* add_double_dirichlet_categorical(ContinuousVectorVariable<counts_type>* prior, 
										    CategoricalVariable<counts_type>* indexA, 
										    CategoricalVariable<counts_type>* indexB, 
										    CategoricalVariable<counts_type>* observation){
      return add_factor<DoubleDirichletCategoricalFactor<counts_type> >(prior, indexA, indexB, observation);
    }

    HierarchicalDirichletCategoricalFactor<counts_type>* add_hierarchical_dirichlet_categorical(ContinuousVectorVariable<counts_type>* priorA, 
												CategoricalVariable<counts_type>* indexA, 
												ContinuousVectorVariable<counts_type>* priorB, 
												CategoricalVariable<counts_type>* indexB, 
												CategoricalVariable<counts_type>* observation){
      return add_factor<HierarchicalDirichletCategoricalFactor<counts_type> >(priorA, indexA, priorB, indexB, observation);
    }

    BetaBernoulliFactor<counts_type>* add_beta_bernoulli(ContinuousMatrixVariable<counts_type>* prior, 
							 CategoricalVariable<counts_type>* index, 
							 MappedCategoricalVariable<counts_type>* observation){
      return add_factor<BetaBernoulliFactor<counts_type> >(prior, index, observation);
    }

    TruncatedBetaBernoulliFactor<counts_type>* add_truncated_beta_bernoulli(ContinuousMatrixVariable<counts_type>* prior, 
									    CategoricalVariable<counts_type>* index, 
									    MappedCategoricalVariable<counts_type>* observation){
      return add_factor<TruncatedBetaBernoulliFactor<counts_type> >(prior, index, observation);
    }

    //
    // Optimizers
    //
    MinkaDirichletOptimizer<counts_type>* add_minka_dirichlet_optimizer(ContinuousVectorVariable<counts_type>* node, std::string group_name, std::string observation_name, bool symmetric){
      return add_optimizer<MinkaDirichletOptimizer<counts_type> >(node, group_name, observation_name, symmetric);
    }

    MinkaDoubleDirichletOptimizer<counts_type>* add_minka_double_dirichlet_optimizer(ContinuousVectorVariable<counts_type>* node, std::string groupA_name, std::string groupB_name, std::string observation_name, bool symmetric){
      return add_optimizer<MinkaDoubleDirichletOptimizer<counts_type> >(node, groupA_name, groupB_name, observation_name, symmetric);
    }

    MinkaBetaOptimizer<counts_type>* add_minka_beta_optimizer(ContinuousMatrixVariable<counts_type>* node, std::string group_name, std::string observation_name, bool symmetric){
      return add_optimizer<MinkaBetaOptimizer<counts_type> >(node, group_name, observation_name, symmetric);
    }

    //
    // Variables
    //
    CategoricalVariable<counts_type>* add_categorical(std::string domain_name, std::string value){
      if(_alphabets.count(domain_name) == 0){
	_alphabets[domain_name] = Alphabet<std::string>(domain_name);
      }
      int value_id = _alphabets.at(domain_name).get_index(value);
      _variables.push_back(new CategoricalVariable<counts_type>(value_id, _alphabets[domain_name]));
      _variables.back()->set_observed(true);
      return dynamic_cast<CategoricalVariable<counts_type>*>(_variables.back());
    }
 
    CategoricalVariable<counts_type>* add_categorical(std::string domain_name, int domain_size){
      if(_alphabets.count(domain_name) == 0){
	_alphabets[domain_name] = Alphabet<std::string>(domain_name, domain_size);
      }
      _variables.push_back(new CategoricalVariable<counts_type>(_alphabets[domain_name]));
      _variables.back()->set_observed(false);
      return dynamic_cast<CategoricalVariable<counts_type>*>(_variables.back());
    }

    CategoricalVariable<counts_type>* add_categorical(std::string domain_name, int domain_size, int max_value){
      if(_alphabets.count(domain_name) == 0){
	_alphabets[domain_name] = Alphabet<std::string>(domain_name, domain_size);
      }
      _variables.push_back(new CategoricalVariable<counts_type>(_alphabets[domain_name], 0, max_value));
      _variables.back()->set_observed(false);
      return dynamic_cast<CategoricalVariable<counts_type>*>(_variables.back());
    }

    MappedCategoricalVariable<counts_type>* add_mapped_categorical(std::string domain_name, std::map<std::string, bool> value){
      if(_alphabets.count(domain_name) == 0){
	_alphabets[domain_name] = Alphabet<std::string>(domain_name);
      }
      std::map<int, bool> rvalue;
      for(auto ppair: value){
	if(ppair.second){
	  rvalue[_alphabets[domain_name].get_index(ppair.first)] = true;
	}
      }
      _variables.push_back(new MappedCategoricalVariable<counts_type>(rvalue, _alphabets[domain_name]));
      return dynamic_cast<MappedCategoricalVariable<counts_type>*>(_variables.back());
    }
        
    ContinuousVectorVariable<counts_type>* add_continuous_vector(std::vector<double> value){
      return add_variable<ContinuousVectorVariable<counts_type> >(value);
    }
    
    ContinuousMatrixVariable<counts_type>* add_continuous_matrix(std::vector<std::vector<double> > value){
      return add_variable<ContinuousMatrixVariable<counts_type> >(value);
    }

    //
    // get a variable
    //
    template<class node_type>
    node_type* get_variable(std::string name){
      for(auto v: _variables){
	if(v->get_name() == name){
	  return dynamic_cast<node_type*>(v);
	}
      }
      std::stringstream ss;
      ss << "Could not find variable '" << name << "'";
      throw GraphmodException(ss.str());
    }

    //
    // Methods
    //
    void compile(){
      for(auto factor: _factors){
	factor->compile(_counts);
      }
    }

    void optimize(int iterations){
      for(auto optimizer: _optimizers){
	optimizer->optimize(_counts, iterations);
      }
    }

    double perplexity(std::vector<std::string> names){
      std::set<std::string> domains;
      std::for_each(names.begin(), names.end(), [&](std::string domain){ domains.insert(domain); });
      std::vector<VariableInterface<counts_type>*> variables;
      for(auto variable: _variables){      
	CategoricalVariable<counts_type>* cv = dynamic_cast<CategoricalVariable<counts_type>*>(variable);
	MappedCategoricalVariable<counts_type>* mv = dynamic_cast<MappedCategoricalVariable<counts_type>*>(variable);
	if((cv != nullptr and domains.count(cv->get_domain_name()) > 0) or (mv != nullptr and domains.count(mv->get_domain_name()) > 0)){
	  //if(cv != nullptr and domains.count(cv->get_domain_name()) > 0){
	  variables.push_back(variable);
	}
      } 
      std::vector<double> lls(variables.size());
      std::transform(variables.begin(), variables.end(), lls.begin(), [&](VariableInterface<counts_type>* v){ return v->log_likelihood(_counts); });
      return std::exp(-accumulate(lls.begin(), lls.end(), 0.0) / lls.size());
    }

    counts_type& get_counts(){
      return _counts;
    }

    counts_type get_counts_copy() const{
      return _counts;
    }

    static void collect_helperA(std::vector<int>& counts, std::vector<FactorInterface<counts_type>*> factors, std::string name){
      //std::cout << "Factors: " << factors.size() << std::endl;
      for(auto factor: factors){
	collect_helperB(counts, factor->get_children(), name);
      }
    }

    static void collect_helperB(std::vector<int>& counts, std::vector<VariableInterface<counts_type>*> variables, std::string name){
      //std::cout << "Variables: " << variables.size() << std::endl;
      for(auto variable: variables){
	auto cv = dynamic_cast<CategoricalVariable<counts_type>*>(variable);
	if(cv != nullptr and cv->get_domain_name() == name){
	  int name_val = cv->get_value();
	  counts[name_val]++;
	}
	collect_helperA(counts, cv->get_children(), name);
      }
    }

    std::vector<std::vector<int> > collect_counts(std::string nameA, std::string nameB){
      std::vector<std::vector<int> > retval(_alphabets[nameA].get_size());
      std::fill(retval.begin(), retval.end(), std::vector<int>(_alphabets[nameB].get_size(), 0));
      for(auto variable: _variables){
	auto cv = dynamic_cast<CategoricalVariable<counts_type>*>(variable);
	if(cv != nullptr and cv->get_domain_name() == nameA){
	  int name_a_value = cv->get_value();
	  std::vector<int> name_b_values(retval[0].size(), 0);
	  //std::cout << cv->type() << " " << cv->get_domain_name() << " parents: " << cv->get_parents().size() << " children: " << cv->get_children().size() << std::endl;
	  collect_helperA(name_b_values, cv->get_children(), nameB);
	  std::transform(retval[name_a_value].begin(), retval[name_a_value].end(), name_b_values.begin(), retval[name_a_value].begin(), [](int cur, int inc){ return cur + inc; });
	}
      }
      return retval;
    }


    Alphabet<std::string>& get_alphabet(std::string name){
      _alphabets[name].set_name(name);
      return _alphabets[name];
    }

    Alphabet<std::string>& get_alphabet(std::string name, int size){
      _alphabets[name].set_name(name);
      for(int i=0; i<size; i++){
	std::stringstream ss;
	ss << i;
	_alphabets[name][ss.str()] = i;
      }
      return _alphabets.at(name);
    }

    std::vector<std::string> get_alphabet_names(){
      std::vector<std::string> retval;
      for(auto apair: _alphabets){
	retval.push_back(apair.second.get_name());
      }
      return retval;
    }

    std::map<std::string, Alphabet<std::string> >& get_alphabets(){
      return _alphabets;
    }

    std::string str() const{
      std::stringstream ss;
      ss << "FactorGraph: " << _variables.size() << " variables, " << _factors.size() << " factors, " << _optimizers.size() << " optimizers";
      return ss.str();
    }

    void save(std::string file_name){
      namespace io = boost::iostreams;
      namespace fs = boost::filesystem;
      fs::path first = fs::extension(file_name), second = fs::extension(fs::path(file_name).stem());
      /*
      for(auto variable: _variables){
	std::cout << variable->type() << " parents: " << variable->get_parents().size() << " children: " << variable->get_children().size() << std::endl;
      }

      for(auto factor: _factors){
	std::cout << factor->type() << " parents: " << factor->get_parents().size() << " children: " << factor->get_children().size() << std::endl;
      }
      */
      
      io::filtering_ostream out;
      if(first == ".gz"){
	out.push(io::gzip_compressor());
      }
      out.push(io::file_descriptor_sink(file_name.c_str()));
      if(first == ".xml" or second == ".xml"){
	throw GraphmodException("XML serialization is not supported");
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
    std::vector<VariableInterface<counts_type>*> get_variables(){
      return _variables;
    }
    std::vector<FactorInterface<counts_type>*> get_factors(){
      return _factors;
    }
    std::vector<OptimizerInterface<counts_type>*> get_optimizers(){
      return _optimizers;
    }

    /*
    std::string xml() const{
      std::stringstream ss;
      ss << "<graphml>" << std::endl<< "\t<graph>" << std::endl;
      for(auto v: _variables){
	ss << indent(v->xml(), 2) << std::endl;
      }
      for(auto f: _factors){
	ss << indent(f->xml(), 2) << std::endl;
      }
      for(auto v: _variables){
	for(auto f: v->get_neighbors()){
	  ss << "<edge source='" << get_id(v) << "' target='" << get_id(f) << "' />" << std::endl;
	}
      }
      //for(auto o: _optimizers){
      //ss << indent(o->xml(), 2) << std::endl;
      //}
      ss << "\t</graph>"<< std::endl << "</graphml>" << std::endl;
      return ss.str();
    }
    */

  private:
    counts_type _counts;
    std::vector<VariableInterface<counts_type>*> _variables;    
    std::vector<FactorInterface<counts_type>*> _factors;
    std::vector<OptimizerInterface<counts_type>*> _optimizers;
    std::map<std::string, Alphabet<std::string> > _alphabets;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar.register_type(static_cast<MinkaDirichletOptimizer<counts_type>*>(NULL));
      ar.register_type(static_cast<MinkaDoubleDirichletOptimizer<counts_type>*>(NULL));
      ar.register_type(static_cast<MinkaBetaOptimizer<counts_type>*>(NULL));
      ar.register_type(static_cast<CategoricalVariable<counts_type>*>(NULL));
      ar.register_type(static_cast<MappedCategoricalVariable<counts_type>*>(NULL));
      ar.register_type(static_cast<ContinuousVectorVariable<counts_type>*>(NULL));
      ar.register_type(static_cast<ContinuousMatrixVariable<counts_type>*>(NULL));
      ar.register_type(static_cast<DirichletCategoricalFactor<counts_type>*>(NULL));
      ar.register_type(static_cast<DoubleDirichletCategoricalFactor<counts_type>*>(NULL));
      ar.register_type(static_cast<HierarchicalDirichletCategoricalFactor<counts_type>*>(NULL));
      ar.register_type(static_cast<BetaBernoulliFactor<counts_type>*>(NULL));
      ar.register_type(static_cast<TruncatedBetaBernoulliFactor<counts_type>*>(NULL));
      ar & _alphabets & _counts & _factors & _variables & _optimizers;
    }
  };
}

#endif
