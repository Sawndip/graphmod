#include "serial_sampler.hpp"

namespace graphmod{
  void SerialSampler::sample(Model& model) const{
    auto& counts = model.get_counts();
    std::mt19937_64 rng;
    rng.seed(static_cast<unsigned int>(time(0)));
    for(auto v: model.get_variables()){
      if(not v->get_observed()){
	v->sample(counts, rng);
      }
    }
  }
}
