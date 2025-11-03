#pragma once

#include "BinaryHeap.hpp"

namespace myDSALib
{
namespace Heap
{

template<typename Ty, typename Compare>
class PriorityQueue
{
private:
    BinaryHeap<Ty, Compare> heap;

public:
    explicit PriorityQueue() = default;
    explicit PriorityQueue(const Compare& comp)
        : heap(comp) { }
    explicit PriorityQueue(const myDSALib::Linear::DynArray<Ty>& container)
        : heap(container) { }

    PriorityQueue(const PriorityQueue&) = delete;
    PriorityQueue& operator=(const PriorityQueue&) = delete;

    PriorityQueue(PriorityQueue&& other)
    {
        this->heap = std::move(other);
    }
    PriorityQueue& operator=(PriorityQueue&& other)
    {
        if(this == &other)
        {
            this->heap = std::move(other);
        }

        return *this;
    }

    ~PriorityQueue() = default;

public:
    // push
    void push(const Ty& elem)
    {
        heap.push(elem);
    }
    // push
    void push(Ty&& elem)
    {
        heap.push(std::forward<Ty>(elem));
    }
    // top
    const Ty& top() const
    {
        return heap.top();
    }
    // pop
    Ty pop()
    {
        return heap.pop();
    }

    // empty
    bool empty() const
    {
        return heap.empty();
    }
    // size
    size_t size() const
    {
        return heap.size();
    }
    // clear
    void clear()
    {
        heap.clear();
    }

};

} // namespace Heap
} // namespace myDSALib
