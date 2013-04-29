#ifndef GRAPHMOD_BETA_BERNOULLI_FACTOR_HPP
#define GRAPHMOD_BETA_BERNOULLI_FACTOR_HPP

#include <functional>
#include "factor.hpp"
#include "variable_interface.hpp"
#include "categorical_variable.hpp"
#include "mapped_categorical_variable.hpp"
#include "continuous_vector_variable.hpp"
#include "probability_vector.hpp"

namespace graphmod{
  template<class counts_type>
  class BetaBernoulliFactor : public Factor<BetaBernoulliFactor<counts_type>, counts_type>{
  public:
    BetaBernoulliFactor(ContinuousVectorVariable<counts_type>* prior, 
			CategoricalVariable<counts_type>* index, 
			MappedCategoricalVariable<counts_type>* observation) : 
      _prior(prior), 
      _index(index), 
      _observation(observation){
      prior->add_neighbor(this);
      index->add_neighbor(this);
      observation->add_neighbor(this);
    }

    BetaBernoulliFactor(){
    }

    virtual ~BetaBernoulliFactor(){
    }

    void compile_implementation(counts_type& counts) const{
      if(_observation->get_domain_size() != _prior->get_value().size()){
	double value = _prior->get_value()[0];
	_prior->get_value().resize(_observation->get_domain_size());
	fill(_prior->get_value().begin(), _prior->get_value().end(), value);
      }
      counts.add_target({_index->get_domain_name(), _observation->get_domain_name()}, {_index->get_domain_size(), _observation->get_domain_size()});
      counts.add_target({_index->get_domain_name()}, {_index->get_domain_size()});
    }

    double evaluate_implementation(counts_type& counts) const{
      std::string index_domain_name = _index->get_domain_name(), observation_domain_name = _observation->get_domain_name();
      unsigned int index_value = _index->get_value(), observation_value = _observation->get_value();
      auto& prior = _prior->get_value();
      int index_observation_count = counts(std::vector<std::string>({index_domain_name, observation_domain_name}), std::vector<unsigned int>({index_value, observation_value}));
      int index_count = counts(std::vector<std::string>({index_domain_name}), std::vector<unsigned int>({index_value}));
      double prior_sum = accumulate(prior.begin(), prior.end(), 0.0);
      return (index_observation_count + prior[observation_value]) / (index_count + prior_sum);
    }

    ProbabilityVector evaluate_implementation(counts_type& counts, const VariableInterface<counts_type>* variable) const{
      std::vector<double> probs;
      std::string index_domain_name = _index->get_domain_name(), observation_domain_name = _observation->get_domain_name();
      if(variable == _index){
	probs.resize(_index->get_domain_size(), 1.0);

	int observation_value = _observation->get_value();
	if(observation_value == -1){
	  return ProbabilityVector(probs);
	}
	auto index_by_observation = counts(index_domain_name, observation_domain_name);
	std::vector<int> index_totals(probs.size());
	std::transform(index_by_observation.begin(), index_by_observation.end(), index_totals.begin(), [](std::vector<int>& row){ 
	    return std::accumulate(row.begin(), row.end(), 0);
	  });

	auto& prior_values = _prior->get_value();
	double prior_value = prior_values[observation_value];
	double prior_total = std::accumulate(prior_values.begin(), prior_values.end(), 0.0);
	std::transform(index_by_observation.begin(), index_by_observation.end(), index_totals.begin(), probs.begin(),
		       [prior_value,prior_total,observation_value](std::vector<int>& counts, int totals){
			 return (counts[observation_value] + prior_value) / (totals + prior_total);
		       });

      }
      else if(variable == _observation){
	probs.resize(_observation->get_domain_size(), 1.0);

	int index_value = _index->get_value();
	if(index_value == -1){
	  return ProbabilityVector(probs);
	}
	auto observation_counts = counts(index_domain_name, observation_domain_name)[index_value];
	auto& prior_values = _prior->get_value();
	//double prior_sum = std::accumulate(prior_values.begin(), prior_values.end(), 0.0);
	//int index_sum = std::accumulate(observation_counts.begin(), observation_counts.end(), 0);
	std::transform(observation_counts.begin(), observation_counts.end(), prior_values.begin(), probs.begin(),
		       [index_value](int c, double p){
			 return c + p;
		       });

      }
      return ProbabilityVector(probs);
    }
    
    void adjust_counts_implementation(counts_type& counts, int weight) const{
      int index_value = _index->get_value(), obs_value = _observation->get_value();
      if(index_value != -1 and obs_value != -1){
	std::string index_domain_name = _index->get_domain_name(), obs_domain_name = _observation->get_domain_name();
	counts.increment({index_domain_name, obs_domain_name}, {index_value, obs_value}, weight);
	counts.increment({index_domain_name}, {index_value}, weight);
      }
    }
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<Factor<BetaBernoulliFactor<counts_type>, counts_type> >(*this);
      ar & _prior;
      ar & _observation;
      ar & _index;
    }
    ContinuousVectorVariable<counts_type>* _prior;
    CategoricalVariable<counts_type>* _index;
    MappedCategoricalVariable<counts_type>* _observation;
  };
}

#endif
