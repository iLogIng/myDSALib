#pragma once
#include "AvlNode.hpp"
#include "../Linear/Stack.hpp"
#include "../Linear/Queue.hpp"
// Balanced BST

namespace myDSALib
{
namespace Tree
{

/**
 * This is a Balanced Binary Search Tree class
 */

template<typename Ty>
class AVL
{
    using Node = AvlNode<Ty>;
    using pNode = Node*;
    using unique_pNode = std::unique_ptr<Node>;

    using stack_t = Linear::DynStack<pNode>;
    using queue_t = Linear::DynQueue<pNode>;
private:
    unique_pNode root;
public:
    explicit AVL(unique_pNode& node)
        : root(std::move(node)) { }

    ~AVL() = default;

    explicit AVL(AVL&& other) noexcept
        : root(std::move(other.root)) { }
    AVL& operator=(AVL&& other) noexcept {
        if(this != &other) {
            this->root = std::move(other.root);
        }
        return *this;
    }

    AVL(const AVL&) = delete;
    AVL& operator=(const AVL&) = delete;

public:
    // remove from node
    unique_pNode removeFrom(unique_pNode& node, const Ty& elem, unique_pNode& rmNode);
    // remove Node
    unique_pNode removeNode(unique_pNode& node);

public:
    // insert from node
    bool insertFrom(unique_pNode& node, const Ty& elem, bool& hightChanged);
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

// Balance Strategy
private:

    // RR like
    void RRrotation(unique_pNode& node);
    // LL like
    void LLrotation(unique_pNode& node);
    // LR like
    void LRrotation(unique_pNode& node);
    // RL like
    void RLrotation(unique_pNode& node);

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

// ==========================================

// Balanced Strategy

// RR like
template<typename Ty>
void AVL<Ty>::RRrotation(unique_pNode& node) {
    unique_pNode newRoot = std::move(node->rch);
    node->rch = std::move(newRoot->lch);
    newRoot->lch = std::move(node);
    node = std::move(newRoot);

    node->lch->renewHight();
    node->renewHight();
}

// LL like
template<typename Ty>
void AVL<Ty>::LLrotation(unique_pNode& node) {
    unique_pNode newRoot = std::move(node->lch);
    node->lch = std::move(newRoot->rch);
    newRoot->lch = std::move(node);
    node = std::move(newRoot);

    node->rch->renewHight();
    node->renewHight();
}

// LR like
template<typename Ty>
void AVL<Ty>::LRrotation(unique_pNode& node) {
    // unique_pNode newRoot = std::move(node->lch->rch);
    // node->lch->rch = std::move(newRoot->lch);
    // newRoot->lch = std::move(node->lch);
    // node->lch = std::move(newRoot->rch);
    // newRoot->rch = std::move(node);
    // node = std::move(newRoot);
    RRrotation(node->lch);
    LLrotation(node);
}

// RL like
template<typename Ty>
void AVL<Ty>::RLrotation(unique_pNode& node) {
    // unique_pNode newRoot = std::move(node->rch->lch);
    // node->rch->lch = std::move(newRoot->rch);
    // newRoot->rch = std::move(node->rch);
    // node->rch = std::move(newRoot->lch);
    // newRoot->lch = std::move(node);
    // node = std::move(newRoot);
    LLrotation(node->rch);
    RRrotation(node);
}

// ===========================================

// insert and remove

template<typename Ty>
bool AVL<Ty>::insertFrom(unique_pNode& node, const Ty& elem, bool& hightChanged) {
    if(node == nullptr) {
        node = std::move(makeAvlNode<Ty>(elem));
        hightChanged = true;
        return true;
    }

    bool inserted = false;
    hightChanged = false;
    bool childHightChanged = false;

    if(elem < node->getData()) {
        inserted = insertFrom(node->lch, elem, childHightChanged);
        if(!inserted)
            return false;
    }
    else if(node->getData() < elem) {
        inserted = insertFrom(node->rch, elem, childHightChanged);
        if(!inserted)
            return false;
    }
    else { // node->getData() == elem
        return false;
    }

    if(!childHightChanged) {
        return true;
    }
    int oldHight = node->getHight();
    node->renewHight();
    hightChanged = (node->getHight() != oldHight);

    int bf = node->l_r_balance_factor();
    if(bf > 1) {
        if(node->hasLeft()) {
            if(elem < node->left()->getData()) {
                LLrotation(node);
            }
            else {
                LRrotation(node);
            }
            hightChanged = false;
        }
    }
    else if(bf < -1) {
        if(node->hasRight()) {
            if(node->right()->getData() < elem) {
                RRrotation(node);
            }
            else {
                RLrotation(node);
            }
            hightChanged = false;
        }
    }

    return true;
}

template<typename Ty>
bool AVL<Ty>::insert(const Ty& elem) {
    bool hightChanged = false;
    return insertFrom(root, elem, hightChanged);
}

template<typename Ty>
typename AVL<Ty>::unique_pNode AVL<Ty>::remove(const Ty& elem) noexcept {
    if(root == nullptr) {
        return nullptr;
    }

    unique_pNode rmNode;
    root = removeFrom(root, elem, rmNode);
    return rmNode;
}

template<typename Ty>
typename AVL<Ty>::unique_pNode AVL<Ty>::removeFrom(unique_pNode& node, const Ty& elem, unique_pNode& rmNode) {
    if(node == nullptr) {
        return nullptr;
    }

    if(elem < node->getData()) {
        node->lch = std::move(removeFrom(node->lch, elem, rmNode));
    }
    else if(node->getData() < elem) {
        node->rch = std::move(removeFrom(node->rch, elem, rmNode));
    }
    else {
        rmNode = std::move(removeNode(node));
    }

    node->renewHight();
    int bf = node->l_r_balance_factor();

    if(bf > 1) {
        if(node->left()->l_r_balance_factor() >= 0) {
            LLrotation(node);
        }
        else {
            LRrotation(node);
        }
    }
    else if(bf < -1) {
        if(node->right()->l_r_balance_factor() >= 0) {
            RRrotation(node);
        }
        else {
            RLrotation(node);
        }
    }

    return std::move(node);
}

template<typename Ty>
typename AVL<Ty>::unique_pNode AVL<Ty>::removeNode(unique_pNode& node) {
    if(node == nullptr) {
        return nullptr;
    }

    unique_pNode rmNode;
    if(node->hasNone()) { // has no children -- the leave node
        rmNode = std::move(node);
    }
    else if(!node->hasRight()) { // only has left
        rmNode = std::move(node);
        node->lch = std::move(rmNode->lch);
    }
    else if(!node->hasLeft()) { // only has right
        rmNode = std::move(node);
        node->rch = std::move(rmNode->rch);
    }
    else { // has twice node
        rmNode = std::move(node);
        pNode rmin = rmNode->right();
        if(!rmin->hasLeft()) {
            rmin->lch = std::move(rmNode->lch);
            node = std::move(rmNode->rch);
        }
        else {
            while(rmin->left()->left()) {
                rmin = rmin->left();
            }
            node = std::move(rmin->lch);
            rmin->lch = std::move(node->rch);
            node->lch = std::move(rmNode->lch);
            node->rch = std::move(rmNode->rch);
        }
    }

    return rmNode;
}

// ============================================

template<typename Ty>
const typename AVL<Ty>::pNode AVL<Ty>::find(const Ty& elem) const noexcept {
    if(root == nullptr) {
        return nullptr;
    }

    pNode cur = root.get();
    while(cur) {
        if(elem == cur->getData()) {
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
const typename AVL<Ty>::pNode AVL<Ty>::findFrom(const pNode node, const Ty& elem) const noexcept {
    if(node == nullptr) {
        return nullptr;
    }

    pNode cur = pNode;
    while(cur) {
        if(cur->getData() == elem) {
            break;
        }
        else if(elem < cur->getData()) {
            cur = cur->left();
        }
        else { // node->getData() < elem
            cur = cur->right();
        }
    }

    return cur;
}

template<typename Ty>
const typename AVL<Ty>::pNode AVL<Ty>::findParent(const Ty& elem) const noexcept {
    if(!root || root->getData() == elem) {
        return nullptr;
    }

    pNode parent = nullptr;
    pNode cur = root.get();
    while(cur) {
        if((cur->hasLeft() && cur->left()->getData() == elem) ||
            (cur->hasRight() && cur->right()->getData() == elem))
        {
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
const typename AVL<Ty>::pNode AVL<Ty>::findParentFrom(const pNode node, const Ty& elem) const noexcept {
    if(!node || node->getData() == elem) {
        return nullptr;
    }

    pNode parent = nullptr;
    pNode cur = node;
    while(cur) {
        if((cur->hasLeft() && cur->left()->getData() == elem) ||
            (cur->hasRight() && cur->right()->getData() == elem))
        {
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
const typename  AVL<Ty>::pNode AVL<Ty>::findMin(pNode cur) const noexcept {
    if(cur == nullptr) {
        return nullptr;
    }

    while(cur->hasLeft()) {
        cur = cur->left();
    }
    return cur;
}

template<typename Ty>
const typename AVL<Ty>::pNode AVL<Ty>::findMax(pNode cur) const noexcept {
    if(cur == nullptr) {
        return nullptr;
    }

    while(cur->hasRight()) {
        cur = cur->right();
    }
    return cur;
}

template<typename Ty>
void AVL<Ty>::clear() noexcept {
    root.reset();
}

}
}