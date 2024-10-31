#ifndef OS_FINAL_GRAPH_HPP
#define OS_FINAL_GRAPH_HPP

#include <string>
#include <vector>
#include <climits>

inline constexpr int NO_EDGE = INT_MIN;

class Edge {
public:
    Edge(unsigned int u, unsigned int v, int weight = 1);

    inline unsigned int u() const { return _u; }
    inline unsigned int v() const { return _v; }
    inline int weight() const { return _weight; }
    inline unsigned int get_other(unsigned int vertex) const { return vertex == _u ? _v : _u; }

    bool operator<(const Edge&) const;

private:
    unsigned int _u;
    unsigned int _v;
    int _weight;

};

class Graph {
public:
    explicit Graph(unsigned int n = 0);

    unsigned int num_vertices() const;

    void add_edge(unsigned int u, unsigned int v, int weight = 1);
    inline void add_edge(Edge e) { add_edge(e.u(), e.v(), e.weight()); }

    void remove_edge(unsigned int u, unsigned int v);

    bool has_edge(unsigned int u, unsigned int v) const;
    int get_weight(unsigned int u, unsigned int v) const;

    bool is_connected() const;

    std::string to_string() const;


private:
    unsigned int _num_vertices;
    std::vector<std::vector<int>> _adj_matrix;

};


#endif //OS_FINAL_GRAPH_HPP
