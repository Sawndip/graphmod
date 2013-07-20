import os
import os.path
import sys
from SCons.Tool import textfile
from glob import glob
import logging
import time

vars = Variables("custom.py")
vars.AddVariables(
    ("OUTPUT_WIDTH", "", 130),

    ("BOOST_INCLUDE", "", "/usr/include"),
    ("BOOST_LIB", "", "/usr/lib"),
    ("PYTHON_PATH", "", "/usr/include/python2.7"),

    BoolVariable("DEBUG", "", False),
    ("OPTIMIZE_FLAGS", "", "-O3 -ffast-math -fno-builtin -pipe -march=native -msse2 -Wall ${USE_OMP and '-fopenmp' or ''}"),
    ("DEBUG_FLAGS", "", "-O0 -ggdb -Wall"),

    ("LOCAL_CXX", "", ""),
    ("LOCAL_SWIG", "", "/usr/bin/swig"),

    # parallel processing options
    BoolVariable("USE_MPI", "", False),
    ("MPICXX", "", "/usr/bin/mpic++"),

    BoolVariable("USE_STD_THREADS", "", False),
    ("STD_THREADS", "", 1),

    BoolVariable("USE_OMP", "", False),
    ("OMP_THREADS", "", 1),
    )

def print_cmd_line(s, target, source, env):
    if len(s) > int(env["OUTPUT_WIDTH"]):
        print s[:int(env["OUTPUT_WIDTH"]) - 10] + "..." + s[-7:]
    else:
        print s

logging.basicConfig(format='%(asctime)s %(message)s', datefmt='%Y-%m-%d %H:%M:%S', level=logging.INFO)


env = Environment(variables=vars, ENV=os.environ, TARFLAGS="-c -z", TARSUFFIX=".tgz",
                  tools=["default", "textfile"] + [x.TOOLS_ADD for x in []],
                  BUILDERS={"Swig" : Builder(action="${LOCAL_SWIG} -o ${TARGETS[0]} -outdir ${SWIGOUTDIR} ${_CPPDEFFLAGS} ${SWIGFLAGS} ${SOURCES[0]}"),
                            "CopyFile" : Builder(action="cp ${SOURCE} ${TARGET}"),
                            },
                  CCFLAGS="${OPTIMIZE_FLAGS} -std=c++11 -Wno-maybe-uninitialized -Wno-unused-variable",
                  SHLIBPREFIX="",
                  LIBS=["python2.7", "boost_filesystem", "boost_serialization", "boost_iostreams", "boost_regex", "boost_system", "boost_program_options", "gomp"],
                  CPPPATH=["${BOOST_INCLUDE}", "src/", "${PYTHON_PATH}"], 
                  LIBPATH=["${BOOST_LIB}"],
                  LINKFLAGS="${OPTIMIZE_FLAGS} -std=c++11",
                  SWIGFLAGS="-O -threads -c++ -python",
                  SWIGOUTDIR="work/",
                  )

if env["LOCAL_CXX"]:
    env.Replace(CXX=env["LOCAL_CXX"])

if env["USE_MPI"]:
    env.Append(CPPDEFINES = "GRAPHMOD_USE_MPI")
    #env.Append(LIBS=["boost_mpi"])
    env["CXX"] = env["MPICXX"]
if env["USE_OMP"]:
    env.Append(CPPDEFINES = "GRAPHMOD_USE_OMP")
    env.Append(CCFLAGS=" -fopenmp")
if env["USE_STD_THREADS"]:
    env.Append(CPPDEFINES = "GRAPHMOD_USE_STD_THREADS")

env['PRINT_CMD_LINE_FUNC'] = print_cmd_line
env.Decider("MD5-timestamp")

factors = [
    "Normal", "Geometric", "Categorical", "Multinomial", "Dirichlet", 
    "BetaBernoulli", "DirichletCategorical", "DoubleDirichletCategorical",
    ]

variables = [["%s%s" % (x, y) for x in ["Continuous", "Discrete"]] for y in ["Scalar", "Vector", "Matrix"]] + ["Categorical"]

count_types = ["DenseCounts", "SparseCounts"]

files = [
    "random",
    "boost_libraries",
    "graphmod_exception",
    "utils", "alphabet", "alphabets", "probability_vector", "log_probability_vector",
    
    #
    # mix-ins
    #
    "has_value", "categorical",
    
    #
    # data-related
    #
    "conll_word", "conll_sentence", "conll_data", "conll_loader", "instance", "instances",
    
    #
    # optimizers
    #
    "optimizer_interface", "optimizer", "minka_dirichlet_optimizer", "minka_beta_optimizer", "minka_double_dirichlet_optimizer",
    #"minka_beta_optimizer", "minka_double_dirichlet_optimizer",
    
    #
    # variables
    #
    "variable_interface", "variable", "continuous_scalar_variable", "continuous_vector_variable", "continuous_matrix_variable", "categorical_variable", "mapped_categorical_variable",
    #"continuous_vector_variable", "continuous_matrix_variable", "discrete_scalar_variable", "discrete_vector_variable", "discrete_matrix_variable",
    
    #
    # factors
    #
    "factor_interface", "factor", "dirichlet_categorical_factor", "beta_bernoulli_factor", "double_dirichlet_categorical_factor", "hierarchical_dirichlet_categorical_factor", "truncated_beta_bernoulli_factor",
    
    #
    # counts
    #
    "counts", "dense_counts",

    "configuration",

    "factor_graph",
    ]

#
# SWIG needs to be told which templates to instantiate and wrap
#
instantiates = {
    "categorical" : [
        ("StringCategorical", "graphmod::Categorical<std::string>"),
        ],
    
    "has_value" : [
        ("HasValueIntIntMap", "graphmod::HasValue<std::map<int, int> >"),
        ("HasValueInt", "graphmod::HasValue<int>"),
        ("HasValueVectorInt", "graphmod::HasValue<std::vector<int> >"),
        ("HasValueMatrixInt", "graphmod::HasValue<std::vector<std::vector<int> > >"),
        ("HasValueDouble", "graphmod::HasValue<double>"),
        ("HasValueVectorDouble", "graphmod::HasValue<std::vector<double> >"),
        ("HasValueMatrixDouble", "graphmod::HasValue<std::vector<std::vector<double> > >"),
        ],
    "instance" : [
        ("InstanceVector", "std::vector<graphmod::Instance>"),
        ],
    "variable_interface" : [
        ("_VariableInterface", "graphmod::VariableInterface<graphmod::Counts<graphmod::DenseCounts> >"),
        ],
    "factor_interface" : [
        ("_FactorInterface", "graphmod::FactorInterface<graphmod::Counts<graphmod::DenseCounts> >"),
        ],

    "variable" : [
        ("_CategoricalVariable", "graphmod::Variable<graphmod::CategoricalVariable<graphmod::Counts<graphmod::DenseCounts> >, graphmod::Counts<graphmod::DenseCounts> >"),
        ("_ContinuousScalarVariable", "graphmod::Variable<graphmod::ContinuousScalarVariable<graphmod::Counts<graphmod::DenseCounts> >, graphmod::Counts<graphmod::DenseCounts> >"),
        ("_ContinuousVectorVariable", "graphmod::Variable<graphmod::ContinuousVectorVariable<graphmod::Counts<graphmod::DenseCounts> >, graphmod::Counts<graphmod::DenseCounts> >"),
        ("_ContinuousMatrixVariable", "graphmod::Variable<graphmod::ContinuousMatrixVariable<graphmod::Counts<graphmod::DenseCounts> >, graphmod::Counts<graphmod::DenseCounts> >"),
        ],
    "factor" : [
        ("_DirichletCategoricalFactor", "graphmod::Factor<graphmod::DirichletCategoricalFactor<graphmod::Counts<graphmod::DenseCounts> >, graphmod::Counts<graphmod::DenseCounts> >"),
        ],

    "conll_word" : [
        ("ConllWordVector", "std::vector<graphmod::ConllWord>"),
        ],
    "conll_sentence" : [
        ("ConllSentenceVector", "std::vector<graphmod::ConllSentence>"),
        ],
    "counts" : [
        ("_Counts", "graphmod::Counts<graphmod::DenseCounts>"),
        ],
    "dense_counts" : [
        ],
    "alphabet" : [
        ("StringAlphabet", "graphmod::Alphabet<std::string>"),
        ("StringAlphabetMap", "std::map<std::string, graphmod::Alphabet<std::string> >"),
        ],
    "alphabets" : [
        ("StringAlphabets", "graphmod::Alphabets<std::string>"),
        ],
    "configuration" : [
        ("_Config", "graphmod::Configuration<graphmod::DenseCounts>"),
        ],

    "factor_graph" : [
        ("_FactorGraph", "graphmod::FactorGraph<graphmod::DenseCounts>"),
        ],
    }


typedefs = [
    ("_VariableInterface", "graphmod::VariableInterface<graphmod::DenseCounts>"),
    ("_FactorInterface", "graphmod::FactorInterface<graphmod::DenseCounts>"),
    ("_OptimizerInterface", "graphmod::OptimizerInterface<graphmod::DenseCounts>"),
    ]


other_instantiates = [
    ("CountKey", "std::vector<std::pair<std::string, int> >"),
    ("UnsignedIntVector", "std::vector<unsigned int>"),
    ("StringBoolMap", "std::map<std::string, bool>"),
    ("matrix_sum", "graphmod::matrix_sum<int>"),
    ("Cube", "std::vector<std::vector<std::vector<int> > >"),
    ("_Variable", "graphmod::VariableInterface<graphmod::DenseCounts>"),
    ("___ContinuousVectorVariable", "graphmod::Variable<graphmod::ContinuousVectorVariable<graphmod::DenseCounts>, graphmod::DenseCounts>"),
    ("___ContinuousMatrixVariable", "graphmod::Variable<graphmod::ContinuousMatrixVariable<graphmod::DenseCounts>, graphmod::DenseCounts>"),
    ("__ContinuousVectorVariable", "graphmod::ContinuousVectorVariable<graphmod::DenseCounts>"),
    ("__ContinuousMatrixVariable", "graphmod::ContinuousMatrixVariable<graphmod::DenseCounts>"),
    ("VariableVector", "std::vector<graphmod::VariableInterface<graphmod::DenseCounts>*>"),
    ("FactorVector", "std::vector<graphmod::FactorInterface<graphmod::DenseCounts>*>"),
    ("OptimizerVector", "std::vector<graphmod::OptimizerInterface<graphmod::DenseCounts>*>"),
    ]


module = {
    "ProbabilityVector" : "ProbabilityVector",
    "LogProbabilityVector" : "LogProbabilityVector",
    "StringBoolMap" : "StringBoolMap",
    "FactorGraph" : "_FactorGraph",
    "DenseCounts" : "DenseCounts",
    "SizeVector" : "UnsignedIntVector",
    "Instances" : "Instances",
    "from_conll" : "from_conll",
    "from_lines" : "from_lines",
    "matrix_sum" : "matrix_sum",
    "cast" : "cast",
    }

hacks = ["Counts", ""]

inlines = """
//namespace graphmod{
//  %template(getvector) FactorGraph<DenseCounts>::get_variable<ContinuousVectorVariable<DenseCounts> >;
//}
//%template(getmatrix) graphmod::FactorGraph<graphmod::DenseCounts>::get_variable<graphmod::ContinuousMatrixVariable<graphmod::DenseCounts> >;
%extend graphmod::FactorGraph<graphmod::DenseCounts>{
public:
%template(getvector) get_variable<ContinuousVectorVariable<DenseCounts> >;
%template(getmatrix) get_variable<ContinuousMatrixVariable<DenseCounts> >;
//int x(){ return 1; }
};

%inline %{
  graphmod::VariableInterface<graphmod::DenseCounts>* cast(graphmod::ContinuousVectorVariable<graphmod::DenseCounts>* d){
    return dynamic_cast<graphmod::VariableInterface<graphmod::DenseCounts>*>(d);
  }
  graphmod::VariableInterface<graphmod::DenseCounts>* cast(graphmod::ContinuousMatrixVariable<graphmod::DenseCounts>* d){
    return dynamic_cast<graphmod::VariableInterface<graphmod::DenseCounts>*>(d);
  }
%}
"""

no_wrap = []
headers = ["%s.hpp" % x for x in files if x not in no_wrap]
sources = [y for y in ["src/%s.cpp" % x for x in files] if os.path.exists(y)]


swig_file = env.Substfile("src/graphmod.i", "data/graphmod_template.i", SUBST_DICT={
        "INCLUDES" : "\n".join(['#include "%s.hpp"' % x for x in files]),
        "HACKS" : "",
        "TYPEDEFS" : "\n".join(["typedef %s %s;" % (v, k) for k, v in typedefs]), 
        "INSTANTIATES" : "\n".join(['%%include "%s.hpp"\n%s' % (name, "\n".join(['%%template(%s) %s;' % (k, v) for k, v in instantiates.get(name, [])])) for name in files]),
        "OTHER_INSTANTIATES" : "\n".join(['%%template(%s) %s;' % (k, v) for k, v in other_instantiates]),
        "INLINES" : inlines,
        })

py_file = env.Textfile("work/graphmod.py", ["from cgraphmod import %s as %s" % (v, k) for k, v in module.iteritems()])

swig_gen, swig_py = env.Swig(["src/cgraphmod_wrap.cc", "work/cgraphmod.py"], swig_file)
env.Depends(swig_gen, ["src/%s" % x for x in headers])

sources = [y for y in [os.path.join("src", "%s.cpp" % (x)) for x in files] if os.path.exists(y)]

wrap = env.SharedLibrary(["work/_cgraphmod.so"], [swig_gen] + sources)
env.Depends(wrap, py_file)
env.Default(wrap)
