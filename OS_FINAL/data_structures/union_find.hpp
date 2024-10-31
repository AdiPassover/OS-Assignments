#ifndef OS_FINAL_UNION_FIND_HPP
#define OS_FINAL_UNION_FIND_HPP

#include <vector>
#include <climits>

class UnionFind {
public:
    explicit UnionFind(unsigned int n = 0);

    unsigned int find(unsigned int node);
    void join(unsigned int node1, unsigned int node2);

private:
    std::vector<unsigned int> _parent;
    std::vector<unsigned int> _rank;

    inline static const unsigned int ROOT = UINT_MAX;
};


#endif //OS_FINAL_UNION_FIND_HPP
