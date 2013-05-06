#ifndef GRAPHMOD_OPTIMIZER_INTERFACE_HPP
#define GRAPHMOD_OPTIMIZER_INTERFACE_HPP

#include <string>
#include "boost_libraries.hpp"

namespace graphmod{
  template<class counts_type>
  class OptimizerInterface{
  public:
    virtual void optimize(const counts_type&, int) const = 0;
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
    }
  };
}

#endif
