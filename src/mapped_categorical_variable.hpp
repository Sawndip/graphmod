#ifndef GRAPHMOD_MAPPED_CATEGORICAL_VARIABLE_HPP
#define GRAPHMOD_MAPPED_CATEGORICAL_VARIABLE_HPP

#include <map>
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
  class MappedCategoricalVariable : public Variable<MappedCategoricalVariable<counts_type>, counts_type>, public HasValue<std::map<int, bool> >, public Categorical<std::string>{
  public:
    using VariableInterface<counts_type>::get_neighbors;
    MappedCategoricalVariable() : 
      Variable<MappedCategoricalVariable<counts_type>, counts_type >(false), 
      HasValue<std::map<int, bool> >(), 
      Categorical<std::string>(), 
      _lower(-1), 
      _upper(-1){
    }
    /*
    MappedCategoricalVariable(std::string value, Alphabet<std::string>& alphabet) : 
      Variable<CategoricalVariable<counts_type>, counts_type >(true), 
      HasValue<int>(alphabet.get_index(value)), 
      Categorical<std::string>(alphabet), 
      _lower(-1), 
      _upper(-1){
    }
    */
    MappedCategoricalVariable(std::map<int, bool> value, Alphabet<std::string>& alphabet) : 
      Variable<MappedCategoricalVariable<counts_type>, counts_type >(true), 
      HasValue<std::map<int, bool> >(value), 
      Categorical<std::string>(alphabet), 
      _lower(-1), 
      _upper(-1){
    }
    MappedCategoricalVariable(Alphabet<std::string>& alphabet) :
      Variable<MappedCategoricalVariable<counts_type>, counts_type >(false), 
      HasValue<std::map<int, bool> >(), 
      Categorical<std::string>(alphabet), 
      _lower(-1), 
      _upper(-1){
    }
    MappedCategoricalVariable(Alphabet<std::string>& alphabet, int lower, int upper) : 
      Variable<MappedCategoricalVariable<counts_type>, counts_type >(false), 
      HasValue<std::map<int, bool> >(), 
      Categorical<std::string>(alphabet), 
      _lower(lower), 
      _upper(upper){
    }
    virtual ~MappedCategoricalVariable(){
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
      /*
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
      */
    }
    virtual std::string get_name() const{
      return "MappedCategoricalVariable";
    }
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<Variable<MappedCategoricalVariable<counts_type>, counts_type > >(*this);
      ar & boost::serialization::base_object<HasValue<std::map<int, bool> > >(*this);
      ar & boost::serialization::base_object<Categorical<std::string> >(*this);
      ar & _lower & _upper;
    }
    int _lower;
    int _upper;
  };
}

#endif
