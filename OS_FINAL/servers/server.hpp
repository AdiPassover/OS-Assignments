#ifndef OS_FINAL_SERVER_HPP
#define OS_FINAL_SERVER_HPP

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "../graph/graph.hpp"
#include "leader_follower/lf_handler.hpp"
#include "pipeline/pipeline_handler.hpp"
#include "../graph/tree.hpp"

#define PORT "9034"

class Server {
public:
    Server();
    virtual ~Server();

    void run();
    void stop();

protected:
    bool _running;
    int _listener;
    std::vector<struct pollfd> _poll_fds;
    std::unordered_map<int, std::pair<Graph, std::unique_ptr<std::mutex>>> _client_graphs;
    std::mutex _graphs_mutex;
    std::mutex _send_mutex;

    inline static constexpr unsigned int BUFFER_SIZE = 1024;
    inline static constexpr unsigned int MAX_CONNECTIONS = 10;

    virtual void handle_message(const std::string &command, int sender_fd) = 0;
    static void send_message(int fd, const std::string& message);
    static void *get_in_addr(struct sockaddr *sa);
    static int get_listener_socket();

};

class LFServer : public Server {
public:
    explicit LFServer(unsigned int num_threads);

protected:
    LFHandler _handler;

    void handle_message(const std::string &command, int sender_fd) override;
};

class PipelineServer : public Server {
public:
    PipelineServer();

private:
    struct PipelineData {
        std::string message;
        int fd;
        std::shared_ptr<Tree> tree;

        inline PipelineData(std::string message, int fd) : message(message), fd(fd), tree(nullptr) {}
        inline PipelineData(std::string error, const PipelineData& other) : message(error), fd(other.fd), tree(nullptr) {}
        inline PipelineData(const PipelineData& other) = default;
    };

protected:
    PipelineHandler<PipelineData> _handler;

    void handle_message(const std::string &command, int sender_fd) override;

private:
    inline static const std::string INVALID_KEY = "INVALID";
};

class ServerFactory {
    inline static constexpr unsigned int DEFAULT_NUM_THREADS = 5;
public:
    static std::unique_ptr<Server> get_server(char type);
};

#endif //OS_FINAL_SERVER_HPP
