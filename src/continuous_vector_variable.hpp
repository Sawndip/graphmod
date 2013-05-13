#ifndef GRAPHMOD_CONTINUOUS_VECTOR_VARIABLE_HPP
#define GRAPHMOD_CONTINUOUS_VECTOR_VARIABLE_HPP

#include <vector>
#include "variable.hpp"
#include "has_value.hpp"
#include "boost_libraries.hpp"
//#include "optimizer_interface.hpp"
//#include "minka_dirichlet_optimizer.hpp"

namespace graphmod{
  template<class counts_type>
  class ContinuousVectorVariable : public Variable<ContinuousVectorVariable<counts_type>, counts_type>, public HasValue<std::vector<double> >{
  public:
    ContinuousVectorVariable() : Variable<ContinuousVectorVariable, counts_type>(false), HasValue<std::vector<double> >(), _symmetric(false){
    }
    ContinuousVectorVariable(std::vector<double> value, bool symmetric) : Variable<ContinuousVectorVariable, counts_type>(true), HasValue<std::vector<double> >(value), _symmetric(symmetric){
    }
    ContinuousVectorVariable(std::vector<double> value) : Variable<ContinuousVectorVariable, counts_type>(true), HasValue<std::vector<double> >(value), _symmetric(false){
    }
    virtual ~ContinuousVectorVariable(){
    }
    void compile(counts_type&){
    }
    void sample_implementation(counts_type&){
    }
    //std::string get_name() const{
    //  return "ContinuousVectorVariable";
    //}
    std::string name() const{
      return "ContinuousVectorVariable";
    }
    bool is_symmetric() const{
      return _symmetric;
    }
    double log_likelihood(counts_type&) const{
      throw GraphmodException("unimplemented: ContinuousVectorVariable::log_likelihood");
    }
    //void add_optimizer(OptimizerInterface<std::vector<double> >*)
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      //ar.register_type(static_cast<MinkaDirichletOptimizer*>(NULL));
      //ar & boost::serialization::base_object<VariableNode>(*this);
      ar & boost::serialization::base_object<Variable<ContinuousVectorVariable<counts_type>, counts_type > >(*this);
      ar & boost::serialization::base_object<HasValue<std::vector<double> > >(*this);
      ar & _symmetric;
      //ar & BOOST_SERIALIZATION_NVP(_optimizers);
    }
    bool _symmetric;
    //std::vector<OptimizerInterface<std::vector<double> >*> _optimizers;
  };
}

#endif
