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
      Categorical<std::string>(){
    }
    MappedCategoricalVariable(std::map<int, bool> value, Alphabet<std::string>& alphabet) : 
      Variable<MappedCategoricalVariable<counts_type>, counts_type >(true), 
      HasValue<std::map<int, bool> >(value), 
      Categorical<std::string>(alphabet){
    }
    MappedCategoricalVariable(Alphabet<std::string>& alphabet, int lower, int upper) : 
      Variable<MappedCategoricalVariable<counts_type>, counts_type >(false), 
      HasValue<std::map<int, bool> >(), 
      Categorical<std::string>(alphabet){
    }
    virtual ~MappedCategoricalVariable(){
    }
    void compile(counts_type& counts){
    }
    ProbabilityVector log_densities(counts_type& counts) const{
      throw GraphmodException("unimplemented: MappedCategoricalVariable::log_densities");
    }
    double log_likelihood(counts_type& counts) const{
      auto factors = get_neighbors();
      std::vector<double> log_densities;
      std::transform(factors.begin(), factors.end(), std::back_inserter(log_densities), [&counts](FactorInterface<counts_type>* factor){
	  return factor->log_density(counts);
	});
      return add_logs(log_densities);
    }
    void sample_implementation(counts_type& counts){
      throw GraphmodException("no implementation for sampling MappedCategoricalVariables!");
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
    }
  };
}

#endif
