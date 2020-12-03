cmake_minimum_required(VERSION 3.0.0)
project(aoc_2020 VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
  set(PROJECT_FLAGS "/Wall /Ox")
else()
  set(PROJECT_FLAGS "-Wall -Wpedantic -Werror -O3")
endif()
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${PROJECT_FLAGS})

include_directories(include)
file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} aoc_*)
foreach(child ${children})
  message(Found\ AoC:\ ${child})
  add_subdirectory(${child})
endforeach()
