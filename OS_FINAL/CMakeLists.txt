cmake_minimum_required(VERSION 3.28)
project(OS_FINAL)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(OS_FINAL
        graph/graph.cpp
        graph/graph.hpp
        graph/mst_algo/MST_algorithms.hpp
        graph/mst_algo/kruskal.cpp
        graph/mst_algo/prim.cpp
        graph/tree.cpp
        graph/tree.hpp
        main.cpp
        data_structures/union_find.cpp
        data_structures/union_find.hpp
        data_structures/fibonacci_heap.cpp
        data_structures/fibonacci_heap.hpp
        tests/test_data_structures.cpp
        tests/test_main.cpp
        tests/test_mst_algos.cpp
        graph/mst_algo/MST_factory.hpp
        servers/leader_follower/lf_handler.cpp
        servers/leader_follower/lf_handler.hpp
        servers/pipeline/pipeline_handler.hpp
        servers/server.cpp
        servers/server.hpp
        servers/client_commands.cpp
        servers/client_commands.hpp
        tests/test_handlers.cpp
        servers/leader_follower/lf_server.cpp
        servers/pipeline/pipeline_server.cpp
)
