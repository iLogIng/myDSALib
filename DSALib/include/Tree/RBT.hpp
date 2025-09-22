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
        : root(std::move(other)) { }
    RBT& operator=(RBT&& other) {
        if(this != &other) {
            this->root = std::move(other.root);
        }
        return *this;
    }

private:
    // get Color nullptr is BLACK
    pNode getColor(pNode node) noexcept {
        if(node == nullptr) {
            return Node::Color::BLACK;
        }
        return node->getColor();
    }

    // left rotation
    void Lrotation(unique_pNode& node) noexcept;
    // right rotation
    void Rrotation(unique_pNode& node) noexcept;

public:
    // insert
    bool insert(const Ty& elem);

    // remove
    unique_pNode remove(const Ty& elem);

    // find
    unique_pNode find(const Ty& elem);

    // clear
    void clear() {
        root.reset();
        nil.reset();
    }

public:

};

// ============================================

template<typename Ty>
void RBT<Ty>::Lrotation(unique_pNode& node) noexcept {
    unique_pNode newRoot = std::move(node->rch);
    node->rch = std::move(newRoot->lch);
    newRoot->lch = std::move(node);
    node = std::move(newRoot);
}

template<typename Ty>
void RBT<Ty>::Rrotation(unique_pNode& node) noexcept {
    unique_pNode newRoot = std::move(node->lch);
    node->lch = std::move(newRoot->rch);
    newRoot->rch = std::move(node);
    node = std::move(newRoot);
}

// ============================================

template<typename Ty>
bool RBT<Ty>::insert(const Ty& elem) {
    return true;
};

}
}