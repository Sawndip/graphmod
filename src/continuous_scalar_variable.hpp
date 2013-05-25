#ifndef GRAPHMOD_CONTINUOUS_SCALAR_VARIABLE_HPP
#define GRAPHMOD_CONTINUOUS_SCALAR_VARIABLE_HPP

#include "variable.hpp"
#include "has_value.hpp"

namespace graphmod{
  template<class counts_type>
  class ContinuousScalarVariable : public Variable<ContinuousScalarVariable<counts_type>, counts_type >, public HasValue<double>{
  public:
    ContinuousScalarVariable(double value) : Variable<ContinuousScalarVariable, counts_type>(true), HasValue<double>(value){
    }
    ContinuousScalarVariable() : Variable<ContinuousScalarVariable, counts_type>(false), HasValue<double>(0.0){
    }
    virtual void sample_implementation(counts_type&){
    }
    double log_likelihood(counts_type&) const{
      throw GraphmodException("unimplemented: ContinuousScalarVariable::log_likelihood");
    }
    virtual std::string type_implementation() const{
      return "ContinuousScalar";
    }
    //static std::string variabletype; // = "CategoricalVariable";
    /*
    static std::string type(){
      return "ContinuousScalarVariable";
    }
    */
    virtual std::string get_name() const{
      return "ContinuousScalarVariable";
    }
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<Variable<ContinuousScalarVariable<counts_type>, counts_type > >(*this);
      ar & boost::serialization::base_object<HasValue<double> >(*this);
      ar & _upper & _lower;
    }
    double _upper;
    double _lower;
  };
  //template<class counts_type>
  //std::string ContinuousScalarVariable<counts_type>::variabletype = "ContinuousScalarVariable";
}

#endif
