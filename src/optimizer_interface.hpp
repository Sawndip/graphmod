#ifndef GRAPHMOD_OPTIMIZER_INTERFACE_HPP
#define GRAPHMOD_OPTIMIZER_INTERFACE_HPP

#include <string>
#include "variable_interface.hpp"
#include "boost_libraries.hpp"

namespace graphmod{
  template<class counts_type>
  class OptimizerInterface{
  public:
    virtual ~OptimizerInterface(){
    }
    virtual OptimizerInterface* clone(std::map<VariableInterface<counts_type>*, VariableInterface<counts_type>*>&) const = 0;
    virtual void optimize(const counts_type&, int) const = 0;
    virtual std::string xml() const = 0;
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
    }
  };
}

#endif
