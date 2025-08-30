#pragma once
#include "dNode.hpp"
#include <memory>

namespace myDSALib
{
namespace Linear
{

template<typename Ty>
class DouList
{
    using Node = dNode<Ty>;
    using pNode = dNode<Ty>*;
    using unique_pNode = std::unique_ptr<dNode<Ty>>;
    class iterator;
    class const_iterator;
private:
    unique_pNode head;      // 头结点 head node
    pNode tail;     // 尾结点 tail node
    size_t count = 0;       // 链表长度 list length
public:
    explicit DouList(const Ty& value) noexcept
        : head(makeDouNode(value)), tail(head.get()), count(1) { }

    explicit DouList(DouList<Ty>&& other) noexcept
        : head(other.head.release()), tail(other.tail), count(other.count) {
            other.head.reset();
            other.tail = nullptr;
            other.count = 0;
        }
    DouList& operator=(DouList&& other) noexcept {
        if(*this == other)
            return *this;

        this->clear();

        this->head = std::move(other.head);
        this->tail = other.tail;
        this->count = other.count;

        other.head.reset();
        other.tail = nullptr;
        other.count = 0;

        return *this;
    }

    template<typename... Args>
    explicit DouList(Args&&... args) noexcept
        : head(makeDouNode<Ty>(std::forward<Args>(args)...)), tail(head.get()), count(1) { }

    ~DouList() = default;

    DouList(const DouList&) = delete;
    DouList& operator=(const DouList&) = delete;

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
    // emplace tail insert
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
    // reverse find iterator by data
    iterator rfind(Ty& val) noexcept;

    // clear this list
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
        friend class DouList<Ty>;
    private:
        pNode current;
        friend class const_iterator;
    public:
        explicit iterator(const pNode& cur) noexcept
            : current(cur) { }
    public:
        Ty& operator*() noexcept {
            return current->data;
        }
        Ty* operator->() noexcept {
            return &(current->data);
        }
        iterator operator++() noexcept {
            if(current)
                current = current->next.get();
            return *this;
        }
        iterator& operator++(int) noexcept {
            iterator temp = *this;
            ++(*this);
            return temp;
        }
        iterator& operator--() noexcept {
            if(current)
                current = current->prev;
            return (*this);
        }
        iterator& operator--(int) noexcept {
            iterator temp = (*this);
            --(*this);
            return temp;
        }
        bool operator==(const iterator& other) const noexcept {
            return this->current == other.current;
        }
        bool operator!=(const iterator& other) const noexcept {
            return !(*this == other);
        }
    };
    // const_iterator for list
    class const_iterator
    {
    private:
        pNode current;
        friend class DouList<Ty>;
    public:
        explicit const_iterator(const pNode& cur) noexcept
            : current(cur) { }
        const_iterator(const iterator& other) noexcept
            : current(other.cur) { }
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
            return (*this);
        }
        const_iterator& operator++(int) noexcept {
            const_iterator temp = (*this);
            ++(*this);
            return temp;
        }
        const_iterator& operator--() noexcept {
            if(current)
                current = current->prev;
            return (*this);
        }
        const_iterator& operator--(int) noexcept {
            const_iterator temp = (*this);
            --(*this);
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
    iterator rbegin() noexcept {
        return iterator(tail);
    }
    iterator rend() noexcept {
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

    const_iterator rbegin() const noexcept {
        return const_iterator(tail);
    }
    const_iterator rend() const noexcept {
        return const_iterator(nullptr);
    }

};

// ==============================================

template<typename Ty, typename... Args>
std::unique_ptr<dNode<Ty>> makeDouNode(Args&&... args) {
    return std::unique_ptr<dNode<Ty>>(new dNode<Ty>(args)...);
}

template<typename Ty>
void DouList<Ty>::push_front(const Ty& val) noexcept {
    unique_pNode node(makeDouNode<Ty>(val));

    if(head) {
        head->prev = node.get();
        node->next = std::move(head);
    }
    else {
        tail = node.get();
    }
    head = std::move(node);
    head->prev = nullptr;

    ++count;
}

template<typename Ty>
void DouList<Ty>::push_front(Ty&& val) noexcept {
    unique_pNode node = makeDouNode<Ty>(std::forward<Ty>(val));

    if(head) {
        head->prev = node.get();
        node->next = std::move(head);
    }
    else {
        tail = node.get();
    }
    head = std::move(node);
    head->prev = nullptr;

    ++count;
}

template<typename Ty>
void DouList<Ty>::push_back(const Ty& val) noexcept {
    unique_pNode node(makeDouNode<Ty>(val));

    if(!head) {
        head = std::move(node);
        tail = head.get();
    }
    else {
        node->prev = tail;
        tail->next = std::move(node);
        tail = tail->next.get();
    }

    ++count;
}

template<typename Ty>
void DouList<Ty>::push_back(Ty&& val) noexcept {
    unique_pNode node = makeDouNode<Ty>(std::forward<Ty>(val));

    if(!head) {
        head = std::move(node);
        tail = head.get();
    }
    else {
        node->prev = tail;
        tail->next = std::move(node);
        tail = tail->next.get();
    }

    ++count;
}

template<typename Ty>
template<typename... Args>
void DouList<Ty>::emplace_front(Args&&... args) noexcept {
    unique_pNode node = makeDouNode<Ty>(std::forward<Ty>(args)...);

    if(head) {
        head->prev = node.get();
        node->next = std::move(head);
    }
    else {
        tail = node.get();
    }
    head = std::move(node);
    head->prev = nullptr;

    ++count;
}

template<typename Ty>
template<typename... Args>
void DouList<Ty>::emplace_back(Args&&... args) noexcept {
    unique_pNode node = makeDouNode<Ty>(std::forward<Ty>(args)...);

    if(!head) {
        head = std::move(node);
        tail = head.get();
    }
    else {
        node->prev = tail;
        tail->next = std::move(node);
        tail = tail->next.get();
    }

    ++count;
}

template<typename Ty>
typename DouList<Ty>::iterator DouList<Ty>::insert(iterator& itr, const Ty& val) noexcept {
    if(itr == end() || itr.current == tail) {
        push_back(val);
        return tail ? iterator(tail) : end();
    }
    unique_pNode node = makeDouNode<Ty>(val);
    node->next = std::move(itr.current->next);
    node->prev = itr.current;
    itr.current->next = std::move(node);

    if(!itr.current->next->next) {
        tail = itr.current->next.get();
    }

    ++count;
    return iterator(itr.current->next.get());
}

template<typename Ty>
typename DouList<Ty>::iterator DouList<Ty>::insert(iterator& itr, Ty&& val) noexcept {
    if(itr == end() || itr.current == tail) {
        push_back(val);
        return tail ? iterator(tail) : end();
    }
    unique_pNode node = makeDouNode<Ty>(std::forward<Ty>(val));
    node->next = std::move(itr.current->next);
    node->prev = itr.current;
    itr.current->next = std::move(node);

    if(!itr.current->next->next) {
        tail = itr.current->next.get();
    }

    ++count;
    return iterator(itr.current->next.get());
}

template<typename Ty>
template<typename... Args>
typename DouList<Ty>::iterator DouList<Ty>::emplace(iterator& itr, Args&&... args) noexcept {
    if(itr == end() || itr.current == tail) {
        push_back(val);
        return tail ? iterator(tail) : end();
    }
    unique_pNode node = makeDouNode<Ty>(std::forward<Args>(args)...);
    node->next = std::move(itr.current->next);
    node->prev = itr.current;
    itr.current->next = std::move(node);

    if(!itr.current->next->next) {
        tail = itr.current->next.get();
    }

    ++count;
    return iterator(itr.current->next.get());
}

template<typename Ty>
std::unique_ptr<dNode<Ty>> DouList<Ty>::pop_front() noexcept {
    if(!head)
        return nullptr;

    unique_pNode pop = std::move(head);
    head = std::move(pop->next);

    if(head) {
        head->prev = nullptr;
    }
    else {
        tail = nullptr;
    }

    pop->prev = nullptr;
    pop->next = nullptr;

    --count;

    return pop;
}

template<typename Ty>
std::unique_ptr<dNode<Ty>> DouList<Ty>::pop_back() noexcept {
    if(!head)
        return nullptr;

    if(head.get() == tail;) {
        return pop_front();
    }

    pNode cur = tail->prev;
    unique_pNode pop = std::move(cur->next);

    cur->next = nullptr;
    tail = cur;

    pop->prev = nullptr;
    pop->next = nullptr;

    --count;

    return pop;
}

template<typename Ty>
std::unique_ptr<dNode<Ty>> DouList<Ty>::remove_node(iterator& pos) noexcept {
    if(!pos.current)
        return nullptr;
    if(pos == begin())
        return pop_front();
    if(pos == end())
        return pop_back();

    pNode pre = pos.current->prev;

    unique_pNode removed = std::move(pre->next);
    pre->next = std::move(removed->next);
    if(pre->next)
        pre->next->prev = pre;

    --count;

    removed->prev = nullptr;
    removed->next = nullptr;

    return removed;
}

template<typename Ty>
typename DouList<Ty>::iterator DouList<Ty>::find(Ty& val) noexcept {
    pNode cur = head.get();
    while(cur) {
        if(cur->data == val)
            return iterator(cur);
        cur = cur->next.get();
    }
    return end();
}

template<typename Ty>
typename DouList<Ty>::iterator DouList<Ty>::rfind(Ty& val) noexcept {
    pNode cur = tail;
    while(cur) {
        if(cur->data == val)
            return iterator(cur);
        cur = cur->prev;
    }
    return rend();
}

template<typename Ty>
void DouList<Ty>::clear() noexcept {
    while(head) {
        head->prev = nullptr;
        head = std::move(head->next);
    }
    count = 0;
    tail = nullptr;
}

// ===============================================

template<typename Ty>
void DouList<Ty>::reverse() noexcept {
    if(!head || !head->next)
        return;

    unique_pNode cur = std::move(head);
    unique_pNode pre(nullptr);
    tail = cur.get();
    while(cur) {
        unique_pNode nex = std::move(cur->next);
        cur->prev = nex.get();
        cur->next = std::move(pre);
        pre = std::move(cur);
        cur = std::move(nex);
    }
    head = std::move(pre);
}

}
}