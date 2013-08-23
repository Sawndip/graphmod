#include "mpi_sampler.hpp"

namespace graphmod{
  namespace mpi = boost::mpi;
  void MPISampler::operator()(std::string input_file, std::string output_file){
    int rank = 0, size = 0, len = 10;
    char name[100] = "localhost";
    char** argv;
    int argc = 0;

    mpi::environment env(argc, argv);

    mpi::communicator world;
    rank = world.rank();
    size = world.size();

    MPI_Get_processor_name(name, &len);

    Model input;
    if(rank == 0){
      input = Model(input_file);
    }
    else{
    }

    if(rank == 0){
      input.save(output_file);
    }
  }
  Model MPISampler::operator()(Model model){
    return model;
  }
}
