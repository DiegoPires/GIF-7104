cmake_minimum_required(VERSION 3.9)
project(openacc)

set(CMAKE_CXX_STANDARD 11)

# set(CMAKE_CXX_COMPILER /usr/local/bin/g++-7)
# set(CMAKE_C_COMPILER /usr/local/bin/gcc-7)
# enable openmp using -fopenmp
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(openacc
        PACC/Assert.hpp
        PACC/config.hpp
        PACC/Tokenizer.cpp
        PACC/Tokenizer.hpp
        exemple.png
        lib/Chrono.hpp
        lib/lodepng.cpp
        lib/lodepng.h
        makefile
        noyau_flou
        noyau_identite
        tp2.cpp)

