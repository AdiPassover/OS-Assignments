cmake_minimum_required(VERSION 3.28)
project(OS_A4)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(OS_A4
        part1/Q1-4/Graph.cpp
        part1/Q1-4/Graph.hpp
        part1/Q1-4/Main.cpp
        part1/Q5-6/hello.c
        part1/Q7/race.c
        part2/Q1/singleton.hpp
        part2/Q1/main.cpp
        part2/Q2/main.cpp
        part2/Q2/guard.hpp
)
