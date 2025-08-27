#pragma once
#include <cstddef>
#include <exception>
#include <utility>

namespace myDSALib
{
namespace Linear
{

template<typename Ty>
class sNode
{
    using psNode = sNode<Ty>*;
private:
    sNode<Ty>* next;
    Ty data;
public:
    explicit sNode(const Ty &d = Ty{})
        : data(d), next(nullptr) { }
    explicit sNode(const Ty &d, const psNode &next_node)
        : data(d), next(next_node) { }

    ~sNode() {
        next = nullptr;
    }

    sNode(const sNode&) = delete;
    sNode& operator=(const sNode&) = delete;

public:

    const psNode Next() const { return next; }
    psNode Next() { return next; }

    const psNode stepNext(size_t step) const {
        psNode temp = this;
        for(size_t i = 0; i < step; ++i) {
            if(hasNext())
                temp = temp->next;
            else
                break;
        }
        return temp;
    }
    psNode stepNext(size_t step) {
        psNode temp = this;
        for(size_t i = 0; i < step && temp->hasNext(); ++i) {
            temp = temp->Next();
        }
        return temp;
    }

    void setNext(psNode node) { next = node; }

    void setData(const Ty& d) { data = d; }
    const Ty& getData() const { return data; }
    Ty& getData() { return data; }

    void insertNext(psNode node) {
        if(!node)
            return;
        node->next = this->next;
        this->next = node;
    }

    psNode removeNext() {
        if(!this->next)
            return nullptr;
        psNode removed = this->next;
        this->next = removed->next;
        removed->next = nullptr;
        return removed;
    }

    bool hasNext() const { return next != nullptr; }

};

template<typename Ty, typename... Args>
sNode<Ty>* makeSigNode(Args&&... args) {
    return new sNode<Ty>(std::forward<Args>(args)...);
}

}
}