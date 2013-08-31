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
      Factor<HierarchicalDirichletCategoricalFactor<counts_type>, counts_type>({priorA, indexA, priorB, indexB}, {observation}),
      _priorA(priorA), 
      _indexA(indexA), 
      _priorB(priorB), 
      _indexB(indexB), 
      _observation(observation)
    {
      priorA->add_child(this);
      indexA->add_child(this);
      priorB->add_child(this);
      indexB->add_child(this);
      observation->add_parent(this);
    }

    HierarchicalDirichletCategoricalFactor(){
    }

    virtual FactorInterface<counts_type>* clone(std::map<VariableInterface<counts_type>*, VariableInterface<counts_type>*>& old_to_new) const{
      return new HierarchicalDirichletCategoricalFactor(
							dynamic_cast<ContinuousVectorVariable<counts_type>*>(old_to_new[_priorA]), 
							dynamic_cast<CategoricalVariable<counts_type>*>(old_to_new[_indexA]), 
							dynamic_cast<ContinuousVectorVariable<counts_type>*>(old_to_new[_priorB]), 
							dynamic_cast<CategoricalVariable<counts_type>*>(old_to_new[_indexB]), 
							dynamic_cast<CategoricalVariable<counts_type>*>(old_to_new[_observation])
							);
    }

    virtual ~HierarchicalDirichletCategoricalFactor(){
    }

    virtual std::string type() const{
      return "HierarchicalDirichletCategorical";
    }

    static inline double log_density_function(const double priorA_sum, 
					      const double priorA_observation, 
					      const double priorB_sum, 
					      const double priorB_observation, 
					      const int marginalA_count, 
					      const int observationA_count,
					      const int marginalB_count,
					      const int observationB_count){
      return std::log(
		      (observationB_count / (priorB_sum + marginalB_count)) + (priorB_observation / (priorB_sum + marginalB_count)) * ((observationA_count + priorA_observation) / (marginalA_count + priorB_sum))
		      );
    }

    void compile_implementation(counts_type& counts) const{
      std::string nameA = _indexA->get_domain_name(), nameB = _indexB->get_domain_name(), nameC = _observation->get_domain_name();
      unsigned int sizeA = _indexA->get_domain_size(), sizeB = _indexB->get_domain_size(), sizeC = _observation->get_domain_size();
      counts.add_target({nameA}, {sizeA});
      counts.add_target({nameA, nameC}, {sizeA, sizeC});
      counts.add_target({nameA, nameB}, {sizeA, sizeB});
      counts.add_target({nameA, nameB, nameC}, {sizeA, sizeB, sizeC});
    }

    double log_density_implementation(counts_type& counts) const{
      return log_densities_implementation(counts, _indexA).at(_indexA->get_value());
    }

    LogProbabilityVector log_densities_implementation(counts_type& counts, const VariableInterface<counts_type>* variable) const{
      std::vector<double> log_probs;

      if(variable == _indexA or variable == _indexB){
	throw GraphmodException("unimplemented: HierarchicalDirichletCategorical::evaluate_implementation over index");
      }
      else if(variable == _observation){
	int size = _observation->get_domain_size();
	std::vector<double> vals(size, 1.0 / size);

	int indexA_value = _indexA->get_value(), indexB_value = _indexB->get_value();
	std::string indexA_domain_name = _indexA->get_domain_name(), indexB_domain_name = _indexB->get_domain_name(), observation_domain_name = _observation->get_domain_name();
	if(indexA_value == -1 or indexB_value == -1){
	  std::vector<double> vals(size, 1.0 / size);
	  return LogProbabilityVector(vals);
	}
	auto priorB = _priorB->get_value(), priorA = _priorA->get_value();
	double priorA_sum = std::accumulate(priorA.begin(), priorA.end(), 0.0);
	double priorB_sum = std::accumulate(priorB.begin(), priorB.end(), 0.0);

	auto A_marginal_counts = counts(indexA_domain_name);
	auto A_observation_counts = counts(indexA_domain_name, observation_domain_name);
	auto AB_marginal_counts = counts(indexA_domain_name, indexB_domain_name);
	auto& AB_observation_counts = counts(indexA_domain_name, indexB_domain_name, observation_domain_name);

	std::vector<int> indices(size);
	std::iota(indices.begin(), indices.end(), 0);
	std::transform(indices.begin(), indices.end(), std::back_inserter(log_probs), [&](int observation_value){
	    return log_density_function(priorA_sum, 
					priorA[observation_value], 
					priorB_sum,
					priorB[observation_value], 
					A_marginal_counts[indexA_value],
					A_observation_counts[indexA_value][observation_value],
					AB_marginal_counts[indexA_value][indexB_value],
					AB_observation_counts[indexA_value][indexB_value][observation_value]);
	  });

      }
      return LogProbabilityVector(log_probs);
    }
    
    void adjust_counts_implementation(counts_type& counts, int weight) const{
      int indexA_value = _indexA->get_value(), indexB_value = _indexB->get_value(), observation_value = _observation->get_value();
      if(indexA_value != -1 and indexB_value != -1 and observation_value != -1){
	std::string indexA_domain_name = _indexA->get_domain_name(), indexB_domain_name = _indexB->get_domain_name(), observation_domain_name = _observation->get_domain_name();
	counts.increment({indexA_domain_name, observation_domain_name}, {indexA_value, observation_value}, weight);
	counts.increment({indexA_domain_name}, {indexA_value}, weight);
	counts.increment({indexA_domain_name, indexB_domain_name, observation_domain_name}, {indexA_value, indexB_value, observation_value}, weight);
	counts.increment({indexA_domain_name, indexB_domain_name}, {indexA_value, indexB_value}, weight);
      }
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
