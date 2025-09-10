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
    unique_psNode next;     // 后继结点 next node
    Ty data;        // 数据元素 data

    void setNext(unique_psNode& node) noexcept { next = std::move(node); }

    void setData(const Ty& d) noexcept { data = d; }
    void setData(Ty&& d) noexcept { data = std::move(d); }
public:
    explicit sNode(const Ty& d = Ty{}) noexcept
        : data(d), next(nullptr) { }
    explicit sNode(const Ty& d, unique_psNode next_node) noexcept
        : data(d), next(std::move(next_node)) { }

    template<typename... Args>
    explicit sNode(Args&&... args) noexcept
        : data(std::forward<Args>(args)...), next(nullptr) { }

    explicit sNode(sNode&& other) noexcept
        : data(std::move(data)), next(std::move(other.next)) { }
    sNode& operator=(sNode&& other) {
        this->next = std::move(other.next);
        this->data = std::move(other.data);
    }

    ~sNode() = default;
    sNode(const sNode&) = delete;
    sNode& operator=(const sNode&) = delete;

public:
    psNode Next() const noexcept { return next.get(); }

    Ty& Data() noexcept { return data; }
    const Ty& Data() const noexcept { return data; }

    bool hasNext() const noexcept { return next != nullptr; }
};

// ==============================================

template<typename Ty, typename... Args>
std::unique_ptr<sNode<Ty>> makeSigNode(Args&&... args) {
    return std::unique_ptr<sNode<Ty>>(new sNode<Ty>(std::forward<Args>(args)...));
}

}
}