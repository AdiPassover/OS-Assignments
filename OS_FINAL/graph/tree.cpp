#include "tree.hpp"
#include <queue>

Tree::Tree(const std::list<Edge> &edges) : _num_vertices(edges.size()+1),
                                           _dist_matrix(edges.size()+1, std::vector<int>(edges.size()+1, 0)),
                                           _weight(0) {
    std::vector<std::list<Edge>> adj_list(_num_vertices, std::list<Edge>());
    for (const Edge& edge : edges) {
        adj_list[edge.u()].push_back(edge);
        adj_list[edge.v()].push_back(edge);
        _weight += edge.weight();
    }
    init_distances(adj_list);
}

Tree::Tree(const std::vector<unsigned int> &parents, const std::vector<int> &parent_weight) : _num_vertices(parents.size()),
                                                                                              _dist_matrix(parents.size(), std::vector<int>(parents.size(), 0)),
                                                                                              _weight(0) {
    std::vector<std::list<Edge>> adj_list(_num_vertices, std::list<Edge>());
    for (unsigned int u = 0; u < _num_vertices; u++) {
        if (parents[u] == _num_vertices) continue;
        adj_list[u].emplace_back(u, parents[u], parent_weight[u]);
        adj_list[parents[u]].emplace_back(parents[u], u, parent_weight[u]);
        _weight += parent_weight[u];
    }
    init_distances(adj_list);
}

int Tree::get_weight() const {
    return _weight;
}

int Tree::get_shortest_distance() const {
    int min = get_distance(0,1);
    for (unsigned int i = 0; i < _num_vertices - 1; i++) {
        for (unsigned int j = i+1; j < _num_vertices; j++) {
            min = std::min(min, get_distance(i, j));
        }
    }
    return min;
}

double Tree::get_avg_distance() const {
    int sum = 0;
    for (unsigned int i = 0; i < _num_vertices - 1; i++) {
        for (unsigned int j = i+1; j < _num_vertices; j++) {
            sum += get_distance(i, j);
        }
    }
    return (double)sum / (double)(_num_vertices * (_num_vertices - 1) / 2);
}

int Tree::get_longest_distance() const {
    int max = get_distance(0,1);
    for (unsigned int i = 0; i < _num_vertices - 1; i++) {
        for (unsigned int j = i+1; j < _num_vertices; j++) {
            max = std::max(max, get_distance(i, j));
        }
    }
    return max;
}

void Tree::bfs(unsigned int src, const std::vector<std::list<Edge>> &adj_list) {
    std::vector<bool> visited(_num_vertices, false);
    std::queue<unsigned int> q;
    q.push(src);

    while (!q.empty()) {
        unsigned int curr = q.front();
        q.pop();
        for (const Edge& edge : adj_list[curr]) {
            unsigned int v = edge.get_other(curr);
            if (!visited[v]) {
                _dist_matrix[src][v] = _dist_matrix[src][curr] + edge.weight();
                visited[v] = true;
                q.push(v);
            }
        }
    }
}

void Tree::init_distances(const std::vector<std::list<Edge>> &adj_list) {
    for (unsigned int src = 0; src < _num_vertices; src++) {
        bfs(src, adj_list);
    }
}