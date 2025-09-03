#pragma once
#include <type_traits>
#include "Array/StaArray.hpp"
#include "Array/DynArray.hpp"

namespace myDSALib
{
namespace Linear
{

// Dynamic Stack
template<typename Ty>
class DynStack
{
private:
    DynArray<Ty> stack;
public:
    DynStack()
        : stack() { }

    ~DynStack() = default;

    DynStack(const DynStack<Ty>&) = delete;
    DynStack& operator=(const DynStack<Ty>&) = delete;

    DynStack(DynStack<Ty>&& other) noexcept
        : stack(std::move(other.stack)) { }
    DynStack& operator=(DynStack<Ty>&& other) noexcept {
        if(this != &other) {
            this->stack = std::move(other.stack);
        }
        return *this;
    }

public:
    // empty
    bool empty() const noexcept { return stack.empty(); }
    // full
    constexpr bool full() const noexcept { return false; }
    // size (note: the size will be changed by the operation)
    const size_t size() const noexcept { return stack.getSize(); }
    // capacity
    const size_t capacity() const noexcept { return stack.capacity(); }

    // get top elem
    Ty& top() {
        if(empty())
            throw std::out_of_range("stack empty");
        return stack.back();
    }
    // get top elem const.Ver.
    const Ty& top() const {
        if(empty())
            throw std::out_of_range("stack empty")
        return stack.back();
    }

    // push top
    bool push(const Ty& elem) {
        stack.push_back(elem);
        return true;
    }

    // emplace top
    bool push(Ty&& elem) {
        stack.emplace_back(std::forward<Ty>(elem));
        return true;
    }

    // pop top
    void pop() {
        if(empty())
            throw std::out_of_range("stack empty");
        stack.pop_back();
    }

    // swap
    void swap(DynStack<Ty>& other) noexcept {
        this->stack.swap(other.stack);
    }

};

// ================================================

// Static Stack
template<typename Ty, size_t N>
class StaStack
{
private:
    StaArray<Ty, N> stack;
    size_t top_idx = static_cast<size_t>(-1);
public:
    StaStack()
        : stack(), top_idx(static_cast<size_t>(-1)) { }

    ~StaStack() = default;

    StaStack(const StaStack<Ty, N>& other) = delete;
    StaStack& operator=(const StaStack<Ty, N>& other) = delete;

    StaStack(StaStack<Ty, N>&& other) noexcept
        : stack(std::move(other.stack)), top_idx(other.top_idx)
        {
            other.top_idx = static_cast<size_t>(-1);
        }
    StaStack& operator=(StaStack<Ty, N>&& other) noexcept {
        if(this != &other)
        {
            this->stack = std::move(other.stack);
            this->top = other.top_idx;
            other.top_idx = static_cast<size_t>(-1);
        }
        return *this;
    }

public:
    // empty
    bool empty() const noexcept { return top_idx == static_cast<size_t>(-1); }
    // full
    constexpr bool full() const noexcept { return top_idx == N - 1; }
    // size
    size_t size() const noexcept { return empty() ? 0 : top_idx + 1;}
    // capacity
    constexpr size_t capacity() const noexcept { return N; }

    // get top elem
    Ty& top() {
        if(empty())
            throw std::out_of_range("stack empty");
        return stack[top_idx];
    }
    // get top elem const.Ver.
    const Ty& top() const {
        if(empty())
            throw std::out_of_range("stack empty");
        return stack.at(top_idx);
    }

    // push top
    bool push(const Ty& elem) {
        if(full())
            throw std::out_of_range("stack full");
        ++top_idx;
        stack[top_idx] = elem;
        return true;
    }

    // emplace top
    bool push(Ty&& elem) {
        if(full())
            throw std::out_of_range("stack full");
        ++top_idx;
        stack[top_idx] = std::move(elem);
        return true;
    }

    // pop top
    void pop() {
        if(empty())
            throw std::out_of_range("stack empty");
        --top_idx;
    }

    // swap
    void swap(StaStack& other) noexcept {
        this->stack.swap(other.stack);
        std::swap(this->top_idx,other.top_idx);
    }

};

// ===============================================

// Stack can be Dynamic or Static
template<typename Ty, size_t N = 0>
class Stack
    : public std::conditional_t<(N == 0), DynStack<Ty>, StaStack<Ty, N>>
{
private:

public:
    using Base = std::conditional_t<(N == 0), DynStack<Ty>, StaStack<Ty, N>>;

    using Base::Base;

    static_assert(N == 0 || N > 0,
        "Stack's size N must be Positive for Static or Zero for Dynamic");

};

}
}
