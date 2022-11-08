cmake_minimum_required(VERSION 3.10)

project(OptiBProg1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmax-errors=2")

add_library(readLgf readLgf.cpp)
add_library(check check.cpp)
add_library(random randomVertex.cpp)
add_library(spanningTree mySpanningTree.cpp)
add_library(shortestPath myShortestPath.cpp)
add_library(steiner mySteinerTree.cpp)

add_executable(Main Main.cpp)
target_link_libraries(Main readLgf check random spanningTree shortestPath steiner)
