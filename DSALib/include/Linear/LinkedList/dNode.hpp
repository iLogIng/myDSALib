#pragma once
#include <cstddef>
#include <exception>
#include <utility>

namespace myDSALib
{
namespace Linear
{

template<typename Ty>
class dNode
{
    using pdNode = dNode<Ty>*;
private:
    Ty data;
    pdNode next;
    pdNode prev;
public:
    explicit dNode(const Ty &d = Ty{})
        : data(d), next(nullptr), prev(nullptr) { }
    explicit dNode(const Ty &d, pdNode prev_node, pdNode next_node)
        : data(d), prev(prev_node), next(next_node) { }

    ~dNode() {
        unlink();
    }

    dNode(const dNode&) = delete;
    dNode& operator=(const dNode&) = delete;

public:

    const pdNode Next() const { return next; }
    pdNode Next() { return next; }
    const pdNode Prev() const { return prev; }
    pdNode Prev() { return prev; }

    void setPrev(const pdNode& node) { prev = node; }
    void setNext(const pdNode& node) { next = node; }

    void setData(const Ty& d) { data = d; }
    const Ty& getData() const { return data; }
    Ty& getData() { return data; }

    void insertNext(pdNode node) {
        if(!node)
            return;
        node->prev = this;
        node->next = this->next;
        if(this->next)
            this->next->prev = node;
        this->next = node;
    }

    pdNode removeNext() {
        if(!this->next)
            return nullptr;
        pdNode removed = this->next;
        if(!removed->next)
            removed->next->prev = this;
        this->next = removed->next;

        removed->prev = nullptr;
        removed->next = nullptr;
        return removed;
    }

    void insertPrev(pdNode node) {
        if(!node)
            return;
        node->next = this;
        node->prev = this->prev;
        if(this->prev)
            this->prev->next = node;
        this->prev = node;
    }

    pdNode removePrev() {
        if(!this->prev)
            return nullptr;
        pdNode removed = this->prev;
        if(!removed->prev)
            removed->prev->next = this;
        this->prev = removed->prev;

        removed->prev = nullptr;
        removed->next = nullptr;
        return removed;
    }

    void unlink() {
        if(prev) prev->next = next;
        if(next) next->prev = prev;
        prev = nullptr;
        next = nullptr;
    }

    bool hasNext() const { return next != nullptr; }
    bool hasPrev() const { return prev != nullptr; }
    bool isLinked() const { return prev != nullptr || next != nullptr; }

};

}
}