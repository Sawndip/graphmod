#ifndef GRAPHMOD_OPTIMIZER_HPP
#define GRAPHMOD_OPTIMIZER_HPP

#include "optimizer_interface.hpp"
#include "boost_libraries.hpp"

namespace graphmod{
  template<class Implementation, class counts_type>
  class Optimizer : public OptimizerInterface<counts_type>{
  public:
    virtual void optimize(const counts_type& counts, int iterations) const{
      static_cast<const Implementation*>(this)->optimize_implementation(counts, iterations);
    }
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<OptimizerInterface<counts_type> >(*this);
    }
  };
}

#endif
