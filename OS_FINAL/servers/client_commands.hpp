#ifndef OS_FINAL_CLIENT_COMMANDS_HPP
#define OS_FINAL_CLIENT_COMMANDS_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "../graph/graph.hpp"


class ClientCommands {
public:
    inline static const std::string NEW_GRAPH_STR = "newgraph";
    inline static const std::string ADD_EDGE_STR = "addedge";
    inline static const std::string REMOVE_EDGE_STR = "removeedge";
    inline static const std::string MST_STR = "mst";
    inline static const std::string PRINT_GRAPH_STR = "printgraph";

    enum Command {
        NEW_GRAPH,
        ADD_EDGE,
        REMOVE_EDGE,
        MST,
        PRINT_GRAPH,
        INVALID
    };

    inline static const std::unordered_map<std::string, Command> command_map = {
            {NEW_GRAPH_STR, NEW_GRAPH},
            {ADD_EDGE_STR, ADD_EDGE},
            {REMOVE_EDGE_STR, REMOVE_EDGE},
            {MST_STR, MST},
            {PRINT_GRAPH_STR, PRINT_GRAPH},
    };

    static Command get_command(const std::string &command_str);

    static std::vector<std::string> split(const std::string &s, char delim = ' ');

    static Graph handle_new_graph(const std::vector<std::string> &args);
    static void handle_add_edge(const std::vector<std::string> &args, Graph& graph);
    static void handle_remove_edge(const std::vector<std::string> &args, Graph& graph);
    static std::string handle_mst(const std::vector<std::string> &args, const Graph& graph);
    static std::string handle_print_graph(const Graph& graph);


    inline static const std::string NEW_GRAPH_RES = "New graph created";
    inline static const std::string ADD_EDGE_RES = "Edge added";
    inline static const std::string REMOVE_EDGE_RES = "Edge removed";
    inline static const std::string INVALID_COMMAND_RES = "Invalid command";


};


#endif //OS_FINAL_CLIENT_COMMANDS_HPP
