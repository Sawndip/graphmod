#ifndef GRAPHMOD_MPI_SAMPLER_HPP
#define GRAPHMOD_MPI_SAMPLER_HPP

#include <boost/mpi.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <mpi.h>

#include "sampler_interface.hpp"
#include "factor_graph.hpp"
#include "dense_counts.hpp"



namespace graphmod{

  typedef FactorGraph<DenseCounts> Model;
  class MPISampler : public SamplerInterface<FactorGraph<DenseCounts> >{
  public:
    virtual void operator()(std::string, std::string);
    virtual Model operator()(Model);
  };
}

#endif
