#ifndef GRAPHMOD_OPENMP_SAMPLER_HPP
#define GRAPHMOD_OPENMP_SAMPLER_HPP

#include <string>
#include <omp.h>
#include "sampler_interface.hpp"
#include "factor_graph.hpp"
#include "dense_counts.hpp"

namespace graphmod{
  typedef FactorGraph<DenseCounts> Model;  
  class OpenMPSampler : public SamplerInterface<FactorGraph<DenseCounts> >{
  public:
    virtual void operator()(std::string, std::string);
    virtual Model operator()(Model);
    Model sample(Model);
  private:
  };
}

#endif
