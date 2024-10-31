#include "Graph.hpp"
#include <stack>
#include <stdexcept>


Graph::Graph(unsigned int nVertices, unsigned int nEdges, int seed) : Graph(nVertices) { // Random graph
    if (nEdges > (nVertices*nVertices - nVertices)/2)
        throw std::invalid_argument("Too many edges");
    srand(seed);

    std::vector<Edge> edges;
    edges.reserve((nVertices*nVertices - nVertices)/2);
    for (unsigned int i = 0; i < nVertices-1; i++) {
        for (unsigned int j = i+1; j < nVertices; j++) {
            edges.push_back({i,j});
        }
    }

    while (edges.size() > nEdges) {
        unsigned int randomIndex = rand() % edges.size();
        std::swap(edges[randomIndex], edges.back()); // Swapping so we can use pop_back instead of erasing the mid
        edges.pop_back();
    }

    for (const auto& edge : edges)
        addEdge(edge);
}


void Graph::addEdge(unsigned int u, unsigned int v) {
    if (u == v) throw std::logic_error("Simple graphs don't have an edge from a vertex to itself.");
    adjMatrix.at(u).at(v) = adjMatrix.at(v).at(u) = true;
}


std::vector<Edge> Graph::getEulerianCircle() const {
    std::vector<unsigned int> degree(numVertices, 0);
    for (unsigned int i = 0; i < numVertices; i++) {
        for (unsigned int j = 0; j < numVertices; j++)
            degree[i] += adjMatrix[i][j];

        if (degree[i] % 2 == 1) return {}; // No Eulerian circle exists if any vertex has odd degree
    }

    unsigned int startVertex = numVertices;
    for (unsigned int i = 0; i < numVertices; i++) {
        if (degree[i] > 0) {
            startVertex = i;
            break;
        }
    }
    if (startVertex == numVertices) return {}; // No edges in the graph

    std::vector<Edge> eulerianCircle;
    std::stack<unsigned int> stack;
    std::vector<std::vector<bool>> adjMatrixCopy = adjMatrix;

    stack.push(startVertex);
    while (!stack.empty()) {
        unsigned int u = stack.top();
        bool foundEdge = false;

        for (unsigned int v = 0; v < numVertices; v++) { // Find an edge from u
            if (adjMatrixCopy[u][v]) {
                adjMatrixCopy[u][v] = adjMatrixCopy[v][u] = false;
                stack.push(v);
                foundEdge = true;
                break;
            }
        }

        if (!foundEdge) { // We finished all the edges from u, add it to the path
            stack.pop();
            if (!stack.empty()) {
                unsigned int v = stack.top();
                eulerianCircle.push_back({u, v});
            }
        }
    }

    return eulerianCircle;
}