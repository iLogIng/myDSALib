#pragma once
#include <memory>
// Balanced BST's Node

namespace myDSALib
{
namespace Tree
{

/**
 * This is a Node class for Balanced BST
 */

template<typename Ty>
class AVL;

template<typename Ty>
class AvlNode
{
    using Node = AvlNode<Ty>;
    using pNode = Node*;
    using unique_pNode = std::unique_ptr<Node>;
    friend class AVL<Ty>;
private:
    unique_pNode lch;
    unique_pNode rch;
    int hight;
    Ty data;

public:
    explicit AvlNode(const Ty& elem = Ty{})
        : lch(nullptr), rch(nullptr), hight(1), data(elem) { }
    explicit AvlNode(const Ty& elem, unique_pNode& left, unique_pNode& right)
        : lch(std::move(left)), rch(std::move(right)), hight(1), data(elem) { }

    ~AvlNode() = default;

    AvlNode(const AvlNode<Ty>&) = delete;
    AvlNode<Ty>& operator=(const AvlNode<Ty>&) = delete;

    explicit AvlNode(AvlNode<Ty>&& other) noexcept
        : lch(std::move(other.lch)), rch(std::move(other.rch)), data(std::move(other.data)) {
            this->hight = other.hight;
            other.hight = 0;
        }
    AvlNode<Ty>& operator=(AvlNode<Ty>&& other) noexcept {
        if(this != &other) {
            this->lch = std::move(other.lch);
            this->rch = std::move(other.rch);
            this->data = std::move(other.data);
            this->hight = other.hight;
            other.hight = 0;
        }
        return *this;
    }

public:
    // data
    Ty& getData() noexcept {
        return data;
    }
    // data const.Ver
    const Ty& getData() const noexcept {
        return data;
    }

    // left child
    pNode left() noexcept {
        return lch.get();
    }
    // left child const.Ver
    const pNode left() const noexcept {
        return lch.get();
    }
    // right child
    pNode right() noexcept {
        return rch.get();
    }
    // right child const.Ver
    const pNode right() const noexcept {
        return rch.get();
    }

    // hight
    int getHight() noexcept {
        return hight;
    }
    // hight const.Ver
    const int getHight() const noexcept {
        return hight;
    }

    // set data
    void setData(const Ty& new_data) {
        data = new_data;
    }
    // set new left
    void setLeft(unique_pNode& new_left) {
        lch = std::move(new_left);
    }
    // release left
    pNode releaseLeft() noexcept {
        return lch.release();
    }
    // set new right
    void setRight(unique_pNode& new_right) {
        rch = std::move(new_right);
    }
    // release right
    pNode releaseRight() noexcept {
        return rch.release();
    }
    // set hight
    void setHight(int new_hight) noexcept {
        hight = new_hight;
    }

    // has left
    const bool hasLeft() const noexcept {
        return lch != nullptr;
    }
    // has right
    const bool hasRight() const noexcept {
        return rch != nullptr;
    }
    // left only
    const bool leftOnly() const noexcept {
        return lch != nullptr && rch == nullptr;
    }
    // right only
    const bool rightOnly() const noexcept {
        return rch != nullptr && lch == nullptr;
    }

    // has only
    const bool hasOnly() const noexcept {
        return (lch != nullptr) ^ (rch != nullptr);
    }
    // has twice
    const bool hasTwice() const noexcept {
        return lch != nullptr && rch != nullptr;
    }
    // has no
    const bool hasNone() const noexcept {
        return lch == nullptr && rch == nullptr;
    }

    static int renewHight(pNode node) noexcept {
        if(node == nullptr)
            return 0;
        return 1 + std::max(renewHight(node->left()), renewHight(node->right()));
    }

    void renewHight() noexcept {
        hight = renewHight(this);
    }

    int l_r_balance_factor() noexcept {
        int lh = lch == nullptr ? 0 : lch->getHight();
        int rh = rch == nullptr ? 0 : rch->getHight();
        return lh - rh;
    }

};

// =============================================

// this func for constructing a unique_ptr Avl Tree's Node
template<typename Ty, typename... Args>
std::unique_ptr<AvlNode<Ty>> makeAvlNode(Args&&... args) {
    return std::unique_ptr<AvlNode<Ty>>(new AvlNode<Ty>(std::forward<Args>(args)...));
}

// this func for constructing a unique_ptr Avl Tree's Null Node
template<typename Ty>
std::unique_ptr<AvlNode<Ty>> makeNulAvlNode() {
    return std::unique_ptr<AvlNode<Ty>>(nullptr);
}

}
}