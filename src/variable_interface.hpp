#ifndef GRAPHMOD_VARIABLE_INTERFACE_HPP
#define GRAPHMOD_VARIABLE_INTERFACE_HPP

#include <vector>
#include <random>
#include <map>
#include <string>
#include "boost_libraries.hpp"
#include "factor_interface.hpp"
#include "alphabet.hpp"

namespace graphmod{
  template<class counts_type>
  class FactorInterface;

  template<class counts_type>
  class VariableInterface{
  public:
    virtual ~VariableInterface(){
    }
    virtual VariableInterface* clone(std::map<std::string, Alphabet<std::string> >&) const = 0;
    virtual void sample(counts_type&, std::mt19937_64&) = 0;
    virtual std::vector<graphmod::FactorInterface<counts_type>*> get_neighbors() const = 0;
    virtual std::vector<graphmod::FactorInterface<counts_type>*> get_parents() const = 0;
    virtual std::vector<graphmod::FactorInterface<counts_type>*> get_children() const = 0;
    virtual void set_observed(bool) = 0;
    virtual bool get_observed() const = 0;
    virtual double log_likelihood(counts_type&) const = 0;
    virtual std::string xml() const = 0;
    virtual std::string name() const = 0;
    virtual std::string type() const = 0;
    virtual std::string get_name() const = 0;
    virtual void set_name(std::string) = 0;
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
    }
  };
}

#endif
