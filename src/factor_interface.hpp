#ifndef GRAPHMOD_FACTOR_INTERFACE_HPP
#define GRAPHMOD_FACTOR_INTERFACE_HPP

#include "probability_vector.hpp"
#include "log_probability_vector.hpp"
#include "variable_interface.hpp"
#include "boost_libraries.hpp"

namespace graphmod{
  template<class counts_type>
  class VariableInterface;

  template<class counts_type>
  class FactorInterface{
  public:
    virtual ~FactorInterface(){
    };
    virtual FactorInterface* clone(std::map<VariableInterface<counts_type>*, VariableInterface<counts_type>*>&) const = 0;
    virtual double density(counts_type&) const = 0;
    virtual std::vector<graphmod::VariableInterface<counts_type>*> get_parents() const = 0;
    virtual std::vector<graphmod::VariableInterface<counts_type>*> get_children() const = 0;
    virtual ProbabilityVector densities(counts_type&, const VariableInterface<counts_type>*) const = 0;
    virtual double log_density(counts_type&) const = 0;
    virtual LogProbabilityVector log_densities(counts_type&, const VariableInterface<counts_type>*) const = 0;
    virtual void adjust_counts(counts_type&, int) const = 0;
    virtual void compile(counts_type&) const = 0;
    virtual std::string xml() const = 0;
    virtual std::string type() const = 0;
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
    }
  };
}

#endif
