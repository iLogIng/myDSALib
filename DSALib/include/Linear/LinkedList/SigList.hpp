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
    class iterator;
    class const_iterator;
private:
    unique_pNode head;      // 头结点 head node
    pNode tail;     // 尾结点 tail node
    size_t count = 0;        // 链表长度 list length
public:
    explicit SigList(const Ty& value) noexcept
        : head(makeSigNode(value)), tail(head.get()), count(1) { }

    explicit SigList(SigList<Ty>&& other)
        : head(other.head.release()), tail(other.tail), count(other.size) {
            other.head.reset();
            other.tail = nullptr;
            other.count = 0;
        }
    SigList& operator=(SigList<Ty>&& other) {
        if(this == &other)
            return *this;

        this->clear();

        this->head = std::move(other.head);
        this->count = other.count;
        this->tail = other.tail;

        other.head.reset();
        other.tail = nullptr;
        other.count = 0;

        return *this;
    }

    template<typename... Args>
    explicit SigList(Args&&... args) noexcept
        : head(makeSigNode<Ty>(std::forward<Args>(args)...)), tail(head.get()), count(1) { }

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

    // emplace head insert
    template<typename... Args>
    void emplace_front(Args&&... args) noexcept;
    // emplace tail inset
    template<typename... Args>
    void emplace_back(Args&&... args) noexcept;

    // insert by iterator
    iterator insert(iterator& itr, const Ty& val) noexcept;
    // move insert by iterator
    iterator insert(iterator& itr, Ty&& val) noexcept;

    // insert emplace
    template<typename... Args>
    iterator emplace(iterator& itr, Args&&... args) noexcept;

    // remove head
    unique_pNode pop_front() noexcept;
    // remove tail
    unique_pNode pop_back() noexcept;

    // remove node by iterator
    unique_pNode remove_node(iterator& pos) noexcept;

    // find iterator by data
    iterator find(Ty& val) noexcept;

    // clear this List
    void clear() noexcept;

    // get size
    size_t size() const noexcept { return count; }
    // if list empty
    bool empty() const noexcept { return count == 0; }

public:
    // reverse this list
    void reverse() noexcept;

public:
    // iterator for list
    class iterator
    {
        friend class SigList<Ty>;
    private:
        pNode current;
        friend class const_iterator;
    public:
        explicit iterator(pNode cur = nullptr) noexcept
            : current(cur) { }
    public:
        Ty& operator*() noexcept {
            return current->data;
        }
        Ty* operator->() noexcept {
            return &(current->data)
        }
        iterator& operator++() noexcept {
            if(current)
                current = current->next.get();
            return *this;
        }
        iterator& operator++(int) noexcept {
            iterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator==(const iterator& other) const noexcept {
            return this->current == other.current;
        }
        bool operator!=(const iterator& other) const noexcept {
            return !(*this == other);
        }

    };
    // const_itrator for list
    class const_iterator
    {
        friend class SigList<Ty>;
    private:
        const pNode current;
    public:
        explicit const_iterator(const pNode cur) noexcept
            : current(cur) { }
        const_iterator(const iterator& other) noexcept
            : current(other.current) { }
    public:
        const Ty& operator*() const noexcept {
            return current->data;
        }
        Ty* operator->() noexcept {
            return &(current->data);
        }
        const_iterator& operator++() noexcept {
            if(current)
                current = current->next.get();
            return *this;
        }
        const_iterator& operator++(int) noexcept {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator==(const const_iterator& other) const noexcept {
            return this->current == other.current;
        }
        bool operator!=(const const_iterator& other) const noexcept {
            return !(*this == other);
        }
    };

    iterator begin() noexcept {
        return iterator(head.get());
    }
    iterator end() noexcept {
        return iterator(nullptr);
    }

    const_iterator begin() const noexcept {
        return const_iterator(head.get());
    }
    const_iterator cbegin() const noexcept {
        return const_iterator(head.get());
    }
    const_iterator end() const noexcept {
        return const_iterator(nullptr);
    }
    const_iterator cend() const noexcept {
        return const_iterator(nullptr);
    }

};

// ==========================================

template<typename Ty, typename... Args>
std::unique_ptr<sNode<Ty>> makeSigNode(Args&&... args) {
    return std::unique_ptr<sNode<Ty>>(new sNode<Ty>(std::forward<Args>(args)...));
}

template<typename Ty>
void SigList<Ty>::push_front(const Ty& val) noexcept {
    unique_pNode node = makeSigNode(val);

    node->next = std::move(head);
    head = std::move(node);

    if(!tail)
        tail = head.get();
    if(!head->next)
        tail = head.get();

    ++count;
}

template<typename Ty>
void SigList<Ty>::push_front(Ty&& val) noexcept {
    unique_pNode node = makeSigNode<Ty>(std::forward<Ty>(val));

    node->next = std::move(head);
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
    unique_pNode node = makeSigNode<Ty>(std::forward<Ty>(val));

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
template<typename... Args>
void SigList<Ty>::emplace_front(Args&&... args) noexcept {
    unique_pNode node = makeSigNode<Ty>(std::forward<Args>(args)...);

    node->setNext(head);
    head = std::move(node);

    if(!tail)
        tail = head.get();
    if(!head->next)
        tail = head->next.get();

    ++count;
}

template<typename Ty>
template<typename... Args>
void SigList<Ty>::emplace_back(Args&&... args) noexcept {
    unique_pNode node = makeSigNode<Ty>(std::forward<Args>(args)...);

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
typename SigList<Ty>::iterator SigList<Ty>::insert(iterator& itr, const Ty& val) noexcept {
    if(itr == end() || itr.current == tail) {
        push_back(val);
        return tail ? iterator(tail) : end();
    }
    unique_pNode node = makeSigNode(val);
    node->next = std::move(itr.current->next);
    itr.current->next = std::move(node);

    if(!itr.current->next->next) {
        tail = itr.current->next.get();
    }

    ++count;
    return iterator(itr.current->next.get());
}

template<typename Ty>
typename SigList<Ty>::iterator SigList<Ty>::insert(iterator& itr, Ty&& val) noexcept {
    if(itr == end() || itr.current == tail) {
        push_back(val);
        return tail ? iterator(tail) : end();
    }
    unique_pNode node(new Node(std::forward<Ty>(val)));
    node->next = std::move(itr.current->next);
    itr.current->next = std::move(node);

    if(!itr.current->next->next) {
        tail = itr.current->next.get();
    }

    ++count;
    return iterator(itr.current->next.get());
}

template<typename Ty>
template<typename... Args>
typename SigList<Ty>::iterator SigList<Ty>::emplace(iterator& itr, Args&&... args) noexcept {
    if(itr == end() || itr.current == tail) {
        unique_pNode node = makeSigNode<Ty>(std::forward<Args>(args)...);
        push_back(node);
        return tail ? iterator(tail) : end();
    }
    unique_pNode node = makeSigNode<Ty>(std::forward<Args>(args)...);
    node->next = std::move(itr.current->next);
    itr.current->next = std::move(node);

    if(!itr.current->next->next) {
        tail = itr.current->next.get();
    }

    ++count;
    return iterator(itr.current->next.get());
}

template<typename Ty>
std::unique_ptr<sNode<Ty>> SigList<Ty>::pop_front() noexcept {
    if(!head)
        return nullptr;

    unique_pNode pop = std::move(head);
    head = std::move(pop->next);
    if(!head)
        tail = nullptr;
    pop->next = nullptr;
    --count;

    return pop;
}

template<typename Ty>
std::unique_ptr<sNode<Ty>> SigList<Ty>::pop_back() noexcept {
    if(!head)
        return nullptr;

    if(head.get() == tail) {
        return pop_front();
    }

    pNode cur = head.get();
    while(cur->next->next) {
        cur = cur->next.get();
    }

    unique_pNode pop = std::move(cur->next);

    cur->next = nullptr;
    tail = cur;

    --count;

    return pop;
}

template<typename Ty>
std::unique_ptr<sNode<Ty>> SigList<Ty>::remove_node(iterator& pos) noexcept {
    if(!pos.current)
        return nullptr;
    if(pos == begin())
        return pop_front();
    if(pos == end())
        return pop_back();

    pNode pre = head.get();
    while(pre->next.get() != pos.current) {
        pre = pre->next.get();
        if(!pre || !pre->next)
            return nullptr;
    }

    unique_pNode removed = std::move(pre->next);
    pre->next = std::move(removed->next);

    removed->next = nullptr;

    --count;

    return removed;
}

template<typename Ty>
typename SigList<Ty>::iterator SigList<Ty>::find(Ty& val) noexcept {
    pNode cur = head.get();
    while(cur) {
        if(cur->data == val)
            return iterator(cur);
        cur = cur->next.get();
    }
    return end();
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