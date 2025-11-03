#pragma once

#include "IHeap.hpp"
#include "../Linear/Array/DynArray.hpp"

namespace myDSALib
{
namespace Heap
{

template<typename Ty, typename Compare = std::less<Ty>>
class BinaryHeap
    : public IHeap<Ty>
{
    using Container = myDSALib::Linear::DynArray<Ty>;
private:
    Container buffer;
    Compare comp;
public:
    explicit BinaryHeap() = default;
    explicit BinaryHeap(const Compare& compare)
        : comp(compare) { }
    explicit BinaryHeap(const Container& container)
        : buffer(container)
    {
        heapify(container);
    }

    BinaryHeap(const BinaryHeap&) = delete;
    BinaryHeap& operator=(const BinaryHeap&) = delete;

    BinaryHeap(BinaryHeap&& other)
    {
        this->buffer = std::move(other.buffer);
        this->comp = std::move(other.comp);
    }
    BinaryHeap& operator=(BinaryHeap&& other)
    {
        if(this == &other)
        {
            this->buffer = std::move(other.buffer);
            this->comp = std::move(other.comp);
        }

        return *this;
    }

    ~BinaryHeap() = default;

private:
    // parent node index
    const size_t parent(size_t index) const;
    // left node index
    const size_t left(size_t index) const;
    // right node index
    const size_t right(size_t index) const;

    // shift up
    virtual void shift_up(size_t index) override;
    // shift down
    virtual void shift_down(size_t index) override;

public:
    // push
    void push(const Ty& elem) override;
    void push(Ty&& elem) override;
    // top
    const Ty& top() const override;
    // pop
    Ty pop() override;

    // empty
    bool empty() const override;
    // size
    size_t size() const override;
    // clear
    void clear() override;

    // array to heap
    void heapify(const Container& buffer) override;
    // merge heap
    void merge(IHeap<Ty>& other) override;

    // heap to array
    Container to_array() const override;
    // check the attribute
    bool validate() const override;

};

template<typename Ty, typename Compare>
const size_t BinaryHeap<Ty, Compare>::parent(size_t index) const
{
    return (index - 1) / 2;
}

template<typename Ty, typename Compare>
const size_t BinaryHeap<Ty, Compare>::left(size_t index) const
{
    return index * 2 + 1;
}

template<typename Ty, typename Compare>
const size_t BinaryHeap<Ty, Compare>::right(size_t index) const
{
    return index * 2 + 2;
}

template<typename Ty, typename Compare>
void BinaryHeap<Ty, Compare>::shift_up(size_t index)
{
    if(index == 0)
    {
        return;
    }

    size_t parent_idx = parent(index);

    while(index > 0 && this->comp(this->buffer[index], this->buffer[parent_idx]))
    {
        std::swap(this->buffer[index], this->buffer[parent_idx]);
        index = parent_idx;
        parent_idx = parent(parent_idx);
    }
}

template<typename Ty, typename Compare>
void BinaryHeap<Ty, Compare>::shift_down(size_t index)
{
    size_t heap_size = this->buffer.getSize();
    while(left(index) < heap_size)
    {
        size_t child = left(index);
        if(child + 1 < heap_size && this->comp(this->buffer[child + 1], this->buffer[child]))
        {
            ++child;
        }
        if(!this->comp(this->buffer[child], this->buffer[index]))
        {
            break;
        }
        std::swap(this->buffer[index], this->buffer[child]);
        index = child;
    }
}

template<typename Ty, typename Compare>
void BinaryHeap<Ty, Compare>::push(const Ty& elem)
{
    this->buffer.push_back(elem);
    shift_up(this->buffer.getSize() - 1);
}

template<typename Ty, typename Compare>
void BinaryHeap<Ty, Compare>::push(Ty&& elem)
{
    this->buffer.push_back(std::forward<Ty>(elem));
    shift_up(this->buffer.getSize() - 1);
}

template<typename Ty, typename Compare>
const Ty& BinaryHeap<Ty, Compare>::top() const
{
    return this->buffer.front();
}

template<typename Ty, typename Compare>
Ty BinaryHeap<Ty, Compare>::pop()
{
    if(empty())
    {
        throw std::runtime_error("Out of range.\n");
    }

    Ty root = this->buffer.front();
    this->buffer[0] = this->buffer.back();
    this->buffer.pop_back();

    if(!empty())
    {
        shift_down(0);
    }

    return root;
}

template<typename Ty, typename Compare>
bool BinaryHeap<Ty, Compare>::empty() const
{
    return this->buffer.empty();
}

template<typename Ty, typename Compare>
size_t BinaryHeap<Ty, Compare>::size() const
{
    return this->buffer.getSize();
}

template<typename Ty, typename Compare>
void BinaryHeap<Ty, Compare>::heapify(const Container& array)
{
    this->buffer = array;

    if(this->buffer.empty())
    {
        return;
    }

    for(int i = (this->buffer.getSize() - 2) / 2; i >= 0; --i)
    {
        shift_down(i);
    }
}

template<typename Ty, typename Compare>
bool BinaryHeap<Ty, Compare>::validate() const
{
    for(size_t i = 0; i < this->buffer.getSize(); ++i)
    {
        size_t lch = this->left(i);
        size_t rch = this->right(i);

        if(lch < this->buffer.getSize() && this->comp(this->buffer[lch], this->buffer[i]))
        {
            return false;
        }
        if(rch < this->buffer.getSize() && this->comp(this->buffer[rch], this->buffer[i]))
        {
            return false;
        }
    }

    return true;
}

template<typename Ty, typename Compare>
void BinaryHeap<Ty, Compare>::clear()
{
    this->buffer.clear();
}

template<typename Ty, typename Compare>
void BinaryHeap<Ty, Compare>::merge(IHeap<Ty>& other)
{
    while(!other.empty())
    {
        this->push(other.pop());
    }
}

template<typename Ty, typename Compare>
typename BinaryHeap<Ty, Compare>::Container BinaryHeap<Ty, Compare>::to_array() const
{
    return this->buffer;
}

// Specializing ============================================================

// Max Top Heap
template<typename Ty, typename Compare>
using MaxHeap = BinaryHeap<Ty, std::greater<Ty>>;

// Min Top Heap
template<typename Ty, typename Compare>
using MinHeap = BinaryHeap<Ty, std::less<Ty>>;

} // namespace Heap
} // namespace myDSALib
