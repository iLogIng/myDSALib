#pragma once
#include <memory>
// Red & Black Tree's Node

namespace myDSALib
{
namespace Tree
{

/**
 * This is an node for the Red Black Tree
 */

template<typename Ty>
class RBT;

template<typename Ty>
class TrbNode
{
    using Node = TrbNode<Ty>;
    using pNode = Node*;
    using unique_pNode = std::unique_ptr<Node>;
    friend class RBT<Ty>;
private:
    // The node's color
    enum class Color { RED, BLACK };
private:
    Color color;
    pNode par;
    unique_pNode lch;
    unique_pNode rch;
    Ty data;

public:
    explicit TrbNode(const Ty& elem = Ty{}, Color c = Color::BLACK)
        : data(elem), lch(nullptr), rch(nullptr), par(nullptr), color(c) { }
    explicit TrbNode(const Ty& elem, unique_pNode& left, unique_pNode& right, pNode parent, Color col)
        : data(elem), lch(std::move(left)), rch(std::move(right)), par(parent), color(col) { }

    ~TrbNode() = default;

    TrbNode(const TrbNode&) = delete;
    TrbNode& operator=(const TrbNode&) = delete;

    TrbNode(TrbNode&& other)
        : data(std::move(other.data)), lch(std::move(other.lch)), rch(std::move(other.rch)), par(other.parent), color(other.color)
    {
        other.par = nullptr;
        other.color = Color::RED;
    }

    TrbNode& operator=(TrbNode&& other) {
        if(this != &other) {
            this->data = std::move(other.data);
            this->lch = std::move(other.lch);
            this->rch = std::move(other.rch);
            this->par = other.par;
            other.par = nullptr;
            this->color = other.color;
            other.color = Color::RED;
        }

        return *this;
    }

public:
    // color
    Color& getColor() { return color; }
    // color const.Ver
    const Color& getColor() const { return color; }

    // data
    Ty& getData() { return data; }
    // data const.Ver
    const Ty& getData() const { return data; }

    // left
    pNode left() { return lch.get(); }
    // left const.Ver
    const pNode left() const { return lch.get(); }

    // right
    pNode right() { return rch.get(); }
    // right const.Ver
    const pNode right() const { return rch.get(); }

    // parent
    pNode parent() { return par; }
    // parent const.Ver
    const pNode parent() const { return par; }

    // set data
    void setData(const Ty& new_data) {
        data = new_data;
    }

    // set RED
    void setRED() {
        color = Color::RED;
    }

    // set BLACK
    void setBLACK() {
        color = Color::BLACK;
    }

    // set parent
    void setParent(pNode new_parent) {
        par = new_parent;
    }

    // set left
    void setLeft(unique_pNode& new_left) {
        lch = std::move(new_left);
    }
    // release left
    pNode releaseLeft() noexcept {
        return lch.release();
    }

    // set right
    void setRight(unique_pNode& new_right) {
        rch = std::move(new_right);
    }
    // release right
    pNode releaseRight() noexcept {
        return rch.release();
    }

    // has parent
    bool hasParent() noexcept {
        return parent != nullptr;
    }

    // has left
    bool hasLeft() noexcept {
        return lch != nullptr;
    }
    // has left only
    bool leftOnly() noexcept {
        return lch != nullptr && rch == nullptr;
    }

    // has right
    bool hasRight() noexcept {
        return rch != nullptr;
    }
    // has right only
    bool rightOnly() noexcept {
        return rch != nullptr && lch == nullptr;
    }

    // has only child
    bool hasOnly() noexcept {
        return (lch != nullptr) ^ (rch != nullptr);
    }
    // has twice child
    bool hasTwice() noexcept {
        return lch != nullptr && rch != nullptr;
    }
    // has no child
    bool hasNone() noexcept {
        return lch == nullptr && rch == nullptr;
    }

    // is leaf node
    bool isLeaf() noexcept {
        return hasNone() && color == Color::BLACK;
    }

};

// =======================================================

// this func for constructing a unique_ptr Red and Black Tree's Node
template<typename Ty, typename... Args>
std::unique_ptr<TrbNode<Ty>> makeTrbNode(Args&&... args) {
    return std::unique_ptr<TrbNode<Ty>>(new TrbNode(std::forward<Args>(args)...));
}

}
}