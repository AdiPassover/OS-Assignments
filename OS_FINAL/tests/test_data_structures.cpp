#include "../data_structures/fibonacci_heap.hpp"
#include "../data_structures/union_find.hpp"
#include "doctest.h"

TEST_CASE("Union find") {
    UnionFind uf(10);
    for (unsigned int i = 0; i < 10; i++)
        CHECK(uf.find(i) == i);

    uf.join(0, 1);
    CHECK(uf.find(0) == uf.find(1));

    uf.join(1, 2);
    CHECK(uf.find(0) == uf.find(2));
    CHECK(uf.find(1) == uf.find(2));

    uf.join(3, 4);
    CHECK(uf.find(3) == uf.find(4));

    uf.join(2, 4);
    CHECK(uf.find(0) == uf.find(3));
    CHECK(uf.find(1) == uf.find(4));
    CHECK(uf.find(2) == uf.find(4));

    uf.join(5, 6);
    CHECK(uf.find(5) == uf.find(6));

    uf.join(7, 8);
    CHECK(uf.find(7) == uf.find(8));

    uf.join(9, 8);
    CHECK(uf.find(7) == uf.find(9));

    uf.join(6, 8);
    CHECK(uf.find(5) == uf.find(9));

    uf.join(3, 5);
    for (unsigned int i = 0; i < 10; i++)
        CHECK(uf.find(i) == uf.find(0));
}

TEST_CASE("Fibonacci heap") {
    SUBCASE("Simple") {
        FibonacciHeap heap;
        CHECK(heap.empty());
        CHECK_THROWS(heap.delete_min());
        CHECK_THROWS(heap.min());

        for (unsigned int i = 0; i < 3; i++)
            heap.insert(i);
        CHECK_FALSE(heap.empty());

        heap.decrease_key(0, 3);
        CHECK(heap.min() == 0);
        CHECK_THROWS(heap.decrease_key(0, 4));
        heap.decrease_key(1, 2);
        CHECK(heap.min() == 1);
        heap.decrease_key(2, 4);
        CHECK(heap.min() == 1);
        CHECK(heap.delete_min() == 1);
        CHECK(heap.min() == 0);
        CHECK(heap.delete_min() == 0);
        CHECK(heap.min() == 2);
        CHECK(heap.delete_min() == 2);
        CHECK(heap.empty());
    }

    SUBCASE("Exiting with non-empty") {
        FibonacciHeap heap;
        for (unsigned int i = 0; i < 10; i++)
            heap.insert(i);
        heap.decrease_key(3, 2);
        CHECK(heap.delete_min() == 3);
    }

    SUBCASE("Complex") {
        FibonacciHeap heap;
        for (unsigned int i = 0; i < 10; i++)
            heap.insert(i);
        heap.decrease_key(0, 0);
        CHECK(heap.delete_min() == 0);
        heap.decrease_key(2, 1);
        CHECK(heap.min() == 2);
        heap.decrease_key(3, 0);
        CHECK(heap.min() == 3);
        CHECK(heap.delete_min() == 3);
        CHECK(heap.min() == 2);
        CHECK(heap.delete_min() == 2);
        heap.decrease_key(6,3);
        CHECK(heap.min() == 6);
        heap.decrease_key(5, 5);
        CHECK(heap.min() == 6);
        CHECK(heap.delete_min() == 6);

        heap.decrease_key(1, -2);
        CHECK(heap.min() == 1);
        heap.decrease_key(4, 0);
        CHECK(heap.min() == 1);
        heap.decrease_key(8, 0);
        CHECK(heap.min() == 1);
        heap.decrease_key(7, 0);
        CHECK(heap.min() == 1);
        heap.decrease_key(7, -9);
        CHECK_THROWS(heap.decrease_key(7, -1));
        CHECK(heap.delete_min() == 7);
        heap.decrease_key(8,-1);
        CHECK(heap.delete_min() == 1);
        CHECK(heap.delete_min() == 8);
        CHECK(heap.delete_min() == 4);
        CHECK(heap.delete_min() == 5);
        CHECK(heap.delete_min() == 9);
        CHECK(heap.empty());
//        CHECK_THROWS(heap.delete_min());
    }
}