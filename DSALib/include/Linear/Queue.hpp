#pragma once
#include <type_traits>
#include "Array/StaArray.hpp"
#include "Array/DynArray.hpp"

namespace myDSALib
{
namespace Linear
{

// Dynamic Queue
template<typename Ty>
class DynQueue
{
private:
    DynArray<Ty> queue;
public:
    DynQueue()
        : queue() { }

    ~DynQueue() = default;

    DynQueue(const DynQueue<Ty>&) = delete;
    DynQueue& operator=(const DynQueue<Ty>&) = delete;

    DynQueue(DynQueue<Ty>&& other) noexcept
        : queue(std::move(other.queue)) { }
    DynQueue& operator=(DynQueue<Ty>&& other) noexcept {
        if(this != &other) {
            this->queue = std::move(other.queue);
        }
        return *this;
    }

public:
    // empty
    const bool empty() const noexcept { return queue.empty(); }
    // full
    constexpr bool full() const noexcept { return false; }
    // size
    const size_t size() const noexcept { return queue.getSize(); }
    // capacity
    const size_t capacity() const noexcept { return queue.getCapacity(); }

    // get front elem
    Ty& front() {
        if(empty())
            throw std::out_of_range("queue empty");
        return queue.back();
    }
    // get front elem const.Ver.
    const Ty& front() const {
        if(empty())
            throw std::out_of_range("queue empty");
        return queue.back();
    }

    // get back elem
    Ty& back() {
        if(empty())
            throw std::out_of_range("queue empty");
        return queue.front();
    }
    // get back elem const.Ver.
    const Ty& back() const {
        if(empty())
            throw std::out_of_range("queue empty");
        return queue.front();
    }

    // push top
    bool push(const Ty& elem) {
        queue.push_front(elem);
        return true;
    }
    // emplace top
    bool push(Ty&& elem) {
        queue.emplace_front(std::forward<Ty>(elem));
        return true;
    }

    // pop back
    void pop() noexcept {
        if(empty())
            throw std::out_of_range("queue empty");
        queue.pop_back();
    }

    // swap
    void swap(DynQueue<Ty>& other) noexcept {
        this->queue.swap(other.queue);
    }

    // clear
    void clear() noexcept { queue.clear(); }

    // clone
    void clone(const DynQueue<Ty>& other) {
        clear();
        queue.clone(other.queue);
    }

};

// ===============================================

// Static Queue
template<typename Ty, size_t N>
class StaQueue
{
private:
    StaArray<Ty, N + 1> queue;
    size_t front_idx = 0;
    size_t back_idx = 0;
public:
    StaQueue()
        : queue(), front_idx(0), back_idx(0) { }

    ~StaQueue() = default;

    StaQueue(const StaQueue<Ty, N>&) = delete;
    StaQueue& operator=(const StaQueue<Ty, N>&) = delete;

    StaQueue(StaQueue<Ty, N>&& other) noexcept
        : queue(std::move(other.queue))
        {
            other.front_idx = 0;
            other.back_idx = 0;
        }
    StaQueue& operator=(StaQueue<Ty, N>&& other) noexcept {
        if(this != &other) {
            this->queue = std::move(other.queue);
            this->top_idx = other.top_idx;
            this->bot_idx = other.bot_idx;
            other.top_idx = 0;
            other.bot_idx = 0;
        }
        return *this;
    }

public:
    // empty
    const bool empty() const noexcept { return front_idx == back_idx; }
    // full
    const bool full() const noexcept { return (back_idx + 1) % queue.size() == front_idx; }
    // size
    const size_t size() const noexcept { return (back_idx - front_idx + queue.size()) % queue.size(); }
    // capacity
    constexpr size_t capacity() const noexcept { return N; }

    // front elem
    Ty& front() {
        if(empty())
            throw std::out_of_range("queue empty");
        return queue[front_idx];
    }
    // front elem const.Ver.
    const Ty& front() const {
        if(empty())
            throw std::out_of_range("queue empty");
        return queue[front_idx];
    }

    // back elem
    Ty& back() {
        if(empty())
            throw std::out_of_range("queue empty");
        return queue[(back_idx - 1 + queue.size()) % queue.size()];
    }
    // back elem const.Ver.
    const Ty& back() const {
        if(empty())
            throw std::out_of_range("queue empty");
        return queue[(back_idx - 1 + queue.size()) % queue.size()];
    }

    // push elem
    bool push(const Ty& elem) {
        if(full())
            throw std::out_of_range("queue full");
        queue[back_idx] = elem;
        back_idx = (back_idx + 1) % queue.size();
        return true;
    }
    // push elem const.Ver.
    bool push(Ty&& elem) {
        if(full())
            throw std::out_of_range("queue full");
        queue[back_idx] = std::move(elem);
        back_idx = (back_idx + 1) % queue.size();
        return true;
    }

    // pop
    void pop() {
        if(empty())
            throw std::out_of_range("queue empty");
        front_idx = (front_idx + 1) % queue.size();
    }

    // swap
    void swap(StaQueue<Ty, N>& other) noexcept {
        this->queue.swap(other.queue);
        std::swap(this->front_idx, other.front_idx);
        std::swap(this->back_idx, other.back_idx);
    }

    // clear
    void clear() noexcept {
        queue.clear();
        front_idx = 0;
        back_idx = 0;
    }

    // clone
    void clone(const StaQueue<Ty, N>& other) {
        clear();
        this->front_idx = other.front_idx;
        this->back_idx = other.back_idx;
        queue.clone(other.queue);
    }

public:
    static_assert(N > 0,
        "Queue's size N must be Positive Number");

};

// ===============================================

// Queue can be Dynamic or Static
template<typename Ty, size_t N = 0>
class Queue
    : public std::conditional_t<(N == 0), DynQueue<Ty>, StaQueue<Ty, N>>
{
private:

public:
    using Base = std::conditional_t<(N == 0), DynQueue<Ty>, StaQueue<Ty, N>>;

    using Base::Base;

    static_assert(N == 0 || N > 0,
        "Queue's size N must be Positive for Static or Zero for Dynamic");

};

}
}