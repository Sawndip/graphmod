#ifndef GM_INSTANCE_HPP
#define GM_INSTANCE_HPP
#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/bimap.hpp>
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

namespace graphmod{
  class Instance : public std::map<std::string, std::vector<int> >{
  public:
    std::vector<int> get(std::string);
    std::string str() const;
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
      ar & boost::serialization::make_nvp("mapint", boost::serialization::base_object<std::map<std::string, std::vector<int> > >(*this));
    }
  };
}

#endif
