cmake_minimum_required(VERSION 3.9)
project(opencl)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

message(STATUS "OpenCL found: ${OPENCL_FOUND}")
message(STATUS "OpenCL includes: ${OPENCL_INCLUDE_DIRS}")
message(STATUS "OpenCL CXX includes: ${OPENCL_HAS_CPP_BINDINGS}")
message(STATUS "OpenCL libraries: ${OPENCL_LIBRARIES}")

add_executable(opencl
        PACC/Assert.hpp
        PACC/config.hpp
        PACC/Tokenizer.cpp
        PACC/Tokenizer.hpp
        exemple.png
        lib/Chrono.hpp
        lib/lodepng.cpp
        lib/lodepng.h
        src/mykernel.cl
        makefile
        noyau_flou
        noyau_identite
        tp2.cpp
        )

target_include_directories (opencl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (opencl ${OpenCL_LIBRARY})