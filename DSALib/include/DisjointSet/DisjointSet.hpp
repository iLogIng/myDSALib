#pragma once

#include "../Linear/Array/DynArray.hpp"

namespace myDSALib
{
namespace DisjointSet
{

class DisjointSet
{
private:
    myDSALib::Linear::DynArray<int> parent;
    myDSALib::Linear::DynArray<int> rank;
    int count; // count the connection

public:
    explicit DisjointSet(int n)
    {
        this->count = n;
        this->parent.resize(n);
        this->rank.resize(n, 0);

        for(int i = 0; i < n; ++i)
        {
            this->parent[i] = i;
        }
    }

    DisjointSet(const DisjointSet&) = delete;
    DisjointSet& operator=(const DisjointSet&) = delete;

    DisjointSet(DisjointSet&& other) noexcept
    {
        this->parent = std::move(other.parent);
        this->rank = std::move(other.rank);
        this->count = other.count;
        other.count = 0;
    }

    DisjointSet& operator=(DisjointSet&& other) noexcept
    {
        if(&other != this)
        {
            this->parent = std::move(other.parent);
            this->rank = std::move(this->rank);
            this->count = other.count;
            other.count = 0;
        }
        return *this;
    }

    ~DisjointSet() = default;

public:
    int find(int p)
    {
        if(p != parent[p])
        {
            parent[p] = find(parent[p]);
        }
        return parent[p];
    }

    bool is_connected(int p, int q)
    {
        return find(p) == find(q);
    }

    void union_element(int p, int q)
    {
        int rootP = find(parent[p]);
        int rootQ = find(parent[q]);

        if(rootP == rootQ)
        {
            return;
        }

        if(rank[rootP] < rank[rootQ])
        {
            parent[rootP] = rootQ;
        }
        else if(rank[rootP] > rank[rootP])
        {
            parent[rootQ] = rootP;
        }
        else
        {
            parent[rootQ] = rootP;
            ++rank[rootP];
        }

        --count;
    }

    int getCount() const
    {
        return count;
    }

    void clear()
    {
        int n = parent.getSize();
        parent.clear();
        rank.clear();

        count = n;
        parent.resize(n);
        rank.resize(n, 0);

        for(int i = 0; i < n; ++i)
        {
            parent[i] = i;
        }
    }
};

} // namespace DisjointSet
} // namespace myDSALib