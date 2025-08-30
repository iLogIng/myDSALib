#pragma once
#include <memory>
#include <utility>

namespace myDSALib
{
namespace Linear
{

/**
 * 是一个为双链表设计的双向结点
 */

template<typename Ty>
class DouList;

template<typename Ty>
class dNode
{
    using pdNode = dNode<Ty>*;
    using unique_pdNode = std::unique_ptr<dNode<Ty>>;
    friend class DouList<Ty>;
private:
    pdNode prev;        // 上一个结点 prev node
    unique_pdNode next;     // 下一个节点 next node
    Ty data;        // 数据元素 data

    void setPrev(unique_pdNode& node) noexcept { prev = node.release(); }

    void setNext(unique_pdNode& node) noexcept { next = std::move(node); }

    void setData(const Ty& d) noexcept { data = d; }
    void setData(Ty&& d) noexcept { data = std::move(d); }
public:
    explicit dNode(const Ty& d = Ty{}) noexcept
        : data(d), prev(nullptr), next(nullptr) { }
    explicit dNode(const Ty& d, unique_pdNode& pnode, unique_pdNode& nnode) noexcept
        : data(d), prev(pnode.release()), next(std::move(nnode)) { }

    template<typename... Args>
    explicit dNode(Args&&...args) noexcept
        : data(std::forward<Args>(args)...), prev(nullptr), next(nullptr) { }

    explicit dNode(dNode&& other) noexcept
        : data(std::move(other.data)), prev(other.prev), next(std::move(other.next)) { }

    ~dNode() = default;

    dNode& operator=(dNode&&) = delete;
    dNode(const dNode&) = delete;
    dNode& operator=(const dNode&) = delete;

public:
    pdNode Prev() const noexcept { return prev; }

    pdNode Next() const noexcept { return next.get(); }

    Ty& Data() noexcept { return data; }
    const Ty& Data() const noexcept { return data; }

    bool hasPrev() const noexcept { return prev != nullptr; }

    bool hasNext() const noexcept { return next != nullptr; }
};

}
}