#include <gtest/gtest.h>
#include "../../DSALib/include/Graph/AdjMatrix.hpp"

using namespace myDSALib::Graph;

TEST(AdjMatrixTestSuit, HasEdgeTest)
{
    AdjMatrix matrix(5);
    matrix.add_edge(Edge(0, 4, 3.0));
    matrix.add_edge(Edge(1, 2, 2.2));
    matrix.add_edge(Edge(1, 3, 1.3));
    matrix.add_edge(Edge(2, 4, 1.1));
    matrix.add_edge(Edge(3, 2, 0.7));
    matrix.add_edge(Edge(4, 3, 4.7));

    EXPECT_EQ(matrix.has_vertex(0), true);
    EXPECT_EQ(matrix.has_vertex(1), true);

    EXPECT_EQ(matrix.has_edge(0, 4), true);
    EXPECT_EQ(matrix.has_edge(1, 2), true);
    EXPECT_EQ(matrix.has_edge(1, 4), false);
    EXPECT_EQ(matrix.has_edge(2, 1), false);
    EXPECT_EQ(matrix.has_edge(3, 3), false);
    EXPECT_EQ(matrix.has_edge(3, 2), true);
    EXPECT_EQ(matrix.has_edge(4, 3), true);
}

TEST(AdjMatrixTestSuit, CountTest)
{
    AdjMatrix matrix(5);
    matrix.add_edge(Edge(0, 4, 3.0));
    matrix.add_edge(Edge(1, 2, 2.2));
    matrix.add_edge(Edge(1, 3, 1.3));
    matrix.add_edge(Edge(2, 4, 1.1));
    matrix.add_edge(Edge(3, 2, 0.7));
    matrix.add_edge(Edge(4, 3, 4.7));

    EXPECT_EQ(matrix.count_edge(), 6);
    EXPECT_EQ(matrix.count_vertex(), 5);
}

TEST(AdjMatrixTestSuit, DijkstraTest)
{
    AdjMatrix matrix(5);
    matrix.add_edge(Edge(0, 1, 1.1));
    matrix.add_edge(Edge(1, 2, 0.7));
    matrix.add_edge(Edge(1, 4, 0.1));
    matrix.add_edge(Edge(2, 3, 1.0));
    matrix.add_edge(Edge(3, 4, 1.9));

    myDSALib::Linear::DynArray<Vertex> path = matrix.Dijkstra(0, 4);
    EXPECT_EQ(path[0], 0);
    // EXPECT_EQ(path[1], 0);
    // EXPECT_EQ(path[2], 1);
    // EXPECT_EQ(path[3], 2);
    EXPECT_EQ(path[4], 1);
}