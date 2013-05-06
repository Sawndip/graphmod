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
    //typedef typename configuration::counts_type counts_type;

    FactorGraph(){
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
    
    void settle_counts(){
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

    void sample(){
      for(auto variable: _variables){
	if(not variable->get_observed()){
	  variable->sample(_counts);
	}
      }
    }


    counts_type& get_counts(){
      return _counts;
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

    std::string str() const{
      std::stringstream ss;
      ss << "FactorGraph: " << _variables.size() << " variables, " << _factors.size() << " factors, " << _optimizers.size() << " optimizers";
      return ss.str();
    }
    std::string xml() const{
      return "</xml>";
    }
    void save(std::string file_name){
      namespace io = boost::iostreams;
      namespace fs = boost::filesystem;
      fs::path first = fs::extension(file_name), second = fs::extension(fs::path(file_name).stem());

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
      ar & _alphabets & _counts & _factors & _variables & _optimizers;
    }
  };
}

#endif
