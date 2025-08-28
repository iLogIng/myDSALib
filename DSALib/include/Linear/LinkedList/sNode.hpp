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
 * 使用 auto node = makeSigNode() 函数进行实例化
 */

template<typename Ty>
class sNode
{
    using psNode = sNode<Ty>*;      // 指向该结点的指针
    using unique_psNode = std::unique_ptr<sNode<Ty>>;       // 指向该结点的unique_ptr
private:
    unique_psNode next;     // 指向下一个结点
    Ty data;        // 存储的数据元素

    // step步长后的结点
    psNode prstepNext(size_t step) {
        psNode temp = this;
        for(size_t i = 0; i < step && temp->hasNext(); ++i) {
            temp = temp->Next();
        }
        return temp;        // 若超过了结点接下来的长度，则返回末结点
    }
    // step步长后的结点const.Ver.
    const psNode prstepNext(size_t step) const {
        const psNode temp = this;
        for(size_t i = 0; i < step && temp->hasHext(); ++i) {
            temp = temp->Next();
        }
        return temp;
    }
public:
    explicit sNode(const Ty &d = Ty{}) noexcept
        : data(d), next(nullptr) { }
    explicit sNode(const Ty &d, const psNode next_node) noexcept
        : data(d), next(std::move(next_node)) { }
    explicit sNode(const Ty &d, unique_psNode next_node) noexcept
        : data(d), next(std::move(next_node)) { }

    // 移动构造
    explicit sNode(sNode&& other) noexcept
        : data(std::move(other.data)), next(std::move(other.next)) { }
    // 移动赋值
    sNode& operator=(sNode&& other) noexcept {
        if(this != &other) {
            this->data = std::move(other.data);
            this->next = std::move(other.next);
        }
        return *this;
    }

    ~sNode() = default;

    // 删除拷贝构造
    sNode(const sNode&) = delete;
    // 删除拷贝赋值
    sNode& operator=(const sNode&) = delete;

public:

    // 后继结点地址const.Ver.
    const psNode Next() const noexcept { return next ? next.get() : nullptr; }
    // 后继结点地址
    psNode Next() noexcept { return next ? next.get() : nullptr; }

    // step步长后的结点
    psNode stepNext(size_t step) noexcept {
        return prstepNext(step);
    }
    // step步长后的结点const.Ver.
    const psNode stepNext(size_t step) const noexcept {
        return prstepNext(step);
    }

    // 从unique_ptr设置后继结点
    void setNext(unique_psNode& node) noexcept { next = std::move(node); }
    // 从原始指针设置后继结点
    void setNext(psNode node) noexcept { next.reset(node); }

    // 释放后继结点
    unique_psNode releaseNext() noexcept { return std::move(next); }

    // 设定数据元素
    void setData(const Ty& d) noexcept { data = d; }
    // 获得数据元素const.Ver.
    const Ty& getData() const noexcept { return data; }
    // 获得数据元素
    Ty& getData() noexcept { return data; }

    // 向后插入一个结点
    void insertNext(unique_psNode& node) noexcept {
        if(!node)
            return;
        node->next = std::move(next);
        next = std::move(node);
    }

    // 移除下一个结点，返回一个指向removed结点的unique_ptr
    unique_psNode removeNext() noexcept {
        if(!this->next)
            return nullptr;
        unique_psNode removed = std::move(next);
        next = std::move(removed->next);
        return removed;
    }

public:
    // 是否存在下一个结点
    bool hasNext() const noexcept { return next != nullptr; }

};

template<typename Ty, typename... Args>
std::unique_ptr<sNode<Ty>> makeSigNode(Args&&... args) {
    return std::unique_ptr<sNode<Ty>>(new sNode<Ty>(std::forward<Args>(args)...));
}
template<typename Ty>
std::unique_ptr<sNode<Ty>> makeSigNode(sNode<Ty>& node) {
    return std::unique_ptr<sNode<Ty>>(&node);
}
template<typename Ty>
std::unique_ptr<sNode<Ty>> makeSigNode(sNode<Ty>* node) {
    return std::unique_ptr<sNode<Ty>>(node);
}

}
}