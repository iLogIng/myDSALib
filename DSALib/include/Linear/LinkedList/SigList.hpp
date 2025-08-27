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
private:
    sNode<Ty>* head; // 头结点
public:
    SigList()
        : head(new sNode<Ty>{}) { }
    SigList(sNode<Ty>* root)
        : SigList()
    {
        head->setNext(root);
    }
    SigList(SigList<Ty>&& other)
        : head(other.head)
    {
        other.head = nullptr;
    }

    ~SigList() {
        psNode cur = head->Next();
        while(cur) {
            psNode nex = cur->Next();
            delete cur;
            cur = nex;
        }
        delete head;
    }

    SigList(const SigList&) = delete;
    SigList& operator=(const SigList&) = delete;

public:
    psNode getRoot() {
        return head->Next();
    }
    void setRoot(psNode node) {
        head->setNext(node);
    }
    psNode getLast() {
        if(!head || !head->Next())
            return nullptr;
        if(hasCircle())
            return nullptr;

        psNode temp = getRoot();
        while(temp->hasNext())
            temp = temp->Next();
        return temp;
    }

    void headInsert(psNode node) {
        node->setNext(head->Next());
        head->insertNext(node);
    }

    void tailInsert(psNode node) {
        psNode temp = getRoot();
        while(temp->hasNext())
            temp = temp->Next();
        temp->insertNext(node);
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
        while(cur) {
            psNode nex = cur->Next();
            cur->setNext(pre);
            pre = cur;
            cur = nex;
        }
        head->setNext(pre);
        return true;
    }



};

}
}