#include "Graph.hpp"
#include <iostream>
#include <getopt.h>

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {

    unsigned int numVertices;
    unsigned int numEdges;
    int seed;
    int option;

    if (argc != 7) {
        fprintf(stderr, "Usage: %s [-n num of vertices] [-m num of edges] [-s seed]\n", argv[0]);
        exit(1);
    }

    while ((option = getopt(argc, argv, "n:m:s:")) != -1) {
        switch (option) {
            case 'n':
                numVertices = atoi(optarg);
                break;
            case 'm':
                numEdges = atoi(optarg);
                break;
            case 's':
                seed = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-n num of vertices] [-m num of edges] [-s seed]\n", argv[0]);
                exit(1);
        }
    }

    Graph graph;
    try {
        graph = Graph(numVertices, numEdges, seed);
    } catch (const std::invalid_argument& e) {
        cout << e.what() << endl;
        return 0;
    }

    std::vector<Edge> circle = graph.getEulerianCircle();
    if (circle.empty()) {
        cout << "No Eulerian circle exists" << endl;
        return 0;
    }
    else cout << "Eulerian path found" << endl;
    cout << circle[0].u;
    for (auto& edge : circle)
        cout << "->" << edge.v;
    cout << endl;

    return 0;
}