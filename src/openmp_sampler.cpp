#include "openmp_sampler.hpp"

namespace graphmod{
  void OpenMPSampler::operator()(std::string input_file, std::string output_file){
  }
  Model OpenMPSampler::operator()(Model input_model){
    return input_model;
  }
  Model OpenMPSampler::sample(Model input_model){
    //input_model.compile();
    //std::cout << input_model.str() << std::endl;
    //return Model(input_model);
    return input_model;
  }
}
