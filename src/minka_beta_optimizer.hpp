#ifndef GRAPHMOD_MINKA_BETA_OPTIMIZER_HPP
#define GRAPHMOD_MINKA_BETA_OPTIMIZER_HPP

#include <vector>
#include <boost/math/special_functions.hpp>
#include "optimizer.hpp"
#include "boost_libraries.hpp"
#include "continuous_matrix_variable.hpp"
#include "minka_dirichlet_optimizer.hpp"

namespace graphmod{
  template<class counts_type>
  class MinkaBetaOptimizer : public Optimizer<MinkaBetaOptimizer<counts_type>, counts_type>{
  public:
    MinkaBetaOptimizer(ContinuousMatrixVariable<counts_type>* node, 
		       std::string group_name, 
		       std::string observation_name, 
		       bool symmetric) : 
      _node(node),
      _group_name(group_name), 
      _observation_name(observation_name), 
      _symmetric(symmetric){
    }
    MinkaBetaOptimizer(){
    }
    
    static inline std::vector<std::vector<double> > asymmetric_optimized(std::vector<std::vector<double> > current, 
									 std::vector<int> group_counts,
									 std::vector<std::vector<int> > group_observation_counts, 
									 int iterations=200
									 ){
      std::vector<std::vector<double> > updated(current);
      for(unsigned int obs=0; obs<current[0].size(); obs++){
	std::vector<double> o_current({current[0][obs], current[1][obs]});
	std::vector<std::vector<int> > o_group_observation_counts(group_observation_counts.size());
	std::transform(group_observation_counts.begin(), group_observation_counts.end(), group_counts.begin(), o_group_observation_counts.begin(),
		       [obs](std::vector<int> group_obs, int group_total){
			 return std::vector<int>({group_obs[obs], group_total - group_obs[obs]});
		       });
	
	auto a_b = MinkaDirichletOptimizer<counts_type>::asymmetric_optimized(o_current, o_group_observation_counts, iterations);
	updated[0][obs] = a_b[0];
	updated[1][obs] = a_b[1];
      }
      return updated;
    }
    
    static inline std::vector<std::vector<double> > symmetric_optimized(std::vector<std::vector<double> > current, 
									std::vector<int> group_counts,
									std::vector<std::vector<int> > group_observation_counts, 
									int iterations=200
									){
      std::vector<std::vector<double> > updated(current);
      return updated;
    }
    
    void optimize_implementation(const counts_type& counts, int iterations) const{
      _symmetric ? _node->set_value(symmetric_optimized(_node->get_value_copy(), counts(_group_name), counts(_group_name, _observation_name), iterations)) : 
	_node->set_value(asymmetric_optimized(_node->get_value_copy(), counts(_group_name), counts(_group_name, _observation_name), iterations));
      auto x = _node->get_value_copy();
      double a = std::accumulate(x[0].begin(), x[0].end(), 0.0) / x[0].size();
      double b = std::accumulate(x[1].begin(), x[1].end(), 0.0) / x[1].size();
      std::cout << "Beta: " << (a + b) / 2.0 << std::endl;
    }
  private:
    ContinuousMatrixVariable<counts_type>* _node;
    std::string _group_name;
    std::string _observation_name;
    bool _symmetric;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::base_object<Optimizer<MinkaBetaOptimizer<counts_type>, counts_type> >(*this);
      ar & _node & _group_name & _observation_name & _symmetric;
    }
  };
}

#endif
