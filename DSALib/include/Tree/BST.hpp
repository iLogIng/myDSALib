#pragma once
#include "BiTNode.hpp"
#include "../Linear/Stack.hpp"
#include "../Linear/Queue.hpp"
// Binary Search Tree

namespace myDSALib
{
namespace Tree
{

/**
 * This is a Binary Search Tree
 * 
 * has no balance strategy
 * 
 * using the iterator for pre/in/post/level Traverse
 */

template<typename Ty>
class BST
{
    using Node = BiTNode<Ty>;
    using pNode = Node*;
    using unique_pNode = std::unique_ptr<Node>;

    using stack_t = Linear::DynStack<pNode>;
    using queue_t = Linear::DynQueue<pNode>;
private:
    unique_pNode root;
public:
    explicit BST(unique_pNode& node)
        : root(std::move(node)) { }

    ~BST() = default;

    explicit BST(BST&& other) noexcept
        : BST(std::move(other.root)) { }
    BST& operator=(BST&& other) noexcept {
        if(this != &other) {
            this->root = std::move(other.root);
        }
        return *this;
    }

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

private:
    // remove from node
    unique_pNode removeFrom(unique_pNode& node, const Ty& elem);
    // remove Node
    unique_pNode removeNode(unique_pNode& node);

public:
    // empty
    bool empty() const noexcept { return root.get() == nullptr; }

    // insert
    bool insert(const Ty& elem);

    // remove
    unique_pNode remove(const Ty& elem) noexcept;

    // find
    const pNode find(const Ty& elem) const noexcept;
    
    // find from node
    const pNode findFrom(const pNode node, const Ty& elem) const noexcept;

    // find parent
    const pNode findParent(const Ty& elem) const noexcept;

    // find parent from node
    const pNode findParentFrom(const pNode node, const Ty& elem) const noexcept;

    // find min of the tree
    const pNode findMin() const noexcept { return findMin(root.get()); }
    // min elem from node
    const pNode findMin(pNode cur) const noexcept;

    // find max of the tree
    const pNode findMax() const noexcept { return findMax(root.get()); }
    // max elem from node
    const pNode findMax(pNode cur) const noexcept;

    // clear
    void clear() noexcept;

// Traverse Iterator
public:
    // traverse order enum
    enum class TraverseOrder
    {
        PreOrder,
        InOrder,
        PostOrder,
        LevelOrder
    };

    // TraverseIterator Interface
    class ITraverseIterator
    {
    public:
        virtual ~ITraverseIterator() = default;
    public:
        virtual void advance() = 0;
        virtual pNode getCurrent() const = 0;
    public:
        virtual Ty& operator*() = 0;
        virtual const Ty& operator*() const = 0;
        virtual ITraverseIterator& operator++() = 0;
        virtual ITraverseIterator& operator++(int) = 0;
        virtual bool operator==(const ITraverseIterator& other) const = 0;
        virtual bool operator!=(const ITraverseIterator& other) const = 0;
    };

    // PreTraverse
    class PreTraverse
        : public ITraverseIterator
    {
    private:
        stack_t stack;
        pNode current;
    public:
        explicit PreTraverse(pNode rnode)
            : current(nullptr) {
                if(rnode) {
                    stack.push(rnode);
                    advance();
                }
            }

        ~PreTraverse() = default;

    public:
        void advance() override {
            if(stack.empty()) {
                current = nullptr;
                return;
            }

            current = stack.top();
            stack.pop();
            if(current->hasRight()) {
                stack.push(current->right());
            }
            if(current->hasLeft()) {
                stack.push(current->left());
            }
        }

        pNode getCurrent() const override {
            return current;
        }

        Ty& operator*() override {
            return current->getData();
        }
        const Ty& operator*() const override {
            return current->getData();
        }

        ITraverseIterator& operator++() override {
            advance();
            return *this;
        }

        ITraverseIterator& operator++(int) override {
            PreTraverse* tmp = new PreTraverse(nullptr);
            tmp->stack.clone(this->stack);
            tmp->current = this->current;
            advance();
            return *tmp;
        }

        bool operator==(const ITraverseIterator& other) const override {
            return this->getCurrent() == other.getCurrent();
        }

        bool operator!=(const ITraverseIterator& other) const override {
            return !(*this == other);
        }

    };

    // InTraverse
    class InTraverse
        : public ITraverseIterator
    {
    private:
        stack_t stack;
        pNode current;
    public:
        explicit InTraverse(pNode rnode)
            : current(nullptr) {
                if(rnode) {
                    pushLeftChain(rnode);
                    advance();
                }
            }

        ~InTraverse() = default;

    private:
        void pushLeftChain(pNode node) {
            while(node) {
                stack.push(node);
                node = node->left();
            }
        }

    public:
        void advance() override {
            if(stack.empty()) {
                current = nullptr;
                return;
            }

            current = stack.top();
            stack.pop();
            pushLeftChain(current->right());
        }

        pNode getCurrent() const override {
            return current;
        }

        Ty& operator*() override {
            return current->getData();
        }
        const Ty& operator*() const override {
            return current->getData();
        }
        
        ITraverseIterator& operator++() override {
            advance();
            return *this;
        }

        ITraverseIterator& operator++(int) override {
            InTraverse* tmp = new InTraverse(nullptr);
            tmp->stack.clone(this->stack);
            tmp->current = this->current;
            advance();
            return *tmp;
        }

        bool operator==(const ITraverseIterator& other) const override {
            return this->getCurrent() == other.getCurrent();
        }

        bool operator!=(const ITraverseIterator& other) const override {
            return !(*this == other);
        }

    };

    // PostTraverse
    class PostTraverse
        : public ITraverseIterator
    {
    private:
        stack_t stack;
        pNode current;
        pNode lastVisited;
    public:
        explicit PostTraverse(pNode rnode)
            : current(nullptr) {
                if(rnode) {
                    pushLeftChain(rnode);
                    advance();
                }
            }

        ~PostTraverse() = default;

    private:
        void pushLeftChain(pNode node) {
            while(node) {
                stack.push(node);
                node = node->left();
            }
        }
    public:
        void advance() override {
            if(stack.empty()) {
                current = nullptr;
                return;
            }

            while(!stack.empty()) {
                pNode node = stack.top();

                if(node->hasRight() && node->right() != lastVisited) {
                    pushLeftChain(node->left());
                    
                }
                else {
                    stack.pop();
                    current = node;
                    lastVisited = node;
                    return;
                }
            }

            current = nullptr;
        }

        pNode getCurrent() const override {
            return current;
        }

        Ty& operator*() override {
            return current->getData();
        }
        const Ty& operator*() const override {
            return current->getData();
        }
        
        ITraverseIterator& operator++() override {
            advance();
            return *this;
        }

        ITraverseIterator& operator++(int) override {
            PostTraverse* tmp = new PostTraverse(nullptr);
            tmp->stack.clone(this->stack);
            tmp->current = this->current;
            advance();
            return *tmp;
        }

        bool operator==(const ITraverseIterator& other) const override {
            return this->getCurrent() == other.getCurrent();
        }

        bool operator!=(const ITraverseIterator& other) const override {
            return !(*this == other);
        }

    };

    // LevelTraverse
    class LevelTraverse
        : public ITraverseIterator
    {
    private:
        queue_t queue;
        pNode current;
    public:
        LevelTraverse(pNode rnode)
            : current(nullptr) {
                if(rnode) {
                    queue.push(rnode);
                    advance();
                }
            }

        ~LevelTraverse() = default;

    public:
        void advance() override {
            if(queue.empty()) {
                current = nullptr;
                return;
            }

            current = queue.front();
            queue.pop();
            if(current->hasLeft()) {
                queue.push(current->left());
            }
            if(current->hasRight()) {
                queue.push(current->right());
            }
        }

        pNode getCurrent() const override {
            return current;
        }

        Ty& operator*() override {
            return current->getData();
        }
        const Ty& operator*() const override {
            return current->getData();
        }
        
        ITraverseIterator& operator++() override {
            advance();
            return *this;
        }

        ITraverseIterator& operator++(int) override {
            LevelTraverse* tmp = new LevelTraverse(nullptr);
            tmp->queue.clone(this->queue);
            tmp->current = this->current;
            advance();
            return *tmp;
        }

        bool operator==(const ITraverseIterator& other) const override {
            return this->getCurrent() == other.getCurrent();
        }

        bool operator!=(const ITraverseIterator& other) const override {
            return !(*this == other);
        }

    };

    // EndTraverseIterator
    class EndTraverseIterator
        : public ITraverseIterator
    {
    public:
        void advance() override {}
        pNode getCurrent() const override { return nullptr; }

        Ty& operator*() override { 
            throw std::runtime_error("Dereferencing end iterator");
        }
        const Ty& operator*() const override { 
            throw std::runtime_error("Dereferencing end iterator");
        }

        ITraverseIterator& operator++() override { return *this; }
        ITraverseIterator& operator++(int) override { return *this; }

        bool operator==(const ITraverseIterator& other) const override {
            return other.getCurrent() == nullptr;
        }
        bool operator!=(const ITraverseIterator& other) const override {
            return !(*this == other);
        }
    };

    // iterator
    class iterator {
    private:
        ITraverseIterator* itr;
    public:
        iterator(ITraverseIterator* otr)
            : itr(otr == nullptr ? new EndTraverseIterator() : otr) { }

        ~iterator() { 
            delete itr;
        }

        iterator(const iterator&) = delete;
        iterator& operator=(const iterator&) = delete;

        iterator(iterator&& other) noexcept
            : itr(other.itr) {
                other.itr = nullptr;
            }

        iterator& operator=(iterator&& other) noexcept {
            if(this != &other) {
                delete itr;
                this->itr = other.itr;
                other.itr = nullptr;
            }
            return *this;
        }

    public:
        Ty& operator*() { return **itr; }
        const Ty& operator*() const { return **itr; }
        
        iterator& operator++() {
            ++*itr;
            return *this;
        }
        
        iterator operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }
        
        bool operator==(const iterator& other) const {
            if(itr->getCurrent() == nullptr && other.itr->getCurrent() == nullptr)
                return true;
            if(itr->getCurrent() == nullptr || other.itr->getCurrent() == nullptr)
                return false;
            return *itr == *(other.itr);
        }
        
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    // const_iterator
    class const_iterator {
    private:
        ITraverseIterator* itr;
    public:
        const_iterator(ITraverseIterator* otr)
            : itr(otr == nullptr ? new EndTraverseIterator() : otr) { }

            ~const_iterator() {
            delete itr;
        }

        const_iterator(const const_iterator&) = delete;
        const_iterator& operator=(const const_iterator&) = delete;

        const_iterator(const_iterator&& other) noexcept
            : itr(other.itr) { }
        const_iterator& operator=(const_iterator&& other) noexcept {
            if(this != &other) {
                delete itr;
                this->itr = other.itr;
                other.itr = nullptr;
            }
            return *this;
        }
    public:
        Ty& operator*() { return **itr; }
        const Ty& operator*() const { return **itr; }
        
        const_iterator& operator++() {
            ++(*itr);
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        bool operator==(const const_iterator& other) const {
            if(itr->getCurrent() == nullptr && other.itr->getCurrent() == nullptr)
                return true;
            if(itr->getCurrent() == nullptr || other.itr->getCurrent() == nullptr)
            return *itr == *(other.itr);
        }
        
        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }
    };

public:
    iterator begin(TraverseOrder order = TraverseOrder::InOrder) {
        switch(order) {
            case TraverseOrder::PreOrder    :   return iterator(new PreTraverse(root.get()));
            case TraverseOrder::InOrder     :   return iterator(new InTraverse(root.get()));
            case TraverseOrder::PostOrder   :   return iterator(new PostTraverse(root.get()));
            case TraverseOrder::LevelOrder  :   return iterator(new LevelTraverse(root.get()));
            default:    return iterator(new EndTraverseIterator());
        }
    }

    iterator end() {
        return iterator(new EndTraverseIterator());
    }

    const_iterator cbegin(TraverseOrder order = TraverseOrder::InOrder) {
        switch(order) {
            case TraverseOrder::PreOrder    :   return const_iterator(new PreTraverse(root.get()));
            case TraverseOrder::InOrder     :   return const_iterator(new InTraverse(root.get()));
            case TraverseOrder::PostOrder   :   return const_iterator(new PostTraverse(root.get()));
            case TraverseOrder::LevelOrder  :   return const_iterator(new LevelTraverse(root.get()));
            default:    return const_iterator(new InTraverse(root.get()));
        }
    }

    const_iterator cend() {
        return const_iterator(new EndTraverseIterator());
    }

};

// ===========================================

template<typename Ty>
bool BST<Ty>::insert(const Ty& elem) {
    if(root == nullptr) {
        root = std::move(makeBiNode<Ty>(elem));
        return true;
    }

    pNode cur = root.get();
    while(true) {
        if(elem < cur->getData()) {
            if(cur->hasLeft()) {
                cur = cur->left();
            }
            else {
                cur->setLeft(makeBiNode<Ty>(elem));
                break;
            }
        }
        else if(cur->getData() < elem) {
            if(cur->hasRight()) {
                cur = cur->right();
            }
            else {
                cur->setRight(makeBiNode<Ty>(elem));
                break;
            }
        }
        else {
            return false;
        }
    }
    return true;
}

template<typename Ty>
typename BST<Ty>::unique_pNode BST<Ty>::removeNode(unique_pNode& node) {
    unique_pNode target = std::move(node);  // get the will be removed node

    // leave node
    if(target->hasNone()) {
        node.reset();
        return target;
    }

    // has only one node
    if(target->hasOnly()) {
        node = target->lch ? std::move(target->lch) : std::move(target->rch);
        return target;
    }

    // has twice nodes
    pNode par = target->right();    // parent node
    if(!par->left()) {  // right sub-tree has no left sub tree
        par->lch = std::move(target->lch);
        node = std::move(target->rch);
        return target;
    }
    else {  // right sub-tree has left sub-tree
        while(par->left()->left()) {  // get min elem node's parent
            par = par->left();
        }
        node = std::move(par->lch); // this node get the right sub-tree's min elem
        // at this time, node has only right node.

        par->lch = std::move(node->rch);    // pick the node(the par->lch)'s right to par->lch
        // at this time, node has no sub nodes.

        // repair node's twice sub nodes
        node->lch = std::move(target->lch);
        node->rch = std::move(target->rch);
        return target;
    }
}

template<typename Ty>
typename BST<Ty>::unique_pNode BST<Ty>::removeFrom(unique_pNode& node, const Ty& elem) {
    if(!node)
        return nullptr;
    if(elem < node->getData())  // turn left to find
        return removeFrom(node->lch, elem);
    else if(node->getData() < elem) // turn right to find
        return removeFrom(node->rch, elem);
    else    // node->getData() == elem
        return removeNode(node);
}

template<typename Ty>
typename BST<Ty>::unique_pNode BST<Ty>::remove(const Ty& elem) noexcept {
    if(root && root->getData() == elem) {
        return removeFrom(root, elem);
    }
    return removeFrom(root, elem);
}

template<typename Ty>
const typename BST<Ty>::pNode BST<Ty>::find(const Ty& elem) const noexcept {
    if(root == nullptr) {
        return nullptr;
    }

    pNode cur = root.get();
    while(cur) {
        if(cur->getData() == elem) {
            break;
        }
        else if(elem < cur->getData()) {
            cur = cur->left();
        }
        else { // cur->getData() < elem
            cur = cur->right();
        }
    }
    return cur;
}

template<typename Ty>
const typename BST<Ty>::pNode BST<Ty>::findFrom(const pNode node, const Ty& elem) const noexcept {
    if(node == nullptr) {
        return nullptr;
    }

    pNode cur = node;
    while(cur) {
        if(cur->getData() == elem) {
            break;
        }
        else if(elem < cur->getData()) {
            cur = cur->left();
        }
        else { // cur->getData() < elem
            cur = cur->right();
        }
    }
    return cur;
}

template<typename Ty>
const typename BST<Ty>::pNode BST<Ty>::findParent(const Ty& elem) const noexcept {
    if(!root || root->getData() == elem) {
        return nullptr;
    }

    pNode parent = nullptr;
    pNode cur = root.get();
    while(cur) {
        if((cur->hasLeft() && cur->left()->getData() == elem) ||
            (cur->hasRight() && cur->right()->getData() == elem))
        {
            return cur
        }

        parent = cur;
        if(elem < cur->getData()) {
            cur = cur->left();
        }
        else {
            cur = cur->right();
        }
    }

    return nullptr;
}

template<typename Ty>
const typename BST<Ty>::pNode BST<Ty>::findParentFrom(const pNode node, const Ty& elem) const noexcept {
    if(!node || node->getData() == elem) {
        return nullptr;
    }

    pNode parent = nullptr;
    pNode cur = node.get();
    while(cur) {
        if((cur->lch && cur->lch->getData() == elem) ||
            (cur->rch && cur->rch->getData() == elem)) {
            return cur;
        }

        parent = cur;
        if(elem < cur->getData()) {
            cur = cur->left();
        }
        else {
            cur = cur->right();
        }
    }

    return nullptr;
}

template<typename Ty>
const typename BST<Ty>::pNode BST<Ty>::findMin(pNode cur) const noexcept {
    if(cur == nullptr) {
        return nullptr;
    }

    while(cur->hasLeft()) {
        cur = cur->left();
    }
    return cur;
}

template<typename Ty>
const typename BST<Ty>::pNode BST<Ty>::findMax(pNode cur) const noexcept {
    if(cur == nullptr) {
        return nullptr;
    }

    while(cur->hasRight()) {
        cur = cur->right();
    }
    return cur;
}

template<typename Ty>
void BST<Ty>::clear() noexcept {
    root.reset();
}

}
}