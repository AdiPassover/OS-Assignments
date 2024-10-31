#include "doctest.h"
#include "../graph/mst_algo/MST_factory.hpp"

TEST_CASE("MST algorithms") {
    std::unique_ptr<MST_algorithm> kruskal = MST_factory::get_algorithm(MST_factory::KRUSKAL);
    std::unique_ptr<MST_algorithm> prim = MST_factory::get_algorithm(MST_factory::PRIM);

    Graph g(5);
    g.add_edge(0,1,3); // <-
    g.add_edge(0,2,6);
    g.add_edge(0,4,7);
    g.add_edge(1,2,2); // <-
    g.add_edge(1,3,5);
    g.add_edge(1,4,6);
    g.add_edge(2,3,8);
    g.add_edge(2,4,2); // <-
    g.add_edge(3,4,4); // <-

    SUBCASE("kruskal simple") {
        Tree t_kruskal = kruskal->get_MST(g);
        CHECK(t_kruskal.get_weight() == 11);
        CHECK(t_kruskal.get_shortest_distance() == 2);
        CHECK(t_kruskal.get_longest_distance() == 11);
        CHECK(t_kruskal.get_avg_distance() == 5.2);
    }

    SUBCASE("prim simple") {
        Tree t_prim = prim->get_MST(g);
        CHECK(t_prim.get_weight() == 11);
        CHECK(t_prim.get_shortest_distance() == 2);
        CHECK(t_prim.get_longest_distance() == 11);
        CHECK(t_prim.get_avg_distance() == 5.2);
    }

    Graph g2(5);
    g2.add_edge(0,1,-3); // <-
    g2.add_edge(0,2,2);
    g2.add_edge(1,2,7);
    g2.add_edge(1,4,-5); // <-
    g2.add_edge(2,4,-7); // <-
    g2.add_edge(3,4,14); // <-

    SUBCASE("kruskal negative edges") {
        Tree t_kruskal = kruskal->get_MST(g2);
        CHECK(t_kruskal.get_weight() == -1);
        CHECK(t_kruskal.get_shortest_distance() == -15);
        CHECK(t_kruskal.get_longest_distance() == 14);
        CHECK(t_kruskal.get_avg_distance() == -1.4);
    }

    SUBCASE("prim negative edges") {
        Tree t_prim = prim->get_MST(g2);
        CHECK(t_prim.get_weight() == -1);
        CHECK(t_prim.get_shortest_distance() == -15);
        CHECK(t_prim.get_longest_distance() == 14);
        CHECK(t_prim.get_avg_distance() == -1.4);
    }

}