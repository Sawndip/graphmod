#ifndef GRAPHMOD_CATEGORICAL_VARIABLE_HPP
#define GRAPHMOD_CATEGORICAL_VARIABLE_HPP

#include <string>
#include <sstream>
#include <cmath>
#include "variable.hpp"
#include "alphabet.hpp"
#include "has_value.hpp"
#include "categorical.hpp"
#include "boost_libraries.hpp"

namespace graphmod{
  template<class counts_type>
  class CategoricalVariable : public Variable<CategoricalVariable<counts_type>, counts_type>, public HasValue<int>, public Categorical<std::string>{
  public:
    using VariableInterface<counts_type>::get_neighbors;

    CategoricalVariable() : 
      Variable<CategoricalVariable<counts_type>, counts_type >(false), 
      HasValue<int>(-1), 
      Categorical<std::string>(), 
      _lower(-1), 
      _upper(-1){
    }

    CategoricalVariable(std::string value, Alphabet<std::string>& alphabet) : 
      Variable<CategoricalVariable<counts_type>, counts_type >(true), 
      HasValue<int>(alphabet.get_index(value)), 
      Categorical<std::string>(alphabet), 
      _lower(-1), 
      _upper(-1){
    }

    CategoricalVariable(int value, Alphabet<std::string>& alphabet) : 
      Variable<CategoricalVariable<counts_type>, counts_type >(true), 
      HasValue<int>(value), 
      Categorical<std::string>(alphabet), 
      _lower(-1), 
      _upper(-1){
    }

    CategoricalVariable(Alphabet<std::string>& alphabet) : 
      Variable<CategoricalVariable<counts_type>, counts_type >(false), 
      HasValue<int>(-1), 
      Categorical<std::string>(alphabet), 
      _lower(-1), 
      _upper(-1){
    }

    CategoricalVariable(Alphabet<std::string>& alphabet, int lower, int upper) : 
      Variable<CategoricalVariable<counts_type>, counts_type >(false), 
      HasValue<int>(-1), 
      Categorical<std::string>(alphabet), 
      _lower(lower), 
      _upper(upper){
    }

    virtual ~CategoricalVariable(){
    }

    void compile(counts_type& counts){
      if(_lower == -1){
	_lower = 0;
      }
      if(_upper == -1){
	_upper = get_domain_size();
      }
    }

    LogProbabilityVector log_densities(counts_type& counts) const{
      LogProbabilityVector log_probs(get_domain_size());
      for(auto factor: get_neighbors()){
	log_probs = log_probs * factor->log_densities(counts, this);
      }
      return log_probs;
    }

    double log_likelihood(counts_type& counts) const{
      auto factors = get_neighbors();
      std::vector<double> log_densities;
      std::transform(factors.begin(), factors.end(), std::back_inserter(log_densities), [&counts](FactorInterface<counts_type>* factor){
	  return factor->log_density(counts);
	});
      return add_logs(log_densities);
	//std::accumulate(log_densities.begin(), log_densities.end(), 0.0); //, [](double x, double y){ return x * y; });
	//log_densities(counts)[get_value_copy()];
    }

    void sample_implementation(counts_type& counts){
      for(auto factor: get_neighbors()){
	factor->adjust_counts(counts, -1);
      }
      set_value(log_densities(counts).sample(global_rng));
      for(auto factor: get_neighbors()){
	factor->adjust_counts(counts, 1);
      }
    }

    virtual std::string get_name() const{
      return "CategoricalVariable";
    }
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<Variable<CategoricalVariable<counts_type>, counts_type > >(*this);
      ar & boost::serialization::base_object<HasValue<int> >(*this);
      ar & boost::serialization::base_object<Categorical<std::string> >(*this);
      ar & _lower & _upper;
    }
    int _lower;
    int _upper;
  };
}

#endif
