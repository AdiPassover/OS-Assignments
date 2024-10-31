#include "MST_algorithms.hpp"
#include "../../data_structures/union_find.hpp"
#include <vector>
#include <list>
#include <algorithm>

Tree Kruskal::find_MST(const Graph &graph) const {
    std::vector<Edge> edges;
    for (unsigned int i = 0; i < graph.num_vertices()-1; i++) {
        for (unsigned int j = i+1; j < graph.num_vertices(); j++) {
            if (graph.has_edge(i, j))
                edges.emplace_back(i, j, graph.get_weight(i, j));
        }
    }
    std::sort(edges.begin(), edges.end());

    UnionFind scc = UnionFind(graph.num_vertices());
    std::list<Edge> mst;

    for (const Edge& edge : edges) {
        if (scc.find(edge.u()) != scc.find(edge.v())) {
            mst.push_back(edge);
            scc.join(edge.u(), edge.v());
        }
    }

    return Tree(mst);
}