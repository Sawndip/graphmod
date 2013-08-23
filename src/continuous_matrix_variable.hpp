#ifndef GRAPHMOD_CONTINUOUS_MATRIX_VARIABLE_HPP
#define GRAPHMOD_CONTINUOUS_MATRIX_VARIABLE_HPP

#include <vector>
#include "variable.hpp"
#include "has_value.hpp"
#include "boost_libraries.hpp"


namespace graphmod{
  template<class counts_type>
  class ContinuousMatrixVariable : public Variable<ContinuousMatrixVariable<counts_type>, counts_type>, public HasValue<std::vector<std::vector<double> > >{
  public:
    ContinuousMatrixVariable() : Variable<ContinuousMatrixVariable, counts_type>(false), HasValue<std::vector<std::vector<double> > >(), _symmetric(false){
    }
    //ContinuousMatrixVariable(std::vector<double> value, bool symmetric) : Variable<ContinuousMatrixVariable, counts_type>(true), HasValue<std::vector<std::vector<double> > >(value), _symmetric(symmetric){
    //}
    ContinuousMatrixVariable(std::vector<std::vector<double> > value) : Variable<ContinuousMatrixVariable, counts_type>(true), HasValue<std::vector<std::vector<double> > >(value), _symmetric(false){
    }
    virtual ~ContinuousMatrixVariable(){
    }
    virtual VariableInterface<counts_type>* clone(std::map<std::string, Alphabet<std::string> >&) const{
      return new ContinuousMatrixVariable(get_value_copy());
    }


    virtual std::string type_implementation() const{
      return "ContinuousMatrix";
    }
    //static std::string variabletype; // = "CategoricalVariable";
    /*
    static std::string type(){
      return "ContinuousMatrixVariable";
      }*/
    std::string name() const{
      return "ContinuousMatrixVariable";
    }
    void compile(counts_type&){
    }
    void sample_implementation(counts_type&, std::mt19937_64&){
    }
    double log_likelihood(counts_type&) const{
      throw GraphmodException("unimplemented: ContinuousMatrixVariable::log_likelihood");
    }
    bool is_symmetric() const{
      return _symmetric;
    }
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<Variable<ContinuousMatrixVariable<counts_type>, counts_type > >(*this);
      ar & boost::serialization::base_object<HasValue<std::vector<std::vector<double> > > >(*this);
      ar & _symmetric;
    }
    bool _symmetric;
  };
}

#endif
