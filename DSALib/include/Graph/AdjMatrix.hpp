#pragma once

#include "GraphInterface.hpp"

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
                Matrix(i, j) = MAX_WEIGHT;
            }
        }

        for(int i = 0; i < this->rank; ++i)
        {
            Matrix(i, i) = 0.0;
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
        if(this != &other)
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
    // add the edge undirected
    bool add_edge_undirected(Vertex u, Vertex v, Weight w) override;
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
    myDSALib::Linear::DynArray<Edge> get_outgoing_edges(Vertex v) const override;
    // get vertex's degree
    size_t degree(Vertex v) const override;
    // get vertex's in degree
    size_t in_degree(Vertex v) const override;
    // get vertex's out degree
    size_t out_degree(Vertex v) const override;

public:
    // breath-first search
    void BFS(Vertex start, void (*pre)(Vertex)) const override;
    // deep-first search
    void DFS(Vertex start, void (*pre)(Vertex)) const override;

    // Dijkstra
    std::pair<myDSALib::Linear::DynArray<Vertex>, myDSALib::Linear::DynArray<Weight>>
    Dijkstra(Vertex from, Vertex to) const override;

    // Bellman-Ford
    std::pair<myDSALib::Linear::DynArray<Vertex>, myDSALib::Linear::DynArray<Weight>>
    Bellman_Ford(Vertex from) const override;

    // Floyd_Wallshall, store the matrix by linear struct
    myDSALib::Linear::DynArray<Weight>
    Floyd_Wallshall() const override;

    // Prim
    myDSALib::Linear::DynArray<Edge>
    Prim() const override;

    // Prim
    myDSALib::Linear::DynArray<Edge>
    Kruskal() const override;

    // topological_sort
    myDSALib::Linear::DynArray<Vertex>
    topological_sort() const override;

    // Active On Edge Net
    std::pair<myDSALib::Linear::DynArray<Weight>, myDSALib::Linear::DynArray<Weight>>
    AOE_net() const override;

    // critical path for AOE
    myDSALib::Linear::DynArray<Edge>
    get_critical_path() const override;

public:
    // clear the AdjMatrix Graph
    void clear() override;

};

bool AdjMatrix::has_vertex(Vertex v) const {
    return 0 < v && v < rank;
}

typename myDSALib::Linear::DynArray<Vertex>
AdjMatrix::get_vertexs() const {
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
    if(Matrix(from, to) != 0 && Matrix(from, to) < MAX_WEIGHT && Matrix(from, to) > MIN_WEIGHT)
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

bool AdjMatrix::add_edge_undirected(Vertex u, Vertex v, Weight w)
{
    if(!this->matrix)
    {
        return false;
    }

    if(!has_edge(u, v) && !has_edge(v, u))
    {
        this->edge_count += 2;
        Matrix(u, v) = w;
        Matrix(v, u) = w;
        return true;
    }

    return false;
}

bool AdjMatrix::remove_edge(Vertex from, Vertex to)
{
    if(!this->matrix)
    {
        return false;
    }

    if(!has_edge(from, to))
    {
        return false;
    }

    Matrix(from, to) = MAX_WEIGHT;

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

typename myDSALib::Linear::DynArray<Edge>
AdjMatrix::get_edges() const
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
            if(has_edge(i, j))
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
            if(has_edge(i, j))
            {
                ++count;
            }
        }
    }

    return count;
}

typename myDSALib::Linear::DynArray<Vertex>
AdjMatrix::get_neighbor_vertexs(Vertex v) const
{
    myDSALib::Linear::DynArray<Vertex> vexs;
    if(!this->matrix)
    {
        return vexs;
    }

    for(int i = 0; i < this->rank; ++i)
    {
        if(has_edge(v, i))
        {
            vexs.push_back(i);
        }
    }

    return vexs;
}

typename myDSALib::Linear::DynArray<Edge>
AdjMatrix::get_incident_edges(Vertex v) const
{
    myDSALib::Linear::DynArray<Edge> edges;

    if(!this->matrix)
    {
        return edges;
    }

    for(int i = 0; i < rank; ++i)
    {
        if(has_edge(i, v))
        {
            edges.push_back(Edge(i, v, Matrix(i, v)));
        }
    }

    return edges;
}

typename myDSALib::Linear::DynArray<Edge>
AdjMatrix::get_outgoing_edges(Vertex v) const
{
    myDSALib::Linear::DynArray<Edge> edges;

    if(!this->matrix)
    {
        return edges;
    }

    for(int i = 0; i < rank; ++i)
    {
        if(has_edge(v, i))
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
        if(has_edge(i, v))
        {
            ++degree;
        }
    }
    for(int i = 0; i < rank; ++i)
    {
        if(has_edge(v, i))
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

    // i -> v
    for(int i = 0; i < rank; ++i)
    {
        if(has_edge(i, v))
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

    // v -> i
    for(int i = 0; i < rank; ++i)
    {
        if(has_edge(v, i))
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
            if(has_edge(vex, i) && !visited[i])
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
            if(has_edge(vex, i) && !visited[i])
            {
                stack.push(i);
                visited[i] = true;
            }
        }
    }
}

#ifndef GRAPH_PRIORITY_QUEUE_VERSION
typename std::pair<myDSALib::Linear::DynArray<Vertex>, myDSALib::Linear::DynArray<Weight>>
AdjMatrix::Dijkstra(Vertex from, Vertex to) const
{
    myDSALib::Linear::DynArray<Vertex> path(this->rank, 0);             // remain the prev node
    myDSALib::Linear::DynArray<bool> visited(this->rank, false);        // record the visited vertex
    myDSALib::Linear::DynArray<Weight> dist(this->rank, MAX_WEIGHT);    // the minimum dist from start to vertex(index)

    if(!this->matrix)
    {
        return std::make_pair(path, dist);
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
            if(!visited[v] && has_edge(min_idx, v) && dist[min_idx] + Matrix(min_idx, v) < dist[v])
            {
                dist[v] = dist[min_idx] + Matrix(min_idx, v);
                path[v] = min_idx;
            }
        }
    }

    return std::make_pair(path, dist);
}
#endif

#ifdef GRAPH_PRIORITY_QUEUE_VERSION
typename std::pair<myDSALib::Linear::DynArray<Vertex>, myDSALib::Linear::DynArray<Weight>>
AdjMatrix::Dijkstra(Vertex from, Vertex to) const
{
    myDSALib::Linear::DynArray<Vertex> path(this->rank, 0);             // remain the pre of vertex
    myDSALib::Linear::DynArray<bool> visited(this->rank, false);
    myDSALib::Linear::DynArray<Weight> dist(this->rank, MAX_WEIGHT);

    if(!this->matrix)
    {
        return std::make_pair(path, dist);
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
        Vertex cur_vex = pqueue.top().second;
        pqueue.pop();

        if(visited[cur_vex])
        {
            continue;
        }

        visited[cur_vex] = true;

        if(cur_vex == to)
        {
            break;
        }

        for(int v = 0; v < this->rank; ++v)
        {
            if(has_edge(cur_vex, v) && !visited[v])
            {
                Weight new_dist = cur_dist + Matrix(cur_vex, v);
                if(new_dist < dist[v])
                {
                    dist[v] = new_dist;
                    path[v] = cur_vex;
                    pqueue.push(std::make_pair(new_dist, v));
                }
            }
        }
    }

    return std::make_pair(path, dist);
}
#endif

typename std::pair<myDSALib::Linear::DynArray<Vertex>, myDSALib::Linear::DynArray<Weight>>
AdjMatrix::Bellman_Ford(Vertex from) const
{
    myDSALib::Linear::DynArray<Weight> dist(this->rank, MAX_WEIGHT);
    myDSALib::Linear::DynArray<Vertex> path(this->rank, 0);

    if(!this->matrix)
    {
        return std::make_pair(path, dist);
    }

    dist[from] = 0;

    // changed canary
    bool changed = false;
    // recompute all the path's weight
    for(int i = 0; i < this->rank - 1; ++i)
    {
        changed = false;
        for(int u = 0; u < this->rank; ++u)
        {
            for(int v = 0; v < this->rank; ++v)
            {
                if(has_edge(u, v) && dist[u] != MAX_WEIGHT && dist[u] + Matrix(u, v) < dist[v])
                {
                    dist[v] = dist[u] + Matrix(u, v);
                    path[v] = u;
                    changed = true;
                }
            }
        }
        if(!changed)
        { // has no change of dist elems
            break;
        }
    }

    // check if the graph contains negative weight cycle
    for(int u = 0; u < this->rank; ++u)
    {
        for(int v = 0; v < this->rank; ++v)
        {
            if(Matrix(u, v) != 0 && dist[u] != MAX_WEIGHT && dist[u] + Matrix(u, v) < dist[v])
            {
                throw std::runtime_error("GRAPH CONTAINS NEGATIVE WEIGHT CYCLE.");
            }
        }
    }

    return std::make_pair(path, dist);
}

typename myDSALib::Linear::DynArray<Weight>
AdjMatrix::Floyd_Wallshall() const
{
    myDSALib::Linear::DynArray<Weight> dist(this->rank * this->rank, MAX_WEIGHT);

    if(!this->matrix)
    {
        return dist;
    }

    // init the dist matrix
    for(int i = 0; i < this->rank; ++i)
    {
        for(int j = 0; j < this->rank; ++j)
        {
            if(i == j)
            {
                dist[i * rank + j] = 0;
            }
            else if(has_edge(i, j))
            {
                dist[i * rank + j] = Matrix(i, j);
            }
        }
    }

    // compute the dist from all vertex to all vertex
    for(int k = 0; k < this->rank; ++k)
    {
        for(int u = 0; u < this->rank; ++u)
        {
            for(int v = 0; v < this->rank; ++v)
            {
                int u_k = u * rank + k; // u to k
                int k_v = k * rank + v; // k to v
                int u_v = u * rank + v; // u to v
                // test if k as the middle vertex from u to v will make the dist(u, v) smaller
                if(dist[u_k] != MAX_WEIGHT && dist[k_v] != MAX_WEIGHT && dist[u_k] + dist[k_v] < dist[u_v])
                {
                    dist[u_v] = dist[u_k] + dist[k_v];
                }
            }
        }
    }

    // also need to check if exist the negative weight cycle
    // bool negcyc = false;
    for(int i = 0; i < this->rank; ++i)
    {
        if(dist[i * rank + i] < 0.0)
        {
            // negcyc = true;
            throw std::runtime_error("GRAPH CONTAINS NEGATIVE WEIGHT CYCLE.");
        }
    }

    return dist;
}

typename myDSALib::Linear::DynArray<Edge>
AdjMatrix::Prim() const
{
    myDSALib::Linear::DynArray<Edge> tree;
    // in MST vertex (visited vertex)
    myDSALib::Linear::DynArray<bool> visited(this->rank, false);

    if(!this->matrix || this->rank < 1)
    {
        return tree;
    }

    // QueueElement Wrapper
    using QueueElement = std::pair<Weight, Vertex>;
    class PairCompare
    {
        public:
            bool operator()(const QueueElement& p1, const QueueElement& p2) const
            {
                return p1.first < p2.first;
            }
    };
    // priority queue
    myDSALib::Heap::PriorityQueue<QueueElement, PairCompare> pqueue;
    // the minimum edge from index vertex
    myDSALib::Linear::DynArray<Weight> key_edge(this->rank, MAX_WEIGHT);
    // vertex's in_degree vertex
    myDSALib::Linear::DynArray<Vertex> parent(this->rank, -1);

    key_edge[0] = 0.0;
    pqueue.push({0.0, 0});

    while(!pqueue.empty())
    {
        QueueElement qelem = pqueue.pop();

        if(visited[qelem.second])
        {
            continue;
        }

        visited[qelem.second] = true;

        if(parent[qelem.second] != -1)
        {
            tree.push_back(Edge(parent[qelem.second], qelem.second, qelem.first));
        }

        for(int v = 0; v < this->rank; ++v)
        {
            Weight wei = Matrix(qelem.second, v);
            if(!visited[v] && wei != 0.0 && wei < key_edge[v])
            {
                key_edge[v] = wei;
                parent[v] = qelem.second;
                pqueue.push({wei, v});
            }
        }
    }

    return tree;
}

typename myDSALib::Linear::DynArray<Edge>
AdjMatrix::Kruskal() const
{
    myDSALib::Linear::DynArray<Edge> tree;

    if(!this->matrix || this->rank < 1)
    {
        return tree;
    }

    // get all the edges of this graph
    myDSALib::Linear::DynArray<Edge> edges = get_edges();
    class EdgeCompare
    {
        public:
            bool operator()(const Edge& e1, const Edge& e2) const
            {
                return e1.weight() < e2.weight();
            }
    };
    // the priority queue of the edges in this graph
    myDSALib::Heap::PriorityQueue<Edge, EdgeCompare> pqueue;

    for(int i = 0; i < edges.getSize(); ++i)
    {
        pqueue.push(edges[i]);
    }

    // the disjointSet to check if path cycled
    myDSALib::DisjointSet::DisjointSet set(this->rank);
    while(!pqueue.empty() && tree.getSize() < this->rank - 1)
    {
        Edge edge = pqueue.pop();
        // check if vertex is not connected
        if(!set.is_connected(edge.from(), edge.to()))
        {
            tree.push_back(edge);
            set.union_element(edge.from(), edge.to());
        }
    }

    // this graph is not connected
    if(tree.getSize() != this->rank - 1)
    {
        throw std::runtime_error("Graph is not connected.");
    }

    return tree;
}

typename myDSALib::Linear::DynArray<Vertex>
AdjMatrix::topological_sort() const
{
    myDSALib::Linear::DynArray<Vertex> sorted;

    if(!this->matrix || this->rank < 1)
    {
        return sorted;
    }

    myDSALib::Linear::DynArray<bool> visited(this->rank, false);
    // count in_degree vertex
    myDSALib::Linear::DynArray<Vertex> in_d_v(this->rank, 0);

    // enqueue the original 0 in_degree vertex
    myDSALib::Linear::Queue<Vertex> queue;
    for(int v = 0; v < this->rank; ++v)
    {
        in_d_v[v] = this->in_degree(v);
        if(in_d_v[v] == 0)
        {
            queue.push(v);
        }
    }

    while(!queue.empty())
    {
        Vertex vex = queue.front();
        queue.pop();
        if(visited[vex])
        {
            continue;
        }
        // record this vertex
        visited[vex] = true;
        // in topological sorted recorder
        sorted.push_back(vex);
        // from vex to i, find the next 0 in_degree vertex
        for(int i = 0; i < this->rank; ++i)
        {
            // has the edge vex -> i
            if(has_edge(vex, i))
            {
                --in_d_v[i];\
                // check the in_degree value
                if(in_d_v[i] == 0)
                {
                    queue.push(i);
                }
            }
            
        }
    }

    if(sorted.getSize() != this->rank)
    {
        throw std::runtime_error("Graph as a cycle, the topology can't be completed.");
    }

    return sorted;
}

std::pair<myDSALib::Linear::DynArray<Weight>, myDSALib::Linear::DynArray<Weight>>
AdjMatrix::AOE_net() const
{
    std::pair<myDSALib::Linear::DynArray<Weight>, myDSALib::Linear::DynArray<Weight>> ve_vl;
    if(!this->matrix || this->rank < 1)
    {
        return ve_vl;
    }
    // the topological sort
    myDSALib::Linear::DynArray<Vertex> topology = this->topological_sort();

    // vertex early
    myDSALib::Linear::DynArray<Weight> ve(this->rank, 0);
    // vertex last
    myDSALib::Linear::DynArray<Weight> vl;

    // compute the ve, find the earliest start time
    for(int u = 0; u < this->rank; ++u)
    {
        Vertex to = topology[u];
        for(int v = 0; v < u; ++v)
        {
            Vertex from = topology[v];
            if(has_edge(from, to))
            {
                Weight new_ve = ve[from] + Matrix(from, to);
                if(new_ve > ve[to])
                {
                    ve[to] = new_ve;
                }
            }
        }
    }

    Weight max_w = 0;
    for(int i = 0; i < ve.getSize(); ++i)
    {
        if(max_w < ve[i])
        {
            max_w = ve[i];
        }
    }

    // compute the vl, find the latest start time
    vl.resize(this->rank, max_w);
    for(int u = this->rank - 1; u >= 0; --u)
    {
        Vertex from = topology[u];
        for(int v = u + 1; v < this->rank; ++v)
        {
            Vertex to = topology[v];
            if(has_edge(from, to))
            {
                Weight new_vl = vl[to] - Matrix(from, to);
                if(new_vl < vl[from])
                {
                    vl[from] = new_vl;
                }
            }
        }
    }

    ve_vl.first = ve;
    ve_vl.second = vl;

    return ve_vl;
}

typename myDSALib::Linear::DynArray<Edge>
AdjMatrix::get_critical_path() const
{
    myDSALib::Linear::DynArray<Edge> path;
    if(!this->matrix || this->rank < 1)
    {
        return path;
    }

    std::pair<myDSALib::Linear::DynArray<Weight>, myDSALib::Linear::DynArray<Weight>> ve_vl = this->AOE_net();

    myDSALib::Linear::DynArray<Edge> edges = this->get_edges();

    for(int i = 0; i < this->rank; ++i)
    {
        Vertex u = edges[i].from();
        Vertex v = edges[i].to();
        Weight e = ve_vl.first[u];
        Weight l = ve_vl.second[v] - Matrix(u, v);

        if(e == l)
        {
            path.push_back(edges[i]);
        }
    }

    return path;
}

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