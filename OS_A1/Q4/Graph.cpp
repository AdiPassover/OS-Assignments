#include "Graph.hpp"

using std::cin;
using std::cout;
using std::endl;

Graph::Graph() {

    cout << "Please enter the number of vertices" << endl;
    int size;
    if (!(cin >> size) || size < 0) {
        cout << "Invalid size" << endl;
        exit(1);
    }
    numVertices = size;

    adjMatrix.resize(numVertices);
    for (unsigned int i = 0; i < numVertices; i++)
        adjMatrix[i].resize(numVertices);

    for (unsigned int i = 0; i < numVertices; i++) {
        for (unsigned int j = 0; j < numVertices; j++) {
            cout << "Please enter the number of weight of edge " << i << "->" << j << endl;
            if (!(cin >> adjMatrix[i][j])) {
                cout << "Invalid weight" << endl;
                exit(1);
            }
            if (adjMatrix[i][j] < 0) {
                cout << "Dijkstra requires non-negative weights" << endl;
                exit(1);
            }
        }
        if (adjMatrix[i][i] != 0) {
            cout << "A simple graph doesn't have edges from a vertex to itself" << endl;
            exit(1);
        }
    }

}

// C++ program for Dijkstra's single source shortest path
// algorithm. The program is for adjacency matrix
// representation of the graph

// A utility function to find the vertex with minimum
// distance value, from the set of vertices not yet included
// in shortest path tree
unsigned int Graph::minDistance(unsigned int dist[], bool sptSet[]) const
{

    // Initialize min value
    unsigned int min = INT_MAX, min_index;

    for (unsigned int v = 0; v < numVertices; v++)
        if (!sptSet[v] && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

// A utility function to print the constructed distance
// array
void Graph::printSolution(unsigned int dist[]) const
{
    cout << "Vertex \t Distance from Source" << endl;
    for (unsigned int i = 0; i < numVertices; i++)
        cout << i << " \t\t\t\t" << dist[i] << endl;
}

// Function that implements Dijkstra's single source
// shortest path algorithm for a graph represented using
// adjacency matrix representation
void Graph::dijkstra(unsigned int src)
{
    unsigned int dist[numVertices]; // The output array.  dist[i] will hold the
    // shortest
    // distance from src to i

    bool sptSet[numVertices]; // sptSet[i] will be true if vertex i is
    // included in shortest
    // path tree or shortest distance from src to i is
    // finalized

    // Initialize all distances as INFINITE and stpSet[] as
    // false
    for (unsigned int i = 0; i < numVertices; i++)
        dist[i] = INT_MAX, sptSet[i] = false;

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Find shortest path for all vertices
    for (unsigned int count = 0; count < numVertices - 1; count++) {
        // Pick the minimum distance vertex from the set of
        // vertices not yet processed. u is always equal to
        // src in the first iteration.
        unsigned int u = minDistance(dist, sptSet);

        // Mark the picked vertex as processed
        sptSet[u] = true;

        // Update dist value of the adjacent vertices of the
        // picked vertex.
        for (unsigned int v = 0; v < numVertices; v++)

            // Update dist[v] only if is not in sptSet,
            // there is an edge from u to v, and total
            // weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!sptSet[v] && adjMatrix[u][v]
                && dist[u] != INT_MAX
                && dist[u] + adjMatrix[u][v] < dist[v])
                dist[v] = dist[u] + adjMatrix[u][v];
    }

    // print the constructed distance array
    printSolution(dist);

}
