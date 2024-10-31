#include "../server.hpp"
#include "../client_commands.hpp"

LFServer::LFServer(unsigned int num_threads) : Server(), _handler(num_threads) {

}

std::string process_command(const std::vector<std::string> &args, Graph& graph) {
    switch (ClientCommands::get_command(args[0])) {
        case ClientCommands::Command::NEW_GRAPH:
            graph = ClientCommands::handle_new_graph(args);
            return ClientCommands::NEW_GRAPH_RES;

        case ClientCommands::Command::ADD_EDGE:
            ClientCommands::handle_add_edge(args, graph);
            return ClientCommands::ADD_EDGE_RES;

        case ClientCommands::Command::REMOVE_EDGE:
            ClientCommands::handle_remove_edge(args, graph);
            return ClientCommands::REMOVE_EDGE_RES;

        case ClientCommands::Command::MST:
            return ClientCommands::handle_mst(args, graph);

        case ClientCommands::Command::PRINT_GRAPH:
            return ClientCommands::handle_print_graph(graph);

        default:
            return ClientCommands::INVALID_COMMAND_RES;
    }
}

void LFServer::handle_message(const std::string &command, int sender_fd) {
    _handler.add_task( [this, command, sender_fd] () {
        std::unique_lock<std::mutex> graphs_lock(_graphs_mutex);
        auto& [graph, graph_mutex] = _client_graphs[sender_fd];
        graphs_lock.unlock();

        std::vector<std::string> args = ClientCommands::split(command);
        std::string response;
        {
            std::lock_guard<std::mutex> lock(*graph_mutex);
            try {
                response = process_command(args, graph);
            } catch (const std::exception &e) {
                response = e.what();
            }
        }
        response.append("\n>> ");

        {
            std::lock_guard<std::mutex> lock(_send_mutex);
            send_message(sender_fd, response);
        }
    });
}
