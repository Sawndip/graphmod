#ifndef GRAPHMOD_SERIAL_SAMPLER_HPP
#define GRAPHMOD_SERIAL_SAMPLER_HPP

#include "factor_graph.hpp"
#include "dense_counts.hpp"

namespace graphmod{
  typedef FactorGraph<DenseCounts> Model;  
  class SerialSampler{
  public:
    void sample(Model&) const;
  private:
  };
}

#endif
