#include "GraphInterface.hpp"

#include <iostream>
#include <utility>
#include <exception>

#include "../Heap/PriorityQueue.hpp"

namespace myDSALib
{
namespace Graph
{

class AdjMatrix
    : public IGraph
{
private:
    std::unique_ptr<Weight> matrix;
    int rank;
    Vertex edge_count;

    Weight& Matrix(const Vertex& from, const Vertex& to) {
        return *(matrix.get() + from * rank + to);
    }
    const Weight& Matrix(const Vertex& from, const Vertex& to) const {
        return *(matrix.get() + from * rank + to);
    }
public:
    explicit AdjMatrix(int rank)
    {
        if(rank < 2) {
            throw std::out_of_range("Illegal rank value.");
        }
        this->rank = rank;

        this->matrix.reset(new Weight[rank * rank]);
        if(!matrix.get()) {
            throw std::out_of_range("Failed to build the matrix.");
        }

        for(int i = 0; i < rank; ++i)
        {
            for(int j = 0; j < rank; ++j)
            {
                Matrix(i, j) = 0.0;
            }
        }

        this->edge_count = 0;
    }

    AdjMatrix(const AdjMatrix&) = delete;
    AdjMatrix& operator=(const AdjMatrix&) = delete;

    AdjMatrix(AdjMatrix&& other)
    {
        this->matrix = std::move(other.matrix);
        this->rank = other.rank;
        this->edge_count = other.edge_count;

        other.rank = 0;
        other.edge_count = 0;
    }
    AdjMatrix& operator=(AdjMatrix&& other)
    {
        if(this == &other)
        {
            this->matrix = std::move(other.matrix);
            this->rank = other.rank;
            this->edge_count = other.edge_count;

            other.rank = 0;
            other.edge_count = 0;
        }

        return *this;
    }

    ~AdjMatrix() = default;

public:
    // get vertex count
    Vertex get_vertex_count() const
    {
        return this->rank;
    }
    // get edge count
    Vertex get_edge_count() const
    {
        return this->edge_count;
    }

public:
    // has vertex
    bool has_vertex(Vertex v) const override;
    // get Vertexs
    myDSALib::Linear::DynArray<Vertex> get_vertexs() const override;
    // count the vertexs
    size_t count_vertex() const override;

    // has edge
    bool has_edge(Vertex from, Vertex to) const override;
    // add the edge
    bool add_edge(const Edge& edge) override;
    // remove the edge
    bool remove_edge(Vertex from, Vertex to) override;
    // edge's weight
    Weight edge_weight(Vertex from, Vertex to) const override;
    // get Edges
    myDSALib::Linear::DynArray<Edge> get_edges() const override;
    // count edges
    size_t count_edge() const override;

    // get neighbor vertexs
    myDSALib::Linear::DynArray<Vertex> get_neighbor_vertexs(Vertex v) const override;
    // get incident edges
    myDSALib::Linear::DynArray<Edge> get_incident_edges(Vertex v) const override;
    // get outgoing edges
    myDSALib::Linear::DynArray<Edge> get_outgoing_deges(Vertex v) const override;
    // get vertex's degree
    size_t degree(Vertex v) const override;
    // get vertex's in degree
    size_t in_degree(Vertex v) const override;
    // get vertex's out degree
    size_t out_degree(Vertex v) const override;

public:
    //breath-first search
    void BFS(Vertex start, void (*pre)(Vertex)) const override;
    // deep-first search
    void DFS(Vertex start, void (*pre)(Vertex)) const override;

    // Dijkstra
    myDSALib::Linear::DynArray<Vertex> Dijkstra(Vertex from, Vertex to) const override;

public:
    // clear the AdjMatrix Graph
    void clear() override;

};

bool AdjMatrix::has_vertex(Vertex v) const {
    return 0 < v && v < rank;
}

typename myDSALib::Linear::DynArray<Vertex> AdjMatrix::get_vertexs() const {
    myDSALib::Linear::DynArray<Vertex> vexs;

    if(!this->matrix)
    {
        vexs.clear();
        return vexs;
    }

    for(int i = 0; i < this->rank; ++i)
    {
        vexs.push_back(i);
    }

    return vexs;
}

size_t AdjMatrix::count_vertex() const {

    if(!this->matrix)
    {
        return 0;
    }

    return this->rank;
}

bool AdjMatrix::has_edge(Vertex from, Vertex to) const
{
    if(!this->matrix)
    {
        return false;
    }
    if(Matrix(from, to) != 0)
    {
        return true;
    }
    return false;
}

bool AdjMatrix::add_edge(const Edge& edge)
{
    if(!this->matrix)
    {
        return false;
    }

    if(has_edge(edge.from(), edge.to()))
    {
        return false;
    }

    Matrix(edge.from(), edge.to()) = edge.weight();
    ++edge_count;
    return true;
}

bool AdjMatrix::remove_edge(Vertex from, Vertex to)
{
    if(!this->matrix)
    {
        return false;
    }

    if(Matrix(from, to) == 0.0)
    {
        return false;
    }

    Matrix(from, to) = 0.0;

    --edge_count;
    return true;
}

Weight AdjMatrix::edge_weight(Vertex from, Vertex to) const
{
    if(!this->matrix || !has_edge(from, to))
    {
        return 0.0;
    }

    return Matrix(from, to);
}

typename myDSALib::Linear::DynArray<Edge> AdjMatrix::get_edges() const
{
    myDSALib::Linear::DynArray<Edge> edges;
    if(!this->matrix)
    {
        return edges;
    }

    for(int i = 0; i < rank; ++i)
    {
        for(int j = 0; j < rank; ++j)
        {
            if(Matrix(i, j) != 0)
            {
                edges.push_back(Edge(i, j, Matrix(i, j)));
            }
        }
    }

    return edges;
}

size_t AdjMatrix::count_edge() const
{
    size_t count = 0;

    if(!this->matrix)
    {
        return 0;
    }

    for(int i = 0; i < rank; ++i)
    {
        for(int j = 0; j < rank; ++j)
        {
            if(Matrix(i, j) != 0)
            {
                ++count;
            }
        }
    }

    return count;
}

typename myDSALib::Linear::DynArray<Vertex> AdjMatrix::get_neighbor_vertexs(Vertex v) const
{
    myDSALib::Linear::DynArray<Vertex> vexs;
    if(!this->matrix)
    {
        return vexs;
    }

    for(int i = 0; i < this->rank; ++i)
    {
        if(Matrix(v, i) != 0)
        {
            vexs.push_back(i);
        }
    }

    return vexs;
}

typename myDSALib::Linear::DynArray<Edge> AdjMatrix::get_incident_edges(Vertex v) const
{
    myDSALib::Linear::DynArray<Edge> edges;

    if(!this->matrix)
    {
        return edges;
    }

    for(int i = 0; i < rank; ++i)
    {
        if(Matrix(i, v) != 0)
        {
            edges.push_back(Edge(i, v, Matrix(i, v)));
        }
    }

    return edges;
}

typename myDSALib::Linear::DynArray<Edge> AdjMatrix::get_outgoing_deges(Vertex v) const
{
    myDSALib::Linear::DynArray<Edge> edges;

    if(!this->matrix)
    {
        return edges;
    }

    for(int i = 0; i < rank; ++i)
    {
        if(Matrix(v, i) != 0)
        {
            edges.push_back(Edge(v, i, Matrix(v, i)));
        }
    }

    return edges;
}

size_t AdjMatrix::degree(Vertex v) const
{
    size_t degree = 0;

    if(!this->matrix || !has_vertex(v))
    {
        return 0;
    }

    for(int i = 0; i < rank; ++i)
    {
        if(Matrix(i, v) != 0)
        {
            ++degree;
        }
    }
    for(int i = 0; i < rank; ++i)
    {
        if(Matrix(v, i) != 0)
        {
            ++degree;
        }
    }

    return degree;
}

size_t AdjMatrix::in_degree(Vertex v) const
{
    size_t in_degree = 0;

    if(!this->matrix || !has_vertex(v))
    {
        return 0;
    }

    for(int i = 0; i < rank; ++i)
    {
        if(Matrix(v, i) != 0)
        {
            ++in_degree;
        }
    }

    return in_degree;
}

size_t AdjMatrix::out_degree(Vertex v) const
{
    size_t out_degree = 0;

    if(!this->matrix || !has_vertex(v))
    {
        return 0;
    }

    for(int i = 0; i < rank; ++i)
    {
        if(Matrix(v, i) != 0)
        {
            ++out_degree;
        }
    }

    return out_degree;
}

// ==================================================================

void AdjMatrix::BFS(Vertex start, void (*pre)(Vertex)) const
{
    if(!this->matrix || start < 0 || start > this->rank)
    {
        return;
    }

    // The visited canary to record the iterated vertex
    myDSALib::Linear::DynArray<bool> visited(this->rank, false);

    // The queue to help the BFS
    myDSALib::Linear::DynQueue<Vertex> queue;

    // push the start vertex into the vertex
    queue.push(start);
    visited[start] = true; // record the start vertex
    while(!queue.empty())
    {
        Vertex vex = queue.front();
        if(pre) // the pre to operate the vertex
        {
            pre(vex);
        }
        for(int i = 0; i < rank; ++i)
        {
            // push and record all the neighbor vertex (every out-degree) into the queue
            if(Matrix(vex, i) != 0 && !visited[i])
            {
                queue.push(i);
                visited[i] = true;
            }
        }
        queue.pop();
    }
}

void AdjMatrix::DFS(Vertex start, void (*pre)(Vertex)) const
{
    if(!this->matrix || start < 0 || start > this->rank)
    {
        return;
    }

    myDSALib::Linear::DynArray<bool> visited(this->rank, false);

    myDSALib::Linear::DynStack<Vertex> stack;

    stack.push(start);
    visited[start] = true;
    while(!stack.empty())
    {
        Vertex vex = stack.top();
        stack.pop();
        if(pre)
        {
            pre(vex);
        }
        for(int i = this->rank - 1; i > 0; --i)
        {
            // push and record the vertex into the stack
            if(Matrix(vex, i) != 0 && !visited[i])
            {
                stack.push(i);
                visited[i] = true;
            }
        }
    }
}

#ifndef GRAPH_PRIORITY_QUEUE_VERSION
typename myDSALib::Linear::DynArray<Vertex> AdjMatrix::Dijkstra(Vertex from, Vertex to) const
{
    myDSALib::Linear::DynArray<Vertex> path(this->rank, 0);             // remain the prev node
    myDSALib::Linear::DynArray<bool> visited(this->rank, false);        // record the visited vertex
    myDSALib::Linear::DynArray<Weight> dist(this->rank, MAX_WEIGHT);    // the minimum dist from start to vertex(index)

    if(!this->matrix)
    {
        return path;
    }

    dist[from] = 0; // dist self to self is 0

    for(int i = 0; i < this->rank - 1; ++i)
    {
        Weight min = MAX_WEIGHT;    // record the minimum dist
        Vertex min_idx = -1;        // record the minimum dist vertex

        // select the minimum dist and min_idx by dist[]
        for(int v = 0; v < this->rank; ++v)
        {
            if(!visited[v] && dist[v] <= min)
            {
                min = dist[v];
                min_idx = v;
            }
        }

        // every vertex has all been visited
        if(min_idx == -1)
        {
            break;
        }

        // record the min_idx
        visited[min_idx] = true;

        for(int v = 0; v < this->rank; ++v)
        {
            // renew the dist from min_idx vertex to v vertex
            if(!visited[v] && Matrix(min_idx, v) != 0 && dist[min_idx] + Matrix(min_idx, v) < dist[v])
            {
                dist[v] = dist[min_idx] + Matrix(min_idx, v);
                path[v] = min_idx;
            }
        }
    }

    return path;
}
#endif

#ifdef GRAPH_PRIORITY_QUEUE_VERSION
typename myDSALib::Linear::DynArray<Vertex> AdjMatrix::Dijkstra(Vertex from, Vertex to) const
{
    myDSALib::Linear::DynArray<Vertex> path(this->rank, 0);             // remain the pre of vertex
    myDSALib::Linear::DynArray<bool> visited(this->rank, false);
    myDSALib::Linear::DynArray<Weight> dist(this->rank, MAX_WEIGHT);

    if(!this->matrix)
    {
        return path;
    }

    using QueueElement = std::pair<Weight, Vertex>;
    class PairCompare{
        public:
            bool operator()(const QueueElement& qe1, const QueueElement& qe2) const
            {
                return qe1.first > qe2.first;
            }
    };
    myDSALib::Heap::PriorityQueue<QueueElement, PairCompare> pqueue;

    dist[from] = 0;
    pqueue.push(std::make_pair(0, from));

    while(!pqueue.empty())
    {
        Weight cur_dist = pqueue.top().first;
        Vertex cur_vertex = pqueue.top().second;
        pqueue.pop();

        if(visited[cur_vertex])
        {
            continue;
        }

        visited[cur_vertex] = true;

        if(cur_vertex == to)
        {
            break;
        }

        for(int v = 0; v < this->rank; ++v)
        {
            if(Matrix(cur_vertex, v) != 0 && !visited[v])
            {
                Weight new_dist = cur_dist + Matrix(cur_vertex, v);
                if(new_dist < dist[v])
                {
                    dist[v] = new_dist;
                    path[v] = cur_vertex;
                    pqueue.push(std::make_pair(new_dist, v));
                }
            }
        }
    }

    return path;
}
#endif

// ==================================================================

void AdjMatrix::clear()
{
    rank = 0;
    edge_count = 0;
    if(!this->matrix)
    {
        return;
    }
    this->matrix.reset();
}

} // namespace Graph
} // namespace myDSALib