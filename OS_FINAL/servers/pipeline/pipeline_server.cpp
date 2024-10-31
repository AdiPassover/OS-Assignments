#include "../server.hpp"
#include "../../graph/mst_algo/MST_factory.hpp"
#include "../client_commands.hpp"

PipelineServer::PipelineServer() : Server(), _handler() {
    PipelineHandler<PipelineData>::ProcessFunction send_message_func = [this](const PipelineData& data) -> PipelineServer::PipelineData {
        std::lock_guard<std::mutex> lock(_send_mutex);
        send_message(data.fd, data.message + "\n>> ");
        return data;
    };

    _handler.add_consecutive_stages(ClientCommands::NEW_GRAPH_STR, {
            [this](const PipelineData& data) {
                std::vector<std::string> args = ClientCommands::split(data.message);
                Graph new_graph = ClientCommands::handle_new_graph(args);

                std::unique_lock<std::mutex> graphs_lock(_graphs_mutex);
                auto& [graph, graph_mutex] = _client_graphs[data.fd];
                graphs_lock.unlock();

                {
                    std::lock_guard<std::mutex> lock(*graph_mutex);
                    graph = new_graph;
                }
                PipelineData new_data(data);
                new_data.message = ClientCommands::NEW_GRAPH_RES;
                return new_data;
            }, send_message_func

        }).add_consecutive_stages(ClientCommands::ADD_EDGE_STR, {
            [this](const PipelineData& data) {
                std::vector<std::string> args = ClientCommands::split(data.message);

                std::unique_lock<std::mutex> graphs_lock(_graphs_mutex);
                auto& [graph, graph_mutex] = _client_graphs[data.fd];
                graphs_lock.unlock();

                {
                    std::lock_guard<std::mutex> lock(*graph_mutex);
                    ClientCommands::handle_add_edge(args, graph);
                }
                PipelineData new_data(data);
                new_data.message = ClientCommands::ADD_EDGE_RES;
                return new_data;
            }, send_message_func

        }).add_consecutive_stages(ClientCommands::REMOVE_EDGE_STR, {
            [this](const PipelineData& data) {
                std::vector<std::string> args = ClientCommands::split(data.message);

                std::unique_lock<std::mutex> graphs_lock(_graphs_mutex);
                auto& [graph, graph_mutex] = _client_graphs[data.fd];
                graphs_lock.unlock();

                {
                    std::lock_guard<std::mutex> lock(*graph_mutex);
                    ClientCommands::handle_remove_edge(args, graph);
                }
                PipelineData new_data(data);
                new_data.message = ClientCommands::REMOVE_EDGE_RES;
                return new_data;
            }, send_message_func

        }).add_consecutive_stages(ClientCommands::MST_STR, {
            [this](const PipelineData& data) {
                std::vector<std::string> args = ClientCommands::split(data.message);
                PipelineData new_data("", data.fd);

                std::unique_lock<std::mutex> graphs_lock(_graphs_mutex);
                auto& [graph, graph_mutex] = _client_graphs[data.fd];
                graphs_lock.unlock();

                {
                    std::lock_guard<std::mutex> lock(*graph_mutex);
                    std::unique_ptr<MST_algorithm> mst_algo = MST_factory::get_algorithm(args[1]);
                    new_data.tree = std::make_shared<Tree>(mst_algo->get_MST(graph));
                }
                return new_data;
            }, [this](const PipelineData& data) {
                if (data.tree == nullptr) return data;
                PipelineData new_data(data);
                new_data.message.append("Total weight of MST: ").append(std::to_string(data.tree->get_weight())).append("\n");
                return new_data;
            }, [this](const PipelineData& data) {
                if (data.tree == nullptr) return data;
                PipelineData new_data(data);
                new_data.message.append("Longest distance between two vertices: ").append(std::to_string(data.tree->get_longest_distance())).append("\n");
                return new_data;
            }, [this](const PipelineData& data) {
                if (data.tree == nullptr) return data;
                PipelineData new_data(data);
                new_data.message.append("Average distance between two vertices: ").append(std::to_string(data.tree->get_avg_distance())).append("\n");
                return new_data;
            }, [this](const PipelineData& data) {
                if (data.tree == nullptr) return data;
                PipelineData new_data(data);
                new_data.message.append("Shortest distance between two vertices: ").append(std::to_string(data.tree->get_shortest_distance()));
                return new_data;
            }, send_message_func

        }).add_consecutive_stages(ClientCommands::PRINT_GRAPH_STR, {
            [this](const PipelineData& data) {
                PipelineData new_data(data);
                std::unique_lock<std::mutex> graphs_lock(_graphs_mutex);
                auto& [graph, graph_mutex] = _client_graphs[data.fd];
                graphs_lock.unlock();

                {
                    std::lock_guard<std::mutex> lock(*graph_mutex);
                    new_data.message = ClientCommands::handle_print_graph(graph);
                }
                return new_data;
            }, send_message_func

        }).add_stage(INVALID_KEY, send_message_func);
}

void PipelineServer::handle_message(const std::string &command, int sender_fd) {
    std::string command_key = command.substr(0, command.find(' '));

    if (ClientCommands::get_command(command_key) == ClientCommands::INVALID) {
        PipelineData data(ClientCommands::INVALID_COMMAND_RES, sender_fd);
        _handler.run_pipeline(INVALID_KEY, data);
    } else {
        PipelineData data(command, sender_fd);
        _handler.run_pipeline(command_key, data);
    }
}
