#ifndef GRAPHMOD_HIERARCHICAL_DIRICHLET_CATEGORICAL_FACTOR_HPP
#define GRAPHMOD_HIERARCHICAL_DIRICHLET_CATEGORICAL_FACTOR_HPP

#include <functional>
#include "factor.hpp"
#include "variable_interface.hpp"
#include "categorical_variable.hpp"
#include "continuous_vector_variable.hpp"
#include "probability_vector.hpp"

namespace graphmod{
  template<class counts_type>
  class HierarchicalDirichletCategoricalFactor : public Factor<HierarchicalDirichletCategoricalFactor<counts_type>, counts_type>{
  public:
    HierarchicalDirichletCategoricalFactor(ContinuousVectorVariable<counts_type>* priorA, 
					   CategoricalVariable<counts_type>* indexA, 
					   ContinuousVectorVariable<counts_type>* priorB, 
					   CategoricalVariable<counts_type>* indexB, 
					   CategoricalVariable<counts_type>* observation) : 
      _priorA(priorA), 
      _indexA(indexA), 
      _priorB(priorB), 
      _indexB(indexB), 
      _observation(observation){
      priorA->add_neighbor(this);
      indexA->add_neighbor(this);
      priorB->add_neighbor(this);
      indexB->add_neighbor(this);
      observation->add_neighbor(this);
    }

    HierarchicalDirichletCategoricalFactor(){
    }

    virtual ~HierarchicalDirichletCategoricalFactor(){
    }

    void compile_implementation(counts_type& counts) const{
      //throw GraphmodException("unimplemented: HierarchicalDirichletCategorical::compile_implementation");
      //vector<int> sizes;
      //vector<string> names;
      //for(auto p: _parameter){
      //names.push_back(p.second->get_domain_name());
      //sizes.push_back(p.second->get_domain_size());
      //vector<int> tempi(sizes);
      //vector<string> temps(names);
      std::string nameA = _indexA->get_domain_name(), nameB = _indexB->get_domain_name(), nameC = _observation->get_domain_name();
      unsigned int sizeA = _indexA->get_domain_size(), sizeB = _indexB->get_domain_size(), sizeC = _observation->get_domain_size();
      counts.add_target({nameA}, {sizeA});
      counts.add_target({nameA, nameC}, {sizeA, sizeC});
      counts.add_target({nameA, nameB}, {sizeA, sizeB});
      counts.add_target({nameA, nameB, nameC}, {sizeA, sizeB, sizeC});
      //counts.add_target(temps, tempi);
      //temps.push_back(_observation->get_domain_name());
      //tempi.push_back(_observation->get_domain_size());
      //counts.add_target(temps, tempi);
      //}

    }

    double log_density_implementation(counts_type& counts) const{
      throw GraphmodException("unimplemented: HierarchicalDirichletCategorical::evaluate_implementation");
      /*
      std::string index_domain_name = _index->get_domain_name(), observation_domain_name = _observation->get_domain_name();
      unsigned int index_value = _index->get_value(), observation_value = _observation->get_value();
      auto& prior = _prior->get_value();
      int index_observation_count = counts(std::vector<std::string>({index_domain_name, observation_domain_name}), std::vector<unsigned int>({index_value, observation_value}));
      int index_count = counts(std::vector<std::string>({index_domain_name}), std::vector<unsigned int>({index_value}));
      double prior_sum = accumulate(prior.begin(), prior.end(), 0.0);
      return (index_observation_count + prior[observation_value]) / (index_count + prior_sum);
      */
    }

    LogProbabilityVector log_densities_implementation(counts_type& counts, const VariableInterface<counts_type>* variable) const{
      throw GraphmodException("unimplemented: HierarchicalDirichletCategorical::evaluate_implementation");
      /*
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
      */
    }
    
    void adjust_counts_implementation(counts_type& counts, int weight) const{
      throw GraphmodException("unimplemented: HierarchicalDirichletCategorical::adjust_counts_implementation");
      /*
      int index_value = _index->get_value(), obs_value = _observation->get_value();
      if(index_value != -1 and obs_value != -1){
	std::string index_domain_name = _index->get_domain_name(), obs_domain_name = _observation->get_domain_name();
	counts.increment({index_domain_name, obs_domain_name}, {index_value, obs_value}, weight);
	counts.increment({index_domain_name}, {index_value}, weight);
      }
      */
    }
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<Factor<HierarchicalDirichletCategoricalFactor<counts_type>, counts_type> >(*this);
      ar & _priorA;
      ar & _indexA;
      ar & _priorB;
      ar & _indexB;
      ar & _observation;
    }
    ContinuousVectorVariable<counts_type>* _priorA;
    CategoricalVariable<counts_type>* _indexA;
    ContinuousVectorVariable<counts_type>* _priorB;
    CategoricalVariable<counts_type>* _indexB;
    CategoricalVariable<counts_type>* _observation;
  };
}

#endif
