#ifndef GRAPHMOD_LOG_PROBABILITY_VECTOR_HPP
#define GRAPHMOD_LOG_PROBABILITY_VECTOR_HPP

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <functional>
#include <sstream>
#include <random>
#include "graphmod_exception.hpp"
#include "probability_vector.hpp"

namespace graphmod{
  /*
    vector of probabilities represented logarithmically
   */
  class ProbabilityVector;
  class LogProbabilityVector : public std::vector<double>{
  public:
    LogProbabilityVector();
    LogProbabilityVector(const LogProbabilityVector&);
    LogProbabilityVector(std::vector<double>);
    LogProbabilityVector(int);
    LogProbabilityVector(ProbabilityVector&);
    LogProbabilityVector operator*(const LogProbabilityVector&);
    LogProbabilityVector operator/(const LogProbabilityVector&);
    LogProbabilityVector operator+(const LogProbabilityVector&);
    LogProbabilityVector operator-(const LogProbabilityVector&);
    LogProbabilityVector operator^(const int);
    int sample(std::mt19937_64&);
    std::string str() const; 
  private:
    void _normalize();
    double _log_sum() const;
  };
}

#endif
