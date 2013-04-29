#include "probability_vector.hpp"

namespace graphmod{
  using namespace std;
  ProbabilityVector::ProbabilityVector(vector<double> probs) : vector<double>(probs){    
    transform(begin(), end(), begin(),
	      [](double value){ return log(value); });
    normalize();
  }

  string ProbabilityVector::str() const{
    stringstream ss;
    ss << "[";
    for_each(begin(), end(), [&ss](double lprob){ 
	ss <<  exp(lprob) << "/" << lprob << ", "; 
      });
    ss << "]";
    return ss.str();
  }

  ProbabilityVector ProbabilityVector::operator^(const int power){
    ProbabilityVector retval(vector<double>(size(), 1.0));
    transform(begin(), end(), retval.begin(), [power](double log_prob){
	return power * log_prob;
      });
    retval.normalize();
    return retval;
  }

  ProbabilityVector ProbabilityVector::operator*(const ProbabilityVector& other){
    ProbabilityVector retval(vector<double>(size(), 1.0));
    transform(begin(), end(), other.begin(), retval.begin(), [](double log_a, double log_b){
	return log_a + log_b;
      });
    retval.normalize();
    return retval;
  }

  ProbabilityVector ProbabilityVector::operator/(const ProbabilityVector& other){
    ProbabilityVector retval(vector<double>(size(), 1.0));
    transform(begin(), end(), other.begin(), retval.begin(), [](double log_a, double log_b){
	return log_a - log_b;
      });
    retval.normalize();
    return retval;
  }

  ProbabilityVector ProbabilityVector::operator+(const ProbabilityVector& other){
    throw GraphmodException("unimplemented probability vector operation: +");
    ProbabilityVector retval(vector<double>(size(), 1.0));
    transform(begin(), end(), other.begin(), retval.begin(), [](double log_a, double log_b){
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
	return large + log(1.0 + exp(neg_diff));
      });
    retval.normalize();
    return retval;
  }

  ProbabilityVector ProbabilityVector::operator-(const ProbabilityVector& other){
    throw GraphmodException("unimplemented probability vector operation: -");
    ProbabilityVector retval(vector<double>(size(), 1.0));
    transform(begin(), end(), other.begin(), retval.begin(), [](double log_a, double log_b){
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
	return large + log(1.0 + exp(neg_diff));
      });
    retval.normalize();
    return retval;
  }

  void ProbabilityVector::normalize(){
    double add = log_sum();
    transform(begin(), end(), begin(), [add](double val){ return val - add; });
  }

  vector<double> ProbabilityVector::probs(){
    vector<double> retval(size());
    normalize();
    transform(begin(), end(), retval.begin(), [](double log_p){ return exp(log_p); });
    return retval;
  }

  double ProbabilityVector::log_sum() const{
    double m = *(max_element(begin(), end()));
    vector<double> temp(size());
    transform(begin(), end(), temp.begin(), [m](double log_i){
	return exp(log_i - m);
      });
    return m + log(accumulate(temp.begin(), temp.end(), 0.0));
  }
}
