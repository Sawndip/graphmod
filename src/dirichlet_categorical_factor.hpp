#ifndef GRAPHMOD_DIRICHLET_CATEGORICAL_FACTOR_HPP
#define GRAPHMOD_DIRICHLET_CATEGORICAL_FACTOR_HPP

#include <cassert>
#include <functional>
#include "factor.hpp"
#include "variable_interface.hpp"
#include "categorical_variable.hpp"
#include "continuous_vector_variable.hpp"
#include "probability_vector.hpp"
#include "utils.hpp"

namespace graphmod{
  template<class counts_type>
  class DirichletCategoricalFactor : public Factor<DirichletCategoricalFactor<counts_type>, counts_type>{
  public:
    DirichletCategoricalFactor(ContinuousVectorVariable<counts_type>* prior, 
			       CategoricalVariable<counts_type>* index, 
			       CategoricalVariable<counts_type>* observation) : 
      Factor<DirichletCategoricalFactor<counts_type>, counts_type>({prior, index}, {observation}),
      _prior(prior), 
      _index(index), 
      _observation(observation)
    {
      prior->add_child(this);
      index->add_child(this);
      observation->add_parent(this);
    }

    DirichletCategoricalFactor(){
    }

    virtual FactorInterface<counts_type>* clone(std::map<VariableInterface<counts_type>*, VariableInterface<counts_type>*>& old_to_new) const{
      return new DirichletCategoricalFactor(
					    dynamic_cast<ContinuousVectorVariable<counts_type>*>(old_to_new[_prior]), 
					    dynamic_cast<CategoricalVariable<counts_type>*>(old_to_new[_index]), 
					    dynamic_cast<CategoricalVariable<counts_type>*>(old_to_new[_observation])
					    );
    }

    virtual ~DirichletCategoricalFactor(){
    }

    virtual std::string type() const{
      return "DirichletCategorical";
    }

    static inline double log_density_function(const double prior_sum, const int index_count, const double observation_prior, const int index_observation_count){
      assert(prior_sum > observation_prior);
      assert(index_count >= index_observation_count);
      return std::log((index_observation_count + observation_prior) / (index_count + prior_sum));
    }

    void compile_implementation(counts_type& counts) const{
      if(_observation->get_domain_size() != _prior->get_value().size()){
	double value = _prior->get_value()[0];
	_prior->get_value().resize(_observation->get_domain_size());
	fill(_prior->get_value().begin(), _prior->get_value().end(), value);
      }
      counts.add_target({_index->get_domain_name(), _observation->get_domain_name()}, {_index->get_domain_size(), _observation->get_domain_size()});
    }

    double log_density_implementation(counts_type& counts) const{
      return log_densities_implementation(counts, _index).at(_index->get_value());
    }

    LogProbabilityVector log_densities_implementation(counts_type& counts, const VariableInterface<counts_type>* variable) const{
      LogProbabilityVector log_probs;
      std::string index_domain_name = _index->get_domain_name(), observation_domain_name = _observation->get_domain_name();
      if(variable == _index){
	int size = _index->get_domain_size();
	int observation_value = _observation->get_value();
	if(observation_value == -1){
	  return LogProbabilityVector(size);
	}
	auto& index_by_observation = counts(index_domain_name, observation_domain_name);
	std::vector<int> index_totals;
	std::transform(index_by_observation.begin(), index_by_observation.end(), std::back_inserter(index_totals), [](std::vector<int>& row){ 
	    return std::accumulate(row.begin(), row.end(), 0);
	  });
	auto& prior_values = _prior->get_value();
	double observation_prior = prior_values[observation_value];
	double prior_sum = std::accumulate(prior_values.begin(), prior_values.end(), 0.0);
	std::transform(index_by_observation.begin(), index_by_observation.end(), index_totals.begin(), std::back_inserter(log_probs),
		       [this,prior_sum,observation_prior,observation_value](std::vector<int>& counts, int index_count){
			 return log_density_function(prior_sum, index_count, observation_prior, counts[observation_value]);
		       });
      }
      else if(variable == _observation){
	int size = _observation->get_domain_size();
	int index_value = _index->get_value();
	if(index_value == -1){
	  return LogProbabilityVector(size);
	}
	auto observation_counts = counts(index_domain_name, observation_domain_name)[index_value];
	auto& prior_values = _prior->get_value();
	double prior_sum = std::accumulate(prior_values.begin(), prior_values.end(), 0.0);
	int index_count = std::accumulate(observation_counts.begin(), observation_counts.end(), 0);
	std::transform(observation_counts.begin(), observation_counts.end(), prior_values.begin(), std::back_inserter(log_probs),
		       [this,index_count,prior_sum](int ind_obs_counts, double observation_prior){
			 return log_density_function(prior_sum, index_count, observation_prior, ind_obs_counts);
		       });
      }
      return log_probs;
    }
    
    void adjust_counts_implementation(counts_type& counts, int weight) const{
      int index_value = _index->get_value(), obs_value = _observation->get_value();
      if(index_value != -1){
	std::string index_domain_name = _index->get_domain_name();
	if(obs_value != -1){
	  std::string obs_domain_name = _observation->get_domain_name();
	  counts.increment({index_domain_name, obs_domain_name}, {index_value, obs_value}, weight);
	}
      }
    }
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<Factor<DirichletCategoricalFactor<counts_type>, counts_type> >(*this);
      ar & _prior;
      ar & _observation;
      ar & _index;
    }
    ContinuousVectorVariable<counts_type>* _prior;
    CategoricalVariable<counts_type>* _index;
    CategoricalVariable<counts_type>* _observation;
  };
}

#endif
