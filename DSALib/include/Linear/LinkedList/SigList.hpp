#pragma once
#include "sNode.hpp"

namespace myDSALib
{
namespace Linear
{

template<typename Ty>
class SigList
{
    using psNode = sNode<Ty>*;
    using unique_psNode = std::unique_ptr<sNode<Ty>>;
private:
    unique_psNode head; // 头结点
    psNode tail = nullptr;

    void ensureHead() {
        if(!head) {
            head = makeSigNode<Ty>(Ty{});
            tail = head.get();
        }
    }
public:
    explicit SigList()
        : head(makeSigNode<Ty>(Ty{})), tail(head.get()) { }
    explicit SigList(unique_psNode root)
        : head(makeSigNode<Ty>(Ty{}))
    {
        head->setNext(std::move(root));
        tail = getLast();
    }
    explicit SigList(SigList<Ty>&& other)
        : head(std::move(other.head)), tail(other.tail)
    {
        other.tail = nullptr;
    }

    SigList& operator=(SigList<Ty>&& other) {
        if(this != &other){
            head = std::move(other.head);
            tail = other.tail;
            other = nullptr;
        }
        return *this;
    }

    ~SigList() = default;

    SigList(const SigList&) = delete;
    SigList& operator=(const SigList&) = delete;

public:
    bool isEmpty() {
        return !head || !head->Next();
    }
    bool hasHead() {
        return head != nullptr;
    }

    psNode getRoot() {
        return head ? head->Next() : nullptr;
    }
    psNode getLast() {
        if(isEmpty() || hasCircle())
            return nullptr;
        return tail;
    }

    void headInsert(unique_psNode node) {
        if(!node)
            return;
        ensureHead();
        if(head->hasNext()) {
            node->setNext(head->Next());
        } else {
            tail = node.get();
        }
        head->setNext(std::move(node));
    }

    void tailInsert(psNode node) {
        ensureHead()
        if(!node)
            return;

        if(!head->hasNext()) {
            head->setNext(std::move(node));
        } else {
            tail->setNext(std::move(node));
        }
        tail->setNext(std::move(node));
    }

    unique_psNode remove(Ty value) {
        psNode prev = findPrevNode(value);
        if(!pre)
            return nullptr;
        unique_psNode removed = prev->removeNext();

        if(tail == removed.get())
            tail = prev;
        return removed;
    }

    psNode findNode(Ty value) {
        psNode temp = getRoot();
        while(temp) {
            if(temp->getData() == value)
                return temp;
            temp = temp->Next();
        }
        return nullptr;
    }
    psNode findPrevNode(Ty value) {
        psNode cur = head.get();
        while(cur && cur->hasNext()) {
            if(cur->Next()->getData() == value)
                return temp;
            cur = cur->Next();
        }
    }

    bool hasCircle() {
        if(!head || !getRoot())
            return false;
        psNode fast = getRoot();
        psNode slow = getRoot();
        while(fast && fast->Next()) {
            fast = fast->Next()->Next();
            slow = slow->Next();
            if(fast == slow)
                return true;
        }
        return false;
    }

    bool reverse() {
        if(hasCircle())
            return false;

        psNode cur = getRoot();
        psNode pre = nullptr;
        psNode newTail = cur;
        while(cur) {
            psNode nex = cur->Next();
            cur->setNext(pre);
            pre = cur;
            cur = nex;
        }
        head->setNext(pre);
        tail = newTail;
        return true;
    }

};

}
}