#include "random.hpp"

namespace graphmod{
  std::mt19937_64 global_rng;
  void initialize_random(){
    global_rng.seed(static_cast<unsigned int>(time(0)));  
  }

  void disable_random(int val){
    global_rng.seed(val);
  }

}
