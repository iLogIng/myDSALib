#pragma once
#include <memory>

namespace myDSALib
{
namespace Linear
{

/**
 * 是一个为单链表设计的单向结点
 */

template<typename Ty>
class SigList;

template<typename Ty>
class sNode
{
    using psNode = sNode<Ty>*;
    using unique_psNode = std::unique_ptr<sNode<Ty>>;
    friend class SigList<Ty>;
private:
    unique_psNode next;
    Ty data;

    void setNext(unique_psNode& node) noexcept { next = std::move(node); }

    void setData(Ty d) noexcept { data = d; }
public:
    explicit sNode(const Ty& d = Ty{}) noexcept
        : data(d), next(nullptr) { }
    explicit sNode(const Ty& d, unique_psNode next_node) noexcept
        : data(d), next(std::move(next_node)) { }
    template<typename... Args>
    explicit sNode(Args&&... args) noexcept
        : data(std::forward<Args>(args)...), next(nullptr) { }

    psNode Next() const noexcept { return next.get(); }

    Ty& Data() noexcept { return data; }

    bool hasNext() const noexcept { return next != nullptr; }
};

}
}