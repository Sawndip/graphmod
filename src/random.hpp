#ifndef GRAPHMOD_RANDOM_HPP
#define GRAPHMOD_RANDOM_HPP

#include <random>

namespace graphmod{
  extern std::mt19937_64 global_rng;
  void initialize_random();
  void disable_random(int);
}

#endif
