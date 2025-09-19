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
public:
    explicit RBT(unique_pNode& node)
        : root(std::move(node)) { }

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

public:
    

};

}
}