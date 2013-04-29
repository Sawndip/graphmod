#ifndef GM_INSTANCES_HPP
#define GM_INSTANCES_HPP

#include <vector>
#include <boost/bimap.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/tokenizer.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include "instance.hpp"
//#include "vector.hpp"

namespace graphmod{
  class Instances : public std::vector<Instance>{
  public:
    Instances();
    Instances(std::string);
    std::vector<std::string> get_lookup_names() const;
    void add(std::map<std::string, std::vector<std::string> >);
    //void print(std::ostream&) const;
    int get_size(std::string);
    std::string get_name(std::string, int);
    std::string str() const;
    std::string xml();
    void save(std::string);
    int get_size();
    bool exists(std::string);
    std::vector<int> get_counts(std::string);
    Instance& get_instance(int);
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & BOOST_SERIALIZATION_NVP(_lookup);
      ar & boost::serialization::make_nvp("vectorinstance", boost::serialization::base_object<vector<Instance> >(*this));
    }
    std::map<std::string, boost::bimap<std::string, int> > _lookup;  
  };
}

#endif
