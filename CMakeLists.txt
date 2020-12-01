cmake_minimum_required(VERSION 3.0.0)
project(aoc_2020 VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Werror -O3")

file(GLOB childrens RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} aoc_*)
foreach(child ${childrens}) 
  message(Found\ AoC:\ ${child})
  add_subdirectory(${child})
endforeach()
