cmake_minimum_required(VERSION 3.0)

project(implicits)
set(CMAKE_CPP_COMPILER gcc)

add_executable(implicits src/main.cpp)
set_property(TARGET implicits PROPERTY CXX_STANDARD 20)
