#ifndef GRAPHMOD_SAMPLER_INTERFACE_HPP
#define GRAPHMOD_SAMPLER_INTERFACE_HPP

#include <string>

namespace graphmod{
  template<class Model>
  class SamplerInterface{
  public:
    virtual ~SamplerInterface(){
    }
    virtual void operator()(std::string, std::string) = 0;
    virtual Model operator()(Model) = 0;
  private:
  };
}

#endif
