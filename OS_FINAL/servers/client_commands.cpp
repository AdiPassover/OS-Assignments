#include <stdexcept>
#include "client_commands.hpp"
#include "../graph/mst_algo/MST_factory.hpp"

std::vector<std::string> ClientCommands::split(const std::string &s, char delim) {
    std::vector<std::string> result;
    for (unsigned int i = 0; i < s.size(); i++) {
        std::string token;
        while (s[i] != delim && i < s.size()) {
            token += s[i];
            i++;
        }
        if (!token.empty()) result.push_back(token);
    }
    return result;
}

ClientCommands::Command ClientCommands::get_command(const std::string &command_str) {
    auto command = command_map.find(command_str);
    return command == command_map.end() ? Command::INVALID : command->second;
}

Graph ClientCommands::handle_new_graph(const std::vector<std::string> &args) {
    if (args.size() < 3) throw std::invalid_argument("Invalid number of arguments for new graph");
    unsigned int n = stoi(args[1]);
    unsigned int m = stoi(args[2]);
    unsigned int i;
    Graph g(n);
    for (i = 3; i+2 < args.size(); i += 3) {
        g.add_edge(stoi(args[i]), stoi(args[i + 1]), stoi(args[i + 2]));
    }
    if (i != m * 3 + 3) throw std::invalid_argument("Invalid number of arguments for new graph");
    return g;
}

void ClientCommands::handle_add_edge(const std::vector<std::string> &args, Graph &graph) {
    if (args.size() != 4) throw std::invalid_argument("Invalid number of arguments for add edge");
    graph.add_edge(stoi(args[1]), stoi(args[2]), stoi(args[3]));
}

void ClientCommands::handle_remove_edge(const std::vector<std::string> &args, Graph &graph) {
    if (args.size() != 3) throw std::invalid_argument("Invalid number of arguments for remove edge");
    graph.remove_edge(stoi(args[1]), stoi(args[2]));
}

std::string ClientCommands::handle_mst(const std::vector<std::string> &args, const Graph &graph) {
    if (args.size() != 2) throw std::invalid_argument("Invalid number of arguments for mst");
    std::unique_ptr<MST_algorithm> mst_algo = MST_factory::get_algorithm(args[1]);
    Tree tree = mst_algo->get_MST(graph);

    std::string result;
    result.append("Total weight of MST: ").append(std::to_string(tree.get_weight())).append("\n");
    result.append("Longest distance between two vertices: ").append(std::to_string(tree.get_longest_distance())).append("\n");
    result.append("Average distance between two vertices: ").append(std::to_string(tree.get_avg_distance())).append("\n");
    result.append("Shortest distance between two vertices: ").append(std::to_string(tree.get_shortest_distance()));
    return result;
}

std::string ClientCommands::handle_print_graph(const Graph &graph) {
    return graph.to_string();
}
