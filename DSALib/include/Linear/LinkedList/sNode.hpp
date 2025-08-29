#pragma once
#include <cstddef>
#include <exception>
#include <utility>
#include <memory>

namespace myDSALib
{
namespace Linear
{

/**
 * 是一个为单链表设计的单向结点
 */

template<typename Ty>
class sNode
{
    using psNode = sNode<Ty>*;
    friend class SigList<Ty>;
private:
    psNode next;
    Ty data;

    void setNext(psNode node) noexcept { next = node; }

    void setData(Ty d) noexcept { data = d}
public:
    explicit sNode(const Ty& d = Ty{}) noexcept
        : data(d), next(nullptr) { }
    explicit sNode(const Ty& d; psNode next_node) noexcept
        : data(d), next(next_node) { }

    psNode Next() const noexcept { return next; }

    Ty& Data() noexcept { return data; }
};

}
}