#pragma once
#include "sNode.hpp"
#include <utility>

namespace myDSALib
{
namespace Linear
{

/**
 * This is a singly list use the sNode.
 */

template<typename Ty>
class SigList
{
    using Node = sNode<Ty>;
    using pNode = sNode<Ty>*;
    using unique_pNode = std::unique_ptr<sNode<Ty>>;
private:
    unique_pNode head;      // 头结点 head node
    pNode tail;     // 尾结点 tail node
    size_t count = 0;        // 链表长度 list length
public:
    explicit SigList(const Ty& value) noexcept
        : head(new Node(value)), tail(head.get()), count(1) { }

    explicit SigList(SigList<Ty>&& other)
        : head(other.head.release()), tail(other->tail), count(other.size) {
            other.head.reset();
            other.size = 0;
        }
    SigList& operator=(SigList<Ty>&& other) {
        if(this == &other)
            return *this;

        this->~SigList();

        this->head = other.head.release();
        this->size = other.size;

        other.head.reset();
        other.size = 0;
    }

    ~SigList() = default;

    SigList(const SigList&) = delete;
    SigList& operator=(const SigList&) = delete;

public:
    // head insert
    void push_front(const Ty& val) noexcept;
    // move head insert
    void push_front(Ty&& val) noexcept;
    // tail insert
    void push_back(const Ty& val) noexcept;
    // move tail insert
    void push_back(Ty&& val) noexcept;

    // remove head
    unique_pNode pop_front() noexcept;
    // remove tail
    unique_pNode pop_back() noexcept;

    // clear this List
    void clear() noexcept;

    // get size
    size_t size() const noexcept { return count; }
    // if list empty
    bool empty() const noexcept { return count == 0; }


public:
    // reverse this list
    void reverse() noexcept;

private:
    // iterator for list
    class iterator
    {
    private:

    public:
    
    };

};

// ==========================================

template<typename Ty, typename... Args>
std::unique_ptr<sNode<Ty>> makeSigNode(Args&&... args) {
    return std::unique_ptr<sNode<Ty>>(new sNode<Ty>(std::forward<Args>(args)...));
}

template<typename Ty>
void SigList<Ty>::push_front(const Ty& val) noexcept {
    unique_pNode node = makeSigNode(val);

    node->setNext(head);
    head = std::move(node);

    if(!tail)
        tail = head.get();
    if(!head->next)
        tail = head.get();

    ++count;
}

template<typename Ty>
void SigList<Ty>::push_front(Ty&& val) noexcept {
    unique_pNode node = makeSigNode<Ty>(val);

    node->setNext(head);
    head = std::move(node);

    if(!tail)
        tail = head.get();
    if(!head->next)
        tail = head.get();

    ++count;
}

template<typename Ty>
void SigList<Ty>::push_back(const Ty& val) noexcept {
    unique_pNode node = makeSigNode<Ty>(val);

    if(!head) {
        head = std::move(node);
        tail = head.get();
    }
    else {
        tail->next = std::move(node);
        tail = tail->next.get();
    }

    ++count;
}

template<typename Ty>
void SigList<Ty>::push_back(Ty&& val) noexcept {
    unique_pNode node = makeSigNode<Ty>(val);

    if(!head) {
        head = std::move(node);
        tail = head.get();
    }
    else {
        tail->next = std::move(node);
        tail = tail->next.get();
    }

    ++count;
}

template<typename Ty>
std::unique_ptr<sNode<Ty>> SigList<Ty>::pop_front() noexcept {
    if(!head)
        return nullptr;

    unique_pNode pop = std::move(head);
    head = std::move(pop->next);
    if(!head)
        tail = nullptr;
    --count;

    return pop;
}

template<typename Ty>
std::unique_ptr<sNode<Ty>> SigList<Ty>::pop_back() noexcept {
    if(!head)
        return nullptr;

    if(!head->next) {
        tail = nullptr;
        --count;
        return std::move(head);
    }

    pNode cur = head.get();
    while(cur->next->next) {
        cur = cur->next.get();
    }
    unique_pNode pop = std::move(cur->next);
    tail = cur;
    --count;

    return pop;
}

template<typename Ty>
void SigList<Ty>::clear() noexcept {
    while(head) {
        head = std::move(head->next);
    }
    count = 0;
    tail = nullptr;
}

// ========================================

template<typename Ty>
void SigList<Ty>::reverse() noexcept {
    if(!head || !head->next)
        return;

    unique_pNode cur = std::move(head);
    unique_pNode pre(nullptr);
    tail = cur.get();
    while(cur) {
        unique_pNode nex = std::move(cur->next);
        cur->next = std::move(pre);
        pre = std::move(cur);
        cur = std::move(nex);
    }
    head = std::move(pre);
}

}
}