#pragma once

#include <memory.h>

#include "../Linear/Array/DynArray.hpp"

namespace myDSALib
{
namespace Graph
{

using Vertex = int;

using Weight = double;

using Path = myDSALib::Linear::DynArray<Vertex>;

class Edge
{
private:
    Vertex from_;       // Vertex from
    Vertex to_;         // Vertex to
    Weight weight_;     // Edge weight
public:
    explicit Edge(Vertex v1, Vertex v2, Weight w = 1.0) noexcept
        : from_(v1), to_(v2), weight_(w) { }

    Edge(const Edge&) = delete;
    Edge& operator=(const Edge&) = delete;

    explicit Edge(Edge&& other) noexcept
    {
        this->from_ = other.from_;
        this->to_ = other.to_;
        this->weight_ = other.weight_;

        other.from_ = 0;
        other.to_ = 0;
        other.weight_ = 0.0;
    }
    Edge& operator=(Edge&& other) noexcept
    {
        if(&other != this)
        {
            this->from_ = other.from_;
            this->to_ = other.to_;
            this->weight_ = other.weight_;

            other.from_ = 0;
            other.to_ = 0;
            other.weight_ = 0;
        }

        return *this;
    }

    ~Edge() = default;

public:
    bool operator==(const Edge& other) noexcept
    {
        return this->from_ == other.from_ && this->to_ == other.to_ && this->weight_ == other.weight_;
    }
    bool operator!=(const Edge& other) noexcept
    {
        return !(*this == other);
    }

public:
    // set vertex from
    void set_vertex_from(Vertex from) noexcept {
        this->from_ = from;
    }
    // Vertex from
    Vertex from() noexcept {
        return this->from_;
    }
    // Vertex from const.Ver
    const Vertex from() const noexcept {
        return this->from_;
    }

    // set Vertex to
    void set_vertex_to(Vertex to) noexcept {
        this->to_ = to;
    }
    // Vertex to
    Vertex to() noexcept {
        return this->to_;
    }
    // Vertex to const.Ver
    const Vertex to() const noexcept {
        return this->to_;
    }

    // set Edge Weight
    void set_edge_weight(Weight weight) noexcept {
        this->weight_ = weight;
    }
    // Edge Weight
    Weight weight() noexcept {
        return this->weight_;
    }
    // Edge Weight
    const Weight weight() const noexcept {
        return this->weight_;
    }

    // reset the edge
    void reset_edge(Vertex from, Vertex to, Weight weight) noexcept {
        this->from_ = from;
        this->to_ = to;
        this->weight_ = weight;
    }

};

class IGraph
{
public:
    virtual ~IGraph() = default;

public:
    // add the vertex
    virtual bool add_vertex(Vertex) = 0;
    // remove the vertex
    virtual bool remove_vertex(Vertex) = 0;
    // has vertex
    virtual bool has_vertex(Vertex) = 0;
    // get Vertexs
    virtual myDSALib::Linear::DynArray<Vertex> get_vertexs() const = 0;
    // count the vertexs
    virtual size_t count_vertex() = 0;

    // add the edge
    virtual bool add_edge(const Edge&) = 0;
    // remove the edge
    virtual bool remove_edge(Vertex from, Vertex to) = 0;
    // has edge
    virtual bool has_edge(Vertex from, Vertex to) = 0;
    // edge's weight
    virtual Weight edge_weight(Vertex from, Vertex to) = 0;
    // get Edges
    virtual myDSALib::Linear::DynArray<Edge> get_edges() const = 0;
    // count edges
    virtual size_t count_edge() = 0;

    // get neighbor vertexs
    virtual myDSALib::Linear::DynArray<Vertex> get_neighbor_vertexs(Vertex v) const = 0;
    // get incident edges
    virtual myDSALib::Linear::DynArray<Edge> get_incident_edges(Vertex v) const = 0;
    // get outgoing edges
    virtual myDSALib::Linear::DynArray<Edge> get_outgoing_deges(Vertex v) const = 0;
    // get vertex's degree
    virtual int degree(Vertex v) = 0;
    // get vertex's in degree
    virtual int in_degree(Vertex v) = 0;
    // get vertex's out degree
    virtual int out_degree(Vertex v) = 0;

    // clear the graph
    virtual void clear() = 0;

};

}   // namespace Graph
}   // namespace myDSALib

