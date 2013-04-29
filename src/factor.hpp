#ifndef GRAPHMOD_FACTOR_HPP
#define GRAPHMOD_FACTOR_HPP

#include "boost_libraries.hpp"
#include "factor_interface.hpp"
//#include "probability_vector.hpp"
//#include "variable_interface.hpp"

namespace graphmod{
  template<class Implementation, class counts_type>
  class Factor : public FactorInterface<counts_type>{
  public:
    virtual double evaluate(counts_type& counts) const{
      return static_cast<const Implementation*>(this)->evaluate_implementation(counts);
    }
    virtual ProbabilityVector evaluate(counts_type& counts, const VariableInterface<counts_type>* variable) const{
      return static_cast<const Implementation*>(this)->evaluate_implementation(counts, variable);
    }
    virtual void adjust_counts(counts_type& counts, int increment) const{
      static_cast<const Implementation*>(this)->adjust_counts_implementation(counts, increment);
    }
    virtual void compile(counts_type& counts) const{
      static_cast<const Implementation*>(this)->compile_implementation(counts);
    }
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<FactorInterface<counts_type> >(*this);
    }
  };
}

#endif
