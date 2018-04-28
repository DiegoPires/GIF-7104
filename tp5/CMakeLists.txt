cmake_minimum_required(VERSION 3.9)
project(tp2)

#set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_COMPILER /usr/local/bin/g++-7)
set(CMAKE_C_COMPILER /usr/local/bin/gcc-7)
# enable openmp using -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(main
        lib/Chrono.hpp
        makefile
        main.cpp
        lib/bitmap_gen.hpp)
