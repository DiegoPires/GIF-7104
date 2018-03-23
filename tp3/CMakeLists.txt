cmake_minimum_required (VERSION 2.6)

project (tp3)

# Require MPI for this project:
find_package(MPI REQUIRED)

#include_directories(/usr/local/Cellar/open-mpi/3.0.0_2/)
#set(CMAKE_C_COMPILER /usr/local/Cellar/open-mpi/3.0.0_2/bin/mpicc)
#set(CMAKE_CXX_COMPILER /usr/local/Cellar/open-mpi/3.0.0_2/bin/mpic++)

include_directories(${MPI_INCLUDE_PATH})

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

add_executable(tp3 main.cpp Matrix.cpp)

INCLUDE(CPack)
