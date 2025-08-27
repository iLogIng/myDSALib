#pragma once
#include "dNode.hpp"

namespace myDSALib
{
namespace Linear
{

template<typename Ty>
class DouList
{
    using pdNode = dNode<Ty>*;
private:
    dNode<Ty>* head;
    dNode<Ty>* tail;
    void repairHead() {
        if(!hasHead())
            head = new dNode(Ty{}, nullptr, tail);
    }
    void repairTail() {
        if(!hasTail())
            tail = new dNode(Ty{}, head, nullptr);
    }
public:
    explicit DouList()
        : head(new dNode(Ty{}, nullptr, tail)), tail(new dNode(Ty{, head, nullptr})) { }
    explicit DouList(pdNode node)
        : DouList()
    {
        head->insertNext(node);
    }
    explicit DouList(pdNode&& other)
        : head(other.head), tail(other.tail)
    {
        other.head = nullptr;
        other.tail = nullptr;
    }

    DouList(const DouList&) = delete;
    DouList& operator=(const DouList&) = delete;

public:
    bool hasList() {
        return (head && head->Next() && tail && head->Next() != tail);
    }
    bool hasHead() {
        return head != nullptr;
    }
    bool hasTail() {
        return tail != nullptr;
    }

    pdNode getRoot() {
        if(!hasList())
            return nullptr;
        return head->Next();
    }
    pdNode getLast() {
        if(!hasList())
            return nullptr;
        return tail->Prev();
    }

    void headInsert(pdNode node) {
        if(!hasList()) {
            if(!hasHead())
                head = new dNode(Ty{}, nullptr, tail);
            if(!hasTail())
                tail = new dNode(Ty{}, head, nullptr);
        }
        head->insertNext(node);
    }

    void repair() {
        if(!hasList())
            return;
        repairHead();
        repairTail();
    }

};

}
}