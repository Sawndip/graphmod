#ifndef GRAPHMOD_PROBABILITY_VECTOR_HPP
#define GRAPHMOD_PROBABILITY_VECTOR_HPP

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <functional>
#include <sstream>
#include "graphmod_exception.hpp"

namespace graphmod{
  /*
    vector of probabilities represented logarithmically
   */
  class ProbabilityVector : public std::vector<double>{
  public:
    ProbabilityVector(std::vector<double>);
    ProbabilityVector operator*(const ProbabilityVector&);
    ProbabilityVector operator/(const ProbabilityVector&);
    ProbabilityVector operator+(const ProbabilityVector&);
    ProbabilityVector operator-(const ProbabilityVector&);
    ProbabilityVector operator^(const int);
    double log_sum() const;
    void normalize();
    std::vector<double> probs();
    std::string str() const; 
  private:
  };
}

#endif
