#ifndef OS_FINAL_FIBONACCI_HEAP_HPP
#define OS_FINAL_FIBONACCI_HEAP_HPP

#include <vector>
#include <unordered_map>
#include <climits>

#include <string>

class FibonacciHeap {
public:
    FibonacciHeap();
    ~FibonacciHeap();

    void insert(unsigned int vertex);
    unsigned int min() const;
    unsigned int delete_min();
    void decrease_key(unsigned int vertex, int new_key);
    bool contains(unsigned int vertex) const;
    bool empty() const;
    int key(unsigned int vertex) const;

private:
    struct Node {
        const unsigned int _vertex;
        Node* _parent;
        Node* _left;
        Node* _right;
        Node* _child;
        int _key;
        unsigned int _degree;
        bool _is_marked;

        inline explicit Node(unsigned int vertex, int key = INT_MAX) : _vertex(vertex), _parent(nullptr), _left(this),
                                                                       _right(this), _child(nullptr), _key(key),
                                                                       _degree(0), _is_marked(false) {}
        ~Node();

        void free_children();
    };

    unsigned int _size;
    Node* _min;
    std::unordered_map<unsigned int, Node*> _nodes;
    std::vector<Node*> _node_of_degree; // for consolidate

    void consolidate(Node* start);

    static void insert_left_to_node(Node* inserted_node, Node* node);
    void insert_left_to_min(Node* node);
    void promote_child(Node* child);
    Node* merge(Node* node1, Node* node2); // returns the parent node

};


#endif //OS_FINAL_FIBONACCI_HEAP_HPP
