#include "union_find.hpp"


UnionFind::UnionFind(unsigned int n) : _parent(n, ROOT), _rank(n, 0) {}

unsigned int UnionFind::find(unsigned int node) {
    if (_parent[node] == ROOT) return node;
    return _parent[node] = find(_parent[node]);
}

void UnionFind::join(unsigned int node1, unsigned int node2) {
    node1 = find(node1);
    node2 = find(node2);
    if (_rank[node1] < _rank[node2])
        _parent[node1] = node2;
    else {
        _parent[node2] = node1;
        _rank[node1] += (_rank[node1] == _rank[node2]);
    }
}
