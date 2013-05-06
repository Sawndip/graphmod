#ifndef GRAPHMOD_DOUBLE_DIRICHLET_CATEGORICAL_FACTOR_HPP
#define GRAPHMOD_DOUBLE_DIRICHLET_CATEGORICAL_FACTOR_HPP

#include <functional>
#include "factor.hpp"
#include "variable_interface.hpp"
#include "categorical_variable.hpp"
#include "continuous_vector_variable.hpp"
#include "probability_vector.hpp"

namespace graphmod{
  template<class counts_type>
  class DoubleDirichletCategoricalFactor : public Factor<DoubleDirichletCategoricalFactor<counts_type>, counts_type>{
  public:
    DoubleDirichletCategoricalFactor(ContinuousVectorVariable<counts_type>* prior, 
				     CategoricalVariable<counts_type>* indexA, 
				     CategoricalVariable<counts_type>* indexB, 
				     CategoricalVariable<counts_type>* observation) : 
      _prior(prior), 
      _indexA(indexA), 
      _indexB(indexB), 
      _observation(observation){
      prior->add_neighbor(this);
      indexA->add_neighbor(this);
      indexB->add_neighbor(this);
      observation->add_neighbor(this);
    }

    DoubleDirichletCategoricalFactor(){
    }

    virtual ~DoubleDirichletCategoricalFactor(){
    }

    static inline double log_density_function(const double prior_sum, const double observation_prior, const int indexA_indexB_count, const int indexA_indexB_observation_count){
      return std::log((indexA_indexB_observation_count + observation_prior) / (indexA_indexB_count + prior_sum));
    }

    void compile_implementation(counts_type& counts) const{
      if(_observation->get_domain_size() != _prior->get_value().size()){
	double value = _prior->get_value()[0];
	_prior->get_value().resize(_observation->get_domain_size());
      
	fill(_prior->get_value().begin(), _prior->get_value().end(), value);
      }

      counts.add_target({_indexA->get_domain_name(), _indexB->get_domain_name(), _observation->get_domain_name()}, 
			{_indexA->get_domain_size(), _indexB->get_domain_size(), _observation->get_domain_size()});

      counts.add_target({_indexA->get_domain_name(), _indexB->get_domain_name()}, 
			{_indexA->get_domain_size(), _indexB->get_domain_size()});
    }

    double log_density_implementation(counts_type& counts) const{
      std::string indexA_domain_name = _indexA->get_domain_name(), indexB_domain_name = _indexB->get_domain_name(), observation_domain_name = _observation->get_domain_name();
      unsigned int indexA_value = _indexA->get_value(), indexB_value = _indexB->get_value(), observation_value = _observation->get_value();
      auto& prior = _prior->get_value();
      int index_observation_count = counts(std::vector<std::string>({indexA_domain_name, indexB_domain_name, observation_domain_name}), std::vector<unsigned int>({indexA_value, indexB_value, observation_value}));
      int index_count = counts(std::vector<std::string>({indexA_domain_name, indexB_domain_name}), std::vector<unsigned int>({indexA_value, indexB_value}));
      double prior_sum = accumulate(prior.begin(), prior.end(), 0.0);
      return log_density_function(prior_sum, prior[observation_value], index_count, index_observation_count);
    }

    LogProbabilityVector log_densities_implementation(counts_type& counts, const VariableInterface<counts_type>* variable) const{
      LogProbabilityVector log_probs;
      std::string indexA_domain_name = _indexA->get_domain_name(), indexB_domain_name = _indexB->get_domain_name(), observation_domain_name = _observation->get_domain_name();
      if(variable == _indexA){
	/*
	int size = _indexA->get_domain_size();
	int observation_value = _observation->get_value();
	if(observation_value == -1){
	  return LogProbabilityVector(size);
	}
	auto index_by_observation = counts(index_domain_name, observation_domain_name);
	std::vector<int> index_totals;
	std::transform(index_by_observation.begin(), index_by_observation.end(), std::back_inserter(index_totals), [](std::vector<int>& row){ 
	    return std::accumulate(row.begin(), row.end(), 0);
	  });
	auto& prior_values = _prior->get_value();
	double prior_value = prior_values[observation_value];
	double prior_total = std::accumulate(prior_values.begin(), prior_values.end(), 0.0);
	std::transform(index_by_observation.begin(), index_by_observation.end(), index_totals.begin(), std::back_inserter(log_probs),
		       [this,prior_value,prior_total,observation_value](std::vector<int>& counts, int totals){
			 return log_density_function(prior_total, prior_value, totals, counts[observation_value]);
		       });
	*/
      }
      else if(variable == _indexB){
	/*
	int size = _indexB->get_domain_size();
	int observation_value = _observation->get_value();
	if(observation_value == -1){
	  return LogProbabilityVector(size);
	}
	auto index_by_observation = counts(index_domain_name, observation_domain_name);
	std::vector<int> index_totals;
	std::transform(index_by_observation.begin(), index_by_observation.end(), std::back_inserter(index_totals), [](std::vector<int>& row){ 
	    return std::accumulate(row.begin(), row.end(), 0);
	  });
	auto& prior_values = _prior->get_value();
	double prior_value = prior_values[observation_value];
	double prior_total = std::accumulate(prior_values.begin(), prior_values.end(), 0.0);
	std::transform(index_by_observation.begin(), index_by_observation.end(), index_totals.begin(), std::back_inserter(log_probs),
		       [this,prior_value,prior_total,observation_value](std::vector<int>& counts, int totals){
			 return log_density_function(prior_total, prior_value, totals, counts[observation_value]);
		       });
	*/
      }
      else if(variable == _observation){
	/*
	int size = _index->get_domain_size();
	int index_value = _index->get_value();
	if(index_value == -1){
	  return LogProbabilityVector(size);
	}
	auto observation_counts = counts(index_domain_name, observation_domain_name)[index_value];
	auto& prior_values = _prior->get_value();
	double prior_sum = std::accumulate(prior_values.begin(), prior_values.end(), 0.0);
	int index_sum = std::accumulate(observation_counts.begin(), observation_counts.end(), 0);
	std::transform(observation_counts.begin(), observation_counts.end(), prior_values.begin(), std::back_inserter(log_probs),
		       [this,index_sum,prior_sum](int ind_obs_counts, double prior_value){
			 return log_density_function(prior_sum, prior_value, index_sum, ind_obs_counts);
		       });
	*/
      }
      return log_probs;
    }
    
    void adjust_counts_implementation(counts_type& counts, int weight) const{
      int indexA_value = _indexA->get_value(), indexB_value = _indexB->get_value(), obs_value = _observation->get_value();
      if(indexA_value != -1 and indexB_value != -1 and obs_value != -1){
	std::string indexA_domain_name = _indexA->get_domain_name(), indexB_domain_name = _indexB->get_domain_name(), obs_domain_name = _observation->get_domain_name();
	counts.increment({indexA_domain_name, indexB_domain_name, obs_domain_name}, {indexA_value, indexB_value, obs_value}, weight);
	counts.increment({indexA_domain_name, indexB_domain_name}, {indexA_value}, weight);
      }
    }
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<Factor<DoubleDirichletCategoricalFactor<counts_type>, counts_type> >(*this);
      ar & _prior;
      ar & _indexA;
      ar & _indexB;
      ar & _observation;
    }
    ContinuousVectorVariable<counts_type>* _prior;
    CategoricalVariable<counts_type>* _indexA;
    CategoricalVariable<counts_type>* _indexB;
    CategoricalVariable<counts_type>* _observation;
  };
}

#endif
