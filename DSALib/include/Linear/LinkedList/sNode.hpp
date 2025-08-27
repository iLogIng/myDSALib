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
    Ty data;
    psNode next;
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

    void setNext(const psNode& node) { next = node; }

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

}
}