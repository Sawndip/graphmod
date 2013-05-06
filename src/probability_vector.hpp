#ifndef GRAPHMOD_PROBABILITY_VECTOR_HPP
#define GRAPHMOD_PROBABILITY_VECTOR_HPP

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <functional>
#include <sstream>
#include "graphmod_exception.hpp"
#include "log_probability_vector.hpp"
#include "utils.hpp"

namespace graphmod{
  /*
    vector of probabilities
   */
  class LogProbabilityVector;
  class ProbabilityVector : public std::vector<double>{
  public:
    ProbabilityVector(std::vector<double>);
    ProbabilityVector(int);
    ProbabilityVector(LogProbabilityVector&);
    ProbabilityVector operator*(const ProbabilityVector&);
    ProbabilityVector operator/(const ProbabilityVector&);
    ProbabilityVector operator+(const ProbabilityVector&);
    ProbabilityVector operator-(const ProbabilityVector&);
    ProbabilityVector operator^(const int);
    int sample(std::mt19937_64& rng);
    std::string str() const; 
  private:
    double _sum() const;
    void _normalize();
    ProbabilityVector();
  };
}

#endif
