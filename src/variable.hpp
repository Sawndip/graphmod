#ifndef GRAPHMOD_VARIABLE_HPP
#define GRAPHMOD_VARIABLE_HPP

#include "variable_interface.hpp"
#include "boost_libraries.hpp"

namespace graphmod{
  template<class Implementation, class counts_type>
  class Variable : public VariableInterface<counts_type>{
  public:
    Variable(bool observed) : _observed(observed){
    }
    virtual bool get_observed() const{
      return _observed;
    }
    virtual void set_observed(bool observed){
      _observed = observed;
    }
    virtual void sample(counts_type& counts){
      static_cast<Implementation*>(this)->sample_implementation(counts);
    }
    virtual std::vector<graphmod::FactorInterface<counts_type>*> get_neighbors() const{
      return _neighbors;
    }
    virtual void add_neighbor(graphmod::FactorInterface<counts_type>* neighbor){
      _neighbors.push_back(neighbor);
    }
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<VariableInterface<counts_type> >(*this);
      ar & _observed & _neighbors;
    }
    bool _observed;
    std::vector<FactorInterface<counts_type>*> _neighbors;
  };
}

#endif
