#ifndef OS_FINAL_MST_ALGORITHM_HPP
#define OS_FINAL_MST_ALGORITHM_HPP

#include "../tree.hpp"
#include <stdexcept>

class MST_algorithm {
public:
    inline Tree get_MST(const Graph& graph) {
        if (!graph.is_connected()) throw std::invalid_argument("Graph is not connected");
        return find_MST(graph);
    }
    virtual ~MST_algorithm() = default;

protected:
    virtual Tree find_MST(const Graph&) const = 0;
};

class Kruskal : public MST_algorithm {
    Tree find_MST(const Graph&) const override;
};

class Prim : public MST_algorithm {
    Tree find_MST(const Graph&) const override;
};


#endif //OS_FINAL_MST_ALGORITHM_HPP
