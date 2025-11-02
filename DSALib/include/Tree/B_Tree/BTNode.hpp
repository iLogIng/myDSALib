// BTreeNode.hpp
#pragma once
#include "../../Linear/Array/DynArray.hpp"

namespace myDSALib {
namespace Tree {

/**
 * This is a class for B-Tree
 */

template<typename Ty, int Order>
class BTNode
{
    using KeyCount = unsigned int;
    using KeyTy = Ty;
    using KeysList = myDSALib::Linear::DynArray<Ty>;
    using Node = BTNode;
    using pNode = Node*;
    using NodesList = myDSALib::Linear::DynArray<pNode>;
    using LeafCheck = bool;
private:
    KeyCount keyCount;          // count the keys
    KeysList keys;                 // the keys
    LeafCheck isLeaf;           // is this node a Leaf?
    NodesList children;         // the children

public:
    explicit BTNode(LeafCheck leaf = false)
        : isLeaf(leaf), keyCount(0)
    {
        keys.reserve(Order - 1);
        children.reserve(Order);
    }

public:
    
};

} // namespace Tree
} // namespace myDSALib