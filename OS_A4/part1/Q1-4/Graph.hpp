#ifndef OS_A4_GRAPH_HPP
#define OS_A4_GRAPH_HPP
#include <vector>


typedef struct {
    unsigned int u;
    unsigned int v;
} Edge;

class Graph {
private:
    unsigned int numVertices;
    std::vector<std::vector<bool>> adjMatrix;

public:
    inline Graph() : numVertices(0) {}
    inline explicit Graph(unsigned int size) : numVertices(size), adjMatrix(size, std::vector<bool>(size)) {}
    Graph(unsigned int nVertices, unsigned int nEdges, int seed = 42); // Generates random graph

    void addEdge(unsigned int u, unsigned int v);
    inline void addEdge(const Edge& e) { addEdge(e.u, e.v); }

    /**
     * @return an Eulerian circle in the graph or empty vector if none exist
     */
    std::vector<Edge> getEulerianCircle() const;

};

#endif //OS_A4_GRAPH_HPP
