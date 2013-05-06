#include "probability_vector.hpp"

namespace graphmod{

  ProbabilityVector::ProbabilityVector(){
  }

  ProbabilityVector::ProbabilityVector(std::vector<double> probs) : std::vector<double>(probs){    
    _normalize();
  }

  ProbabilityVector::ProbabilityVector(int size) : std::vector<double>(size, 1.0 / size){
  }

  ProbabilityVector::ProbabilityVector(LogProbabilityVector& log_probs){
    std::transform(log_probs.begin(), log_probs.end(), back_inserter(*this), [](double log_p){ return std::exp(log_p); });
    _normalize();
  }

  int ProbabilityVector::sample(std::mt19937_64& rng){
    std::vector<double> summed;
    std::partial_sum(begin(), end(), std::back_inserter(summed), std::plus<double>());
    std::uniform_real_distribution<> dist(0.0, 1.0);
    double r = dist(rng);
    return std::distance(summed.begin(), std::upper_bound(summed.begin(), summed.end(), r));    
  }

  std::string ProbabilityVector::str() const{
    std::stringstream ss;
    ss << "[";
    std::for_each(begin(), end(), [&ss](double prob){ 
	ss << prob << ", "; 
      });
    ss << "]";
    return ss.str();
  }

  ProbabilityVector ProbabilityVector::operator^(const int power){
    ProbabilityVector retval(std::vector<double>(size(), 1.0));
    std::transform(begin(), end(), retval.begin(), [power](double log_prob){
	return power * log_prob;
      });
    retval._normalize();
    return retval;
  }

  ProbabilityVector ProbabilityVector::operator*(const ProbabilityVector& other){
    ProbabilityVector retval;
    std::transform(begin(), end(), other.begin(), std::back_inserter(retval), [](double p_a, double p_b){
	return p_a * p_b;
      });
    retval._normalize();
    return retval;
  }

  ProbabilityVector ProbabilityVector::operator/(const ProbabilityVector& other){
    ProbabilityVector retval;
    std::transform(begin(), end(), other.begin(), std::back_inserter(retval), [](double p_a, double p_b){
	return p_a / p_b;
      });
    retval._normalize();
    return retval;
  }

  ProbabilityVector ProbabilityVector::operator+(const ProbabilityVector& other){
    ProbabilityVector retval;
    std::transform(begin(), end(), other.begin(), retval.begin(), [](double a, double b){ return a + b; });
    return retval;
  }

  ProbabilityVector ProbabilityVector::operator-(const ProbabilityVector& other){
    ProbabilityVector retval;
    std::transform(begin(), end(), other.begin(), retval.begin(), [](double a, double b){ return a - b; });
    return retval;
  }

  void ProbabilityVector::_normalize(){
    double add = _sum();
    std::transform(begin(), end(), begin(), [add](double val){ return val / add; });
  }

  double ProbabilityVector::_sum() const{
    return std::accumulate(begin(), end(), 0.0);
  }
}
