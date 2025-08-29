#pragma once

namespace myDSALib
{
namespace Linear
{

/**
 * 是一个为双链表设计的双向结点
 */

template<typename Ty>
class dNode
{
    using pdNode = dNode<Ty>*;
    friend class DouList<Ty>;
private:
    pdNode prev;
    pdNode next;
    Ty data;

    void setPrev(pdNode node) noexcept { prev = node; }

    void setNext(pdNode node) noexcept { next = node; }

    void setData(Ty d) noexcept { data = d; }
public:
    explicit dNode(const Ty d& = Ty{}) noexcept
        : data(d), prev(nullptr), next(nullptr) { }
    explicit dNode(const Ty& d, dNode prev_, dNode next_) noexcept
        : data(d), prev(prev_), next(next_) { }

    dNode Prev() const noexcept { return prev; }

    dNode Next() const noexcept { return next; }

    Ty& Data() noexcept { return data; }
};

}
}