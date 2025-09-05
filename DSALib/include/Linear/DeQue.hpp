#pragma once
#include "Array/DynArray.hpp"

namespace myDSALib
{
namespace Linear
{

template<typename Ty>
class DeQue
{
private:
    DynArray<Ty> head;      // b: head |<b==========f
    DynArray<Ty> tail;      // f==========b>| tail :b
public:
    DeQue()
        : head(), tail() { }

    ~DeQue() = default;

    DeQue(const DeQue<Ty>&) = delete;
    DeQue& operator=(const DeQue<Ty>&) = delete;

    DeQue(DeQue<Ty>&& other) noexcept
        : head(std::move(other.head)), tail(std::move(other.tail)) { }
    DeQue operator=(DeQue<Ty>&& other) {
        if(this != other) {
            this->head = std::move(other.head);
            this->tail = std::move(other.tail);
        }
        return *this;
    }

public:
    // empty
    const bool empty() const noexcept { return head.empty() && tail.empty(); }
    // full
    constexpr bool full() const noexcept { return false; }
    // size
    const size_t size() const noexcept { return head.getSize() + tail.getSize(); }
    // capacity
    const size_t capacity() const noexcept { return head.getCapacity() + tail.getCapacity(); }

    // get front elem
    Ty& front() noexcept { return head.back(); }
    // get front elem const.Ver.
    const Ty& front() const noexcept { return head.back(); }

    // get back elem
    Ty& back() noexcept { return tail.back(); }
    // get back elem const.Ver.
    const Ty& back() const noexcept { return tail.back(); }

    // push front
    bool push_front(const Ty& elem) {
        head.push_back(elem);
        rebalance();
        return true;
    }
    // emplace front
    bool push_front(Ty&& elem) {
        head.push_back(std::forward<Ty>(elem));
        rebalance();
        return true;
    }
    // push back
    bool push_back(const Ty& elem) {
        tail.push_back(elem);
        rebalance();
        return true;
    }
    // emplace back
    bool push_back(Ty&& elem) {
        tail.push_back(std::forward<Ty>(elem));
        rebalance();
        return true;
    }

    // pop front
    void pop_front() noexcept {
        if(empty())
            throw std::out_of_range("deque empty()");
        if(head.empty())
            tail.pop_front();
        else
            head.pop_back();
        shrink();
    }

    // pop back
    void pop_back() noexcept {
        if(empty())
            throw std::out_of_range("deque empty()");
        if(tail.empty())
            head.pop_front();
        else
            tail.pop_back();
        shrink();
    }

    void swap(DeQue<Ty>& other) noexcept {
        this->head.swap(other.head);
        this->tail.swap(other.tail);
    }

    void clear() noexcept {
        head.clear();
        tail.clear();
    }

public:

    // rebalance the DeQue
    void rebalance() {
        if(head.getSize() == tail.getSize()) {
            size_t sub = capacity() - size();
            if((sub << 2) < capacity()) {
                head.reserve(head.size() * 2);
                tail.reserve(tail.size() * 2);
            }
            return;
        }
        else if()

        DynArray<Ty>* out;
        DynArray<Ty>* get;
        if(head.getSize() > tail.getCapacity()) {
            out = &head;
            get = &tail;
        }
        else {
            out = &tail;
            get = &head;
        }

        while(out->getSize() - get->getSize() > 1) {
            get->push_front(std::move(out->front()));
            out->pop_front();
        }
    }

    // shrink to fit
    void shrink_to_fit() {
        rebalance();
        head.shrink_to_fit();
        tail.shrink_to_fit();
    }

    // shrink the large capacity
    void shrink() {
        rebalance();
        size_t sub = capacity() - size();
        if((sub << 1) > capacity()) {
            head.reserve(head.getSize() + (sub >> 2));
            tail.reserve(tail.getSize() + (sub >> 2));
        }
    }

    // reserve
    void reserve(size_t new_capacity) {
        if(capacity() > new_capacity)
            return;
        rebalance();
        head.reserve(new_capacity >> 1);
        tail.reserve(new_capacity >> 1);
    }

};

}
}