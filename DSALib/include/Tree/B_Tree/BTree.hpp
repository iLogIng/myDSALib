#pragma once
#include "BTNode.hpp"

namespace myDSALib
{
namespace Tree
{

template<typename Ty, unsigned int Order = 3>
class BTree
{
    using KeyType = Ty;
    using NodePtr = typename BTNode<KeyType, Order>::NodePtr;
private:
    NodePtr root_;
    size_t size_;

public:
    BTree()
        : root_(std::shared_ptr<BTNode<KeyType, Order>>(true)), size_(0) { }

    ~BTree() = default;

    BTree(const BTree&) = delete;
    BTree& operator=(const BTree&) = delete;

    BTree(BTree&& other)
        : root_(std::move(other.root_)), size_(other.size_)
    {
        other.size_ = 0;
    }
    BTree& operator=(BTree&& other) {
        if(this != &other) {
            this->root_ = std::move(other.root_);
            this->size_ = other.size_;
            other.size_ = 0;
        }
        return *this;
    }

public:

};

} // namespace Tree
} // namespace myDSALib