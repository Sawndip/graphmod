#ifndef GRAPHMOD_MINKA_DIRICHLET_OPTIMIZER_HPP
#define GRAPHMOD_MINKA_DIRICHLET_OPTIMIZER_HPP

#include <vector>
#include <boost/math/special_functions.hpp>
#include "optimizer.hpp"
#include "boost_libraries.hpp"
#include "continuous_vector_variable.hpp"

namespace graphmod{
  template<class counts_type>
  class MinkaDirichletOptimizer : public Optimizer<MinkaDirichletOptimizer<counts_type>, counts_type>{
  public:
    MinkaDirichletOptimizer(ContinuousVectorVariable<counts_type>* node, 
			    std::string group_name, 
			    std::string observation_name, 
			    bool symmetric) : 
      _node(node), 
      _group_name(group_name), 
      _observation_name(observation_name), 
      _symmetric(symmetric){
    }
    MinkaDirichletOptimizer(){
    }
    void optimize_implementation(const counts_type& counts) const{
      auto group_by_observation = counts(_group_name, _observation_name);
      int num_group_types = group_by_observation.size();
      int num_observation_types = group_by_observation[0].size();
      std::vector<int> group_counts(num_group_types);
      std::vector<int> observation_counts(num_observation_types, 0);
      for(int observation=0; observation<num_observation_types; observation++){
	for(int group=0; group<num_group_types; group++){
	  observation_counts[observation] += group_by_observation[group][observation];
	}
      }
      std::transform(group_by_observation.begin(), group_by_observation.end(), group_counts.begin(), [](std::vector<int> group){ return accumulate(group.begin(), group.end(), 0); });
      auto& actual_value = _node->get_value();
      for(int iteration=0; iteration<200; iteration++){
	if(_symmetric){
	  double current = accumulate(actual_value.begin(), actual_value.end(), 0.0) / actual_value.size();
	  double numerator(0.0), denominator(0.0);
	  for(auto row: group_by_observation){
	    for(auto value: row){
	      numerator += boost::math::digamma(value + current);
	    }
	  }
	  numerator -= num_group_types * num_observation_types * boost::math::digamma(current);
	  for(auto value: group_counts){
	    denominator += boost::math::digamma(value + num_observation_types * current);
	  }
	  denominator -= num_group_types * boost::math::digamma(num_observation_types * current);
	  denominator *= num_observation_types;
	  double new_value = current * numerator / denominator;
	  fill(actual_value.begin(), actual_value.end(), new_value);
	}
	else{
	  double current_sum = accumulate(actual_value.begin(), actual_value.end(), 0.0);
	  double current_sum_digamma = boost::math::digamma(current_sum);
	  for(int observation_type=0; observation_type<num_observation_types; observation_type++){
	    double numerator(0.0), denominator(0.0);
	    for(int group_type=0; group_type<num_group_types; group_type++){
	      numerator += boost::math::digamma(group_by_observation[group_type][observation_type] + actual_value[observation_type]);
	      denominator += boost::math::digamma(group_counts[group_type] + current_sum);
	    }
	    if(not numerator > 0){
	      std::stringstream ss;
	      ss << observation_counts[observation_type];
	      throw GraphmodException(ss.str());
	    }
	    numerator -= num_group_types * boost::math::digamma(actual_value[observation_type]);
	    denominator -= num_group_types * current_sum_digamma;
	    actual_value[observation_type] *= numerator / denominator;
	    actual_value[observation_type] += .00001;
	  }
	}
      }
      auto& x = _node->get_value();
      std::cout << std::accumulate(x.begin(), x.end(), 0.0) / x.size() << std::endl;
    }
  private:
    ContinuousVectorVariable<counts_type>* _node;
    std::string _group_name;
    std::string _observation_name;
    bool _symmetric;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<Optimizer<MinkaDirichletOptimizer<counts_type>, counts_type> >(*this);
      ar & _node & _group_name & _observation_name & _symmetric;
    }
  };
}

#endif
