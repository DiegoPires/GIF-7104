cmake_minimum_required(VERSION 3.9)
project(openacc)

set(CMAKE_CXX_STANDARD 11)

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

