#ifndef GRAPHMOD_MINKA_DIRICHLET_OPTIMIZER_HPP
#define GRAPHMOD_MINKA_DIRICHLET_OPTIMIZER_HPP

#include <vector>
#include <boost/math/special_functions.hpp>
#include "optimizer.hpp"
#include "boost_libraries.hpp"
#include "continuous_vector_variable.hpp"
#include "utils.hpp"

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
    
    virtual OptimizerInterface<counts_type>* clone(std::map<VariableInterface<counts_type>*, VariableInterface<counts_type>*>& old_to_new) const{
      return new MinkaDirichletOptimizer(dynamic_cast<ContinuousVectorVariable<counts_type>*>(old_to_new[_node]), _group_name, _observation_name, _symmetric);
    }

    /*
      a' = a * 
           (sum_g_o(psi(n_go + a)) - num_g * num_o * psi(a)) / 
	   (num_o * (sum_g(psi(num_g + a_sum)) - num_g * psi(a_sum)))
     */
    static inline std::vector<double> symmetric_optimized(std::vector<double> current, std::vector<std::vector<int> > group_observation_counts, int iterations=200, int iteration=0){
      throw GraphmodException("unimplemented: MinkaDirichletOptimizer::symmetric_optimized");
      /*
      int num_obs = current.size();
      double current_value = std::accumulate(current.begin(), current.end(), 0.0) / current.size();
      std::vector<int> observation_sums;
      std::transform(group_observation_counts.begin(), group_observation_counts.end(), std::back_inserter(observation_sums), [](std::vector<int> counts){
	  return std::accumulate(counts.begin(), counts.end(), 0);
	});
      int total_count = std::accumulate(observation_sums.begin(), observation_sums.end(), 0);
      std::vector<double> updated(num_obs, current_value);
      return (iteration == iterations) ? updated : symmetric_optimized(updated, group_observation_counts, iterations, iteration + 1);
      */
    }

    /*
      a_o' = a_o * 
             (sum_g(psi(n_go + a_o)) - num_g * psi(a_o)) / 
	     (sum_g(psi(n_g + a_sum)) - num_g * psi(a_sum))
     */
    static inline std::vector<double> asymmetric_optimized(std::vector<double> current, std::vector<std::vector<int> > group_observation_counts, int iterations=200, int iteration=0){
      int num_groups = group_observation_counts.size();
      std::vector<std::vector<int> > observation_group_counts = transpose(group_observation_counts);
      double current_sum = std::accumulate(current.begin(), current.end(), 0.0);

      std::vector<double> numerators, denominators, coefficients, updated;

      std::transform(observation_group_counts.begin(), observation_group_counts.end(), current.begin(), std::back_inserter(numerators), [=](std::vector<int> obs, double cur){
	  std::vector<double> psi_groups;
	  std::transform(obs.begin(), obs.end(), std::back_inserter(psi_groups), [cur](int c){ return boost::math::digamma(c + cur); });
	  return std::accumulate(psi_groups.begin(), psi_groups.end(), 0.0) - num_groups * boost::math::digamma(cur);
	});
      std::vector<double> temp;
      std::transform(group_observation_counts.begin(), group_observation_counts.end(), std::back_inserter(temp), [=](std::vector<int> group){
	  return boost::math::digamma(std::accumulate(group.begin(), group.end(), 0) + num_groups * current_sum);
	});
      double denominator = std::accumulate(temp.begin(), temp.end(), 0.0) + boost::math::digamma(current_sum);
      std::transform(numerators.begin(), numerators.end(), std::back_inserter(coefficients), [denominator](double numerator){
	  return numerator / denominator;
	});

      std::transform(current.begin(), current.end(), coefficients.begin(), std::back_inserter(updated), [](double cur, double coef){
	  return cur * coef + .00001;
	});
      return (iteration == iterations) ? updated : asymmetric_optimized(updated, group_observation_counts, iterations, iteration + 1);
    }

    void optimize_implementation(const counts_type& counts, int iterations) const{
      _symmetric ? _node->set_value(symmetric_optimized(_node->get_value_copy(), counts(_group_name, _observation_name), iterations)) : 
	_node->set_value(asymmetric_optimized(_node->get_value_copy(), counts(_group_name, _observation_name), iterations));
      auto x = _node->get_value_copy();
      std::cout << "Dirichlet: " << _observation_name << " "<< std::accumulate(x.begin(), x.end(), 0.0) / x.size() << std::endl;
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
