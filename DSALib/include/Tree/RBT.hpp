#pragma once
#include "TrbNode.hpp"
// Red & Black Tree

namespace myDSALib
{
namespace Tree
{

/**
 * This is a Red Black Tree class
 */

template<typename Ty>
class RBT
{
    using Node = TrbNode<Ty>;
    using pNode = Node*;
    using unique_pNode = std::unique_ptr<Node>;
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

    // insert fixup
    void insertFixup(pNode node) noexcept;

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

    // find Max
    pNode findMax() const noexcept;

    // clear
    void clear() {
        root.reset();
        nil.reset();
    }

public:

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

// Insert =====================================

template<typename Ty>
void RBT<Ty>::insertFixup(pNode node) noexcept {
    // node is not root and node's parent's color is RED
    while(node != root.get() && getColor(node->parent()) == Node::Color::RED) {
        if(node->parent() == node->parent()->parent()->left()) {
            pNode uncle = node->uncle();
            if(getColor(uncle) == Node::Color::RED) {
                node->parent()->setBLACK();
                uncle->setBLACK();
                node->parent()->parent()->setRED();
                node = node->parent()->parent();
            }
            else {
                if(node == node->parent()->right()) {
                    node = node->parent();
                    unique_pNode& ref = getNodeRef(node);
                    Lrotation(ref);
                    node = ref->left();
                }
                node->parent()->setBLACK();
                node->parent()->parent()->setBLACK();
                unique_pNode& ref = getNodeRef(node->parent()->parent());
                Rrotation(ref);
                break;
            }
        }
        else {
            pNode uncle = node->uncle();
            if(getColor(uncle) == Node::Color::RED) {
                node->parent()->setBLACK();
                uncle->setBLACK();
                node->parent()->parent()->setRED();
                node = node->parent()->parent();
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
    root->setBLACK();
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

    unique_pNode newNode = std::move(makeTrbNode<Ty>(elem));
    newNode->setParent(parent);
    newNode->setRED();
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

// template<typename Ty>
// RBT<Ty>::unique_pNode RBT<Ty>::remove(const Ty& elem) {
//     return ;
// }

}
}