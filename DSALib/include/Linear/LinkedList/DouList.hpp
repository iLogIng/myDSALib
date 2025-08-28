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
            head = new dNode<Ty>(Ty{}, nullptr, tail);
    }
    void repairTail() {
        if(!hasTail())
            tail = new dNode<Ty>(Ty{}, head, nullptr);
    }
public:
    explicit DouList()
        : head(new dNode<Ty>(Ty{}, nullptr, tail)), tail(new dNode<Ty>(Ty{}, head, nullptr)) { }
    explicit DouList(pdNode node)
        : DouList()
    {
        head->insertNext(node);
    }
    explicit DouList(DouList<Ty>&& other)
        : head(other.head), tail(other.tail)
    {
        other.head = nullptr;
        other.tail = nullptr;
    }

    ~DouList() {
        if(!head)
            return;
        pdNode cur = getRoot();
        while(cur) {
            pdNode nex = cur->Next();
            delete cur;
            cur = nex;
        }
        delete head;
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

    pdNode findNode(Ty value) {
        pdNode temp = getRoot();
        while(temp) {
            if(temp->getData() == value)
                return temp;
            temp = temp->Next();
        }
        return nullptr;
    }
    pdNode rfindNode(Ty value) {
        pdNode temp = getLast();
        while(temp) {
            if(temp->getData() == value)
                return temp;
            temp = temp->Prev();
        }
        return nullptr;
    }

    void headInsert(pdNode node) {
        if(!hasList()) {
            repair();
        }
        head->insertNext(node);
    }

    void tailInsert(pdNode node) {
        if(!hasList()) {
            repair();
        }
        tail->insertPrev(node);
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