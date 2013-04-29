%module cgraphmod
%include "typemaps.i"
%include "std_vector.i"
%include "std_set.i"
%include "std_string.i"
%include "std_map.i"
%include "std_pair.i"

%naturalvar;

%rename(__index__) *::operator[];
%rename(__str__) *::str;

%define STD_TEMPLATE_WRAP(prefix, T...)
  namespace std{
    %template(prefix ## Map) map<string, T>;
    %template(prefix ## Vector) vector<T>;
    %template(prefix ## VVector) vector<vector<T> >;
    %template(String ## Vector ## prefix ## Map) map<string, vector<T> >;    
    %template(String ## Map ## prefix ## Map) map<string, map<string, T> >;    
  }
%enddef

STD_TEMPLATE_WRAP(Int, int)
STD_TEMPLATE_WRAP(Double, double)
STD_TEMPLATE_WRAP(String, string)
%template(MapVectorPairStringIntInt) std::map<std::vector<std::pair<std::string, int> >, int>;

%{
INCLUDES

TYPEDEFS
%}

HACKS

INSTANTIATES

OTHER_INSTANTIATES

%pythoncode %{
  _cgraphmod.initialize_random();
%}
