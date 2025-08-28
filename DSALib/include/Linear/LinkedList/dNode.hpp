#pragma once
#include <cstddef>
#include <exception>
#include <utility>

namespace myDSALib
{
namespace Linear
{

/**
 * 是一个为双链表设计的双向结点
 * 使用 auto node = makeDouNode() 函数进行实例化
 */

template<typename Ty>
class dNode
{
    using pdNode = dNode<Ty>*;
    using unique_pdNode = std::unique_ptr<dNode<Ty>>;
private:
    std::unique_ptr<dNode<Ty>> next;        // 前驱节点
    std::unique_ptr<dNode<Ty>> prev;        // 后继节点
    Ty data;        // 数据元素

    // step步长后的结点
    pdNode prstepNext(size_t step) {
        pdNode temp = this;
        for(size_t i = 0; i < step && temp->hasNext(); ++i) {
            temp = temp->Next();
        }
        return temp;
    }
    // step步长后的结点const.Ver.
    const pdNode prstepNext(size_t step) const {
        const pdNode temp = this;
        for(size_t i = 0; i < step && temp->hasNext(); ++i) {
            temp = temp->Next();
        }
        return temp;
    }
    // step步长前的结点
    pdNode prstepPrev(size_t step) {
        pdNode temp = this;
        for(size_t i = 0; i < step && temp->hasPrev(); ++i) {
            temp = temp->Prev();
        }
        return temp;
    }
    // step步长前的结点cosnt.Ver.
    const pdNode prstepPrev(size_t step) const {
        const pdNode temp = this;
        for(size_t i = 0; i < step && temp->hasPrev(); ++i) {
            temp = temp->Prev();
        }
        return temp;
    }

public:
    explicit dNode(const Ty &d = Ty{}) noexcept
        : data(d), next(nullptr), prev(nullptr) { }
    explicit dNode(const Ty &d, pdNode prev_node, pdNode next_node) noexcept
        : data(d), prev(prev_node), next(next_node) { }
    explicit dNode(const Ty &d, unique_pdNode prev_node, unique_pdNode next_node) noexcept
        : data(d), prev(std::move(prev_node)), next(std::move(next)) { }

    // 移动构造
    explicit dNode(dNode&& other) noexcept
        : data(std::move(d)), prev(std::move(other.prev)), next(std::move(other.next)) { }
    // 移动赋值
    dNode& operator=(dNode&& other) noexcept {
        if(this != &other) {
            this->data = std::move(other.data);
            this->prev = std::move(other.prev);
            this->next = std::move(other.next);
        }
        return *this;
    }

    ~dNode() = default;

    // 删除拷贝构造
    dNode(const dNode&) = delete;
    // 删除赋值构造
    dNode& operator=(const dNode&) = delete;

public:
    // 后继结点地址const.Ver.
    const pdNode Next() const noexcept { return next ? next.get() : nullptr; }
    // 后继结点地址
    pdNode Next() noexcept { return next ? next.get() : nullptr; }
    // 前驱结点地址const.Ver.
    const pdNode Prev() const noexcept { return prev ? next.get() : nullptr; }
    // 前驱结点地址
    pdNode Prev() noexcept { return prev ? prev.get() : nullptr; }

    // step步长后结点
    pdNode stepNext(size_t step) noexcept {
        return prstepNext(step);
    }
    // step步长后结点const.Ver.
    const pdNode stepNext(size_t step) const noexcept {
        return prstepNext(step);
    }
    // step步长前结点
    pdNode stepPrev(size_t step) noexcept {
        return prstepPrev(step);
    }
    // step步长前结点const.Ver.
    pdNode stepPrev(size_t step) const noexcept {
        return prstepPrev(step);
    }

    // 从unique_ptr设置前驱结点
    void setPrev(unique_pdNode& node) noexcept { prev = std::move(node); }
    // 从原始指针设置前驱结点
    void setPrev(pdNode node) noexcept { prev.reset(node); }
    // 从unique_ptr设置后继结点
    void setNext(unique_pdNode& node) noexcept { next = std::move(node); }
    // 从原始指针设置后继结点
    void setNext(pdNode node) noexcept { next.reset(node); }

    // 释放后继结点
    unique_pdNode releaseNext() noexcept { return next.release(); }
    // 释放前驱结点
    unique_pdNode releasePrev() noexcept { return prev.release(); }

    // 设置数据元素
    void setData(const Ty& d) noexcept { data = d; }
    // 获得数据元素const.Ver.
    const Ty& getData() const noexcept { return data; }
    // 获得数据元素
    Ty& getData() noexcept { return data; }

    // 向后插入一个结点
    void insertNext(unique_pdNode& node) noexcept {
        if(!node)
            return;
        node->prev.reset(this);
        node->next = std::move(next);
        if(this->next)
            this->next->prev = std::move(node);
        this->next = std::move(node);
    }

    // 移除下一个结点
    unique_pdNode removeNext() {
        if(!this->next)
            return nullptr;
        unique_pdNode removed = std::move(next);
        if(removed->next)
            removed->next->prev.reset(this);
        this->next = std::move(removed->next);

        return removed;
    }

    // 向前插入一个结点
    void insertPrev(unique_pdNode& node) {
        if(!node)
            return;
        node->next.reset(this);
        node->prev = std::move(prev);
        if(this->prev)
            this->prev->next = std::move(node);
        this->prev = std::move(node);
    }

    // 移除下一个结点
    unique_pdNode removePrev() {
        if(!this->prev)
            return nullptr;
        unique_pdNode removed = std::move(prev);
        if(removed->prev)
            removed->prev->next.reset(this);
        this->prev = std::move(removed->prev);

        return removed;
    }

public:
    // 是否有后继
    bool hasNext() const noexcept { return next.get() != nullptr; }
    // 是否有前驱
    bool hasPrev() const noexcept { return prev.get() != nullptr; }

};

template<typename Ty, typename... Args>
std::unique_ptr<dNode<Ty>> makeDouNode(Args&&... args) {
    return std::unique_ptr<dNode<Ty>>(new dNode<Ty>(std::forward<Args>(args)...));
}
template<typename Ty>
std::unique_ptr<dNode<Ty>> makeDouNode(dNode<Ty>& node) {
    return std::unique_ptr<dNode<Ty>>(&node);
}
template<typename Ty>
std::unique_ptr<dNode<Ty>> makeDouNode(dNode<Ty>* node) {
    return std::unique_ptr<dNode<Ty>>(node);
}

}
}