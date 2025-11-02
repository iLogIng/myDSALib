#include "GraphInterface.hpp"

#include <iostream>
#include <exception>

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
    Vertex vertex_count;
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
            return;
        }
        this->rank = rank;

        this->matrix.reset(new Weight[rank * rank]);
        if(!matrix.get()) {
            throw std::out_of_range("Failed to build the matrix.");
            return;
        }

        for(int i = 0; i < rank; ++i)
        {
            for(int j = 0; j < rank; ++j)
            {
                Matrix(i, j) = 0.0;
            }
        }

        this->vertex_count = 0;
        this->edge_count = 0;
    }

    AdjMatrix(const AdjMatrix&) = delete;
    AdjMatrix& operator=(const AdjMatrix&) = delete;

    AdjMatrix(AdjMatrix&& other)
    {
        this->matrix = std::move(other.matrix);
        this->rank = other.rank;
        this->vertex_count = other.vertex_count;
        this->edge_count = other.edge_count;

        other.rank = 0;
        other.vertex_count = 0;
        other.edge_count = 0;
    }
    AdjMatrix& operator=(AdjMatrix&& other)
    {
        if(&other == this)
        {
            this->matrix = std::move(other.matrix);
            this->rank = other.rank;
            this->vertex_count = other.vertex_count;
            this->edge_count = other.edge_count;

            other.rank = 0;
            other.vertex_count = 0;
            other.edge_count = 0;
        }

        return *this;
    }

    ~AdjMatrix() = default;

public:
    // get vertex count
    Vertex get_vertex_count() const
    {
        return this->vertex_count;
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
    if(!this->matrix.get())
    {
        return false;
    }
    for(int i = 0; i < rank; ++i)
    {
        if(Matrix(v, i) != 0)
        {
            return true;
        }
    }
    return false;
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
        for(int j = 0; j < rank; ++j)
        {
            if(Matrix(i, j) != 0)
            {
                vexs.push_back(Matrix(i, j));
            }
        }
    }

    return vexs;
}

size_t AdjMatrix::count_vertex() const {
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
                break;
            }
        }
    }

    return count;
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

    if(!has_vertex(edge.from()))
    {
        ++vertex_count;
    }
    if(!has_vertex(edge.to()))
    {
        ++vertex_count;
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

    Matrix(from, to) = 0.0;

    if(!has_vertex(from))
    {
        --vertex_count;
    }
    if(!has_vertex(to))
    {
        --vertex_count;
    }

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

    myDSALib::Linear::DynArray<bool> visited(this->rank, false);

    myDSALib::Linear::DynQueue<Vertex> queue;

    queue.push(start);
    visited[start] = true;
    while(!queue.empty())
    {
        Vertex vex = queue.front();
        if(pre)
        {
            pre(vex);
        }
        for(int i = 0; i < rank; ++i)
        {
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
        for(int i = this->rank; i > 0; --i)
        {
            if(Matrix(vex, i) != 0 && !visited[i])
            {
                stack.push(i);
                visited[i] = true;
            }
        }
    }
}

typename myDSALib::Linear::DynArray<Vertex> AdjMatrix::Dijkstra(Vertex from, Vertex to) const
{
    myDSALib::Linear::DynArray<Vertex> path(this->rank, 0);             // remain the prev node
    myDSALib::Linear::DynArray<bool> visited(this->rank, false);
    myDSALib::Linear::DynArray<Weight> dist(this->rank, MAX_WEIGHT);

    if(!this->matrix)
    {
        return path;
    }

    dist[from] = 0;

    for(int i = 0; i < this->rank - 1; ++i)
    {
        Weight min = MAX_WEIGHT;
        Vertex min_idx = -1;
        for(int v = 0; v < this->rank; ++v)
        {
            if(!visited[v] && dist[v] <= min)
            {
                min = dist[v];
                min_idx = v;
            }
        }

        if(min_idx == -1)
        {
            break;
        }

        visited[min_idx] = true;

        for(int v = 0; v < this->rank; ++v)
        {
            if(!visited[v] && Matrix(i, v) != 0 && dist[min_idx] + Matrix(min_idx, v) < dist[v])
            {
                dist[v] = dist[min_idx] + Matrix(min_idx, v);
                path[v] = min_idx;
            }
        }
    }

    return path;
}

// ==================================================================

void AdjMatrix::clear()
{
    rank = 0;
    vertex_count = 0;
    edge_count = 0;
    if(!this->matrix)
    {
        return;
    }
    this->matrix.reset();
}

}   // namespace Graph
}   // namespace myDSALib