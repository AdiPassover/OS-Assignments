cmake_minimum_required(VERSION 3.28)
project(OS_A3)

set(CMAKE_CXX_STANDARD 14)

include_directories(Q3)

add_executable(OS_A3
        Q3/Point.cpp
        Q3/Point.hpp
        Q3/ConvexHull.cpp
        Q3/Main.cpp
        Q1/Main.cpp
        Q1/ConvexHull.cpp
        Q0/AtomClient.cpp
        Q0/AtomStorageServer.cpp
        Q0/UDSClient.cpp
        Q4/Server.cpp
        Q5-6/Server.cpp
        Q0/ClientAPI.hpp
        Q0/ClientAPI.cpp
        Q5-6/Reactor.hpp
        Q5-6/Reactor.cpp
        Q7/Server.cpp
        Q8-9/Server.cpp
        Q8-9/Proactor.cpp
        Q10/Server.cpp
)
