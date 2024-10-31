#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <limits.h>

using std::vector;
using std::string;

class Graph {
private:
    unsigned int numVertices;
    vector<vector<int>> adjMatrix;

    unsigned int minDistance(unsigned int dist[], bool sptSet[]) const;
    void printSolution(unsigned int dist[]) const;

public:
    Graph();

    unsigned int getNumVertices() const { return numVertices; }

    void dijkstra(unsigned int startingVertex);

};