#pragma once
#include "BTNode.hpp"

namespace myDSALib
{
namespace Tree
{

template<typename Ty, unsigned int Order = 3>
class BTree
{
    using KeyTy = Ty;
    using NodePtr = typename BTNode<KeyTy, Order>::NodePtr;

};

}
}