#include "Graph.hpp"

int main(int argc, char* argv[]) {
    Graph g;

    unsigned int src;
    std::cout << "Please enter the source vertex" << std::endl;
    if (!(std::cin >> src) || src >= g.getNumVertices()) {
        std::cout << "Illegal vertex" << std::endl;
        exit(1);
    }

    g.dijkstra(src);

    return 0;
}