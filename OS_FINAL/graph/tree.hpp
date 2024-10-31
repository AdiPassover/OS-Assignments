#ifndef OS_FINAL_TREE_HPP
#define OS_FINAL_TREE_HPP

#include "graph.hpp"
#include <list>

class Tree {
public:
    explicit Tree(const std::list<Edge> &edges);
    Tree(const std::vector<unsigned int> &parents, const std::vector<int> & parent_weight);

    int get_weight() const;
    int get_shortest_distance() const;
    double get_avg_distance() const;
    int get_longest_distance() const;

private:
    const unsigned int _num_vertices;
    std::vector<std::vector<int>> _dist_matrix;
    int _weight;

    void init_distances(const std::vector<std::list<Edge>> &adj_list);
    void bfs(unsigned int src, const std::vector<std::list<Edge>> &adj_list);
    inline int get_distance(unsigned int src, unsigned int dest) const { return _dist_matrix[src][dest]; }

};


#endif //OS_FINAL_TREE_HPP
