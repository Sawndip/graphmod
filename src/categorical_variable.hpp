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
    ProbabilityVector log_densities(counts_type& counts) const{
      ProbabilityVector probs(std::vector<double>(get_domain_size(), 1.0));
      for(auto factor: get_neighbors()){
	probs = probs * factor->evaluate(counts, this);
      }
      return probs;
    }
    double log_likelihood(counts_type& counts){
      return log_densities(counts)[get_value()];
    }
    void sample_implementation(counts_type& counts){
      for(auto factor: get_neighbors()){
	factor->adjust_counts(counts, -1);
      }
      ProbabilityVector log_probs = log_densities(counts);
      auto probs = log_probs.probs();
      if(_upper != -1){
	probs.resize(_upper + 1);
      }
      std::discrete_distribution<> dist(probs.begin(), probs.end());
      int new_val = dist(global_rng);
      set_value(new_val);
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
