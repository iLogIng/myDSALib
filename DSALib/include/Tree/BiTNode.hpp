#pragma once
#include <memory>
// Binary Tree Node

namespace myDSALib
{
namespace Tree
{

/**
 * This class is the Binary Tree's Node
 */

template<typename Ty>
class BST;

template<typename Ty>
class BiTNode
{
    using Node = BiTNode<Ty>;
    using pNode = Node*;
    using unique_pNode = std::unique_ptr<Node>;
    friend class BST<Ty>;
private:
    unique_pNode lch;       // left node
    unique_pNode rch;       // right node
    Ty data;
public:
    explicit BiTNode(const Ty& elem = Ty{})
        : lch(nullptr), rch(nullptr), data(elem) { }
    explicit BiTNode(const Ty& elem, unique_pNode& left, unique_pNode& right)
        : lch(std::move(left)), rch(std::move(right)), data(elem) { }

    ~BiTNode() = default;

    BiTNode(const BiTNode<Ty>&) = delete;
    BiTNode<Ty>& operator=(const BiTNode<Ty>&) = delete;

    explicit BiTNode(BiTNode<Ty>&& other) noexcept
        : lch(std::move(other.lch)), rch(std::move(other.rch)), data(std::move(other.data)) { }
    BiTNode<Ty>& operator=(BiTNode<Ty>&& other) noexcept {
        if(this != &other) {
            this->lch = std::move(other.lch);
            this->rch = std::move(other.rch);
            this->data = std::move(other.data);
        }
        return *this;
    }

public:
    // data
    Ty& getData() noexcept {
        return data;
    }
    // data const.Ver.
    const Ty& getData() const noexcept {
        return data;
    }

    // left child
    pNode left() noexcept {
        return lch.get();
    }
    // left child const.Ver.
    const pNode left() const noexcept {
        return lch.get();
    }
    // right child
    pNode right() noexcept {
        return rch.get();
    }
    // right child
    const pNode right() const noexcept {
        return rch.get();
    }

    // set data
    void setData(const Ty& new_data) {
        data = new_data;
    }

    // set left child
    void setLeft(unique_pNode& new_left) {
        lch = std::move(new_left);
    }

    // set right child
    void setRight(unique_pNode& new_right) {
        rch = std::move(new_right);
    }
    // release right child
    pNode releaseRight() noexcept {
        return rch.release();
    }

    // has left child
    const bool hasLeft() const noexcept {
        return lch != nullptr;
    }
    // has left only
    const bool leftOnly() const noexcept {
        return lch != nullptr && rch == nullptr;
    }

    // has right child
    const bool hasRight() const noexcept {
        return rch != nullptr;
    }
    // has right only
    const bool rightOnly() const noexcept {
        return rch != nullptr && lch == nullptr;
    }

    // has only child
    const bool hasOnly() const noexcept {
        return (lch != nullptr) ^ (rch != nullptr);
    }
    // has twice children
    const bool hasTwice() const noexcept {
        return lch != nullptr && rch != nullptr;
    }
    // has no child
    const bool hasNone() const noexcept {
        return lch == nullptr && rch == nullptr;
    }

};

// ============================================

// this func for constructing a unique_ptr Binary Tree's Node
template<typename Ty, typename... Args>
std::unique_ptr<BiTNode<Ty>> makeBiNode(Args&&... args) {
    return std::unique_ptr<BiTNode<Ty>>(new BiTNode<Ty>(std::forward<Args>(args)...));
}

// this func for constructing a unique_ptr Binary Tree's Null Node
template<typename Ty>
std::unique_ptr<BiTNode<Ty>> makeNulBiNode() {
    return std::unique_ptr<BiTNode<Ty>>(nullptr);
}

}
}