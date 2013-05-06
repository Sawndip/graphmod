#include "log_probability_vector.hpp"

namespace graphmod{

  LogProbabilityVector::LogProbabilityVector() : std::vector<double>(){
  }

  LogProbabilityVector::LogProbabilityVector(std::vector<double> log_probs) : std::vector<double>(log_probs){    
    _normalize();
  }

  LogProbabilityVector::LogProbabilityVector(const LogProbabilityVector& log_probs) : std::vector<double>(log_probs){    
    _normalize();
  }

  LogProbabilityVector::LogProbabilityVector(int size) : std::vector<double>(size, std::log(1.0 / size)){
  }



  LogProbabilityVector::LogProbabilityVector(ProbabilityVector& probs){    
    std::transform(probs.begin(), probs.end(), back_inserter(*this),
		   [](double value){ return std::log(value); });
    _normalize();
  }
  
  int LogProbabilityVector::sample(std::mt19937_64& rng){
    std::vector<double> summed;
    std::partial_sum(begin(), end(), std::back_inserter(summed), [](double log_a, double log_b){
	double small, large;
	if(log_a < log_b){
	  small = log_a;
	  large = log_b;
	}
	else{
	  small = log_b;
	  large = log_a;
	}
	double neg_diff = small - large;
	if(neg_diff < -20){
	  return large;
	}
	return large + std::log(1.0 + std::exp(neg_diff));
      });
    std::uniform_real_distribution<> dist(0.0, 1.0);
    double r = std::log(dist(rng));
    return std::distance(summed.begin(), std::upper_bound(summed.begin(), summed.end(), r));    
  }


  std::string LogProbabilityVector::str() const{
    std::stringstream ss;
    ss << "[";
    std::for_each(begin(), end(), [&ss](double lprob){ 
	ss <<  std::exp(lprob) << "/" << lprob << ", "; 
      });
    ss << "]";
    return ss.str();
  }

  LogProbabilityVector LogProbabilityVector::operator^(const int power){
    LogProbabilityVector retval(std::vector<double>(size(), 1.0));
    std::transform(begin(), end(), retval.begin(), [power](double log_prob){
	return power * log_prob;
      });
    retval._normalize();
    return retval;
  }

  LogProbabilityVector LogProbabilityVector::operator*(const LogProbabilityVector& other){
    LogProbabilityVector retval(std::vector<double>(size(), 1.0));
    std::transform(begin(), end(), other.begin(), retval.begin(), [](double log_a, double log_b){
	return log_a + log_b;
      });
    retval._normalize();
    return retval;
  }

  LogProbabilityVector LogProbabilityVector::operator/(const LogProbabilityVector& other){
    LogProbabilityVector retval(std::vector<double>(size(), 1.0));
    std::transform(begin(), end(), other.begin(), retval.begin(), [](double log_a, double log_b){
	return log_a - log_b;
      });
    retval._normalize();
    return retval;
  }

  LogProbabilityVector LogProbabilityVector::operator+(const LogProbabilityVector& other){
    throw GraphmodException("unimplemented probability vector operation: +");
    LogProbabilityVector retval(std::vector<double>(size(), 1.0));
    std::transform(begin(), end(), other.begin(), retval.begin(), [](double log_a, double log_b){
	double small, large;
	if(log_a < log_b){
	  small = log_a;
	  large = log_b;
	}
	else{
	  small = log_b;
	  large = log_a;
	}
	double neg_diff = small - large;
	if(neg_diff < -20){
	  return large;
	}
	return large + std::log(1.0 + std::exp(neg_diff));
      });
    retval._normalize();
    return retval;
  }

  LogProbabilityVector LogProbabilityVector::operator-(const LogProbabilityVector& other){
    throw GraphmodException("unimplemented probability vector operation: -");
    LogProbabilityVector retval(std::vector<double>(size(), 1.0));
    std::transform(begin(), end(), other.begin(), retval.begin(), [](double log_a, double log_b){
	double small, large;
	if(log_a < log_b){
	  small = log_a;
	  large = log_b;
	}
	else{
	  small = log_b;
	  large = log_a;
	}
	double neg_diff = small - large;
	if(neg_diff < -20){
	  return large;
	}
	return large + log(1.0 + std::exp(neg_diff));
      });
    retval._normalize();
    return retval;
  }

  void LogProbabilityVector::_normalize(){
    double add = _log_sum();
    std::transform(begin(), end(), begin(), [add](double val){ return val - add; });
  }
  /*
  std::vector<double> LogProbabilityVector::probs(){
    std::vector<double> retval(size());
    normalize();
    transform(begin(), end(), retval.begin(), [](double log_p){ return exp(log_p); });
    return retval;
  }
  */
  double LogProbabilityVector::_log_sum() const{
    double m = *(std::max_element(begin(), end()));
    std::vector<double> temp(size());
    std::transform(begin(), end(), temp.begin(), [m](double log_i){
	return exp(log_i - m);
      });
    return m + std::log(std::accumulate(temp.begin(), temp.end(), 0.0));
  }
}
