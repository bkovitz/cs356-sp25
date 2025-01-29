cmake_minimum_required(VERSION 3.10)
project(garage)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(garage garage.cpp)
