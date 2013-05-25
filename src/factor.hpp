#ifndef GRAPHMOD_FACTOR_HPP
#define GRAPHMOD_FACTOR_HPP

#include "boost_libraries.hpp"
#include "factor_interface.hpp"
#include "probability_vector.hpp"
#include "log_probability_vector.hpp"
#include "variable_interface.hpp"

namespace graphmod{
  template<class Implementation, class counts_type>
  class Factor : public FactorInterface<counts_type>{
  public:
    using FactorInterface<counts_type>::type;
    Factor(){
    }
    Factor(std::vector<VariableInterface<counts_type>*> parents, std::vector<VariableInterface<counts_type>*> children) : _parents(parents), _children(children){
    }
    virtual double density(counts_type& counts) const{
      return std::exp(log_density(counts));
    }
    virtual std::vector<graphmod::VariableInterface<counts_type>*> get_parents() const{
      return _parents;
    }
    virtual std::vector<graphmod::VariableInterface<counts_type>*> get_children() const{
      return _children;
      //return _neighbors;
    }
    virtual ProbabilityVector densities(counts_type& counts, const VariableInterface<counts_type>* variable) const{
      return ProbabilityVector(log_densities(counts, variable));
    }
    virtual double log_density(counts_type& counts) const{
      return static_cast<const Implementation*>(this)->log_density_implementation(counts);
    }
    virtual LogProbabilityVector log_densities(counts_type& counts, const VariableInterface<counts_type>* variable) const{
      return static_cast<const Implementation*>(this)->log_densities_implementation(counts, variable);
    }
    virtual void adjust_counts(counts_type& counts, int increment) const{
      static_cast<const Implementation*>(this)->adjust_counts_implementation(counts, increment);
    }
    virtual void compile(counts_type& counts) const{
      static_cast<const Implementation*>(this)->compile_implementation(counts);
    }
    /*static std::string type(){
      return static_cast<const Implementation*>(this)->type();
      }*/
    virtual std::string xml() const{
      std::stringstream ss;
      ss << "<node id='" << get_id(this) << "'>" << std::endl;
      ss << "<data key='type'>Factor</data>" << std::endl;
      ss << "<data key='name'>" << type() << "</data>" << std::endl;
      ss << "</node>";
      return ss.str();
    }
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<FactorInterface<counts_type> >(*this);
      ar & _parents & _children;
    }
    std::vector<VariableInterface<counts_type>*> _parents;
    std::vector<VariableInterface<counts_type>*> _children;
  };
}

#endif
