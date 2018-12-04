cmake_minimum_required(VERSION 3.3)
project(MorphOpsOpenMP)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(SOURCE_FILES main.cpp Image.h Image.cpp PPM.h PPM.cpp Utils.h StructuringElement.h StructuringElement.cpp MorphableOperators.h MorphableOperators.cpp bitmap_image.hpp)

add_executable(MorphOpsOpenMP ${SOURCE_FILES})