#pragma once
#include "TrbNode.hpp"
#include "../Linear/Stack.hpp"
#include "../Linear/Queue.hpp"
// Red & Black Tree

namespace myDSALib
{
namespace Tree
{

/**
 * This is a Red Black Tree class
 * 
 * 1. The tree's node's Color only BLACK or RED
 * 2. The root must be BLACK node
 * 3. The leave nodes all BLACK node
 * 4. The RED node must have twice BLACK nodes
 * 5. The left and right sub tree of the sub root in this tree has same BLACK HIGHT
 * 
 * the most important func is the
 *  - insertFixup
 *  - removeFixup
 * 
 * NOT to count the BLACK HIGHT but to fix the main rule (4 AND 5)
 */

template<typename Ty>
class RBT
{
    using Node = TrbNode<Ty>;
    using pNode = Node*;
    using unique_pNode = std::unique_ptr<Node>;

    using stack_t = Linear::DynStack<pNode>;
    using queue_t = Linear::DynQueue<pNode>;
private:
    unique_pNode root;
    unique_pNode nil;
public:
    explicit RBT(unique_pNode& node)
        : root(std::move(node)) {
            nil = std::move(makeTrbNode<Ty>(Ty{}, Node::Color::BLACK));
            if(root) {
                root->setParent(nil.get());
            }
        }

    ~RBT() = default;

    RBT(const RBT&) = delete;
    RBT& operator=(const RBT&) = delete;

    RBT(RBT&& other)
        : root(std::move(other.root)), nil(std::move(other.nil)) { }
    RBT& operator=(RBT&& other) {
        if(this != &other) {
            this->root = std::move(other.root);
            this->nil = std::move(other.nil);
        }
        return *this;
    }

private:
    // get Color nullptr is BLACK
    typename Node::Color getColor(pNode node) noexcept {
        if(node == nullptr) {
            return Node::Color::BLACK;
        }
        return node->getColor();
    }

    // get the unique_pNode of node
    unique_pNode& getNodeRef(pNode node) noexcept {
        if(node->parent() == nullptr) {
            return root;
        }
        else {
            if(node->parent()->left() == node) {
                return node->parent()->lch;
            }
            else if(node->parent()->right() == node) {
                return node->parent()->rch;
            }
        }
    }

    // left rotation
    void Lrotation(unique_pNode& node) noexcept;
    // right rotation
    void Rrotation(unique_pNode& node) noexcept;

    // transplant the rmd and other node
    void transplant(pNode u, pNode v) noexcept;

    //
    unique_pNode extractNode(pNode node) noexcept;

    // insert fixup
    void insertFixup(pNode node) noexcept;
    // remove fixup
    void removeFixup(pNode node) noexcept;

public:
    // insert
    bool insert(const Ty& elem);

    // remove
    unique_pNode remove(const Ty& elem);

    // find
    pNode find(const Ty& elem) const noexcept;

    // find from
    pNode findFrom(pNode node, const Ty& elem) const noexcept;

    // find Min
    pNode findMin() const noexcept;

    // find Min From
    pNode findMinFrom(pNode node) const noexcept;

    // find Max
    pNode findMax() const noexcept;

    // find Max From
    pNode findMaxFrom(pNode node) const noexcept;

    // clear
    void clear() {
        root.reset();
        nil.reset();
    }

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

// Rotation ===================================

// Left Rotation
template<typename Ty>
void RBT<Ty>::Lrotation(unique_pNode& node) noexcept {
    unique_pNode newRoot = std::move(node->rch);
    newRoot->setParent(node->parent());  // fix parent
    if(newRoot->lch != nullptr) {
        newRoot->lch->setParent(node.get());    // fix parent
    }
    node->rch = std::move(newRoot->lch);
    node->setParent(newRoot.get()); // fix parent
    newRoot->lch = std::move(node);
    node = std::move(newRoot);
}

// Right Rotation
template<typename Ty>
void RBT<Ty>::Rrotation(unique_pNode& node) noexcept {
    unique_pNode newRoot = std::move(node->lch);
    newRoot->setParent(node->parent()); // fix parent
    if(newRoot->rch != nullptr) {
        newRoot->rch->setParent(node.get());    // fix parent
    }
    node->lch = std::move(newRoot->rch);
    node->setParent(newRoot.get()); // fix parent
    newRoot->rch = std::move(node);
    node = std::move(newRoot);
}

// Find =======================================

template<typename Ty>
typename RBT<Ty>::pNode RBT<Ty>::findFrom(pNode node, const Ty& elem) const noexcept {
    if(!node) {
        return nullptr;
    }

    pNode cur = node;
    while(cur) {
        if(elem < cur->getData()) {
            cur = cur->left();
        }
        else if(cur->getData() < elem) {
            cur = cur->right();
        }
        else {
            break;
        }
    }

    return cur;
}

template<typename Ty>
typename RBT<Ty>::pNode RBT<Ty>::find(const Ty& elem) const noexcept {
    if(!root) {
        return nullptr;
    }

    pNode cur = root.get();
    while(cur) {
        if(elem < cur->getData()) {
            cur = cur->left();
        }
        else if(cur->getData() < elem) {
            cur = cur->right();
        }
        else {
            break;
        }
    }

    return cur;
}

template<typename Ty>
typename RBT<Ty>::pNode RBT<Ty>::findMin() const noexcept {
    if(!root) {
        return nullptr;
    }

    pNode cur = root.get();
    while(cur->left()) {
        cur = cur->left();
    }

    return cur;
}

template<typename Ty>
typename RBT<Ty>::pNode RBT<Ty>::findMinFrom(pNode node) const noexcept {
    if(!node) {
        return nullptr;
    }

    while(node->left()) {
        node = node->left();
    }

    return node;
}

template<typename Ty>
typename RBT<Ty>::pNode RBT<Ty>::findMax() const noexcept {
    if(!root) {
        return nullptr;
    }

    pNode cur = root.get();
    while(cur->right()) {
        cur = cur->right();
    }

    return cur;
}

template<typename Ty>
typename RBT<Ty>::pNode RBT<Ty>::findMaxFrom(pNode node) const noexcept {
    if(!node) {
        return nullptr;
    }

    while(node->right()) {
        node = node->right();
    }

    return node;
}

// Insert =====================================

// the main operation for insert is this insertFixup
template<typename Ty>
void RBT<Ty>::insertFixup(pNode node) noexcept {
    // this init node is a RED node

    // this node is not root and node's parent is RED
    while(node != root.get() && getColor(node->parent()) == Node::Color::RED) {
        // if parent is grand's left
        if(node->parent() == node->parent()->parent()->left()) {
            pNode uncle = node->uncle();
            // if the uncle is a RED node
            if(getColor(uncle) == Node::Color::RED) {   // just neet to change the color
                // set parent and uncle as BLACK node
                node->parent()->setBLACK();
                uncle->setBLACK();
                // set grand as RED node
                node->parent()->parent()->setRED();
                node = node->parent()->parent();    // climb up to fix the branch
            }
            else {  // uncle is a BLACK node, neet rotation operation
                if(node == node->parent()->right()) {   // is parent's right node, must has twice different rotation
                    node = node->parent();
                    unique_pNode& ref = getNodeRef(node);
                    Lrotation(ref); // set g-lpar-rsun to a l-sub-branch line
                    node = ref->left();
                }
                // is parent's left node
                node->parent()->setBLACK();
                node->parent()->parent()->setBLACK();
                unique_pNode& ref = getNodeRef(node->parent()->parent());
                Rrotation(ref); // grand node to rotate
                break;  // has perfectly Rotated
            }
        }   // node's parent is the right child
        else {
            // same logic with different Rotation order
            pNode uncle = node->uncle();
            if(getColor(uncle) == Node::Color::RED) {
                node->parent()->setBLACK();
                uncle->setBLACK();
                node->parent()->parent()->setRED();
                node = node->parent()->parent();    // climb up to fix the branch
            }
            else {
                if(node == node->parent()->left()) {
                    node = node->parent();
                    unique_pNode& ref = getNodeRef(node);
                    Rrotation(ref);
                    node = ref->left();
                }
                node->parent()->setBLACK();
                node->parent()->parent()->setRED();
                unique_pNode& ref = getNodeRef(node->parent()->parent());
                Lrotation(ref);
                break;
            }
        }
    }

    root->setBLACK();   // fix the root's color
}

template<typename Ty>
bool RBT<Ty>::insert(const Ty& elem) {
    if(!root) {
        root = std::move(makeTrbNode<Ty>(elem));
        if(!nil) {
            nil = std::move(makeTrbNode<Ty>(Ty{}, Node::Color::BLACK));
        }
        root->setParent(nil.get());
        root->setBLACK();
        return true;
    }

    pNode cur = root.get();
    pNode parent = nullptr;
    while(cur) {
        parent = cur;

        if(elem < cur->getData()) {
            cur = cur->left();
        }
        else if(cur->getData() < elem) {
            cur = cur->right();
        }
        else {
            return false;
        }
    }

    // the init new node is RED
    unique_pNode newNode = std::move(makeTrbNode<Ty>(elem, Node::Color::RED));
    newNode->setParent(parent);
    pNode normal = newNode.get();

    if(elem < parent->getData()) {
        parent->setLeft(newNode);
    }
    else {
        parent->setRight(newNode);
    }

    insertFixup(normal);

    return true;
};

// Remove ========================================

template<typename Ty>
void RBT<Ty>::transplant(pNode u, pNode v) noexcept {
    if(u->parent() == nil.get()) {
        // u is root
        root.reset(v? getNodeRef(u).release() : nullptr);
        if(root) {
            root->setParent(nil.get());
        }
    }
    else if(u == u->par->left()) {
        if(v) {
            u->parent()->lch.reset(getNodeRef(v).release());
            v->setParent(u->parent());
        }
        else {
            u->parent()->lch.reset();
        }
    }
    else {
        if(v) {
            u->parent()->rch.reset(getNodeRef(v).release());
            v->setParent(u->parent());
        }
        else {
            u->parent()->lch.reset();
        }
    }

    if(v) {
        v->setParent(u->parent());
    }
}

// node is the taking place node
template<typename Ty>
void RBT<Ty>::removeFixup(pNode node) noexcept {
    // make the virtual BLACK node climbs up
    while(node != root.get() && getColor(node) == Node::Color::BLACK) {
        // node is parent's left    sibling is parent's right
        if(node == node->par->left()) {
            pNode sibling = node->sibling();
            // node's sibling is RED node
            if(getColor(sibling) == Node::Color::RED) {
                // change the sibling and parent to BLACK node
                sibling->setBLACK();
                node->par->setRED();
                // Left rotation
                unique_pNode& pref = getNodeRef(node->parent());
                Lrotation(pref);
                // make sibling is the new right node
                sibling = node->par->right();
            }
            // sibling's twice all BLACK nodes
            if(getColor(sibling->left()) == Node::Color::BLACK && getColor(sibling->right()) == Node::Color::BLACK) {
                sibling->setRED();
                node = node->parent();
            }
            else {
                // sibling's right child is BLACK node to make twice different Rotation
                if(getColor(sibling->right()) == Node::Color::BLACK) {
                    sibling->left()->setBLACK();    // ensure sibling's left is BLACK node
                    sibling->setRED();  // turn sibling to RED node
                    // Rotation from sibling
                    unique_pNode& sref = getNodeRef(sibling);
                    Rrotation(sref);
                    sibling = node->par->right();
                }
                // sibling get the parent's color
                sibling->color = node->par->getColor();
                node->par->setBLACK();  // set parent to BLACK node to fix the rule
                sibling->right()->setBLACK();
                unique_pNode& pref = getNodeRef(node->parent());
                Lrotation(pref);
                node = root.get();
            }
        }   // node is parent's right   sibling is parent's left
        else {
            pNode sibling = node->sibling();
            if(getColor(sibling) == Node::Color::RED) {
                sibling->setBLACK();
                node->par->setRED();
                unique_pNode& pref = getNodeRef(node->parent());
                Rrotation(pref);
                // make sibling is the new left node
                sibling = node->par->left();
            }
            if(getColor(sibling->left()) == Node::Color::BLACK && getColor(sibling->right()) == Node::Color::BLACK) {
                sibling->setRED();
                node = node->parent();
            }
            else {
                if(getColor(sibling->left()) == Node::Color::BLACK) {
                    sibling->right()->setBLACK();
                    sibling->setRED();
                    unique_pNode& sref = getNodeRef(sibling);
                    Lrotation(sref);
                    sibling = node->par->left();
                }
                sibling->color = node->par->getColor();
                node->par->setBLACK();
                sibling->left()->setBLACK();
                unique_pNode& pref = getNodeRef(node->parent());
                Rrotation(pref);
                node = root.get();
            }
        }
    }
    node->setBLACK();
}

template<typename Ty>
typename RBT<Ty>::unique_pNode RBT<Ty>::extractNode(pNode node) noexcept {
    if (node->parent() == nil.get()) {
        // Node is the root
        unique_pNode result = std::move(root);
        root = nullptr;
        return result;
    }
    else if (node == node->parent()->left()) {
        // Node is the left child
        return std::move(node->parent()->lch);
    }
    else {
        // Node is the right child
        return std::move(node->parent()->rch);
    }
}

template<typename Ty>
typename RBT<Ty>::unique_pNode RBT<Ty>::remove(const Ty& elem) {
    if (!root) {
        return nullptr;
    }

    pNode target = find(elem);  // the target node with data == elem
    if (!target) {
        return nullptr;
    }

    unique_pNode& result = getNodeRef(target);
    pNode y = target;           // the will removed node
    pNode x = nullptr;          // the replace node of y
    typename Node::Color y_original_color = y->getColor();

    if (!target->left() || target->left() == nil.get()) {
        x = target->right() == nil.get() ? nullptr : target->right();
        transplant(target, x);
    }
    else if (!target->right() || target->right() == nil.get()) {
        x = target->left() == nil.get() ? nullptr : target->left();
        transplant(target, x);
    }
    else {
        y = findMinFrom(target->right());
        y_original_color = y->getColor();
        x = y->right() == nil.get() ? nullptr : y->right();
        
        if (y->parent() != target) {
            transplant(y, x);
            y->setRight(std::move(target->rch));
            y->right()->setParent(y);
        }
        
        transplant(target, y);
        y->setLeft(std::move(target->lch));
        y->left()->setParent(y);
        y->color = target->getColor();
    }

    if (y_original_color == Node::Color::BLACK && x) {
        removeFixup(x);
    }

    return std::move(result);
}

}
}